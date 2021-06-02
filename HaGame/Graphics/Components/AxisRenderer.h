#ifndef AXIS_RENDERER_H
#define AXIS_RENDERER_H

#include "../Color.h"
#include "../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct AxisRenderer {
			Color xColor = Color::red();
			Color yColor = Color::green();
			Color zColor = Color::blue();

			float axisLength = 1.0f;
			ShaderProgram* shader;
		};
	}
}

#endif