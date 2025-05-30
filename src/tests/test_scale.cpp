#include <gtest/gtest.h>

#include "../backend/viver.h"

using namespace s21;

TEST(ModelScaleTest, Scale) {
  MODEL.clear();
  double p1_x = 1.0;
  double p1_y = 2.0;
  double p1_z = 3.0;
  double p2_x = 4.0;
  double p2_y = 5.0;
  double p2_z = 6.0;
  MODEL.add_point({p1_x, p1_y, p1_z});
  MODEL.add_point({p2_x, p2_y, p2_z});
  MODEL.update_min_max_values({p1_x, p1_y, p1_z});

  double scale = 2.0;

  MODEL.model_scale(scale);
  double max_length = 3;
  double scale_factor = scale / max_length;

  EXPECT_NEAR(MODEL.get_points()[0][0], p1_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], p1_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], p1_z * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][0], p2_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], p2_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], p2_z * scale_factor, 1e-6);

  MODEL.clear();
}

TEST(ModelScaleTest, TooLowScale) {
  MODEL.clear();
  double p1_x = 1.0;
  double p1_y = 2.0;
  double p1_z = 3.0;
  double p2_x = 4.0;
  double p2_y = 5.0;
  double p2_z = 6.0;
  MODEL.add_point({p1_x, p1_y, p1_z});
  MODEL.add_point({p2_x, p2_y, p2_z});
  MODEL.update_min_max_values({p1_x, p1_y, p1_z});

  double scale = -2.0;

  MODEL.model_scale(scale);
  double max_length = 3;

  if (scale < MINIMUM_SCALE) {
    scale = MINIMUM_SCALE;
  }
  double scale_factor = scale / max_length;

  EXPECT_NEAR(MODEL.get_points()[0][0], p1_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], p1_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], p1_z * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][0], p2_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], p2_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], p2_z * scale_factor, 1e-6);

  MODEL.clear();
}

TEST(ModelScaleTest, NullLengthScale) {
  MODEL.clear();
  double p1_x = 1.0;
  double p1_y = 2.0;
  double p1_z = 3.0;
  double p2_x = 4.0;
  double p2_y = 5.0;
  double p2_z = 6.0;
  MODEL.add_point({p1_x, p1_y, p1_z});
  MODEL.add_point({p2_x, p2_y, p2_z});

  double scale = -2.0;

  MODEL.model_scale(scale);

  EXPECT_NEAR(MODEL.get_points()[0][0], p1_x, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], p1_y, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], p1_z, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][0], p2_x, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], p2_y, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], p2_z, 1e-6);

  MODEL.clear();
}

TEST(ModelScaleTest, MaxScale) {
  MODEL.clear();
  double p1_x = 1.0;
  double p1_y = 2.0;
  double p1_z = 3.0;
  double p2_x = 4.0;
  double p2_y = 5.0;
  double p2_z = 6.0;
  MODEL.add_point({p1_x, p1_y, p1_z});
  MODEL.add_point({p2_x, p2_y, p2_z});
  MODEL.update_min_max_values({p1_x, p1_y, p1_z});

  double scale = 100.0;

  MODEL.model_scale(scale);

  double max_length = 3;

  if (scale > MAXIMUM_SCALE) {
    scale = MAXIMUM_SCALE;
  }

  double scale_factor = scale / max_length;

  EXPECT_NEAR(MODEL.get_points()[0][0], p1_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], p1_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], p1_z * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][0], p2_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], p2_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], p2_z * scale_factor, 1e-6);

  MODEL.clear();
}

TEST(ModelScaleTest, MinScaleFactor) {
  MODEL.clear();
  double p1_x = 1.0;
  double p1_y = 2.0;
  double p1_z = 3.0;
  double p2_x = 100005.0;
  double p2_y = 5.0;
  double p2_z = 6.0;
  MODEL.add_point({p1_x, p1_y, p1_z});
  MODEL.add_point({p2_x, p2_y, p2_z});

  MODEL.update_min_max_values({p2_x, p2_y, p2_z});
  double scale = -2;

  MODEL.model_scale(scale);

  double max_length = 100004;

  if (scale < MINIMUM_SCALE) {
    scale = MINIMUM_SCALE;
  }
  double scale_factor = scale / max_length;

  if (scale_factor < MINIMUM_SCALE_FACTOR) {
    scale_factor = MINIMUM_SCALE_FACTOR;
  }

  EXPECT_NEAR(MODEL.get_points()[0][0], p1_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][1], p1_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[0][2], p1_z * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][0], p2_x * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][1], p2_y * scale_factor, 1e-6);
  EXPECT_NEAR(MODEL.get_points()[1][2], p2_z * scale_factor, 1e-6);

  MODEL.clear();
}