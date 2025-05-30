#ifndef VIVER_H
#define VIVER_H
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define MODEL (s21::Model::getInstance())

#define MINIMUM_SCALE 0.01
#define MAXIMUM_SCALE 10.0

#define MAX_SHIFT 100
#define MIN_SHIFT -100

#define MINIMUM_SCALE_FACTOR 0.000001

#define ERR 1
#define OK 0

#define STRIPPLE_FACTOR 5
#define STRIPPLE_TEMPLATE 0x0F0F

namespace s21 {

enum Axis_type {
  X,
  Y,
  Z,
};

enum Projection_type {
  PARALLEL,
  CENTER,
};

enum Vertices_type {
  NONE,
  SMOOTH,
  SQUARE,
};

enum Lines_type {
  SOLID,
  DOTTED,
};

enum Сonfiguration_parameter {
  ROTATE_X_,
  ROTATE_Y_,
  ROTATE_Z_,
  SHIFT_X_,
  SHIFT_Y_,
  SHIFT_Z_,
  LINES_WEIGHT_,
  VERTEX_SIZE_,
};

struct configuration {
  int rotate_x_, rotate_y_, rotate_z_, shift_x_, shift_y_, shift_z_,
      lines_weight_, vertex_size_;
  double scale_;
};

constexpr double DOUBLE_MIN = std::numeric_limits<double>::lowest();
constexpr double DOUBLE_MAX = std::numeric_limits<double>::max();

struct point {
  std::array<double, 3> coords;

  double &operator[](size_t index) { return coords[index]; }
  const double &operator[](size_t index) const { return coords[index]; }
};

struct edge {
  std::vector<unsigned int> vertexes_list;

  void push_back(unsigned int vertex) { vertexes_list.push_back(vertex); }
  size_t size() const { return vertexes_list.size(); }

  unsigned int &operator[](size_t index) { return vertexes_list[index]; }
  const unsigned int &operator[](size_t index) const {
    return vertexes_list[index];
  }

  void clear() { vertexes_list.clear(); }

  bool empty() const { return vertexes_list.empty(); }
};

class Model {
 public:
  static Model &getInstance() {
    static Model instance;
    return instance;
  }
  void clear();
  void shrink_vectors();

  void normalize();
  void model_scale(double desired_scale);
  void rotate_model(size_t axis_type, double angle);
  void shift_model(size_t axis_type, double shift_value);
  unsigned int get_number_of_points() const;
  unsigned int get_number_of_edges() const;

  void add_point(const point &point);
  void add_edge(const edge &edge);
  const std::vector<point> &get_points() const;
  const std::vector<edge> &get_edges() const;

  void set_min_values(const point &new_min) { min_values = new_min; };
  void set_max_values(const point &new_max) { max_values = new_max; };
  const point &get_min_values() const;
  const point &get_max_values() const;

  void update_min_max_values(const point &new_point);

  void set_configuration(const configuration &new_config) {
    config = new_config;
  };
  const configuration &get_configuration() const { return config; };

  void set_config_param(Сonfiguration_parameter param, const int &new_value);
  int get_config_param(Сonfiguration_parameter param) const;
  void set_config_scale(const double &new_scale) { config.scale_ = new_scale; };
  const double &get_config_scale() const { return config.scale_; };

 private:
  Model();
  ~Model();
  Model(const Model &) = delete;
  void operator=(const Model &) = delete;

  std::vector<point> points_list;
  std::vector<edge> edges_list;

  point min_values;
  point max_values;

  void center();

  configuration config;
};

class Parser {
 public:
  static Parser &getInstance() {
    static Parser instance;
    return instance;
  }

  void clear_parser();
  void parse_file(std::string file_name);
  const bool &get_err_flag() const;

  const std::vector<point> &get_points() const { return points_list; }
  const std::vector<edge> &get_edges() const { return edges_list; }

  const point &get_min_values() const { return min_values; };
  const point &get_max_values() const { return max_values; };

  void add_point(const point &point);
  void add_edge(const edge &edge);

  void shrink_vectors();
  void update_min_max_values(const point &new_point);

 private:
  Parser();
  ~Parser();
  Parser(const Parser &) = delete;
  void operator=(const Parser &) = delete;

  bool err_flag;

  void get_points_and_edges(std::ifstream &file);
  const point get_point(std::istringstream &stream);
  const edge get_edge(std::istringstream &stream);

  std::vector<point> points_list;
  std::vector<edge> edges_list;

  point min_values;
  point max_values;
};

class ModelFactory {
 public:
  static bool create_model(const std::string &file_name) {
    Parser &parser = Parser::getInstance();
    parser.clear_parser();
    parser.parse_file(file_name);

    if (parser.get_err_flag()) {
      return ERR;
    }
    std::vector<point> points = parser.get_points();
    std::vector<edge> edges = parser.get_edges();

    for (const auto &p : points) {
      MODEL.add_point(p);
    }
    for (const auto &e : edges) {
      MODEL.add_edge(e);
    }

    MODEL.set_min_values(parser.get_min_values());
    MODEL.set_max_values(parser.get_max_values());

    return OK;
  }
};

}  // namespace s21

#endif  // VIVER_H
