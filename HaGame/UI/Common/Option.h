#ifndef UI_OPTION_H
#define UI_OPTION_H

#include <string>

namespace hagame {
	namespace ui {
		struct Option {
			std::string label;
			std::string value;

			Option() :
				label(""),
				value("")
			{}

			Option(std::string label) :
				label(label),
				value(label)
			{}

			Option(std::string label, std::string value):
				label(label),
				value(value)
			{}
		};
	}
}

#endif
