#include <gtest/gtest.h>

#include "../backend/viver.h"

TEST(ModelRotateTest, RotateAroundX) {
  MODEL.clear();

  MODEL.add_point({1.0, 2.0, 3.0});
  MODEL.rotate_model(s21::Axis_type::X, M_PI / 2);

  EXPECT_NEAR(MODEL.get_points()[0][0], 1.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], -3.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], 2.0, 1e-6);
  MODEL.clear();
}

TEST(ModelRotateTest, RotateAroundY) {
  MODEL.clear();

  MODEL.add_point({1.0, 2.0, 3.0});
  MODEL.rotate_model(s21::Axis_type::Y, M_PI / 2);

  EXPECT_NEAR(MODEL.get_points()[0][0], 3.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], 2.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], -1.0, 1e-6);
}

TEST(ModelRotateTest, RotateAroundZ) {
  MODEL.clear();

  MODEL.add_point({1.0, 2.0, 3.0});

  MODEL.rotate_model(s21::Axis_type::Z, M_PI / 2);

  EXPECT_NEAR(MODEL.get_points()[0][0], -2.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], 1.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], 3.0, 1e-6);
}

TEST(ModelRotateTest, RotateMultiplePoints) {
  MODEL.clear();

  MODEL.add_point({1.0, 2.0, 3.0});
  MODEL.add_point({4.0, 5.0, 6.0});

  MODEL.rotate_model(s21::Axis_type::Y, M_PI / 2);

  EXPECT_NEAR(MODEL.get_points()[0][0], 3.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], 2.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], -1.0, 1e-6);

  EXPECT_NEAR(MODEL.get_points()[1][0], 6.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], 5.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], -4.0, 1e-6);
}

TEST(ModelRotateTest, IncorrectAxis) {
  MODEL.clear();
  MODEL.add_point({1.0, 2.0, 3.0});
  MODEL.add_point({4.0, 5.0, 6.0});
  size_t incorrect_axis = 4;

  double shift = 11.0;
  MODEL.rotate_model(incorrect_axis, shift);

  EXPECT_NEAR(MODEL.get_points()[0][0], 1.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], 2.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], 3.0, 1e-6);

  EXPECT_NEAR(MODEL.get_points()[1][0], 4.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], 5.0, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], 6.0, 1e-6);
}