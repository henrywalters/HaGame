#ifndef ROUTINES
#define ROUTINES
#include "Mesh.h"
#include "Sprite.h"
#include "Particle.h"
#include "Window.h"
#include "OpenGL.h"
#include "Primitives.h"

namespace hagame {
	namespace graphics {

		void drawMesh(GLMesh mesh);
		void drawCube(GLuint vao, Ptr<ShaderProgram> shader, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawTexture(GLuint vao, Ptr<ShaderProgram> shader, Ptr<Texture> texture, hagame::math::Hypercube<3, float> cube, Color color, float rotation = 0.0f);
		void drawSprite(GLuint vao, Ptr<ShaderProgram> shader, Sprite sprite);
		void drawParticle(GLuint vao, Ptr<ShaderProgram> shader, Particle particle);
	}
}

#endif