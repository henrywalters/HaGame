#ifndef DRAWING_SYSTEM_H
#define DRAWING_SYSTEM_H

#include "./../../HaGame/HaGame.h"


struct Stroke {

	unsigned int VAO, VBO;
	int quadCount = 0;
	float size;
	Array<Vec2> points;
	Rect boundingBox;

	Stroke(float _size) : size(_size) {
		points = Array<Vec2>();
	}

	~Stroke() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, quadCount * 6);
		glBindVertexArray(0);
	}
};

struct DynamicStroke {

	const int MAX_QUAD_COUNT = 1000000;

	unsigned int VAO, VBO;
	int quadCount = 0;
	float size;
	Array<Vec2> points;
	Array<Vec2> data;
	Rect boundingBox;

	DynamicStroke(float _size) : size(_size) {
		points = Array<Vec2>();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * MAX_QUAD_COUNT, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	~DynamicStroke() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void addPoint(Vec2 point) {
		points.push_back(point);
		int pointCount = points.size() - 1;
		if (pointCount > 2) {
			Vec2 delta = points[pointCount] - points[pointCount - 2];
			
			float steps = (delta.magnitude() / size) * 2;
			Vec2 midpoint = (points[pointCount] + points[pointCount - 1]) * 0.5f;
			int iterations = ceil(steps);
			Array<Vec2> pointData;
			for (int j = 0; j < ceil(steps); j++) {

				Vec2 smoothPoint = bezier(points[pointCount - 2], midpoint, points[pointCount - 1], j / (float)iterations);

				Vec2 a = smoothPoint + Vec2({ -size / 2, -size / 2 });
				Vec2 b = smoothPoint + Vec2({ size / 2, -size / 2 });
				Vec2 c = smoothPoint + Vec2({ size / 2, size / 2 });
				Vec2 d = smoothPoint + Vec2({ -size / 2, size / 2 });

				pointData.insert(pointData.end(), {
					a, b, c,
					a, c, d
					});

			}

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, quadCount * sizeof(Vec2), sizeof(Vec2) * pointData.size(), &pointData[0]);

			quadCount += pointData.size();
		}
	}

	void clear() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glClearBufferData(GL_ARRAY_BUFFER, GL_RG32F, GL_RG32F, GL_FLOAT, NULL);

		quadCount = 0;
		points.clear();
		data.clear();
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, quadCount * 6);
		glBindVertexArray(0);
	}

	void copyToStroke(Stroke* stroke) {
		stroke->quadCount = quadCount;
		stroke->points = points;
		glGenVertexArrays(1, &stroke->VAO);
		glGenBuffers(1, &stroke->VBO);
		glBindVertexArray(stroke->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, stroke->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * quadCount, NULL, GL_STATIC_DRAW);

		glBindBuffer(GL_COPY_READ_BUFFER, VBO);
		glBindBuffer(GL_COPY_WRITE_BUFFER, stroke->VBO);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(Vec2) * quadCount);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class DrawingSystem: public hagame::ecs::System {

public:

	Array<Ptr<Stroke>> strokes;
	bool drawing = false;
	//Ptr<Stroke> currentStroke;
	Ptr<DynamicStroke> currentStroke;
	float strokeSize = 3.0f;

	String getSystemName() {
		return "DrawingSystem";
	}

	void onSystemStart() {
		strokes = Array<Ptr<Stroke>>();
		drawing = game->input.keyboardMouse.mouse.left;
		currentStroke = std::make_shared<DynamicStroke>(strokeSize);
	}

	void handleStrokes() {
		Vec2 mousePos = game->input.keyboardMouse.mouse.position;
		mousePos[1] = game->window->size[1] - mousePos[1];

		if (game->input.keyboardMouse.mouse.left && !drawing) {
			//currentStroke->clear();
			drawing = true;
		}

		if (!game->input.keyboardMouse.mouse.left && drawing) {
			drawing = false;
			auto stroke = std::make_shared<Stroke>(currentStroke->size);
			currentStroke->copyToStroke(stroke.get());
			stroke->boundingBox = currentStroke->boundingBox;
			currentStroke->clear();

			strokes.push_back(stroke);
		}

		if (drawing) {
			for (int i = 0; i < 2; i++) {

				if (currentStroke->points.size() == 0) {
					currentStroke->boundingBox = Rect(mousePos, Vec2::Zero());
				}

				if (mousePos[i] < currentStroke->boundingBox.pos[i]) {
					currentStroke->boundingBox.size[i] += currentStroke->boundingBox.pos[i] - mousePos[i];
					currentStroke->boundingBox.pos[i] = mousePos[i];
					std::cout << "Less than bottom\n";
				}
					
				if (mousePos[i] > currentStroke->boundingBox.pos[i] + currentStroke->boundingBox.size[i]) {
					std::cout << "Greater than top\n";
					currentStroke->boundingBox.size[i] = mousePos[i] - currentStroke->boundingBox.pos[i];
				}
					
			}
			currentStroke->addPoint(mousePos);
		}
	}

	void drawStrokes() {
		auto orth = Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1);
		auto shader = game->resources->getShaderProgram("color");
		auto drawingShader = game->resources->getShaderProgram("drawing");

		if (currentStroke != nullptr) {
			drawingShader->use();
			drawingShader->setMat4("projection", orth);
			drawingShader->setVec4("color", hagame::graphics::Color::green());
			currentStroke->draw();
		}
		
		
		for (auto stroke : strokes) {
			shader->use();
			shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth);
			hagame::graphics::drawRect(stroke->boundingBox, hagame::graphics::Color::blue(), shader);
			drawingShader->use();
			drawingShader->setMat4("projection", orth);
			drawingShader->setVec4("color", hagame::graphics::Color::green());
			stroke->draw();
		}
	}

	void onSystemUpdate(double dt) {
		handleStrokes();
		drawStrokes();
	}
};

#endif
