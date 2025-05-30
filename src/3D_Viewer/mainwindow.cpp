#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      openGLWidget(new MyOpenGLWidget(this)),
      controller(openGLWidget->controller),
      record_timer_(new QTimer(this)),
      gif_image_(nullptr),
      frame_count_(0),
      is_recording_(false) {
  ui->setupUi(this);

  controller->updateState(0, 0, 0, 0, 0, 0, 1, 1, 100);
  openGLWidget->move(20, 60);
  openGLWidget->resize(600, 600);

  updateFrame(0, Qt::black);
  updateFrame(1, Qt::white);
  updateFrame(2, Qt::green);
  update();

  QSettings settings;
  connect(ui->buttonSelectFile, SIGNAL(clicked()), this,
          SLOT(read_and_other()));
  connect(ui->sliderRotateAxis_x, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderRotateAxis_y, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderRotateAxis_z, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderShiftAxis_x, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderShiftAxis_y, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderShiftAxis_z, SIGNAL(valueChanged(int)), this,
          SLOT(slider()));
  connect(ui->sliderScale, SIGNAL(valueChanged(int)), this, SLOT(slider()));
  connect(ui->sliderEdgeWidth, SIGNAL(valueChanged(int)), this, SLOT(slider()));
  connect(ui->sliderTopSize, SIGNAL(valueChanged(int)), this, SLOT(slider()));
  connect(ui->inpRotateAxis_x, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpRotateAxis_y, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpRotateAxis_z, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpShiftAxis_x, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpShiftAxis_y, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpShiftAxis_z, SIGNAL(returnPressed()), this,
          SLOT(inputTable()));
  connect(ui->inpScale, SIGNAL(returnPressed()), this, SLOT(inputTable()));
  connect(ui->inpEdgeWidth, SIGNAL(returnPressed()), this, SLOT(inputTable()));
  connect(ui->inpTopSize, SIGNAL(returnPressed()), this, SLOT(inputTable()));
  connect(ui->leverPlaneParalel, &QRadioButton::toggled, this,
          &MainWindow::onProjectionChanged);
  connect(ui->leverPlaneCenter, &QRadioButton::toggled, this,
          &MainWindow::onProjectionChanged);
  connect(ui->leverTopNothing, &QRadioButton::toggled, this,
          &MainWindow::onVertexTypeChanged);
  connect(ui->leverTopRound, &QRadioButton::toggled, this,
          &MainWindow::onVertexTypeChanged);
  connect(ui->leverTopSquare, &QRadioButton::toggled, this,
          &MainWindow::onVertexTypeChanged);

  connect(ui->leverDotted, &QRadioButton::toggled, this,
          &MainWindow::onLinesTypeChanged);
  connect(ui->leverSolid, &QRadioButton::toggled, this,
          &MainWindow::onLinesTypeChanged);
  connect(ui->buttoSaveJPEG, &QPushButton::clicked, this,
          [this]() { saveImage("JPEG"); });
  connect(ui->buttonSaveBMP, &QPushButton::clicked, this,
          [this]() { saveImage("BMP"); });

  connect(ui->buttonSaveGIF, &QPushButton::clicked, this,
          &MainWindow::startRecord);
  connect(record_timer_, &QTimer::timeout, this, &MainWindow::getFrame);

  connect(ui->buttonBackSelectColor, SIGNAL(clicked()), this,
          SLOT(updateBackColor()));
  connect(ui->buttonEdgeSelectColor, SIGNAL(clicked()), this,
          SLOT(updateEdgeColor()));
  connect(ui->buttonTopSeletColor, SIGNAL(clicked()), this,
          SLOT(updateTopColor()));
  connect(ui->buttonResetSettings, SIGNAL(clicked()), this,
          SLOT(resetSettings()));
}

MainWindow::~MainWindow() {
  controller->save_settings();
  save_style_settings();
  delete ui;
  delete openGLWidget;
  controller = nullptr;
}

