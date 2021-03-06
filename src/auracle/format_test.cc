#include "format.hh"

#include <iostream>
#include <sstream>

#include "aur/package.hh"
#include "gtest/gtest.h"

class ScopedCapturer {
 public:
  ScopedCapturer(std::ostream& stream) : stream_(stream) {
    stream_.rdbuf(buffer_.rdbuf());
  }

  ~ScopedCapturer() { stream_.rdbuf(original_sbuf_); }

  std::string GetCapturedOutput() {
    auto str = buffer_.str();
    buffer_.str(std::string());
    return str;
  }

 private:
  std::stringstream buffer_;
  std::streambuf* original_sbuf_ = std::cout.rdbuf();
  std::ostream& stream_;
};

aur::Package MakePackage() {
  using namespace std::chrono_literals;

  aur::Package p;

  // string
  p.name = "cower";
  p.version = "1.2.3";

  // floating point
  p.popularity = 5.20238;

  // datetime
  p.submitted = 1499013608s;

  // lists
  p.conflicts = {
      "auracle",
      "cower",
      "cower-git",
  };

  return p;
}

TEST(FormatTest, DetectsInvalidFormats) {
  std::string err;
  EXPECT_FALSE(format::FormatIsValid("{invalid}", &err));
  EXPECT_FALSE(err.empty());
}

TEST(FormatTest, CustomStringFormat) {
  ScopedCapturer capture(std::cout);

  format::Custom("{name} -> {version}", MakePackage());

  EXPECT_EQ(capture.GetCapturedOutput(), "cower -> 1.2.3\n");
}

TEST(FormatTest, CustomFloatFormat) {
  ScopedCapturer capture(std::cout);

  auto p = MakePackage();

  format::Custom("{popularity}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "5.20238\n");

  format::Custom("{popularity:.2f}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "5.20\n");
}

TEST(FormatTest, CustomDateTimeFormat) {
  ScopedCapturer capture(std::cout);

  auto p = MakePackage();

  format::Custom("{submitted}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "Sun Jul  2 16:40:08 2017\n");

  format::Custom("{submitted:%s}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "1499013608\n");
}

TEST(FormatTest, ListFormat) {
  ScopedCapturer capture(std::cout);

  auto p = MakePackage();

  format::Custom("{conflicts}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "auracle  cower  cower-git\n");

  format::Custom("{conflicts::,,}", p);
  EXPECT_EQ(capture.GetCapturedOutput(), "auracle:,,cower:,,cower-git\n");
}
