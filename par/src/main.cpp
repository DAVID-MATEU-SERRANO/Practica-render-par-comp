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

#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#include <tbb/partitioner.h>

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

    // PASOS DEFINIDOS EN 2.2.5

    /*TODO: 1. tbb::this_task_arena::max_concurrency() para obtener el número máximo de hilos que se
      pueden usar (2.2.6)
      - Se debe probar con distintos números de hilos y EXPLICAR EN MEMORIA EL VALOR ÓPTIMO ELEGIDO
      (3.2)
      - int num_threads = ...;
    */

    /* TODO: 1. tbb::global_control para limitar el número de hilos a usar (2.3.2)
      - Se utiliza num_threads definido en el paso anterior
    */

    /* TODO: 2. Vectores de semillas generado antes del bucle (2.2.7)
      - Uno para el de rayos y otro para el de materiales (ambos con tamaño igual al número de
      hilos)
      - Se generan sus valores con la semilla obtenida de la configuración (bucle que rellena los
      dos vectores)
    */

    /* TODO: 3. tbb::enumerate_thread_specific para privatizar cada generador (2.2.3, 2.2.4)
      - Se usa un tipo atómico para evitar que dos hilos usen el mismo índice
      - Cada hilo obtendrá una copia local (privada) de los generadores (2.2.2)
    */

    std::mt19937_64 rng(scene.get_rays_rng_seed());        // BORRAR
    std::mt19937_64 m_rng(scene.get_material_rng_seed());  // BORRAR

    // Se debe probar con distintos tamaños de grano y EXPLICAR EN MEMORIA EL VALOR ÓPTIMO ELEGIDO Y
    // SI ESTO ES RELEVANTE (3.2)
    int const grain_size = 0;

    // Uso de parallel_for con blocked_range2d (2.3.4)
    tbb::parallel_for(
        tbb::blocked_range2d<int>(0, image_height, grain_size, 0, image_width, grain_size),
        [&](tbb::blocked_range2d<int> const & r) {
          // TODO: Se obtiene la referencia a las copias locales (generadores privados) con .local()
          // (2.2.4) -> FALTA

          for (int f = r.rows().begin(); f != r.rows().end(); ++f) {
            for (int c = r.cols().begin(); c != r.cols().end(); ++c) {
              // TODO: 3. Se usan los generadores locales (privados) -> CAMBIAR ABAJO
              Pixel const pixel = scene.get_pixel_color(f, c, rng, m_rng);

              std::size_t const index =
                  static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
                  static_cast<std::size_t>(c);
              pixels_aos.set(index, pixel);
            }
          }
        },
        // TODO: Probar con los tres tipos de estrategias de división para distintos números de
        // hilos y tamaños de grano y EXPLICAR EN MEMORIA EL VALOR ÓPTIMO ELEGIDO (2.3.3, 3.2)
        tbb::auto_partitioner());

    // El PPM necesita escritura secuencial para que los píxeles se guarden en orden
    pixels_aos.write(ppm_file);

    ppm_file.close();
    return 0;

  } catch (std::exception const & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
