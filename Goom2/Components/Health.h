#ifndef G_HEALTH_H
#define G_HEALTH_H

#include "./../../HaGame/HaGame.h"

struct Health {
	float maxHealth = 100.0f;
	float health = 100.0f;
	float maxShield = 100.0f;
	float shield = 0.0f;

	bool destroyOnDead = true;

	Ptr<hagame::graphics::VertexBuffer<Vec4>> vbo;
	Ptr<hagame::graphics::VertexArray> vao;

	Health() {
		vbo = hagame::graphics::VertexBuffer<Vec4>::Static(hagame::graphics::QuadVertexVectors);
		vao = std::make_shared<hagame::graphics::VertexArray>();
		vao->initialize();
		vao->defineAttribute<Vec4>(vbo.get(), hagame::graphics::DataType::Float, 0, 4);
	}

	~Health() {
		vbo->clear();
	}

	bool isAlive() {
		return health > 0;
	}

	float percentHealth() {
		return health / maxHealth;
	}

	float percentShield() {
		return shield / maxShield;
	}

	void damage(float dmg) {
		if (dmg < shield) {
			shield -= dmg;
		}
		else if (dmg > shield && shield > 0) {
			dmg -= shield;
			shield = 0.0f;
		}
		else if (dmg < health) {
			health -= dmg;
		}
		else {
			health = 0.0f;
		}
	}

	void addHealth(float amount) {
		health = clamp<float>(health + amount, 0, maxHealth);
	}

	void addShield(float amount) {
		shield = clamp<float>(shield + amount, 0, maxShield);
	}
};

#endif
