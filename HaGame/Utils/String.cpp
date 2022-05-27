#include "String.h"

bool stringContains(String str, char delim) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == delim)
			return true;
	}
	return false;
}

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

String stringReplace(String str, char replace, String with)
{
	String out = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == replace) {
			for (int j = 0; j < with.size(); j++) {
				out += with[j];
			}
		}
		else {
			out += str[i];
		}
	}
	return out;
}

String stringTrim(String str)
{
	String out = "";
	bool foundNonWhiteSpace = false;
	int lastWhiteSpace = 0;
	int lastNonWhiteSpace = 0;

	for (int i = 0; i < str.size(); i++) {
		if (!foundNonWhiteSpace) {
			if (str[i] != ' ') {
				foundNonWhiteSpace = true;
				lastWhiteSpace = i;
				out += str[i];
			}
		}
		else {
			if (str[i] != ' ') {
				lastNonWhiteSpace = i;
			}
		}
	}

	for (int i = lastWhiteSpace + 1; i <= lastNonWhiteSpace; i++) {
		out += str[i];
	}

	return out;
}

String stringSlice(String str, int start, int end)
{
	String out = "";
	for (int i = start; i < end; i++) {
		out += str[i];
	}
	return out;
}

Array<String> stringPartition(String str, char start, char end)
{
	Array<String> partitions;
	String part = "";
	bool inPart = false;

	for (int i = 0; i < str.size(); i++) {
		auto c = str[i];
		if (c == start) {
			part = "";
			inPart = true;
		}
		else if (c == end && inPart) {
			partitions.push_back(part);
			inPart = false;
		}
		else {
			part += c;
		}
	}

	return partitions;
}
