// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "HaGame3D_v1.h"
#include "SDL.h"

int main()
{	
	hagame::graphics::Window window = hagame::graphics::Window(hagame::math::Vector<2, uint32_t>({ 800, 800 }), "DEMO");
	auto fs = hagame::utils::FileSystem("../../../HaGame3D_v1/Assets");

	auto vert = fs.readFile("Shaders/simple_vert.glsl");
	auto frag = fs.readFile("Shaders/simple_Frag.glsl");

	auto vShader = hagame::graphics::Shader::LoadVertex(vert);
	auto fShader = hagame::graphics::Shader::LoadFragment(frag);

	auto sProgram = hagame::graphics::ShaderProgram(vShader, fShader);

	auto gamepad = new hagame::input::devices::Gamepad(0);
	bool running = true;

	// window.clearColor = hagame::graphics::Color::parseHex("#1ee8a5");

	auto flattened = hagame::graphics::flattenVectorArray<3, float>(hagame::graphics::Cube.vertices).data();

	for (int i = 0; i < 12; i++) {
		std::cout << flattened[i] << std::endl;
	}

	hagame::graphics::GLMesh glMesh = hagame::graphics::GLMesh::FromMesh(hagame::graphics::Cube);

	GLint vertexPosAttrib = glGetAttribLocation(sProgram.id, "aVertexPos");
	GLint normalAttrib = glGetAttribLocation(sProgram.id, "aNormal");

	auto camera = hagame::graphics::Camera(Vec3UInt32({ 400, 400 }));

	auto perspective = camera.getProjectionMatrix();
	auto view = camera.getViewMatrix();

	std::vector<hagame::Transform> transforms = std::vector<hagame::Transform>();

	std::cout << glMat4ToString(glm::mat4(1.0f)) << std::endl;

	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			hagame::Transform transform = hagame::Transform();
			transform.move(Vec3({ i * 3.0f, -1.0f, j * 3.0f }) );
			transforms.push_back(transform);
		}
	}

	float dt = 0;

	while (running) {

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		auto t1 = hagame::utils::Clock::Now();

		gamepad->pollDevice();
		if (gamepad->startPressed) {
			std::cout << "Start" << std::endl;
			running = false;
		}

		if (gamepad->homePressed) {
			running = false;
		}

		window.clear();

		if (gamepad->lAxis.magnitude() > 0.2) {
			camera.position -= dt * Vec3({ gamepad->lAxis[0], 0.0f, gamepad->lAxis[1] });
		}

		std::cout << "Drawing : " << hagame::graphics::Cube.indices.size() * transforms.size() << " triangles \n";

		

		auto vp = camera.getProjectionMatrix() * camera.getViewMatrix();

		for (int i = 0; i < transforms.size(); i++) {

			glUseProgram(sProgram.id);

			auto mvpLoc = camera.getMVPUniformLocation(sProgram);

			auto mvp = vp* transforms[i].getModelMatrix();

			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

			glEnableVertexAttribArray(vertexPosAttrib);
			glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexBuffer);
			glVertexAttribPointer(vertexPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

			glEnableVertexAttribArray(normalAttrib);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.normalBuffer);
			glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.indexBuffer);

			glDrawElements(GL_TRIANGLES, hagame::graphics::Cube.indices.size() * 3, GL_UNSIGNED_INT, NULL);

			glDisableVertexAttribArray(vertexPosAttrib);
		}

		window.render();

		dt = (hagame::utils::Clock::Now() - t1) / 1000000.0f;
		auto FPS = 1 / (dt );

		std::cout << FPS << std::endl;

	}
	return 0;
}
