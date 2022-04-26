#ifndef HEALTH_BAR_RENDERER
#define HEALTH_BAR_RENDERER

#include "Health.h"
#include "./../../HaGame/Graphics/Quad.hpp"
#include "./../../HaGame/Graphics/Color.h"
#include "./../../HaGame/Graphics/ShaderProgram.h"

using namespace hagame::graphics;

class HealthBarRenderer {
public:

	Color color = Color::red();
	ShaderProgram* shader;

	HealthBarRenderer(Vec2 size, float maxHealth = 100) :
		m_size(size),
		m_maxHealth(maxHealth) {
		updateQuad();
	}

	void setHealth(float health) {
		if (health != m_health) {
			m_health = health;
		}
		updateQuad();
	}

	Quad* getQuad() {
		return quad.get();
	}

private:

	Ptr<Quad> quad;
	float m_maxHealth;
	float m_health;
	Vec2 m_size;

	void updateQuad() {
		if (quad == nullptr) {
			quad = std::make_shared<Quad>(m_size);
			quad->setOrigin(Vec2::Zero());
		}
		quad->setSize(Vec2(m_health / m_maxHealth * m_size[0], m_size[1]), false);
	}
};

#endif
