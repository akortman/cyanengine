#include "string.hpp"

#include "cyan/src/logging/error.hpp"


std::string cyan::string_util::file_contents(const std::string& path)
{
    // Implementation modified from https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream f(path);
    if (!f) {
        throw cyan::Error("file_contents() could not open file at \"{}\" with strerror {}",
                path, std::strerror(errno));
    }
    return stream_contents(f);
}


std::string cyan::string_util::stream_contents(std::istream& is)
{
    std::string str;

    is.seekg(0, std::ios::end);
    str.reserve(is.tellg());
    is.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(is)),
               std::istreambuf_iterator<char>());
    return str;
}



std::string& cyan::string_util::ltrim_inplace(std::string& str)
{
    /**
     * https://stackoverflow.com/a/29892589
     */
    auto it = std::find_if(
            str.begin(),
            str.end(),
            [](char ch) {
                return !std::isspace<char>(ch, std::locale::classic());
            });
    str.erase(str.begin(), it);
    return str;
}


std::string& cyan::string_util::rtrim_inplace(std::string& str)
{
    /**
     * https://stackoverflow.com/a/29892589
     */
    auto it = std::find_if(
            str.rbegin(),
            str.rend(),
            [](char ch) {
                return !std::isspace<char>(ch, std::locale::classic());
            });
    str.erase(it.base(), str.end());
    return str;
}


std::string& cyan::string_util::trim_inplace(std::string& str)
{
    /**
     * https://stackoverflow.com/a/29892589
     */
    return ltrim_inplace(rtrim_inplace(str));
}


std::string cyan::string_util::trim(const std::string& str)
{
    auto s = str;
    return ltrim_inplace(rtrim_inplace(s));
}


bool cyan::string_util::startswith(const std::string& text, const std::string& start)
{
    if (text.size() < start.size()) return false;
    return text.substr(0, start.size()) == start;
}


bool cyan::string_util::endswith(const std::string& text, const std::string& end)
{
    if (text.size() < end.size()) return false;
    return text.substr(text.size() - end.size()) == end;
}


void cyan::string_util::replace_all_inplace(std::string& str, const std::string& search, const std::string& replacement)
{
    if (str.empty() || search.empty()) return;
    // Thanks to https://stackoverflow.com/a/14678852 (user Czarek Tomczak) for solution
    std::size_t pos = 0;
    while ((pos = str.find(search, pos)) != std::string::npos) {
        str.replace(pos, search.length(), replacement);
        pos += replacement.length();
    }
}


std::string cyan::string_util::to_snake_case(const char* name)
{
    std::size_t pos = 0;
    std::string result;
    while (name[pos] != '\0') {
        if (pos == 0 && std::isupper(name[pos])) {
            result += std::tolower(name[pos]);
        } else if (std::isupper(name[pos])) {
            result += "_";
            result += std::tolower(name[pos]);
        } else {
            result += name[pos];
        }

        pos += 1;
    }

    return result;
}


