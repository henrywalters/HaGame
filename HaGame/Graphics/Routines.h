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
#include "Cone.hpp"
#include "RectPrism.hpp"
#include "./../Math/Surface.h"
#include "./../Math/Capsule.h"
#include "./../Math/Line.h"
#include "./../Math/Ray.h"
#include "./../Math/NSphere.h"

namespace hagame {
	namespace graphics {

		struct MeshCache {
			Ptr<hagame::graphics::Circle> circle;
			Ptr<hagame::graphics::Cone> cone;
			Ptr<hagame::graphics::RectPrism> cube;
			Ptr<hagame::graphics::Line> line;
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

		static Ptr<hagame::graphics::Cone> getConeCache() {
			if (meshCache.cone == NULL) {
				meshCache.cone = std::make_shared<Cone>(1.0f, 1.0f);
			}
			return meshCache.cone;
		}

		static Ptr<hagame::graphics::RectPrism> getCubeCache() {
			if (meshCache.cube == NULL) {
				meshCache.cube = std::make_shared<RectPrism>(Vec3(1.0f));
			}
			return meshCache.cube;
		}

		static Ptr<hagame::graphics::Line> getLineCache() {
			if (meshCache.line == NULL) {
				meshCache.line = std::make_shared<Line>(Vec3::Zero(), Vec3::Top());
			}
			return meshCache.line;
		}

		void drawLine(Line line, Color color, ShaderProgram* shader);
		void drawLine(hagame::math::Line line, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawRect(Rect rect, Color color, ShaderProgram* shader);
		void drawCubeOutline(Cube cube, Color color, ShaderProgram* shader);
		void drawSurface(hagame::math::Surface surface, Mat4 transform, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawSphereOutline(Vec3 position, float radius, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawCupsuleOutline(Vec3 pointA, Vec3 pointB, float radius, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawCircle(Vec3 position, Vec3 axis, float radius, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawTriangle(hagame::math::Triangle tri, Color color, ShaderProgram* shader, float thickness = 0.01f);
		void drawCone(Vec3 position, Vec3 axis, float radius, float height, Color color, ShaderProgram* shader);
		void drawCube(Vec3 position, Vec3 size, Color color, ShaderProgram* shader);
		void drawRay(hagame::math::Ray ray, Color color, ShaderProgram* shader);
		void drawRotationGizmo(Vec3 position, float radius, ShaderProgram* shader, int axis = -1);
		void drawMovementGizmo(Vec3 position, float size, ShaderProgram* shader, int axis = -1);
		void drawScaleGizmo(Vec3 position, float size, ShaderProgram* shader, int axis = -1);
		
		void drawText(ShaderProgram* shader, Font* font, String message, Color color, Vec3 pos = Vec3::Zero(), float maxLength = 0.0f);
	}
}

#endif