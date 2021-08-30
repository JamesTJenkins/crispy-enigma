#include "Materials.h"
#include <fstream>

namespace Data {
    std::vector<char> Shader::ReadBinaryFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()){
			throw std::runtime_error("Failed to read file.");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}