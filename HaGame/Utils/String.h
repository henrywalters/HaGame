#ifndef STRING
#define STRING
#include "Aliases.h"

Array<String> stringSplit(String str, char delim);
String stringJoin(Array<String> strs, String delim);
String stringReplace(String str, char replace, String with);

#endif