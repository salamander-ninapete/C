#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QCoreApplication>
#include <QObject>
#include <QSettings>

#include "../backend/viver.h"

namespace s21 {

class MainWindow;
class MyOpenGLWidget;

class Controller : public QObject {
  Q_OBJECT

 public:
  explicit Controller(QObject *parent = nullptr);
  ~Controller();
  void save_settings();

  void updateState(int rotate_x, int rotate_y, int rotate_z, int shift_x,
                   int shift_y, int shift_z, int lines_weight, int vertex_size,
                   double scale);
  void onSliderShiftChanged(s21::Axis_type axis, double value);

  void updateScale(double new_scale);
  void updateVerticesSize(int size);
  void updateLinesSize(int size);
  int getVerticesSize() const;
  int getLinesSize() const;

  void updateRotation(size_t x_y_z, int ang);

  void clear_model() { MODEL.clear(); };

  void normalize_model() { MODEL.normalize(); };

  const std::vector<point> &get_points() { return MODEL.get_points(); };
  const std::vector<edge> &get_edges() { return MODEL.get_edges(); };

  unsigned int get_number_of_points() const {
    return MODEL.get_number_of_points();
  };
  unsigned int get_number_of_edges() const {
    return MODEL.get_number_of_edges();
  };
};

}  // namespace s21

#endif  // CONTROLLER_H
