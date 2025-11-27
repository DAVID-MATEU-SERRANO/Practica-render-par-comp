#include "../../common/include/scene.hpp"
#include "../aos/include/image_aos.hpp"
#include <cstddef>
#include <gtest/gtest.h>

using namespace render;

TEST(pixel_aos, constructs_with_given_size) {
  PixelAOS const p0(0);
  EXPECT_EQ(p0.pixels.size(), 0U);

  PixelAOS const p5(5);
  EXPECT_EQ(p5.pixels.size(), 5U);
}

TEST(pixel_aos, set_writes_pixel_at_index) {
  PixelAOS p(3);
  Pixel const red{255, 0, 0};
  Pixel const green{0, 255, 0};
  Pixel const blue{0, 0, 255};

  p.set(0, red);
  p.set(1, green);
  p.set(2, blue);

  ASSERT_EQ(p.pixels.size(), 3U);
  EXPECT_EQ(p.pixels[0].r, 255);
  EXPECT_EQ(p.pixels[0].g, 0);
  EXPECT_EQ(p.pixels[0].b, 0);

  EXPECT_EQ(p.pixels[1].r, 0);
  EXPECT_EQ(p.pixels[1].g, 255);
  EXPECT_EQ(p.pixels[1].b, 0);

  EXPECT_EQ(p.pixels[2].r, 0);
  EXPECT_EQ(p.pixels[2].g, 0);
  EXPECT_EQ(p.pixels[2].b, 255);
}

namespace {

  Pixel make_pixel(int f, int c) {
    return Pixel{static_cast<unsigned char>(10 * f), static_cast<unsigned char>(20 * c), 255};
  }

  void expect_pixel(PixelAOS const & p, std::size_t idx, Pixel const & expected) {
    EXPECT_EQ(p.pixels[idx].r, expected.r);
    EXPECT_EQ(p.pixels[idx].g, expected.g);
    EXPECT_EQ(p.pixels[idx].b, expected.b);
  }

}  // namespace

TEST(pixel_aos, row_major_indexing_matches_main_formula) {
  std::size_t const width  = 3;
  std::size_t const height = 2;
  std::size_t const total  = width * height;

  PixelAOS p(total);

  // Inicializamos todos los pixels
  for (std::size_t f = 0; f < height; ++f) {
    for (std::size_t c = 0; c < width; ++c) {
      p.set(f * width + c, make_pixel(static_cast<int>(f), static_cast<int>(c)));
    }
  }

  // Verificación automática de los pixels clave
  expect_pixel(p, 0, make_pixel(0, 0));
  expect_pixel(p, 2, make_pixel(0, 2));
  expect_pixel(p, 3, make_pixel(1, 0));
  expect_pixel(p, 5, make_pixel(1, 2));
}
