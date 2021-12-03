#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Quad.hpp"
#include "Disc.hpp"
#include "Color.h"
#include "../Utils/Random.h"
#include "./../Utils/Aliases.h"
#include "../Vendor/imgui/imgui.h"
#include "../Utils/Timer.h"

namespace hagame {
	namespace graphics {

		const int MAX_PARTICLE_COUNT = 10000;

		struct Particle {
			Vec3 pos;
			Vec2 size;
			Vec4 color;
			float speed;
			Vec3 dir;
			double createdAt;
			float aliveFor;
		};

		struct EmitterSettings {
			// If this is set to true, the emitter will never stop
			bool infiniteLifeTime = true;

			// If infiniteLife is set to false, the emitter will emit for this long
			float lifeTime = 1.0f;

			// The delay between each emission
			float delay = 0.016f;

			// How many particles to emit per emission
			int particlesEmitted = 1;

			float angle = PI / 4;

			float minSpeed = 0.1f;
			float maxSpeed = 0.3f;

			float minSize = 0.01;
			float maxSize = 0.05;

			float minLife = 0.1f;
			float maxLife = 0.5f;

			Color startColor = Color(1.0f, 0.0f, 0.0f, 0.5f);
			Color endColor = Color(1.0f, 1.0f, 0.0f, 0.25f);

			Vec3 accel = Vec3({0, 0.01f, 0});
		};

		class ParticleEmitter {
			Ptr<VertexBuffer<Particle>> particleBuffer;
			Ptr<Quad> quad;
			Ptr<Disc> disc;
			Array<Particle> particles;
			utils::Random rand;

			double lastEmission = 0.0;

			double sortTime = 0.0;
			double updateTime = 0.0;
			double renderTime = 0.0;

			void initializeBuffers() {
				particleBuffer = VertexBuffer<Particle>::Dynamic(MAX_PARTICLE_COUNT);
				//quad = std::make_shared<Quad>(1.0f, 1.0f);
				//auto vao = quad->getMesh()->getVAO();
				disc = std::make_shared<Disc>(1.0f);
				auto vao = disc->getMesh()->getVAO();
				particleBuffer->bind();
				vao->bind();
				vao->defineAttribute(particleBuffer.get(), DataType::Float, 3, 3, offsetof(Particle, pos));
				vao->defineAttribute(particleBuffer.get(), DataType::Float, 4, 2, offsetof(Particle, size));
				vao->defineAttribute(particleBuffer.get(), DataType::Float, 5, 4, offsetof(Particle, color));
				vao->setInstanced(3);
				vao->setInstanced(4);
				vao->setInstanced(5);
			}

			void updateBuffers() {
				clearBuffers();
				particleBuffer->update(0, particles);
			}

			void clearBuffers() {
				particleBuffer->clear();
			}

			Vec3 getRandomDir() {
				auto r1 = Quat(rand.real(-settings.angle / 2.0f, settings.angle / 2.0f), Vec3::Right());
				auto r2 = Quat(rand.real(-settings.angle / 2.0f, settings.angle / 2.0f), Vec3::Face());

				Vec3 dir = Vec3::Top();
				dir = r1.rotatePoint(dir);
				dir = r2.rotatePoint(dir);

				return dir;
			}

		public:

			EmitterSettings settings;

			ParticleEmitter() {
				initializeBuffers();
				Particle p, p2, p3;
				p.pos = Vec3::Zero();
				p.color = Color::blue();
				p.size = Vec2(0.25f);
				particles.push_back(p);

				p2.pos = Vec3(1.0f);
				p2.color = Color::red();
				p2.size = Vec2(0.25f);
				particles.push_back(p2);

				p3.pos = Vec3(2.5f);
				p3.color = Color::white();
				p3.size = Vec2(0.25f);
				particles.push_back(p3);

				updateBuffers();
			}

			void update(double elapsedTime) {

				if (elapsedTime - lastEmission >= settings.delay) {
					lastEmission = elapsedTime;

					for (int i = 0; i < settings.particlesEmitted; i++) {
						Particle p;
						p.color = Color(255, 0, 0, 0.25f);
						p.size = Vec2(rand.real(settings.minSize, settings.maxSize));
						p.pos = Vec3::Zero();
						p.dir = getRandomDir();
						p.speed = rand.real(settings.minSpeed, settings.maxSpeed);
						p.createdAt = elapsedTime;
						p.aliveFor = rand.real(settings.minLife, settings.maxLife);
						particles.push_back(p);
					}
				}

				Array<Particle> alive;

				utils::Timer timer;

				for (auto particle : particles) {

					if (elapsedTime - particle.createdAt < particle.aliveFor) {
						float T = (elapsedTime - particle.createdAt);
						float t = T / particle.aliveFor;
						particle.pos = (particle.dir * T * particle.speed) + settings.accel * T * T;
						particle.color = Color::Lerp(settings.startColor, settings.endColor, t);
						// particle.color = Color::white();
						alive.push_back(particle);
					}
				}

				particles = alive;

				updateTime = timer.elapsed();
			}

			void sortByDistance(Vec3 emitterPos, Vec3 cameraPos) {
				utils::Timer timer;
				auto offset = emitterPos - cameraPos;
				std::sort(particles.begin(), particles.end(), [this, offset](Particle a, Particle b) -> bool {
					return (a.pos + offset).magnitude() > (b.pos + offset).magnitude();
				});
				sortTime = timer.elapsed();
			}

			void drawUI() {
				ImGui::Begin("Emitter Settings");

				ImGui::Text(("Triangle count: " + std::to_string(particles.size() * disc->getDivisions())).c_str());
				ImGui::Text(("Sort time: " + std::to_string(sortTime)).c_str());
				ImGui::Text(("Update time: " + std::to_string(updateTime)).c_str());
				ImGui::Text(("Render time: " + std::to_string(renderTime)).c_str());

				ImGui::ColorPicker4("Start Color", settings.startColor.vector);
				ImGui::ColorPicker4("End Color", settings.endColor.vector);
				ImGui::DragFloat("Min speed", &settings.minSpeed, 0.01f, 0.0f, 5.0f);
				ImGui::DragFloat("Max speed", &settings.maxSpeed, 0.01f, 0.0f, 5.0f);
				ImGui::DragInt("Particles", &settings.particlesEmitted, 1, 0, 1000);
				ImGui::DragFloat("Delay", &settings.delay, 0.001, 0.0f, 1.0f);
				ImGui::DragFloat3("Acceleration", settings.accel.vector, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Angle", &settings.angle, 0.1f, 0.0f, PI * 2);
				ImGui::DragFloat("Min particle life", &settings.minLife, 0.01f, 0.0f, 5.0f);
				ImGui::DragFloat("Max particle life", &settings.maxLife, 0.01f, 0.0f, 5.0f);
				ImGui::DragFloat("Min particle Size", &settings.minSize, 0.00001f, 0.0f, 0.1f, "%.6f");
				ImGui::DragFloat("Max particle Size", &settings.maxSize, 0.00001f, 0.0f, 0.1f, "%.6f");

				ImGui::End();
			}

			void draw() {
				utils::Timer timer;
				updateBuffers();
				// quad->getMesh()->getVAO()->bind();
				disc->getMesh()->getVAO()->bind();
				glDrawArraysInstanced(GL_TRIANGLES, 0, disc->getDivisions() * 3, MAX_PARTICLE_COUNT);
				renderTime = timer.elapsed();
			}
		};
	}
}

#endif
