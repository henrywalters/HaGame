// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "HaGame3D_v1.h"
#include "SDL.h"

const int MAP[10][10] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 1, 1, 0, 0, 0, 1 },
	{ 1, 0, 0, 1, 1, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 0, 1, 0, 0, 1, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

int main()
{	
	hagame::graphics::Window window = hagame::graphics::Window(hagame::math::Vector<2, uint32_t>({ 800, 800 }), "DEMO");
	auto fs = hagame::utils::FileSystem("../../../HaGame3D_v1/Assets");

	auto vert = fs.readFile("Shaders/simple_vert.glsl");
	auto frag = fs.readFile("Shaders/simple_frag.glsl");

	auto vShader = hagame::graphics::Shader::LoadVertex(vert);
	auto fShader = hagame::graphics::Shader::LoadFragment(frag);

	auto sProgram = hagame::graphics::ShaderProgram(vShader, fShader);

	auto colorShader = hagame::graphics::ShaderProgram(
		hagame::graphics::Shader::LoadVertex(fs.readFile("Shaders/color_vert.glsl")),
		hagame::graphics::Shader::LoadFragment(fs.readFile("Shaders/color_frag.glsl"))
	);

	auto gamepad = new hagame::input::devices::Gamepad(0);
	bool running = true;

	// window.clearColor = hagame::graphics::Color::parseHex("#1ee8a5");

	hagame::graphics::GLMesh glMesh = hagame::graphics::GLMesh::FromMesh(hagame::graphics::Cube);

	GLint vertexPosAttrib = glGetAttribLocation(sProgram.id, "aVertexPos");
	GLint normalAttrib = glGetAttribLocation(sProgram.id, "aNormal");
	GLint texAttrib = glGetAttribLocation(sProgram.id, "aTex");

	auto camera = hagame::graphics::Camera(Vec3UInt32({ 400, 400 }));

	auto perspective = camera.getProjectionMatrix();
	auto view = camera.getViewMatrix();

	std::vector<hagame::Transform> transforms = std::vector<hagame::Transform>();

	std::cout << glMat4ToString(glm::mat4(1.0f)) << std::endl;

	hagame::graphics::Texture texture = hagame::graphics::Texture(fs.getFullPath("Textures/crate.jpg"));

	hagame::Transform light = hagame::Transform();

	light.position = Vec3({ 10.0f, 2.0f, 10.0f });

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (MAP[j][i] == 1) {
				hagame::Transform transform = hagame::Transform();
				transform.grow(Vec3({ 1.0f, 1.0f, 1.0f }));
				transform.move(Vec3({ i * 2.0f, -1.0f, j * 2.0f }));
				transforms.push_back(transform);
			}
		}
	}

	float dt = 0;
	float t = 0;

	window.clearColor = hagame::graphics::Color::blue();

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

		glEnable(GL_DEPTH_TEST);
		window.clear();

		float speed = 5;

		if (gamepad->lAxis.magnitude() > 0.2) {
			camera.position -= speed * dt * camera.direction.normalized() * gamepad->lAxis[1];
		}

		if (gamepad->rAxis.magnitude() > 0.2) {
			camera.xRot += gamepad->rAxis[0] * dt;
			camera.yRot -= gamepad->rAxis[1] * dt;
		}

		// std::cout << "Drawing : " << hagame::graphics::Cube.indices.size() * transforms.size() << " triangles \n";

		t += dt;

		light.position[1] = sin(t) * 5.0f;

		auto vp = camera.getProjectionMatrix() * camera.getViewMatrix();

		glUseProgram(colorShader.id);

		colorShader.setUniformMat4("Model", light.getModelMatrix());
		colorShader.setUniformVec3("Color", glVec3(Vec3({ 1.0, 1.0, 1.0 })));

		colorShader.setUniformMat4("View", camera.getViewMatrix());
		colorShader.setUniformMat4("Projection", camera.getProjectionMatrix());

		glEnableVertexAttribArray(vertexPosAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexBuffer);
		glVertexAttribPointer(vertexPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

		glEnableVertexAttribArray(normalAttrib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.normalBuffer);
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

		glEnableVertexAttribArray(texAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, glMesh.textureBuffer);
		glVertexAttribPointer(texAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec2), NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.indexBuffer);

		glDrawElements(GL_TRIANGLES, hagame::graphics::Cube.indices.size() * 3, GL_UNSIGNED_INT, NULL);

		glDisableVertexAttribArray(vertexPosAttrib);
		glDisableVertexAttribArray(normalAttrib);
		glDisableVertexAttribArray(texAttrib);


		glUseProgram(sProgram.id);
		sProgram.setUniformFloat("AmbientStrength", 0.5);
		sProgram.setUniformFloat("SpecularStrength", 0.8);
		sProgram.setUniformFloat("Shininess", 32);
		sProgram.setUniformVec3("ViewPos", glVec3(camera.position));
		sProgram.setUniformVec3("AmbientColor", glVec3(Vec3({ 1.0, 1.0, 1.0 })));
		sProgram.setUniformVec3("LightColor", glVec3(Vec3({ 1.0, 1.0, 1.0 })));
		sProgram.setUniformVec3("LightPos", glVec3(light.position));
		sProgram.setUniformMat4("View", camera.getViewMatrix());
		sProgram.setUniformMat4("Projection", camera.getProjectionMatrix());

		for (int i = 0; i < transforms.size(); i++) {

			// transforms[i].rotation += dt;
			
			sProgram.setUniformMat4("Model", transforms[i].getModelMatrix());

			glEnableVertexAttribArray(vertexPosAttrib);
			glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexBuffer);
			glVertexAttribPointer(vertexPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

			glEnableVertexAttribArray(normalAttrib);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.normalBuffer);
			glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL);

			glEnableVertexAttribArray(texAttrib);
			glBindBuffer(GL_ARRAY_BUFFER, glMesh.textureBuffer);
			glVertexAttribPointer(texAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec2), NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.indexBuffer);

			glDrawElements(GL_TRIANGLES, hagame::graphics::Cube.indices.size() * 3, GL_UNSIGNED_INT, NULL);

			glDisableVertexAttribArray(vertexPosAttrib);
			//glDisableVertexAttribArray(normalAttrib);
			//glDisableVertexAttribArray(texAttrib);
		}

		window.render();

		dt = (hagame::utils::Clock::Now() - t1) / 1000000.0f;
		auto FPS = 1 / (dt );

		// std::cout << FPS << std::endl;

	}
	return 0;
}