void MainWindow::save_style_settings() {
  QString settingsFile =
      QCoreApplication::applicationDirPath() + "/viewer_settings.ini";
  QSettings settings(settingsFile, QSettings::IniFormat);
  if (ui->leverPlaneParalel->isChecked()) {
    settings.setValue("projection_type", s21::Projection_type::PARALLEL);
  } else {
    settings.setValue("projection_type", s21::Projection_type::CENTER);
  }
  if (ui->leverSolid->isChecked()) {
    settings.setValue("lines_type", s21::Lines_type::SOLID);
  } else {
    settings.setValue("lines_type", s21::Lines_type::DOTTED);
  }

  if (ui->leverTopNothing->isChecked()) {
    settings.setValue("vertices_type", s21::Vertices_type::NONE);
  } else if (ui->leverTopRound->isChecked()) {
    settings.setValue("vertices_type", s21::Vertices_type::SMOOTH);
  } else {
    settings.setValue("vertices_type", s21::Vertices_type::SQUARE);
  }

  settings.setValue("r_top", openGLWidget->getTopR());
  settings.setValue("g_top", openGLWidget->getTopG());
  settings.setValue("b_top", openGLWidget->getTopB());

  settings.setValue("r_edge", openGLWidget->getEdgeR());
  settings.setValue("g_edge", openGLWidget->getEdgeG());
  settings.setValue("b_edge", openGLWidget->getEdgeB());

  settings.setValue("r_back", openGLWidget->getBackR());
  settings.setValue("g_back", openGLWidget->getBackG());
  settings.setValue("b_back", openGLWidget->getBackB());
}

void MainWindow::load_settings() {
  QString settingsFile =
      QCoreApplication::applicationDirPath() + "/viewer_settings.ini";
  if (!QFile::exists(settingsFile)) {
    return;
  }
  QSettings settings(settingsFile, QSettings::IniFormat);

  int rotate_x = settings.value("rotate_x-axis").toInt();
  int rotate_y = settings.value("rotate_y-axis").toInt();
  int rotate_z = settings.value("rotate_z-axis").toInt();
  int shift_x = settings.value("shift_x-axis").toInt();
  int shift_y = settings.value("shift_y-axis").toInt();
  int shift_z = settings.value("shift_z-axis").toInt();
  int vertex_size = settings.value("vertex_size").toInt();
  int lines_weight = settings.value("lines_weight").toInt();
  double scale = settings.value("scale").toDouble();
  int projection_type = settings.value("projection_type").toInt();
  int lines_type = settings.value("lines_type").toInt();
  int vertices_type = settings.value("vertices_type").toInt();

  ui->sliderRotateAxis_x->setValue(rotate_x);
  ui->sliderRotateAxis_y->setValue(rotate_y);
  ui->sliderRotateAxis_z->setValue(rotate_z);
  ui->sliderShiftAxis_x->setValue(shift_x);
  ui->sliderShiftAxis_y->setValue(shift_y);
  ui->sliderShiftAxis_z->setValue(shift_z);
  ui->sliderScale->setValue(scale);
  ui->sliderEdgeWidth->setValue(lines_weight);
  ui->sliderTopSize->setValue(vertex_size);

  if (projection_type == s21::Projection_type::PARALLEL) {
    ui->leverPlaneParalel->setChecked(true);
  } else {
    ui->leverPlaneCenter->setChecked(true);
  }

  if (lines_type == s21::Lines_type::SOLID) {
    ui->leverSolid->setChecked(true);
  } else {
    ui->leverDotted->setChecked(true);
  }

  if (vertices_type == s21::Vertices_type::NONE) {
    ui->leverTopNothing->setChecked(true);
  } else if (vertices_type == s21::Vertices_type::SMOOTH) {
    ui->leverTopRound->setChecked(true);
  } else {
    ui->leverTopSquare->setChecked(true);
  }

  openGLWidget->setTopR(settings.value("r_top").toDouble());
  openGLWidget->setTopG(settings.value("g_top").toDouble());
  openGLWidget->setTopB(settings.value("b_top").toDouble());

  openGLWidget->setEdgeR(settings.value("r_edge").toDouble());
  openGLWidget->setEdgeG(settings.value("g_edge").toDouble());
  openGLWidget->setEdgeB(settings.value("b_edge").toDouble());

  openGLWidget->setBackR(settings.value("r_back").toDouble());
  openGLWidget->setBackG(settings.value("g_back").toDouble());
  openGLWidget->setBackB(settings.value("b_back").toDouble());

  QColor color_Back, color_Top, color_Edge;
  color_Back.setRgbF(openGLWidget->getBackR(), openGLWidget->getBackG(),
                     openGLWidget->getBackB(), 1);
  color_Top.setRgbF(openGLWidget->getTopR(), openGLWidget->getTopG(),
                    openGLWidget->getTopB(), 1);
  color_Edge.setRgbF(openGLWidget->getEdgeR(), openGLWidget->getEdgeG(),
                     openGLWidget->getEdgeB(), 1);

  updateFrame(0, color_Back);
  updateFrame(1, color_Edge);
  updateFrame(2, color_Top);

  ui->inpRotateAxis_x->setText(settings.value("rotate_x-axis").toString());
  ui->inpRotateAxis_y->setText(settings.value("rotate_y-axis").toString());
  ui->inpRotateAxis_z->setText(settings.value("rotate_z-axis").toString());
  ui->inpShiftAxis_x->setText(settings.value("shift_x-axis").toString());
  ui->inpShiftAxis_y->setText(settings.value("shift_y-axis").toString());
  ui->inpShiftAxis_z->setText(settings.value("shift_z-axis").toString());
  ui->inpTopSize->setText(settings.value("vertex_size").toString());
  ui->inpEdgeWidth->setText(settings.value("lines_weight").toString());
  ui->inpScale->setText(QString::number(scale / 100));

  controller->updateState(rotate_x, rotate_y, rotate_z, shift_x, shift_y,
                          shift_z, lines_weight, vertex_size, scale);
}

