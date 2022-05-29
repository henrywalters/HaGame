#ifndef EXR_H
#define EXR_H

#include "./../Vendor/tinyexr/tinyexr.cc"
#include "./../Utils/Aliases.h"
#include "./../Utils/File.h"
#include "./../Graphics/RawTexture.h"

namespace hagame {
	namespace graphics {
		class EXR {
		public:

			EXR(String path) {
				parse(path);
			}

			void parse(String path) {

				float* data;
				Vec2Int size;

				m_layers.clear();
				m_textures.clear();

				checkError(EXRLayers(path.c_str(), m_layers, &m_error));

				for (auto layer : m_layers) {
					
					auto parts = stringSplit(layer, '.');

					std::cout << "Parsing Layer " << layer << "\n";

					if (parts.size() == 2 && hasElement(xyzLayers, parts[1])) {
						checkError(
							LoadEXRWithLayerXYZ(
								&data, 
								&size[0],
								&size[1],
								path.c_str(), 
								layer.c_str(), 
								&m_error
							)
						);
					}
					else {
						checkError(
							LoadEXRWithLayer(
								&data,
								&size[0],
								&size[1],
								path.c_str(),
								layer.c_str(),
								&m_error
							)
						);
					}

					m_textures.insert(
						std::make_pair(
							layer,
							std::make_shared<RawTexture<GL_RGBA16F>>(size, data)
						)
					);
				}
			}

		private:

			const char* m_error = nullptr;
			Array<String> m_layers;
			Map<String, Ptr<RawTexture<GL_RGBA16F>>> m_textures;
			const Array<String> xyzLayers = {
				"Normal"
			};

			bool checkError(int ret) {
				if (ret != TINYEXR_SUCCESS) {
					if (m_error) {
						fprintf(stderr, "ERR : %s\n", m_error);
						FreeEXRErrorMessage(m_error); // release memory of error message.
						throw new std::exception("TinyEXR Error!");
					}
				}
			}
			

		};
	}
}

#endif
