#ifndef VIEWEROPENGLWIDGET_H
#define VIEWEROPENGLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>

#include "../backend/viver.h"
#include "controller.h"

namespace s21 {

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Controller *controller;

  explicit MyOpenGLWidget(QWidget *parent = nullptr);
  ~MyOpenGLWidget();

  void printVertices();
  void printLines();
  void setProjection(s21::Projection_type type);
  void manageProjection();
  void setVertexType(s21::Vertices_type type);
  void setVertexSize(size_t size);
  void setLinesType(s21::Lines_type type);
  void setLinesSize(size_t size);
  void linesViewHandler();
  void vertexViewHandler();
  void setVertexAmount(size_t amount);
  s21::Projection_type getProjectionType() const;

  void set_vertices(std::vector<point> vertices) { vertices_ = vertices; }

  void set_indices(std::vector<edge> indices) { indices_ = indices; }

  void setBackground();

  void setBackR(double color) { back_r_ = color; }
  void setBackG(double color) { back_g_ = color; }
  void setBackB(double color) { back_b_ = color; }

  void setEdgeR(double color) { edge_r_ = color; }
  void setEdgeG(double color) { edge_g_ = color; }
  void setEdgeB(double color) { edge_b_ = color; }

  void setTopR(double color) { top_r_ = color; }
  void setTopG(double color) { top_g_ = color; }
  void setTopB(double color) { top_b_ = color; }

  double getBackR() { return back_r_; }
  double getBackG() { return back_g_; }
  double getBackB() { return back_b_; }

  double getEdgeR() { return edge_r_; }
  double getEdgeG() { return edge_g_; }
  double getEdgeB() { return edge_b_; }

  double getTopR() { return top_r_; }
  double getTopG() { return top_g_; }
  double getTopB() { return top_b_; }

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

 private:
  std::vector<point> vertices_;
  std::vector<edge> indices_;

  void loadModelData();
  s21::Projection_type projection_type_;
  s21::Vertices_type vertices_type_;
  s21::Lines_type lines_type_;

  size_t vertex_size_;
  size_t lines_size_;
  size_t vertex_amount_;

  double back_r_ = 0, back_g_ = 0, back_b_ = 0, edge_r_ = 1, edge_g_ = 1,
         edge_b_ = 1, top_r_ = 0, top_g_ = 1, top_b_ = 0;
};

}  // namespace s21
#endif  // VIEWEROPENGLWIDGET_H