void MainWindow::processSelectedFile(const QString &filePath) {
  if (launch_flag_) {
    controller->save_settings();
    save_style_settings();
  }
  controller->clear_model();

  if (ModelFactory::create_model(filePath.toStdString())) {
    std::cerr << "Ошибка при создании модели из файла!" << std::endl;
    return;
  }

  controller->normalize_model();

  unsigned int points_amount = controller->get_number_of_points() + 1;
  openGLWidget->setVertexAmount(points_amount);

  ui->fieldCountEdge->setText(
      QString::number(controller->get_number_of_edges()));
  ui->fieldCountTop->setText(
      QString::number(controller->get_number_of_points()));
  ui->fieldSelectFile->setText(filePath);

  controller->updateState(0, 0, 0, 0, 0, 0, 1, 1, 100);
  load_settings();
  openGLWidget->update();

  launch_flag_ = true;
}

void MainWindow::inputTable() {
  bool ok;
  QString rotate_x = ui->inpRotateAxis_x->text();
  QString rotate_y = ui->inpRotateAxis_y->text();
  QString rotate_z = ui->inpRotateAxis_z->text();
  QString shift_x = ui->inpShiftAxis_x->text();
  QString shift_y = ui->inpShiftAxis_y->text();
  QString shift_z = ui->inpShiftAxis_z->text();
  QString scale = ui->inpScale->text();
  QString lines_weight = ui->inpEdgeWidth->text();
  QString vertex_size = ui->inpTopSize->text();

  ui->sliderRotateAxis_x->setValue(rotate_x.toInt(&ok));
  ui->sliderRotateAxis_y->setValue(rotate_y.toInt(&ok));
  ui->sliderRotateAxis_z->setValue(rotate_z.toInt(&ok));
  ui->sliderShiftAxis_x->setValue(shift_x.toInt(&ok));
  ui->sliderShiftAxis_y->setValue(shift_y.toInt(&ok));
  ui->sliderShiftAxis_z->setValue(shift_z.toInt(&ok));
  ui->sliderScale->setValue((scale.toDouble(&ok) * 100));
  ui->sliderEdgeWidth->setValue(lines_weight.toInt(&ok));
  ui->sliderTopSize->setValue(vertex_size.toInt(&ok));
}

