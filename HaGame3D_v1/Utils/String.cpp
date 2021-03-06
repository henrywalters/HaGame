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

String stringJoin(Array<String> strs, String delim)
{
	String out = "";
	int size = strs.size();
	for (int i = 0; i < size; i++) {
		out += strs[i];
		if (i < size - 1) {
			out += delim;
		}
	}
	return out;
}
