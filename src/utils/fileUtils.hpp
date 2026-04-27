#include <string>
#include <vector>

struct MeshStructure {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

std::string getExecutableDirectory();
std::string readFile(const std::string& path);
MeshStructure readObjFile(const std::string& name);