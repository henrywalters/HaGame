#include <iostream>
#include <string>

#define TINYEXR_IMPLEMENTATION
#include "./../../HaGame/Vendor/tinyexr/tinyexr.h"
#include "./../../HaGame/Vendor/tinyexr/deps/miniz/miniz.c"
#include "./../../HaGame/Vendor/stb/stb_image_write.cpp"
#include "./../../HaGame/Utils/String.cpp"

const char* EXR_FILE = "0001.exr";

int main() {
	int ret;
	float* out;
	int width;
	int height;
	const char* err = nullptr;

	std::cout << EXR_FILE << "\n";

	std::vector<std::string> layers;

	ret = EXRLayers(EXR_FILE, layers, &err);

	if (ret != TINYEXR_SUCCESS) {
		if (err) {
			fprintf(stderr, "ERR : %s\n", err);
			FreeEXRErrorMessage(err); // release memory of error message.
			return 1;
		}
	}

	for (auto layer : layers) {
		std::cout << layer << "\n";

		//stbi_write_png("Combined.png", width, height, 4, out, width * sizeof(float));
		auto parts = stringSplit(layer, '.');

		if (parts[1] == "Normal") {
			ret = LoadEXRWithLayerXYZ(&out, &width, &height, EXR_FILE, layer.c_str(), &err);
			stbi_write_hdr((parts[0] + "." + parts[1] + ".hdr").c_str(), width, height, 3, out);

		}
		else {
			ret = LoadEXRWithLayer(&out, &width, &height, EXR_FILE, layer.c_str(), &err);
			stbi_write_hdr((parts[0] + "." + parts[1] + ".hdr").c_str(), width, height, 4, out);

		}
		
		//ret = LoadEXR(&out, &width, &height, EXR_FILE, &err);



		
	}


	return 0;
}