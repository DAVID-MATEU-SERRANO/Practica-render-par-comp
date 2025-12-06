#include "../include/scene.hpp"
#include "../include/color.hpp"
#include "../include/cylinder.hpp"
#include "../include/matte.hpp"
#include "../include/metal.hpp"
#include "../include/parse_exception.hpp"
#include "../include/point.hpp"
#include "../include/pov.hpp"
#include "../include/ray.hpp"
#include "../include/refractive.hpp"
#include "../include/vector.hpp"
#include <cstddef>
#include <cstdint>
#include <oneapi/tbb/blocked_range.h>
#include <oneapi/tbb/enumerable_thread_specific.h>
#include <oneapi/tbb/global_control.h>
#include <oneapi/tbb/parallel_reduce.h>
#include <random>
#include <string>

namespace render {

  bool Scene::test_sphere_intersections(Ray & ray, IntersectionInfo & info) {
    bool found_intersection = false;

    for (auto const & sphere : spheres) {
      bool front_face = false;
      if (ray.sphere_intersection(sphere, front_face) and ray.get_intersection_distance() >= 1e-3) {
        if (ray.get_intersection_distance() < info.closest_distance) {
          info.closest_distance   = ray.get_intersection_distance();
          info.closest_point      = ray.get_point_intersection();
          info.closest_normal     = ray.get_normal_vector();
          info.closest_material   = sphere.get_material();
          info.closest_front_face = front_face;
          found_intersection      = true;
        }
      }
    }
    return found_intersection;
  }

  bool Scene::update_closest_hit(Ray const & ray, Cylinder const & cylinder,
                                 IntersectionInfo & info, bool front_face) {
    // Comprobar si la intersección actual es más cercana que la registrada
    if (ray.get_intersection_distance() < info.closest_distance) {
      // Actualizar todos los campos de la intersección más cercana
      info.closest_distance   = ray.get_intersection_distance();
      info.closest_point      = ray.get_point_intersection();
      info.closest_normal     = ray.get_normal_vector();
      info.closest_material   = cylinder.get_material();
      info.closest_front_face = front_face;

      return true;  // Éxito: La intersección fue actualizada
    }
    return false;  // No se actualizó
  }

  bool Scene::test_cylinder_intersections(Ray & ray, IntersectionInfo & info) {
    bool found_intersection = false;
    for (auto const & cylinder : cylinders) {
      bool front_face  = true;
      bool current_hit = false;

      if (ray.cylinder_side_intersection(cylinder, front_face) and
          ray.get_intersection_distance() >= 1e-3)
      {
        current_hit        = update_closest_hit(ray, cylinder, info, front_face);
        found_intersection = found_intersection or current_hit;
      }

      if (ray.cylinder_upper_base_intersection(cylinder, front_face) and
          ray.get_intersection_distance() >= 1e-3)
      {
        current_hit        = update_closest_hit(ray, cylinder, info, front_face);
        found_intersection = found_intersection or current_hit;
      }

      if (ray.cylinder_lower_base_intersection(cylinder, front_face) and
          ray.get_intersection_distance() >= 1e-3)
      {
        current_hit        = update_closest_hit(ray, cylinder, info, front_face);
        found_intersection = found_intersection or current_hit;
      }
    }
    return found_intersection;
  }

  void Scene::find_closest_intersection(Ray & ray, bool & front_face_out) {
    IntersectionInfo info;

    bool found_intersection = test_sphere_intersections(ray, info);
    found_intersection      = test_cylinder_intersections(ray, info) or found_intersection;

    if (found_intersection) {
      ray.set_intersection_distance(info.closest_distance);
      ray.set_point_intersection(info.closest_point);
      ray.set_normal_vector(info.closest_normal);
      ray.set_intersection_material(info.closest_material);
      front_face_out = info.closest_front_face;
    } else {
      ray.set_intersection_distance(-1.0);
      front_face_out = true;
    }
  }

