#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>

#include "../backend/viver.h"
#include "QtGifImage-master/src/gifimage/qgifimage.h"
#include "ViewerOpenGLWidget.h"
#include "controller.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class MyOpenGLWidget;
}  // namespace Ui
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void read_and_other();
  void slider();
  void inputTable();
  void startRecord();
  void getFrame();
  void stopRecord();
  void updateBackColor();
  void updateEdgeColor();
  void updateTopColor();
  void resetSettings();

 private:
  bool launch_flag_ = 0;
  Ui::MainWindow *ui;
  MyOpenGLWidget *openGLWidget;
  Controller *controller;
  QTimer *record_timer_;
  QGifImage *gif_image_;
  int frame_count_;
  bool is_recording_;
  QString filePath_;
  void on_openFileButton_clicked();
  void processSelectedFile(const QString &filePath);
  void updateState(int rotate_x, int rotate_y, int rotate_z, int shift_x,
                   int shift_y, int shift_z, double scale, int lines_weight_,
                   int vertex_size_);

  void onSliderShiftChanged(s21::Axis_type axis, double value);
  void onProjectionChanged(bool is_changed);
  void onVertexTypeChanged(bool is_changed);
  void onLinesTypeChanged(bool is_changed);

  void saveImage(const QString &format);
  void save_style_settings();
  void updateColor(int type, const QColor &color);
  void updateFrame(int some, const QColor &color);
  void load_settings();
};

}  // namespace s21
#endif  // MAINWINDOW_H
