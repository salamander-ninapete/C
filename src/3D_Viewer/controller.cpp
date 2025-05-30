#include "controller.h"

namespace s21 {
Controller::Controller(QObject *parent) : QObject(parent) {}

Controller::~Controller() {}

void Controller::save_settings() {
  QString settingsFile =
      QCoreApplication::applicationDirPath() + "/viewer_settings.ini";
  QSettings settings(settingsFile, QSettings::IniFormat);
  configuration config = MODEL.get_configuration();
  settings.setValue("rotate_x-axis", config.rotate_x_);
  settings.setValue("rotate_y-axis", config.rotate_y_);
  settings.setValue("rotate_z-axis", config.rotate_z_);

  settings.setValue("shift_x-axis", config.shift_x_);
  settings.setValue("shift_y-axis", config.shift_y_);
  settings.setValue("shift_z-axis", config.shift_z_);

  settings.setValue("vertex_size", config.vertex_size_);
  settings.setValue("lines_weight", config.lines_weight_);
  settings.setValue("scale", config.scale_);
}

void Controller::updateState(int rotate_x, int rotate_y, int rotate_z,
                             int shift_x, int shift_y, int shift_z,
                             int lines_weight, int vertex_size, double scale) {
  configuration old_config = MODEL.get_configuration();
  if (rotate_x != old_config.rotate_x_) updateRotation(0, rotate_x);
  if (rotate_y != old_config.rotate_y_) updateRotation(1, rotate_y);
  if (rotate_z != old_config.rotate_z_) updateRotation(2, rotate_z);
  if (shift_x != old_config.shift_x_) {
    onSliderShiftChanged(s21::Axis_type::X, shift_x);
  }
  if (shift_y != old_config.shift_y_) {
    onSliderShiftChanged(s21::Axis_type::Y, shift_y);
  }
  if (shift_z != old_config.shift_z_) {
    onSliderShiftChanged(s21::Axis_type::Z, shift_z);
  }
  if (scale != old_config.scale_) updateScale(scale);
  if (lines_weight != old_config.lines_weight_) updateLinesSize(lines_weight);
  if (vertex_size != old_config.vertex_size_) updateVerticesSize(vertex_size);
}

void Controller::onSliderShiftChanged(s21::Axis_type axis, double value) {
  double shift = value;

  if (axis == s21::Axis_type::X) {
    shift -= MODEL.get_config_param(SHIFT_X_);
  }
  if (axis == s21::Axis_type::Y) {
    shift -= MODEL.get_config_param(SHIFT_Y_);
  }
  if (axis == s21::Axis_type::Z) {
    shift -= MODEL.get_config_param(SHIFT_Z_);
  }

  double absolute_shift = shift / 50;

  MODEL.shift_model(axis, absolute_shift);

  if (axis == s21::Axis_type::X) MODEL.set_config_param(SHIFT_X_, value);
  if (axis == s21::Axis_type::Y) MODEL.set_config_param(SHIFT_Y_, value);
  if (axis == s21::Axis_type::Z) MODEL.set_config_param(SHIFT_Z_, value);
}

void Controller::updateScale(double new_scale) {
  double scale = (new_scale / 100);

  MODEL.model_scale(scale);
  MODEL.set_config_scale(new_scale);
}

void Controller::updateVerticesSize(int size) {
  MODEL.set_config_param(VERTEX_SIZE_, size);
}

void Controller::updateLinesSize(int size) {
  MODEL.set_config_param(LINES_WEIGHT_, size);
}

int Controller::getLinesSize() const {
  return MODEL.get_config_param(LINES_WEIGHT_);
}

int Controller::getVerticesSize() const {
  return MODEL.get_config_param(VERTEX_SIZE_);
}

void Controller::updateRotation(size_t x_y_z, int ang) {
  double angle = (static_cast<double>(ang) * (M_PI / 180.0)), rotate = 0;

  if (x_y_z == 0) {
    rotate = angle - (static_cast<double>(MODEL.get_config_param(ROTATE_X_)) *
                      (M_PI / 180.0));
    MODEL.rotate_model(x_y_z, rotate);
    MODEL.set_config_param(ROTATE_X_, ang);
  } else if (x_y_z == 1) {
    rotate = angle - (static_cast<double>(MODEL.get_config_param(ROTATE_Y_)) *
                      (M_PI / 180.0));
    MODEL.rotate_model(x_y_z, rotate);
    MODEL.set_config_param(ROTATE_Y_, ang);

  } else if (x_y_z == 2) {
    rotate = angle - (static_cast<double>(MODEL.get_config_param(ROTATE_Z_)) *
                      (M_PI / 180.0));
    MODEL.rotate_model(x_y_z, rotate);
    MODEL.set_config_param(ROTATE_Z_, ang);
  }
}

}  // namespace s21
