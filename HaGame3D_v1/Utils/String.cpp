#include "String.h"

Array<String> stringSplit(String str, char delim)
{
	Array<String> parts = {};
	String tmp = "";

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == delim) {
			parts.push_back(tmp);
			tmp = "";
		}
		else {
			tmp += str[i];
		}
	}

	if (tmp.length() > 0) {
		parts.push_back(tmp);
	}

	return parts;
}