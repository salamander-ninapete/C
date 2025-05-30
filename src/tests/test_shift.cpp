#include <gtest/gtest.h>

#include "../backend/viver.h"

using namespace s21;

TEST(ModelShiftTest, ShiftAlongX) {
  MODEL.clear();
  double x = 1.0;
  double y = 2.0;
  double z = 3.0;

  MODEL.add_point({x, y, z});

  double shift = 0.3;

  double shift_res_x_1 = x + shift;
  double shift_res_y_1 = y;
  double shift_res_z_1 = z;

  MODEL.shift_model(Axis_type::X, shift);

  EXPECT_NEAR(MODEL.get_points()[0][0], shift_res_x_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], shift_res_y_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], shift_res_z_1, 1e-6);
  MODEL.clear();
}

TEST(ModelShiftTest, ShiftAlongY) {
  MODEL.clear();
  double x = 1.0;
  double y = 2.0;
  double z = 3.0;

  MODEL.add_point({x, y, z});

  double shift = -0.2;
  double shift_res_x_1 = x;
  double shift_res_y_1 = y + shift;
  double shift_res_z_1 = z;

  MODEL.shift_model(Axis_type::Y, shift);

  EXPECT_NEAR(MODEL.get_points()[0][0], shift_res_x_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], shift_res_y_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], shift_res_z_1, 1e-6);
  MODEL.clear();
}

TEST(ModelShiftTest, ShiftAlongZ) {
  MODEL.clear();
  double x = 1.0;
  double y = 2.0;
  double z = 3.0;

  MODEL.add_point({x, y, z});

  double shift = 11.0;
  double shift_res_x_1 = x;
  double shift_res_y_1 = y;
  double shift_res_z_1 = z + shift;

  MODEL.shift_model(Axis_type::Z, shift);

  EXPECT_NEAR(MODEL.get_points()[0][0], shift_res_x_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], shift_res_y_1, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], shift_res_z_1, 1e-6);
  MODEL.clear();
}

TEST(ModelShiftTest, IncorrectAxis) {
  MODEL.clear();
  double x = 1.0;
  double y = 2.0;
  double z = 3.0;
  size_t incorrect_axis = 4;
  MODEL.add_point({x, y, z});

  double shift = 11.0;
  MODEL.shift_model(incorrect_axis, shift);

  EXPECT_NEAR(MODEL.get_points()[0][0], x, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], y, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], z, 1e-6);
}