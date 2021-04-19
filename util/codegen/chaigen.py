"""
Generation of chai bindings for codegen utility.
"""

import os
from common import copyable_types, codegen_initial_comment, prelude, file_is_generated
from pprint import pprint


class ChaiBindingsGenerator:
    """
    Manager for generating chaiscript bindings for codegen templates.
    """
    def __init__(self, namespace=""):
        """
        Create a ChaiBindingsGenerator.
        :param namespace: The namespace to place the binding generation functions in, e.g. "cyan::generated"
        """
        self.namespace = namespace
        if namespace != "":
            self.namespace_prefix = namespace + "::"
        else:
            self.namespace_prefix = ""
        self.bindings = {}

    def add_chai_bindings(self, target_path, codegen_template):
        """
        Add chai binding information for the given template file and it's location.
        :param target_path: The path to the file to be created.
        :param codegen_template: The JSON template file (already opened)
        :return: None
        """
        try:
            namespace_prefix = "{}::".format(codegen_template["namespace"])
        except KeyError:
            namespace_prefix = ""
        ns_struct_name = namespace_prefix + codegen_template["name"]

        # Generate a binding for each data member.
        member_binding_template = "{{ chaiscript::fun(&{struct_name}::{member_name}), \"{member_name}\" }}"
        members = []
        for m in codegen_template["data"]:
            member_binding = member_binding_template.format(
                struct_name=ns_struct_name,
                member_name=m["name"],
            )
            members.append(member_binding)
        members = ",\n".join(members)

        bindings_template = "    chaiscript::utility::add_class<{ns_struct_name}>(\n" \
                            "        *m,\n" \
                            "        \"{struct_name}Component\",\n" \
                            "        {{ chaiscript::constructor<{ns_struct_name}()>(),\n" \
                            "          chaiscript::constructor<{ns_struct_name}({member_ctor_args})>()\n" \
                            "        }},\n" \
                            "        {{\n" \
                            "            {members}\n" \
                            "        }}\n" \
                            "    );"

        member_ctor_args = ", ".join(
            ("{}" if m["type"] in copyable_types else "const {}&").format(m["type"])
            for m in codegen_template["data"])

        code_body = bindings_template.format(
            ns_struct_name=ns_struct_name,
            struct_name=codegen_template["name"],
            member_ctor_args=member_ctor_args,
            members=members)

        function_decl = "void make_{struct_name}_chai_bindings(chaiscript::ModulePtr m);".format(
            struct_name=codegen_template["name"])
        function_body = "void {namespace_prefix}make_{struct_name}_chai_bindings(chaiscript::ModulePtr m) {{\n" \
                        "    {code_body}\n" \
                        "}}\n"
        function_body = function_body.format(
            namespace_prefix=self.namespace_prefix,
            struct_name=codegen_template["name"],
            code_body=code_body)

        self.bindings[codegen_template["name"]] = {
            "file_path": target_path,
            "function_decl": function_decl,
            "function_body": function_body
        }

    def make_chai_binding_combined_fn(self):
        """
        Make a function declaration and body. The function will call all (already generated) binding generator functions
        for codegen templates.
        :return: A tuple of strings (decl, fn), where decl is the function declaration and fn is the function body
        """
        combined_decl = "void make_all_codegen_chai_bindings(chaiscript::ModulePtr m);"
        combined_fn = "void {}make_all_codegen_chai_bindings(chaiscript::ModulePtr m) {{\n".format(
            self.namespace_prefix)
        for name, _ in self.bindings.items():
            combined_fn += "    make_{}_chai_bindings(m);\n".format(name)
        combined_fn += "}"
        return combined_decl, combined_fn

    def create_binding_files(self, hpp_path, cpp_path):
        """
        Create all chaiscript binding files to generate bindings for templates added with add_chai_bindings.
        :param hpp_path: The path to the header file to generate
        :param cpp_path: The path to the C++ source file to generate
        :return: None
        """
        hpp_includes = "#include \"chaiscript/chaiscript.hpp\"\n"
        hpp_includes = hpp_includes + "".join(
            "#include \"{}\"\n".format(b["file_path"]) for _, b in self.bindings.items())
        cpp_includes = "#include \"{}\"\n\n".format(os.path.basename(hpp_path))
        cpp_includes += "".join("#include \"{}\"\n".format(b["file_path"]) for _, b in self.bindings.items())

        combined_decl, combined_fn = self.make_chai_binding_combined_fn()

        declarations = "\n".join(b["function_decl"] for _, b in self.bindings.items())
        declarations += "\n" + combined_decl
        function_bodies = "\n".join(b["function_body"] for _, b in self.bindings.items())
        function_bodies += "\n" + combined_fn

        if self.namespace != "":
            namespace_start_code = "\n\nnamespace {} {{\n".format(self.namespace)
            namespace_end_code = "\n}\n"
        else:
            namespace_start_code = ""
            namespace_end_code = ""

        header_code = (
            prelude.format(codegen_initial_comment)
            + hpp_includes
            + namespace_start_code
            + declarations
            + namespace_end_code)

        source_code = (
            codegen_initial_comment + "\n"
            + cpp_includes
            + function_bodies
        )

        if file_is_generated(hpp_path):
            with open(hpp_path, 'w+') as f:
                f.write(header_code)
        else:
            print(("ERROR: Cannot create chaigen header at \"{}\" because the file already exists and does not " +
                   "start with '{}'").format(hpp_path, codegen_initial_comment))

        if file_is_generated(cpp_path):
            with open(cpp_path, 'w+') as f:
                f.write(source_code)
        else:
            print(("ERROR: Cannot create chaigen source at \"{}\" because the file already exists and does not " +
                   "start with '{}'").format(cpp_path, codegen_initial_comment))
