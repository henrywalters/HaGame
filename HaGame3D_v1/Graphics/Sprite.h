#ifndef SPRITE
#define SPRITE
#include "ShaderProgram.h"
#include "Texture.h"
#include "Color.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		class Sprite {
		public:
			Ptr<Texture> texture;
			Rect rect;
			// The position of the origin relative to the sprites position <x, y>. Rotations will occur about the origin.
			Vec2 origin;
			float rotation;
			Color color;
			float zIndex;

			Sprite(Ptr<Texture> _texture, Rect _rect = Rect::Zero(), float _rot = 0, Color _color = Color(1.0f, 1.0f, 1.0f, 1.0f), float _zIndex = 0.0f) : 
				texture(_texture), 
				rect(_rect),
				rotation(_rot), 
				color(_color),
				zIndex(_zIndex)
			{
				origin = rect.size * 0.5f;
			}

			Vec3 getPosition() {
				return Vec3({rect.pos[0], rect.pos[1], zIndex});
			}

			Vec3 getSize() {
				return Vec3({ rect.size[0], rect.size[1], 0.0f });
			}

			Vec3 getOrigin() {
				return Vec3({ origin[0], origin[1], 0.0f });
			}
		};
	}
}

#endif