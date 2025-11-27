#ifndef PIXEL_SOA_HPP
#define PIXEL_SOA_HPP

#include "../../common/include/scene.hpp"

namespace render {

  class PixelSOA {
  public:
    std::vector<std::uint8_t> r;
    std::vector<std::uint8_t> g;
    std::vector<std::uint8_t> b;

    PixelSOA(size_t size) {
      r.resize(size);
      g.resize(size);
      b.resize(size);
    }

    void set(size_t index, Pixel const & pixel) {
      r[index] = pixel.r;
      g[index] = pixel.g;
      b[index] = pixel.b;
    }
  };

}  // namespace render

#endif
