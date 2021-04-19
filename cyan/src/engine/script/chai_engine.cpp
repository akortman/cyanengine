#include "chai_engine.hpp"

#include "cyan/src/logging/logger.hpp"
#include "cyan/src/logging/error.hpp"

using namespace cyan;

chaiscript::ChaiScript& ChaiEngine::get_chai_object()
{
    return chai;
}


ChaiEngine::RunReport ChaiEngine::run(const std::string& code)
{
    try {
        chai.eval(code);
    } catch (std::exception& e) {
        LOG(WARN, "(Chai) Exception encountered: {}", e.what());
        return RunReport(false, e.what());
    }
    return RunReport();
}


ChaiEngine::RunReport ChaiEngine::run_file(const std::string& path)
{
    try {
        chai.eval_file(path);
    } catch (std::exception& e) {
        LOG(WARN, "(Chai) Exception: {}", e.what());
        return RunReport(false, e.what());
    }
    return RunReport();
}


void ChaiEngine::RunReport::ensure_ok() const
{
    if (!ok) {
        throw cyan::Error("script ensure_ok failed: {}", what);
    }
}
