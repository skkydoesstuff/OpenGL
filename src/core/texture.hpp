#pragma once

#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;

private:
    unsigned int id;
};