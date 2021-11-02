#ifndef UUID_H
#define UUID_H

#include <random>

namespace hagame {
	namespace utils {

		static std::random_device device;
		static std::mt19937_64 engine(device());
		static std::uniform_int_distribution<uint64_t> uniDist;

		class UUID {
		public:

			UUID() : uuid(uniDist(engine)) {}
			UUID(uint64_t _uuid) : uuid(_uuid) {}
			UUID(const UUID& _uuid) = default;

			operator uint64_t() const {
				return uuid;
			}

			bool operator ==(const UUID& _uuid) {
				return uuid == _uuid;
			}

			bool operator !=(const UUID& _uuid) {
				return uuid != _uuid;
			}

		private:

			uint64_t uuid;
		};
	}
}

namespace std {
	template <>
	struct hash<hagame::utils::UUID>
	{
		std::size_t operator()(const hagame::utils::UUID& uuid) const {
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

	ostream& operator<<(ostream& os, const hagame::utils::UUID& uuid) {
		os << (uint64_t) uuid;
		return os;
	}
}

#endif