void MainWindow::slider() {
  int rotate_x = ui->sliderRotateAxis_x->value();
  int rotate_y = ui->sliderRotateAxis_y->value();
  int rotate_z = ui->sliderRotateAxis_z->value();
  int shift_x = ui->sliderShiftAxis_x->value();
  int shift_y = ui->sliderShiftAxis_y->value();
  int shift_z = ui->sliderShiftAxis_z->value();
  int scale = ui->sliderScale->value();
  int lines_weight = ui->sliderEdgeWidth->value();
  int vertex_size = ui->sliderTopSize->value();

  ui->inpRotateAxis_x->setText(QString::number(rotate_x));
  ui->inpRotateAxis_y->setText(QString::number(rotate_y));
  ui->inpRotateAxis_z->setText(QString::number(rotate_z));
  ui->inpShiftAxis_x->setText(QString::number(shift_x));
  ui->inpShiftAxis_y->setText(QString::number(shift_y));
  ui->inpShiftAxis_z->setText(QString::number(shift_z));
  ui->inpScale->setText(QString::number((static_cast<double>(scale) / 100)));
  ui->inpEdgeWidth->setText(QString::number(lines_weight));
  ui->inpTopSize->setText(QString::number(vertex_size));

  controller->updateState(rotate_x, rotate_y, rotate_z, shift_x, shift_y,
                          shift_z, lines_weight, vertex_size, scale);
  openGLWidget->update();
}

void MainWindow::read_and_other() {
  QString filePath =
      QFileDialog::getOpenFileName(this, tr("Открыть файл"), QDir::homePath(),
                                   tr("Все файлы (*);;Текстовые файлы "
                                      "(*.txt);;OBJ файлы (*.obj)"));

  if (!filePath.isEmpty()) {
    filePath_ = filePath;

    processSelectedFile(filePath);
  }
}

void MainWindow::onProjectionChanged(bool checked) {
  if (checked) {
    s21::Projection_type type = openGLWidget->getProjectionType();
    if (ui->leverPlaneParalel->isChecked() &&
        type != s21::Projection_type::PARALLEL) {
      openGLWidget->setProjection(s21::Projection_type::PARALLEL);
      openGLWidget->update();
    } else if (type != s21::Projection_type::CENTER) {
      openGLWidget->setProjection(s21::Projection_type::CENTER);
      openGLWidget->update();
    }
  }
}

void MainWindow::onVertexTypeChanged(bool checked) {
  if (checked) {
    if (ui->leverTopNothing->isChecked()) {
      openGLWidget->setVertexType(s21::Vertices_type::NONE);
    } else if (ui->leverTopRound->isChecked()) {
      openGLWidget->setVertexType(s21::Vertices_type::SMOOTH);
    } else {
      openGLWidget->setVertexType(s21::Vertices_type::SQUARE);
    }
    openGLWidget->update();
  }
}

void MainWindow::onLinesTypeChanged(bool checked) {
  if (checked) {
    if (ui->leverDotted->isChecked()) {
      openGLWidget->setLinesType(s21::Lines_type::DOTTED);
    } else {
      openGLWidget->setLinesType(s21::Lines_type::SOLID);
    }
    openGLWidget->update();
  }
}

void MainWindow::saveImage(const QString &format) {
  QFileDialog image_save(this);
  QString image = image_save.getSaveFileName(
      this, "Save Image", "model",
      format + " Files (*." + format.toLower() + ");;All Files (*)");
  if (!image.isEmpty()) {
    openGLWidget->grabFramebuffer().save(image);
  }
}

void MainWindow::startRecord() {
  if (is_recording_) return;

  gif_image_ = new QGifImage;
  gif_image_->setDefaultDelay(100);

  is_recording_ = true;
  frame_count_ = 0;
  record_timer_->start(100);
}

void MainWindow::getFrame() {
  if (!is_recording_) return;

  QImage frame = openGLWidget->grabFramebuffer().scaled(
      640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  gif_image_->addFrame(frame);
  ++frame_count_;

  if (frame_count_ >= 50) {
    stopRecord();
  }
}

void MainWindow::stopRecord() {
  record_timer_->stop();
  is_recording_ = false;

  QString file_path = QFileDialog::getSaveFileName(
      this, "Saving GIF", "screen_cast.gif", "GIF Files (*.gif)");

  if (file_path.isEmpty()) {
    std::cout << "GIF saving canceld" << std::endl;
  }

  gif_image_->save(file_path);

  delete gif_image_;
}

void MainWindow::updateBackColor() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
  updateColor(0, color);
}
void MainWindow::updateEdgeColor() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
  updateColor(1, color);
}
void MainWindow::updateTopColor() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
  updateColor(2, color);
}

