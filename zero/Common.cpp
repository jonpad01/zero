#include "Common.h"
#include <random>



namespace zero {

std::string Lowercase(std::string_view str) {
  std::string result;

  std::string_view name = str;

  // remove "^" that gets placed on names when biller is down
  if (!name.empty() && name[0] == '^') {
    name = name.substr(1);
  }

  result.resize(name.size());

  std::transform(name.begin(), name.end(), result.begin(), ::tolower);

  return result;
}

// Very simple tokenizer. Doesn't treat quoted strings as one token.
std::vector<std::string_view> Tokenize(std::string_view message, char delim) {
  std::vector<std::string_view> tokens;

  std::size_t pos = 0;

  while ((pos = message.find(delim)) != std::string::npos) {
    // Skip over multiple delims in a row
    if (pos > 0) {
      tokens.push_back(message.substr(0, pos));
    }

    message = message.substr(pos + 1);
  }

  if (!message.empty()) {
    tokens.push_back(message);
  }

  return tokens;
}

std::vector<std::string_view> SplitString(std::string_view string, std::string_view delim) {
  std::vector<std::string_view> result;

  std::size_t offset = 0;
  std::size_t start = 0;

  while ((offset = string.find(delim, offset)) != std::string::npos) {
    std::string_view split = string.substr(start, offset - start);

    result.push_back(split);

    offset += delim.size();
    start = offset;
  }

  result.push_back(string.substr(start));

  return result;
}

std::vector<std::string> SplitString(const std::string& string, const std::string& delim) {
  std::vector<std::string> result;

  std::size_t offset = 0;
  std::size_t start = 0;

  while ((offset = string.find(delim, offset)) != std::string::npos) {
    std::string split = string.substr(start, offset - start);

    result.push_back(split);

    offset += delim.size();
    start = offset;
  }

  result.push_back(string.substr(start));

  return result;
}

bool IsDigit(const std::string& str) {
  bool result = true;
  for (std::size_t i = 0; i < str.size(); i++) {
    if (!std::isdigit(str[i])) {
      result = false;
      break;
    }
  }
  return result;
}

bool IsWhitespace(char c) {
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
    return true;
  }
  return false;
}

std::string RemoveWhitespace(const std::string& str) {
  std::string result;

  for (std::size_t i = 0; i < str.size(); i++) {
    if (!IsWhitespace(str[i])) {
      result.push_back(str[i]);
    }
  }

  return result;
}

std::string GetShipName(int ship) {
  switch (ship) {
    case 0: {
      return "Warbird";
    }
    case 1: {
      return "Javelin";
    }
    case 2: {
      return "Spider";
    }
    case 3: {
      return "Leviathan";
    }
    case 4: {
      return "Terrier";
    }
    case 5: {
      return "Weasel";
    }
    case 6: {
      return "Lancaster";
    }
    case 7: {
      return "Shark";
    }
    case 8: {
      return "Spectator";
    }
    default: {
      return "";
    }
  }
  return "";
}

unsigned int GetRandomNumber(unsigned int min, unsigned int max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(min, max);

  return num(rng);
}

}  // namespace zero