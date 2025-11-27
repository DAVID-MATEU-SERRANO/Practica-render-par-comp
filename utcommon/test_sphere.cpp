#include "../common/include/color.hpp"
#include "../common/include/matte.hpp"
#include "../common/include/point.hpp"
#include "../common/include/sphere.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <variant>

namespace {

  // Helper para crear un material Matte válido
  render::Matte create_default_matte() {
    render::Color const white{1.0, 1.0, 1.0};
    return {"default_mat", white};
  }

  // Parámetros de prueba
  render::Point const VALID_CENTER{5.0, 0.0, 0.0};
  double const VALID_RADIUS               = 2.5;
  render::t_material const VALID_MATERIAL = create_default_matte();

  // Pruebas para getters de Sphere

  TEST(test_sphere, getters_return_correct_values) {
    render::Point const center{100.0, 50.0, 20.0};
    double const radius = 5.0;

    render::Sphere const s(center, radius, VALID_MATERIAL);

    // get_radius()
    EXPECT_DOUBLE_EQ(s.get_radius(), 5.0);

    // get_center()
    render::Point const retrieved_center = s.get_center();
    EXPECT_DOUBLE_EQ(retrieved_center.get_x(), 100.0);
    EXPECT_DOUBLE_EQ(retrieved_center.get_y(), 50.0);

    // get_material()
    EXPECT_TRUE(std::holds_alternative<render::Matte>(s.get_material()));
  }

}  // namespace
