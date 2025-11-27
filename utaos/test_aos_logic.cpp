#include "../../common/include/logic.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace render;

TEST(aos_validate_arguments, invalid_arg_count_throws) {
  {
    std::vector<std::string> const args = {"render-aos"};
    EXPECT_THROW(validate_arguments((int) args.size(), args), std::invalid_argument);
  }
  {
    std::vector<std::string> const args = {"render-aos", "config.txt", "scene.txt"};
    EXPECT_THROW(validate_arguments((int) args.size(), args), std::invalid_argument);
  }
}

TEST(aos_validate_arguments, valid_arg_count_ok) {
  std::vector<std::string> const args = {"render-aos", "config.txt", "scene.txt", "out.ppm"};
  EXPECT_NO_THROW(validate_arguments((int) args.size(), args));
}
