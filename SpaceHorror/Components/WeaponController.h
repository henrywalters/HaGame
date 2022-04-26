#ifndef WEAPON_CONTROLLER_H
#define WEAPON_CONTROLLER_H

#include "../Common/Weapons.h"

class WeaponController {
public:

	bool playerControlled = false;

	void setWeapon(Weapon weapon, int ammoInClip, int remainingAmmo) {
		m_weapon = weapon;
		m_totalAmmo = clamp(remainingAmmo, 0, m_weapon.totalCapacity);
		m_clipAmmo = clamp(ammoInClip, 0, m_weapon.clipCapacity);
	}

	Weapon getWeapon() { return m_weapon; }

	int getAmmoInClip() { return m_clipAmmo; }

	int getRemainingAmmo() { return m_totalAmmo; }

	void update(double dt) {
		if (!m_canFire) {
			m_timeSinceLastFire += dt;

			if (m_timeSinceLastFire >= m_weapon.fireRate) {
				m_timeSinceLastFire = 0.0;
				m_canFire = true;
			}
		}
	}

	void addAmmo(int amount) {
		int canAdd = m_weapon.totalCapacity - m_totalAmmo;
		m_totalAmmo += clamp<int>(0, amount, canAdd);
	}

	bool reload() {
		int missingInClip = m_weapon.clipCapacity - m_clipAmmo;
		if (m_totalAmmo <= 0) {
			return false;
		}
		else if (m_totalAmmo < missingInClip) {
			int canAdd = missingInClip - m_totalAmmo;
			m_clipAmmo += canAdd;
			m_totalAmmo -= canAdd;
			return true;
		}
		else {
			m_clipAmmo += missingInClip;
			m_totalAmmo -= missingInClip;
			return true;
		}
	}

	bool fire(bool ignoreAmmo = false) {
		if (m_canFire && (ignoreAmmo || m_clipAmmo > 0)) {
			m_canFire = false;
			return true;
		}
		return false;
	}

	hagame::math::Interval<float> getCrosshair(Vec3 velocity) {
		float speed = velocity.magnitude();
		float r = sigmoid(speed) * m_weapon.bullet.spreadRunFactor;
		return hagame::math::Interval<float>(r + m_weapon.bullet.spreadMean, r + r * (m_weapon.bullet.spreadMean + m_weapon.bullet.spreadStdDev));
	}

private:

	Weapon m_weapon;
	bool m_canFire = true;
	double m_timeSinceLastFire = 0.0;

	int m_clipAmmo = 0;
	int m_totalAmmo = 0;
};

#endif
