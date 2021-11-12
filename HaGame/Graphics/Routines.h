#ifndef ROUTINES
#define ROUTINES

#include "Mesh.h"
#include "Sprite.h"
#include "Window.h"
#include "OpenGL.h"
#include "Primitives.h"
#include "Line.h"
#include "Font.h"
#include "Text.h"
#include "Circle.hpp"
#include "./../Math/NSphere.h"

namespace hagame {
	namespace graphics {

		struct MeshCache {
			Ptr<hagame::graphics::Circle> circle;
		};

		/*void drawMesh(Mesh mesh, ShaderProgram* shader);
		void drawCube(GLuint vao, ShaderProgram* shader, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawTexture(GLuint vao, ShaderProgram* shader, Texture* texture, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawSprite(GLuint vao, ShaderProgram* shader, Sprite sprite);
		void drawParticle(GLuint vao, ShaderProgram* shader, Particle particle);
		*/

		static MeshCache meshCache;
		static Text textBuffer;

		static Ptr<hagame::graphics::Circle> getCircleCache() {
			if (meshCache.circle == NULL) {
				meshCache.circle = std::make_shared<Circle>(1.0f);
			}
			return meshCache.circle;
		}

		void drawLine(Line line, ShaderProgram* shader);
		void drawRect(Rect rect, Color color, ShaderProgram* shader);
		void drawCubeOutline(Cube cube, Color color, ShaderProgram* shader);
		void drawSphereOutline(Vec3 position, float radius, Color color, ShaderProgram* shader);
		
		void drawText(ShaderProgram* shader, Font* font, String message, Color color, Vec3 pos = Vec3::Zero(), float maxLength = 0.0f);
	}
}

#endif