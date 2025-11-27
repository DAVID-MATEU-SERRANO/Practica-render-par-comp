#include "../../common/include/logic.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

namespace fs = std::filesystem;

TEST(aos_smoke, writes_ppm_header_correctly) {
  fs::path const tmp = fs::temp_directory_path() / "aos_header_only.ppm";
  {
    std::ofstream out(tmp);
    ASSERT_TRUE(out.is_open());
    render::write_ppm_header(out, 3, 2);
  }

  std::ifstream in(tmp);
  ASSERT_TRUE(in.is_open());
  std::string line;

  ASSERT_TRUE(std::getline(in, line));
  EXPECT_EQ(line, "P3");
  ASSERT_TRUE(std::getline(in, line));
  EXPECT_EQ(line, "3 2");
  ASSERT_TRUE(std::getline(in, line));
  EXPECT_EQ(line, "255");

  in.close();
  fs::remove(tmp);
}
