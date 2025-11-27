#include "../../common/include/logic.hpp"
#include "../../common/include/pov.hpp"

#include "../../common/include/scene.hpp"
#include "../include/image_soa.hpp"
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace render;

int main(int argc, char * argv[]) {
  try {
    std::vector<std::string> arguments(argv, argv + argc);
    validate_arguments(argc, arguments);

    Scene scene = load_scene(arguments[1], arguments[2]);

    int const image_height = scene.get_pov().get_image_height();
    int const image_width  = scene.get_pov().get_image_width();
    std::size_t const total_pixels =
        static_cast<std::size_t>(image_width) * static_cast<std::size_t>(image_height);

    PixelSOA pixels_soa(total_pixels);
    std::ofstream ppm_file(arguments[3]);
    write_ppm_header(ppm_file, image_width, image_height);

    std::mt19937_64 rng(scene.get_rays_rng_seed());
    std::mt19937_64 m_rng(scene.get_material_rng_seed());

    for (int f = 0; f < image_height; ++f) {
      for (int c = 0; c < image_width; ++c) {
        Pixel const pixel = scene.get_pixel_color(f, c, rng, m_rng);
        std::size_t const index =
            static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
            static_cast<std::size_t>(c);
        pixels_soa.set(index, pixel);
        ppm_file << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
                 << static_cast<int>(pixel.b) << "\n";
      }
    }

    ppm_file.close();
    return 0;

  } catch (std::exception const & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
