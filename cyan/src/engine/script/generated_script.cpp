#include "generated_script.hpp"


void cyan::chai_add_codegen_generated_library(cyan::ChaiEngine& chai_engine)
{
    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
    generated::make_all_codegen_chai_bindings(m);
    chai_engine.get_chai_object().add(m);
}
