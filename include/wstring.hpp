#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <wchar.h>
#include <vector>
#include <regex>
//wstring=>string
inline std::wstring String2WString(const std::string& input)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(input);
}
// convert wstring to string 
inline std::string WString2String(const std::wstring& input)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(input);
}
std::vector<std::string> stringSplit(const std::string& str, char delim) {
    std::string s;
    s.append(1, delim);
    std::regex reg(s);
    std::vector<std::string> elems(std::sregex_token_iterator(str.begin(), str.end(), reg, -1),
        std::sregex_token_iterator());
    return elems;
}