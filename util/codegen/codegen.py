"""
codegen is a python script utility used to build codegen json files into C++ source code.
"""

import sys
import argparse
import json
import os
from common import copyable_types, codegen_initial_comment, prelude, file_is_generated
from chaigen import ChaiBindingsGenerator

template_file_extension = ".json"
output_file_extension = ".hpp"

struct_template = """
struct {struct_name} {{
{constructors}
{data_members}
}};
"""


def make_namespaced(struct_body, namespace):
    """
    Add a namespace around a struct body.
    """
    if namespace == "":
        return struct_body
    return "namespace {namespace} {{\n{struct_body}}}\n".format(**locals())


def make_member(member):
    """
    Turn a member data entry into a C++ type declaration.
    """
    return "{} {};".format(member["type"], member["name"])


def make_default_constructor(codegen_template):
    """
    Generate a default constructor from members.
    """
    ctor_template = "{struct_name}() {initializers} {{}}"
    initializers = []
    for member in codegen_template["data"]:
        default_value = "" if "default" not in member else member["default"]
        initializers.append("{}({})".format(member["name"], default_value))
    initializers = ", ".join(initializers)
    if len(codegen_template["data"]) > 0:
        initializers = ": " + initializers

    return ctor_template.format(struct_name=codegen_template["name"], initializers=initializers)


def make_member_constructor(codegen_template, argnames=True):
    """
    Generate a member-list constructor from members. e.g. if we have DebugName struct with member 'std::string name',
    this will generate a constructor `DebugName::DebugName(const std::string& name)`
    """

    ctor_template = "{struct_name}({args}) : {initializers} {{}}"
    initializers = []
    args = []
    for member in codegen_template["data"]:
        argname = member["name"] + "_"
        if member["type"] in copyable_types:
            args.append("{} {}".format(member["type"], argname))
        else:
            args.append("const {}& {}".format(member["type"], argname))
        initializers.append("{}({})".format(member["name"], argname))
    initializers = ", ".join(initializers)
    nargs = len(args)
    args = ", ".join(args)

    ctor_code = ctor_template.format(struct_name=codegen_template["name"], args=args, initializers=initializers)

    # Constructors with one argument should be marked explicit to prevent unintentional conversions.
    if nargs == 1:
        ctor_code = "explicit " + ctor_code

    return ctor_code


def generate_code(codegen_file, output_file, chai_binder: ChaiBindingsGenerator):
    """
    Given a single codegen file template, generate and return the corresponding C++ code.
    """
    with open(codegen_file, 'r') as f:
        codegen_template = json.load(f)

    struct_name = codegen_template["name"]
    namespace = "" if "namespace" not in codegen_template else codegen_template["namespace"]
    if "data" in codegen_template:
        data_members = [make_member(m) for m in codegen_template["data"]]
        data_members = ["    {}".format(m) for m in data_members]
        data_members = "\n".join(data_members)

        constructors = [make_default_constructor(codegen_template)]
        if len(data_members) > 0: constructors.append(make_member_constructor(codegen_template))
        constructors = "".join("    {}\n".format(c) for c in constructors)
    else:
        data_members = ""
        constructors = ""

    # Add chai bindings if the option is set.
    try:
        if "chai_bindings" in codegen_template["generate"]:
            chai_binder.add_chai_bindings(output_file, codegen_template)
    except KeyError:
        pass

    struct_body = struct_template.format(
        struct_name=struct_name,
        constructors=constructors,
        data_members=data_members
    )

    if "include" in codegen_template:
        includes = "".join("#include <{}>\n".format(inc) for inc in codegen_template["include"])
    else:
        includes = ""

    code = (prelude.format(codegen_initial_comment)
            + includes
            + make_namespaced(struct_body, namespace))

    return {"struct_name": struct_name,
            "namespaced_struct_name": struct_name if namespace == "" else namespace + "::" + struct_name,
            "code": code,
            "xlisted": [] if "xlisted" not in codegen_template else codegen_template["xlisted"],
            "include_in": [] if "include_in" not in codegen_template else codegen_template["include_in"]}


