
#include "ViewerOpenGLWidget.h"

namespace s21 {

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), controller(new Controller(this)) {
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(2, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  setFormat(format);
}

MyOpenGLWidget::~MyOpenGLWidget() { delete controller; }

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::paintGL() {
  setBackground();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  manageProjection();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -5.0f);
  glColor3f(1.0f, 1.0f, 1.0f);

  if (vertices_type_ != s21::Vertices_type::NONE) {
    vertexViewHandler();
    printVertices();
  }
  linesViewHandler();
  printLines();
}

void MyOpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyOpenGLWidget::printLines() {
  auto indices = controller->get_edges();
  auto vertices = controller->get_points();
  glBegin(GL_LINES);
  for (const auto &edgeIndices : indices) {
    unsigned int edge_size = edgeIndices.size();
    for (unsigned int i = 0; i < edge_size; ++i) {
      unsigned long index1 = edgeIndices[i];
      unsigned long index2 = edgeIndices[(i + 1) % edge_size];

      if (index1 >= 0 && index1 < vertex_amount_ && index2 >= 0 &&
          index2 < vertex_amount_) {
        glVertex3f(vertices[index1][0], vertices[index1][1],
                   vertices[index1][2]);
        glVertex3f(vertices[index2][0], vertices[index2][1],
                   vertices[index2][2]);
      } else {
        qDebug() << "Ошибка: индекс вершины выходит за пределы диапазона ";
      }
    }
  }
  glEnd();
}

void MyOpenGLWidget::setProjection(s21::Projection_type type) {
  projection_type_ = type;
}

s21::Projection_type MyOpenGLWidget::getProjectionType() const {
  return projection_type_;
}

void MyOpenGLWidget::manageProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection_type_ == s21::Projection_type::PARALLEL) {
    glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
  } else {
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
  }
}

void MyOpenGLWidget::printVertices() {
  auto vertices = controller->get_points();
  glEnableClientState(GL_VERTEX_ARRAY);
  glBegin(GL_POINTS);
  for (size_t i = 1; i < vertex_amount_; ++i) {
    glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
  }
  glEnd();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void MyOpenGLWidget::setVertexType(s21::Vertices_type type) {
  vertices_type_ = type;
}

void MyOpenGLWidget::setVertexSize(size_t size) { vertex_size_ = size; }

void MyOpenGLWidget::setLinesType(s21::Lines_type type) { lines_type_ = type; }

void MyOpenGLWidget::setLinesSize(size_t size) { lines_size_ = size; }

void MyOpenGLWidget::linesViewHandler() {
  if (lines_type_ == s21::Lines_type::DOTTED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(STRIPPLE_FACTOR, STRIPPLE_TEMPLATE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glLineWidth(controller->getLinesSize());
  glColor3f(edge_r_, edge_g_, edge_b_);
}

void MyOpenGLWidget::vertexViewHandler() {
  if (vertices_type_ == s21::Vertices_type::SMOOTH) {
    glEnable(GL_POINT_SMOOTH);
  } else if (vertices_type_ == s21::Vertices_type::SQUARE) {
    glDisable(GL_POINT_SMOOTH);
  }
  glPointSize(controller->getVerticesSize());
  glColor3f(top_r_, top_g_, top_b_);
}
void MyOpenGLWidget::setBackground() {
  glClearColor(back_r_, back_g_, back_b_, 1);
}

void MyOpenGLWidget::setVertexAmount(size_t amount) { vertex_amount_ = amount; }
}  // namespace s21
