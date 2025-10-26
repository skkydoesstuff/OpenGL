#include "utils.hpp"

#include <string>
#include <fstream>
#include <iostream>

std::string readFileIntoString(std::string filepath) {
	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filepath << std::endl;
	}

	std::string fileContent(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

	file.close();
	
	return fileContent;
}
