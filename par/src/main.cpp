#include "../../common/include/logic.hpp"
#include "../../common/include/pov.hpp"
#include "../../common/include/scene.hpp"
#include "../include/image_aos.hpp"
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

    PixelAOS pixels_aos(total_pixels);
    std::ofstream ppm_file(arguments[3]);
    write_ppm_header(ppm_file, image_width, image_height);

    for (int f = 0; f < image_height; ++f) {
      for (int c = 0; c < image_width; ++c) {
        Pixel const pixel =
            scene.get_pixel_color(f, c, scene.get_rays_rng_seed(), scene.get_material_rng_seed());
        std::size_t const index =
            static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
            static_cast<std::size_t>(c);
        pixels_aos.set(index, pixel);
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
