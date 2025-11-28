#ifndef PIXEL_AOS_HPP
#define PIXEL_AOS_HPP

#include "../../common/include/scene.hpp"

namespace render {

  class PixelAOS {
  public:
    std::vector<Pixel> pixels;

    PixelAOS(size_t size) : pixels(size) { }

    void set(size_t index, Pixel const & pixel) { pixels[index] = pixel; }
  };

}  // namespace render

#endif
