#include "viver.h"

namespace s21 {

/**
 * @brief Конструктор по умолчанию для класса Parser
 *
 * Инициализирует парсер с флагом ошибки OK (отсутствие ошибок).
 * Не требует дополнительных параметров и выполняет минимальную инициализацию.
 *
 * @post err_flag установлен в значение OK
 * @see clear_parser()
 */
Parser::Parser() { err_flag = OK; }

/**
 * @brief Деструктор класса Parser
 *
 * Стандартный деструктор. В текущей реализации не выполняет дополнительных
 * действий, так как управление памятью осуществляется контейнерами STL.
 *
 * @note При наследовании требует виртуального объявления
 */
Parser::~Parser() {}

/**
 * @brief Очищает все внутренние данные парсера
 *
 * Сбрасывает состояние парсера к начальному, удаляя все временные данные.
 * Используется для повторного использования парсера или при обработке ошибок.
 *
 * @note Алгоритм работы:
 * 1. Очищает список точек
 * 2. Очищает список рёбер
 * 3. Сбрасывает минимальные и максимальные значения координат
 *
 * @post points_list и edges_list полностью очищены
 * @post min_values и max_values сброшены в 0
 * @post Состояние err_flag не изменяется
 *
 * @see Parser()
 * @see shrink_vectors()
 */
void Parser::clear_parser() {
  points_list.clear();
  edges_list.clear();
  min_values.coords.fill(0);
  max_values.coords.fill(0);
}

/**
 * @brief Обновляет граничные значения координат модели
 *
 * Сравнивает координаты новой точки с текущими минимальными и максимальными
 * значениями и при необходимости обновляет их. Используется при добавлении
 * новых точек в модель.
 *
 * @param new_point Точка для сравнения и обновления границ
 * @note Алгоритм работы:
 * 1. Для каждой координаты (X, Y, Z):
 *    - Сравнивает с текущим min_values и обновляет при необходимости
 *    - Сравнивает с текущим max_values и обновляет при необходимости
 *
 * @post min_values и max_values содержат актуальные граничные значения модели
 * @warning Не проверяет входные данные на NaN/Infinity
 *
 * @see clear_parser()
 */
void Parser::update_min_max_values(const point &new_point) {
  for (size_t i = 0; i < 3; ++i) {
    min_values[i] = std::min(min_values[i], new_point[i]);
    max_values[i] = std::max(max_values[i], new_point[i]);
  }
}

/**
 * @brief Оптимизирует использование памяти векторами парсера
 *
 * Уменьшает емкость (capacity) векторов points_list и edges_list до
 * соответствия их текущему размеру (size). Используется для освобождения
 * неиспользуемой памяти после завершения парсинга.
 *
 * @note Эквивалентно вызову:
 * - points_list.shrink_to_fit()
 * - edges_list.shrink_to_fit()
 *
 * @post Векторы используют минимально необходимую память
 * @note Обычно вызывается после завершения парсинга и всех манипуляций с
 * данными
 *
 * @see clear_parser()
 */
void Parser::shrink_vectors() {
  points_list.shrink_to_fit();
  edges_list.shrink_to_fit();
}

/**
 * @brief Добавляет точку в список точек модели
 *
 * Добавляет новую точку в конец вектора points_list. Не выполняет проверок
 * на валидность входных данных. Используется в процессе парсинга файла.
 *
 * @param new_point Точка для добавления
 * @post Размер points_list увеличивается на 1
 * @note Для обновления граничных значений требуется отдельный вызов
 * update_min_max_values
 *
 * @see update_min_max_values()
 * @see add_edge()
 */
void Parser::add_point(const point &new_point) {
  points_list.push_back(new_point);
}

/**
 * @brief Добавляет ребро в список рёбер модели
 *
 * Добавляет новое ребро в конец вектора edges_list. Не выполняет проверок
 * на валидность индексов вершин. Используется в процессе парсинга файла.
 *
 * @param new_edge Ребро для добавления
 * @post Размер edges_list увеличивается на 1
 *
 * @see add_point()
 */
void Parser::add_edge(const edge &new_edge) { edges_list.push_back(new_edge); }

/**
 * @brief Возвращает текущий флаг ошибки парсера
 *
 * Позволяет проверить состояние парсера после операций. Константный метод,
 * не изменяющий состояние объекта.
 *
 * @return Константная ссылка на текущее значение err_flag
 * @see parse_file()
 */
const bool &Parser::get_err_flag() const { return err_flag; }

/**
 * @brief Парсит файл модели и загружает данные
 *
 * Основной метод класса, выполняющий чтение и разбор файла 3D-модели.
 * Обрабатывает ошибки открытия файла, сбрасывает предыдущее состояние
 * и управляет процессом загрузки данных.
 *
 * @param file_name Путь к файлу для парсинга
 * @note Алгоритм работы:
 * 1. Сбрасывает флаг ошибки
 * 2. Открывает файл
 * 3. При ошибке открытия устанавливает флаг ERR
 * 4. Читает точки и рёбра через get_points_and_edges
 * 5. Оптимизирует память shrink_vectors
 * 6. Закрывает файл
 *
 * @post При успешном парсинге points_list и edges_list содержат данные модели
 * @post min_values и max_values содержат корректные границы модели
 * @warning Выводит сообщение об ошибке в stderr при проблемах с файлом
 *
 * @see get_points_and_edges()
 * @see clear_parser()
 */
void Parser::parse_file(std::string file_name) {
  err_flag = OK;
  std::ifstream file(file_name);
  if (!file) {
    std::cerr << "Ошибка открытия файла" << std::endl;
    err_flag = ERR;
  }
  get_points_and_edges(file);

  shrink_vectors();

  file.close();
}

/**
 * @brief Читает точки и рёбра из открытого файла
 *
 * Внутренний метод, обрабатывающий содержимое файла построчно.
 * Распознаёт вершины (v) и грани (f), игнорирует остальные данные.
 * Добавляет нулевую точку и ребро как маркеры начала данных.
 *
 * @param file Открытый файловый поток для чтения
 * @note Особенности реализации:
 * 1. Добавляет нулевую точку (0,0,0) как первую в списке
 * 2. Добавляет нулевое ребро [0] как первое в списке
 * 3. Обрабатывает только строки, начинающиеся с v или f
 * 4. Делегирует парсинг точек и рёбер методам get_point и get_edge
 *
 * @post points_list содержит все вершины из файла плюс нулевую точку
 * @post edges_list содержит все грани из файла плюс нулевое ребро
 *
 * @see get_point()
 * @see get_edge()
 */
void Parser::get_points_and_edges(std::ifstream &file) {
  point zero_point{{0, 0, 0}};

  edge zero_edge;
  zero_edge.push_back(0);

  add_point(zero_point);
  add_edge(zero_edge);

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string label;
    iss >> label;

    if (label.length() == 1 && label == "v") {
      add_point(get_point(iss));
    } else if (label.length() == 1 && label == "f") {
      add_edge(get_edge(iss));
    }
  }
}

