#pragma once
#include <string>
#include <algorithm>
#include <vector>

#ifndef COMMON_H
#define COMMON_H

namespace zero {

std::string Lowercase(std::string_view str);
std::vector<std::string_view> Tokenize(std::string_view message, char delim);
std::vector<std::string_view> SplitString(std::string_view string, std::string_view delim);
std::vector<std::string> SplitString(const std::string& string, const std::string& delim);
bool IsDigit(const std::string& str);
bool IsWhitespace(char c);
std::string RemoveWhitespace(const std::string& str);
std::string GetShipName(int ship);
}
#endif