void MainWindow::updateColor(int some, const QColor &color) {
  if (color.isValid()) {
    double r = color.redF(), g = color.greenF(), b = color.blueF();
    if (some == 0) {
      openGLWidget->setBackR(r);
      openGLWidget->setBackG(g);
      openGLWidget->setBackB(b);

      openGLWidget->setBackground();

    } else if (some == 1) {
      openGLWidget->setEdgeR(r);
      openGLWidget->setEdgeG(g);
      openGLWidget->setEdgeB(b);

    } else if (some == 2) {
      openGLWidget->setTopR(r);
      openGLWidget->setTopG(g);
      openGLWidget->setTopB(b);
    }
    updateFrame(some, color);
  }
  openGLWidget->update();
}

void MainWindow::updateFrame(int some, const QColor &color) {
  if (some == 0) {
    ui->frameBackColor->setStyleSheet(
        QString("background-color: %1;").arg(color.name()));
  } else if (some == 1) {
    ui->frameEdgeColor->setStyleSheet(
        QString("background-color: %1;").arg(color.name()));
  } else if (some == 2) {
    ui->frameTopColor->setStyleSheet(
        QString("background-color: %1;").arg(color.name()));
  }
}

void MainWindow::resetSettings() {
  ui->sliderRotateAxis_x->setValue(0);
  ui->sliderRotateAxis_y->setValue(0);
  ui->sliderRotateAxis_z->setValue(0);
  ui->sliderShiftAxis_x->setValue(0);
  ui->sliderShiftAxis_y->setValue(0);
  ui->sliderShiftAxis_z->setValue(0);
  ui->sliderScale->setValue(100);
  ui->sliderEdgeWidth->setValue(1);
  ui->sliderTopSize->setValue(1);
  ui->inpRotateAxis_x->setText(QString::number(0));
  ui->inpRotateAxis_y->setText(QString::number(0));
  ui->inpRotateAxis_z->setText(QString::number(0));
  ui->inpShiftAxis_x->setText(QString::number(0));
  ui->inpShiftAxis_y->setText(QString::number(0));
  ui->inpShiftAxis_z->setText(QString::number(0));
  ui->inpScale->setText(QString::number((1)));
  ui->inpEdgeWidth->setText(QString::number(1));
  ui->inpTopSize->setText(QString::number(1));
  ui->leverPlaneCenter->setChecked(true);
  ui->leverPlaneParalel->setChecked(true);
  ui->leverTopNothing->setChecked(true);
  ui->leverSolid->setChecked(true);
  ui->inpRotateAxis_x->setText(QString::number(0));
  ui->inpRotateAxis_y->setText(QString::number(0));
  ui->inpRotateAxis_z->setText(QString::number(0));
  ui->inpShiftAxis_x->setText(QString::number(0));
  ui->inpShiftAxis_y->setText(QString::number(0));
  ui->inpShiftAxis_z->setText(QString::number(0));
  ui->inpScale->setText(QString::number(1));
  ui->inpEdgeWidth->setText(QString::number(1));
  ui->inpTopSize->setText(QString::number(1));
  updateColor(0, Qt::black);
  updateColor(1, Qt::white);
  updateColor(2, Qt::green);
  openGLWidget->setTopR(0);
  openGLWidget->setTopG(1);
  openGLWidget->setTopB(0);
  openGLWidget->setBackR(0);
  openGLWidget->setBackG(0);
  openGLWidget->setBackB(0);
  openGLWidget->setEdgeR(1);
  openGLWidget->setEdgeG(1);
  openGLWidget->setEdgeB(1);
  processSelectedFile(filePath_);
  openGLWidget->update();
}

}  // namespace s21
