#include "chai_engine.hpp"

#include "cyan/src/logging/logger.hpp"

using namespace cyan;

chaiscript::ChaiScript& ChaiEngine::get_chai_object()
{
    return chai;
}


void ChaiEngine::run(const std::string& code)
{
    try {
        chai.eval(code);
    } catch (std::exception& e) {
        LOG(WARN, "(Chai) Exception encountered: {}", e.what());
    }
}


void ChaiEngine::run_file(const std::string& path)
{
    try {
        chai.eval_file(path);
    } catch (std::exception& e) {
        LOG(WARN, "(Chai) Exception: {}", e.what());
    }
}