  Color Scene::depth_ray(Point current_origin, Vector current_direction, Color ray_color,
                         std::mt19937_64 & m_rng) {
    // Función auxiliar para get_pixel_color
    for (int depth = 0; depth < max_depth; ++depth) {
      Ray current_ray(current_origin, current_direction, ray_color);
      bool front_face = true;
      find_closest_intersection(current_ray, front_face);
      current_ray.color_contribution(background_dark_color, background_light_color, m_rng,
                                     front_face);
      ray_color.multiply_in_place(current_ray.get_intersection_color());
      if (current_ray.get_intersection_distance() == -1.0) {
        break;
      }

      if (depth == max_depth - 1) {
        ray_color = Color(0.0, 0.0, 0.0);
        break;
      }
      current_origin    = current_ray.get_point_intersection();
      current_direction = current_ray.get_reflected_direction().normalized();
    }
    return ray_color;
  }

  Pixel Scene::get_pixel_color(int f, int c, std::mt19937_64 & rng, std::mt19937_64 & m_rng) {
    std::uniform_real_distribution<double> dist(-0.5, 0.5);
    Vector const dx           = pov.pw_horizontal_vector().dot(1.0 / pov.get_image_width());
    Vector const dy           = pov.pw_vertical_vector().dot(1.0 / pov.get_image_height());
    Point const initial_point = pov.get_camera_position();

    Color final_accumulated_color = tbb::parallel_reduce(
        tbb::blocked_range<std::size_t>(0, static_cast<std::size_t>(samples_per_pixel)),
        Color(0.0, 0.0, 0.0),
        [&](tbb::blocked_range<std::size_t> const & r, Color local_sum) {
          for (std::size_t ray_counter = r.begin(); ray_counter != r.end(); ++ray_counter) {
            // El código de trazado de un rayo va aquí
            double const rx = dist(rng);
            double const ry = dist(rng);
            Point const q =
                pov.get_proyection_window().get_origin().add(dx.dot(c + rx)).add(dy.dot(f + ry));
            Point current_origin     = initial_point;
            Vector current_direction = q.substract(pov.get_camera_position()).normalized();
            Color ray_color(1.0, 1.0, 1.0);

            Color final_ray_color = depth_ray(current_origin, current_direction, ray_color, m_rng);
            local_sum.add_in_place(final_ray_color);
          }
          return local_sum;
        },
        [](Color x, Color y) {
          x.add_in_place(y);
          return x;
        });

    final_accumulated_color.multiply_in_place(1.0 / static_cast<double>(samples_per_pixel));
    final_accumulated_color.apply_gamma_correction(gamma);
    return {static_cast<std::uint8_t>(255.0 * final_accumulated_color.get_r()),
            static_cast<std::uint8_t>(255.0 * final_accumulated_color.get_g()),
            static_cast<std::uint8_t>(255.0 * final_accumulated_color.get_b())};
  }

  // add
  void Scene::add_material_matte(Matte const & matte, std::string const & line_content) {
    if (material_index.contains(matte.get_name())) {
      parse::throw_material_exists(matte.get_name(), line_content);
    }
    std::size_t const new_index      = mattes.size();
    material_index[matte.get_name()] = new_index * 10 + 0;
    mattes.push_back(matte);
  }

  void Scene::add_material_metal(Metal const & metal, std::string const & line_content) {
    if (material_index.contains(metal.get_name())) {
      parse::throw_material_exists(metal.get_name(), line_content);
    }
    std::size_t const new_index      = metals.size();
    material_index[metal.get_name()] = new_index * 10 + 1;
    metals.push_back(metal);
  }

  void Scene::add_material_refractive(Refractive const & refractive,
                                      std::string const & line_content) {
    if (material_index.contains(refractive.get_name())) {
      parse::throw_material_exists(refractive.get_name(), line_content);
    }
    std::size_t const new_index           = refractives.size();
    material_index[refractive.get_name()] = new_index * 10 + 2;
    refractives.push_back(refractive);
  }

}  // namespace render
