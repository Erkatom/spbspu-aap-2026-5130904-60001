#include <cctype>
#include <cstddef>
#include <iostream>
#include <new>

namespace ignashev {
  char* readLine(std::istream& in);
  size_t getStringLength(const char* str);
  int hasRepeatedDigits(const char* str);
  char* removeVowels(char* dest, const char* src);
}

namespace {
  int isLatinVowel(char ch)
  {
    const unsigned char uch = static_cast< unsigned char >(ch);
    if (!std::isalpha(uch)) {
      return 0;
    }
    const int lower = std::tolower(uch);
    const int isVowel = (lower == 'a') || (lower == 'e') || (lower == 'i')
        || (lower == 'o') || (lower == 'u') || (lower == 'y');
    return isVowel ? 1 : 0;
  }
}

char* ignashev::readLine(std::istream& in)
{
  const size_t initialCapacity = 16;
  const size_t growthFactor = 2;
  const size_t nullTermSpace = 1;
  size_t capacity = initialCapacity;
  size_t length = 0;
  char* buffer = new char[capacity];
  char current = 0;

  in >> std::noskipws;
  while ((in >> current) && (current != '\n')) {
    if (length + nullTermSpace == capacity) {
      const size_t newCapacity = capacity * growthFactor;
      char* newBuffer = nullptr;
      try {
        newBuffer = new char[newCapacity];
      } catch (const std::bad_alloc&) {
        delete[] buffer;
        throw;
      }
      for (size_t i = 0; i < length; ++i) {
        newBuffer[i] = buffer[i];
      }
      delete[] buffer;
      buffer = newBuffer;
      capacity = newCapacity;
    }
    buffer[length] = current;
    ++length;
  }
  in >> std::skipws;

  if ((length == 0) && (!in)) {
    delete[] buffer;
    return nullptr;
  }

  buffer[length] = '\0';
  return buffer;
}

size_t ignashev::getStringLength(const char* str)
{
  size_t length = 0;
  while (str[length] != '\0') {
    ++length;
  }
  return length;
}

int ignashev::hasRepeatedDigits(const char* str)
{
  const size_t digitsCount = 10;
  const int found = 1;
  const int notFound = 0;
  size_t counts[digitsCount] = {};

  for (size_t i = 0; str[i] != '\0'; ++i) {
    const unsigned char uch = static_cast< unsigned char >(str[i]);
    if (std::isdigit(uch)) {
      const size_t digitIndex = static_cast< size_t >(str[i] - '0');
      ++counts[digitIndex];
      if (counts[digitIndex] > found) {
        return found;
      }
    }
  }
  return notFound;
}

char* ignashev::removeVowels(char* dest, const char* src)
{
  size_t destIndex = 0;
  for (size_t i = 0; src[i] != '\0'; ++i) {
    if (isLatinVowel(src[i]) == 0) {
      dest[destIndex] = src[i];
      ++destIndex;
    }
  }
  dest[destIndex] = '\0';
  return dest;
}

int main()
{
  char* input = nullptr;
  try {
    input = ignashev::readLine(std::cin);
  } catch (const std::bad_alloc&) {
    std::cerr << "Memory allocation failed\n";
    return 1;
  }

  if (input == nullptr) {
    std::cerr << "Failed to read input string\n";
    return 1;
  }

  const size_t nullTermSpace = 1;
  const size_t length = ignashev::getStringLength(input);
  char* withoutVowels = nullptr;
  try {
    withoutVowels = new char[length + nullTermSpace];
  } catch (const std::bad_alloc&) {
    delete[] input;
    std::cerr << "Memory allocation failed\n";
    return 1;
  }

  const int repeatedDigits = ignashev::hasRepeatedDigits(input);
  ignashev::removeVowels(withoutVowels, input);

  std::cout << repeatedDigits << '\n';
  std::cout << withoutVowels << '\n';

  delete[] withoutVowels;
  delete[] input;
  return 0;
}