def main():
    parser = argparse.ArgumentParser(description='Generate C++ code from .json template files')
    parser.add_argument(
        '--output', '-o', type=str, help='the directory to output files to')
    parser.add_argument(
        '--input', '-i', type=str, help='a directory to scan for input files')
    args = parser.parse_args()

    generated_files = []
    xlisted = {}
    includes = {}
    chai_bindings_generator = ChaiBindingsGenerator(namespace="cyan::generated")

    for root, dirs, files in os.walk(args.input):
        for file in files:
            if not file.endswith(template_file_extension):
                continue

            template_path = os.path.join(root, file).replace("\\", "/")
            output_path = os.path.join(args.output, os.path.relpath(root, args.input), file).replace("\\", "/")
            output_path = output_path.replace(template_file_extension, output_file_extension)

            if not file_is_generated(output_path):
                print(("ERROR: Cannot create {} from {} because the file already exists and does not " +
                       "start with '{}'").format(output_path, template_path, codegen_initial_comment))
                continue

            print("Generating file {} from {}".format(output_path, template_path))
            generated_file = generate_code(template_path, output_path, chai_bindings_generator)

            # Save file information to an x-list.
            for xlist in generated_file["xlisted"]:
                xlist_name = xlist[0]
                xlist_file_path = os.path.join(args.output, os.path.relpath(root, args.input), xlist[1]).replace("\\", "/")
                xlist_key = "{}:{}".format(xlist_file_path, xlist_name)
                if xlist_key not in xlisted:
                    xlisted[xlist_key] = {"entries": [], "listname": xlist_name, "fname": xlist_file_path}
                xlisted[xlist_key]["entries"].append(generated_file["struct_name"])

            # save file information to an include list.
            for inc in generated_file["include_in"]:
                include_file_path = os.path.join(args.output, os.path.relpath(root, args.input), inc).replace("\\", "/")
                if include_file_path not in includes:
                    includes[include_file_path] = set()
                includes[include_file_path].add(output_path)

            # Ensure the path to the file-to-be-generated exists.
            if not os.path.exists(os.path.dirname(output_path)):
                os.makedirs(os.path.dirname(output_path))

            # Write the generated file
            with open(output_path, 'w+') as f:
                f.write(generated_file["code"])
            generated_files.append(output_path)

    # Generate x-list.
    for xlist_key, xlist in xlisted.items():
        xlist_code = """#define {} \\\n""".format(xlist["listname"])
        xlist_code = xlist_code + "\\\n".join(["    X({})".format(xentry) for xentry in xlist["entries"]])
        xlist_code = prelude.format(codegen_initial_comment) + xlist_code

        # Make sure we only overwrite files that have been generated
        if not file_is_generated(xlist["fname"]):
            print(("ERROR: Cannot create X-list file {} because the file already exists and does " +
                   "not start with '{}'").format(xlist["fname"], codegen_initial_comment))
            continue

        print("Generating X-Macro list file {}".format(xlist["fname"]))
        with open(xlist["fname"], 'w+') as f:
            f.write(xlist_code)
        generated_files.append(xlist["fname"])

    # Generate files specified with include_in.
    for include_file, includes in includes.items():
        inc_code = ["#include \"{}\"".format(inc) for inc in includes]
        inc_code = "\n".join(inc_code)
        inc_code = prelude.format(codegen_initial_comment) + inc_code

        include_file = include_file.replace("\\", "/")

        # Make sure we only overwrite files that have been generated
        if not file_is_generated(include_file):
            print(("ERROR: Cannot create include list file {} because the file already exists and does " +
                   "not start with '{}'").format(include_file, codegen_initial_comment))
            continue

        print("Generating include list file {}".format(include_file))
        with open(include_file, 'w+') as f:
            f.write(inc_code)
        generated_files.append(include_file)

    # Generate Chai bindings
    chai_hpp = os.path.join(args.output, "chai_bindings.hpp").replace("\\", "/")
    chai_cpp = os.path.join(args.output, "chai_bindings.cpp").replace("\\", "/")
    generated_files.extend([chai_hpp, chai_cpp])
    print("Generating chai bindings files: {}, {}".format(chai_hpp, chai_cpp))
    chai_bindings_generator.create_binding_files(chai_hpp, chai_cpp)

    # Generate CMakeLists.txt
    cmake_code = "SET(CYAN_GENERATED_SRC " + " ".join(generated_files) + " PARENT_SCOPE)\n"
    print("Generating CMakeLists.txt subdirectory file")
    with open(os.path.join(args.output, "CMakeLists.txt"), 'w+') as f:
        f.write(cmake_code)

    print("codegen complete (generated {} files):".format(len(generated_files)))
    for i, f in enumerate(generated_files):
        print("    {}\t\"{}\"".format(i + 1, f))


if __name__ == '__main__':
    main()
