#pragma once
#include <string>

class Texture {
public:
    Texture(const std::string& path, bool flip = true);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    unsigned int id() const { return handle; }

private:
    unsigned int handle = 0;
};
