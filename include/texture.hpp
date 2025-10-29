#pragma once

#include <string>

class Texture {
private:
    unsigned int id;
    int width, height, channels;
    
public:
    Texture(const std::string& path, bool generateMipmaps = true);
    
    ~Texture();
    
    Texture& operator=(Texture&& other) noexcept;
    
    void bind(unsigned int unit = 0) const;
    
    void unbind() const;
    
    unsigned int getId() const;
    int getWidth() const;
    int getHeight() const;
};