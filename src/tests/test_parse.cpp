#include <gtest/gtest.h>

#include "../backend/viver.h"
using namespace s21;

TEST(Parsing, TestGetInitFlag) {
  bool flag = Parser::getInstance().get_err_flag();
  EXPECT_EQ(flag, false);
}

TEST(Parsing, TestParseDefault) {
  s21::Parser &parser = s21::Parser::getInstance();
  std::string file_path = "./obj_files/cat.obj";

  parser.parse_file(file_path);

  bool flag = s21::Parser::getInstance().get_err_flag();
  parser.clear_parser();
  EXPECT_EQ(flag, OK);
}

TEST(Parsing, TestParseErrorOpen) {
  s21::Parser &parser = s21::Parser::getInstance();
  std::string file_path = "./models/not_exist.obj";

  parser.parse_file(file_path);

  bool flag = s21::Parser::getInstance().get_err_flag();
  parser.clear_parser();
  EXPECT_EQ(flag, ERR);
}

TEST(Parsing, TestGetPointsAndEdgesDefault) {
  s21::Parser &parser = s21::Parser::getInstance();
  MODEL.clear();
  std::string file_path = "./obj_files/cat.obj";
  ModelFactory::create_model(file_path);

  unsigned int num_points = MODEL.get_number_of_points();
  unsigned int num_edges = MODEL.get_number_of_edges();
  parser.clear_parser();
  MODEL.clear();
  EXPECT_EQ(num_points, 35290);
  EXPECT_EQ(num_edges, 35288);
}