/**
 * @brief Парсит точку из строки файла
 *
 * Извлекает три координаты точки из входного потока и обновляет
 * граничные значения модели. Поддерживает научный формат чисел.
 *
 * @param iss Входной строковый поток с данными точки
 * @return Новая точка с прочитанными координатами
 * @note Автоматически обновляет min_values и max_values
 * @warning Не проверяет корректность числового формата
 * @warning При ошибках чтения может вернуть частично инициализированную точку
 *
 * @see update_min_max_values()
 */
const point Parser::get_point(std::istringstream &iss) {
  point new_point;
  iss >> new_point[0] >> new_point[1] >> new_point[2];
  update_min_max_values(new_point);
  return new_point;
}

/**
 * @brief Парсит ребро из строки файла
 *
 * Обрабатывает определение грани (face), извлекая индексы вершин.
 * Поддерживает форматы "f v1 v2 v3" и "f v1/vt1 v2/vt2 v3/vt3".
 * Конвертирует индексы файла (обычно 1-based) в 0-based массив.
 *
 * @param iss Входной строковый поток с данными грани
 * @return Новое ребро со списком индексов вершин
 * @note Особенности обработки:
 * 1. Игнорирует текст после '/' (координаты текстур)
 * 2. Преобразует строки в числа через stoul
 * 3. Оптимизирует память ребра перед возвратом
 *
 * @warning Может выбросить исключение при некорректных числовых данных
 * @warning Не проверяет валидность индексов вершин
 */
const edge Parser::get_edge(std::istringstream &iss) {
  size_t pos;
  edge new_edge;
  std::string subline;
  std::string str_to_convert;
  unsigned int num;
  while (iss >> subline) {
    pos = subline.find('/');
    if (pos != std::string::npos) {
      str_to_convert = subline.substr(0, pos);
    }
    num = std::stoul(str_to_convert);
    new_edge.push_back(num);
  }

  new_edge.vertexes_list.shrink_to_fit();
  return new_edge;
}

}  // namespace s21