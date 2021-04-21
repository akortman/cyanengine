/**
 * String-related utility functions.
 */

#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <locale>


namespace cyan::string_util {
    /// Get the entire contents of a file at the specified path as a string.
    std::string file_contents(const std::string& path);

    /// Get the entire contents of an input stream as a string.
    extern std::string stream_contents(std::istream& is);

    /// Trim a string of whitespace on the left side only. Operates in-place (will modify the argument).
    std::string& ltrim_inplace(std::string& str);

    /// Trim a string of whitespace on the right side only. Operates in-place (will modify the argument).
    std::string& rtrim_inplace(std::string& str);

    /// Trim both sides of a string of whitespace. Operates in-place (will modify the argument).
    std::string& trim_inplace(std::string & str);

    /// Trim both sides of a string of whitespace. Returns the trimmed string, leaving the argument intact.
    std::string trim(const std::string& str);

    /// Test if a string text starts with the same contents of another string.
    bool startswith(const std::string& text, const std::string& start);

    /// Test if a string text ends with the same contents of another string.
    bool endswith(const std::string& text, const std::string& end);

    /**
     * Replace each instance of a substring within a string with another string.
     * Thanks to https://stackoverflow.com/a/14678852 (user Czarek Tomczak)
     * @param str
     * @param search
     * @param replacement
     */
    void replace_all_inplace(std::string& str, const std::string& search, const std::string& replacement);

    /**
     * Split a string into a number of strings every time the char 'delim' is encountered.
     * String segments are added to a Container object via `push_back`
     * Written by Martin Broadhurst, modified by cyanengine dev
     * http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
     */
    template <class Container>
    void split(const std::string& s, Container& cont, char delim = ' ')
    {
        std::stringstream ss(s);
        std::string token;
        while (std::getline(ss, token, delim)) {
            if (!token.empty()) cont.push_back(token);
        }
    }

    /**
     * Convert from PascalCase (or camelCase) to snake_case.
     * @param name The name of the component (in PascalCase) to convert
     * @return The name in snake_case
     */
    std::string to_snake_case(const char* name);

    /**
     * Given a container of strings, joins the strings together with a delimiter between
     * each string segment.
     * e.g.         join({"a", "b", "cde"}, "::")
     * returns      "a::b::cde"
     */
    template <class Container>
    std::string join(Container& cont, const std::string& delim = "")
    {
        if (cont.empty()) return "";
        std::string str = cont[0];
        for (int i = 1; i < cont.size(); i += 1) {
            str += delim;
            str += cont[i];
        }
        return str;
    }

    /**
     * Given a pair of start and end iterators that contain strings, joins the strings together with a delimiter between
     * each string segment.
     * e.g.         std::vector v{"a", "b", "cde"};
     *              join(v.begin(), v.end(), "::")
     * returns      "a::b::cde"
     */
    template <class Iterator>
    std::string join(Iterator start, Iterator end, const std::string& delim = "")
    {
        if (start == end) return "";
        std::string str = "";
        for (auto i = start; i != end; i += 1) {
            if (i != start) str += delim;
            str += *i;
        }
        return str;
    }
}