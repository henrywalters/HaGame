#ifndef ROUTINES
#define ROUTINES
#include "Mesh.h"
#include "Sprite.h"
#include "Particle.h"
#include "Window.h"
#include "OpenGL.h"
#include "Primitives.h"
#include "Line.h"

namespace hagame {
	namespace graphics {

		/*void drawMesh(Mesh mesh, ShaderProgram* shader);
		void drawCube(GLuint vao, ShaderProgram* shader, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawTexture(GLuint vao, ShaderProgram* shader, Texture* texture, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawSprite(GLuint vao, ShaderProgram* shader, Sprite sprite);
		void drawParticle(GLuint vao, ShaderProgram* shader, Particle particle);
		*/
		void drawLine(Line line, ShaderProgram* shader);
		void drawCubeOutline(Cube cube, Color color, ShaderProgram* shader);
	}
}

#endif