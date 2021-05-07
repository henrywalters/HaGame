#ifndef IMAGE_H
#define IMAGE_H

#include "../Utils/Aliases.h"
#include "./Color.h"
#include "../Vendor/stb/stb_image.h"

namespace hagame {
	namespace graphics {

		enum ImageType {
			RGBA,
			RGB,
			GA,
			G
		};

		class Image {
		private:

			int getStbiEnum(ImageType type) {
				switch (type) {
				case ImageType::RGBA:
					return STBI_rgb_alpha;
				case ImageType::RGB:
					return STBI_rgb;
				case ImageType::G:
					return STBI_grey;
				case ImageType::GA:
					return STBI_grey_alpha;
				}
			}
			
		public:
			String path;
			Vec2Int size;
			int channels;
			unsigned char* data;
			ImageType type;

			Image(String _path, ImageType _type): path(_path), type(_type) {
				data = stbi_load(path.c_str(), &size[0], &size[1], &channels, getStbiEnum(type));

				if (data == nullptr) {
					std::cout << "Failed to load texture: " << path << std::endl;
					throw new std::exception("Failed to load texture. Check console for more details.");
				}
			}

			~Image() {
				stbi_image_free(data);
			}

			Color getColor(Vec2Int idx) {
				unsigned bytePerPixel = channels;
				unsigned char* p = data + ((idx[0] + size[0] * idx[1]) * bytePerPixel);
				int r = static_cast<int>(p[0]); 
				int g = static_cast<int>(p[1]); 
				int b = static_cast<int>(p[2]);
				
				if (channels == 3) {
					return Color(r, g, b);
				}
				else {
					return Color(r, g, b, static_cast<int>(p[3]));
				}
			}
		};
	}
}

#endif
