#pragma once 

#include <vector>

class Drawable;

class Renderer {
private:
  std::vector<const Drawable*> drawables;

public:
  void add(const Drawable& d);

  void drawAll() const;
};