#ifndef STRING
#define STRING
#include "Aliases.h"

// Split a string into several elements based on a delimeter
Array<String> stringSplit(String str, char delim);

// Join a string combining them with a delimeter
String stringJoin(Array<String> strs, String delim);

// Replace all occurances of a character with a given string
String stringReplace(String str, char replace, String with);

// Strip all leading and trailing whitespace
String stringTrim(String str);

// Extract all characters of str from start to end exclusive
String stringSlice(String str, int start, int end);

// Partition a string into sub strings, wrapped in the start and end char
Array<String> stringPartition(String str, char start, char end);

#endif