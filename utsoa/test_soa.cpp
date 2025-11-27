#include "../../common/include/logic.hpp"
#include "../../common/include/scene.hpp"   // Pixel
#include "../../soa/include/image_soa.hpp"  // PixelSOA
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

namespace {

  // ----------------------------
  // 1) Indexación fila-mayor
  // ----------------------------
  TEST(test_soa_index, row_major_formula_is_correct) {
    int const W = 1'920;

    auto idx = [](int f, int c, int W) -> std::size_t {
      return static_cast<std::size_t>(f) * static_cast<std::size_t>(W) +
             static_cast<std::size_t>(c);
    };

    EXPECT_EQ(idx(0, 0, W), 0U);
    EXPECT_EQ(idx(0, 960, W), 960U);
    EXPECT_EQ(idx(1, 0, W), static_cast<std::size_t>(W));
    EXPECT_EQ(idx(10, 50, W), static_cast<std::size_t>(10 * W + 50));

    // Último índice en W=10, H=5 -> 49
    int const w = 10, h = 5;
    std::size_t last = static_cast<std::size_t>(h - 1) * static_cast<std::size_t>(w) +
                       static_cast<std::size_t>(w - 1);
    EXPECT_EQ(last, 49U);
  }

  // ----------------------------------------------
  // 2) SOA: r/g/b se actualizan correctamente
  // ----------------------------------------------
  TEST(test_soa_storage, set_writes_each_channel) {
    std::size_t const N = 1'000;
    render::PixelSOA soa(N);

    render::Pixel p1{255, 10, 0};
    render::Pixel p2{0, 200, 50};

    soa.set(10, p1);
    soa.set(500, p2);

    ASSERT_EQ(soa.r.size(), N);
    ASSERT_EQ(soa.g.size(), N);
    ASSERT_EQ(soa.b.size(), N);

    // Índice 10 == p1
    EXPECT_EQ(soa.r[10], p1.r);
    EXPECT_EQ(soa.g[10], p1.g);
    EXPECT_EQ(soa.b[10], p1.b);

    // Índice 500 == p2
    EXPECT_EQ(soa.r[500], p2.r);
    EXPECT_EQ(soa.g[500], p2.g);
    EXPECT_EQ(soa.b[500], p2.b);

    // Canales/índices distintos tienen valores distintos
    EXPECT_NE(soa.g[10], soa.g[500]);
  }

  // ----------------------------------------------
  // 3) Smoke: cabecera PPM
  // ----------------------------------------------

  using std::filesystem::path;

  TEST(test_soa_ppm, writes_valid_ppm_header) {
    path tmp = "tmp_ppm_header.ppm";
    {
      std::ofstream ofs(tmp);
      ASSERT_TRUE(ofs.is_open());
      render::write_ppm_header(ofs, 3, 2);
    }
    std::ifstream ifs(tmp);
    std::string got(std::istreambuf_iterator<char>(ifs), {});
    EXPECT_EQ(got, "P3\n3 2\n255\n");
    std::filesystem::remove(tmp);
  }

}  // namespace
