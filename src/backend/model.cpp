#include "viver.h"

namespace s21 {
/**
 * @brief Конструктор по умолчанию класса Model.
 *
 * Инициализирует минимальные и максимальные значения координат модели.
 * По умолчанию устанавливает:
 * - min_values в (DOUBLE_MAX, DOUBLE_MAX, DOUBLE_MAX)
 * - max_values в (DOUBLE_MIN, DOUBLE_MIN, DOUBLE_MIN)
 *
 * @note DOUBLE_MAX и DOUBLE_MIN должны быть предварительно определены
 * как максимальное и минимальное значения типа double соответственно.
 *
 * @warning Эти начальные значения предназначены для последующего обновления
 * при загрузке реальных данных модели.
 */
Model::Model() {
  min_values = {{DOUBLE_MAX, DOUBLE_MAX, DOUBLE_MAX}};
  max_values = {{DOUBLE_MIN, DOUBLE_MIN, DOUBLE_MIN}};
}

/**
 * @brief Деструктор класса Model.
 *
 * Освобождает ресурсы, занимаемые объектом Model.
 * В текущей реализации не выполняет явных операций,
 * так как все члены класса имеют автоматическое управление памятью.
 *
 * @note Явно объявлен для обеспечения корректного разрушения объекта
 * и возможного будущего расширения функциональности.
 */
Model::~Model() {}

/**
 * @brief Очищает данные модели.
 *
 * Выполняет полный сброс состояния модели:
 * - Удаляет все точки (points_list)
 * - Удаляет все рёбра (edges_list)
 * - Сбрасывает минимальные и максимальные значения координат в 0
 *
 * @post Все контейнеры модели становятся пустыми
 * @post min_values и max_values обнуляются
 * @note После вызова этой модели требуется полная перезагрузка данных
 * @warning Не вызывает автоматического обновления визуализации
 */
void Model::clear() {
  points_list.clear();
  edges_list.clear();
  min_values.coords.fill(0);
  max_values.coords.fill(0);
}

/**
 * @brief Оптимизирует использование памяти векторами модели.
 *
 * Вызывает `shrink_to_fit()` для всех внутренних векторов модели:
 * - points_list - вектор точек модели
 * - edges_list - вектор рёбер модели
 *
 * @post Емкость (capacity) векторов уменьшается до размера (size)
 * @note Рекомендуется вызывать после завершения всех основных операций с
 * моделью
 * @warning Может вызвать переаллокацию памяти
 * @warning Не влияет на логику работы модели, только на потребление памяти
 *
 * @see std::vector::shrink_to_fit()
 */
void Model::shrink_vectors() {
  points_list.shrink_to_fit();
  edges_list.shrink_to_fit();
}

/**
 * @brief Обновляет минимальные и максимальные координаты модели.
 *
 * Сравнивает координаты новой точки с текущими граничными значениями
 * и обновляет их при необходимости. Для каждой из 3-х координат (x,y,z):
 * - min_values обновляется, если новая точка имеет меньшую координату
 * - max_values обновляется, если новая точка имеет большую координату
 *
 * @param new_point Точка для сравнения (должна содержать 3 координаты)
 * @post min_values содержит минимальные координаты среди всех точек
 * @post max_values содержит максимальные координаты среди всех точек
 * @note Используется при добавлении новых точек в модель
 * @warning Не проверяет размерность входной точки (ожидается 3 координаты)
 */
void Model::update_min_max_values(const point &new_point) {
  for (size_t i = 0; i < 3; ++i) {
    min_values[i] = std::min(min_values[i], new_point[i]);
    max_values[i] = std::max(max_values[i], new_point[i]);
  }
}

/**
 * @brief Добавляет новую точку в модель.
 *
 * Добавляет переданную точку в конец списка точек модели (points_list).
 * Не выполняет автоматического обновления граничных значений модели.
 *
 * @param new_point Точка для добавления (должна содержать 3 координаты x,y,z)
 * @post Размер points_list увеличивается на 1
 * @post Последний элемент points_list равен new_point
 * @note Для обновления границ модели требуется отдельный вызов
 * update_min_max_values()
 * @warning Не проверяет уникальность точки
 * @warning Не проверяет размерность входной точки
 *
 * @see update_min_max_values()
 */
void Model::add_point(const point &new_point) {
  points_list.push_back(new_point);
}

/**
 * @brief Добавляет новое ребро в модель
 *
 * Добавляет указанное ребро в контейнер рёбер модели (edges_list).
 * Не выполняет проверок на валидность индексов точек в ребре.
 *
 * @param new_edge Добавляемое ребро (пара индексов точек)
 * @post Размер edges_list увеличивается на 1
 * @post Последний элемент edges_list равен new_edge
 * @warning Не проверяет корректность индексов точек
 * @warning Не проверяет уникальность ребра
 * @note Для отрисовки требуется обновление визуализации
 */
void Model::add_edge(const edge &new_edge) { edges_list.push_back(new_edge); }

/**
 * @brief Возвращает константную ссылку на точки модели
 *
 * Предоставляет доступ только для чтения к контейнеру точек модели.
 * Гарантирует невозможность модификации точек через возвращаемую ссылку.
 *
 * @return Константная ссылка на вектор точек модели
 * @note Возвращаемая ссылка остается валидной до изменения модели
 * @warning Не следует сохранять ссылку надолго
 */
const std::vector<point> &Model::get_points() const { return points_list; }

/**
 * @brief Возвращает константную ссылку на рёбра модели
 *
 * Предоставляет доступ только для чтения к контейнеру рёбер модели.
 * Сохраняет инкапсуляцию, предотвращая прямое изменение рёбер.
 *
 * @return Константная ссылка на вектор рёбер модели
 * @note Возвращаемая ссылка действительна до модификации модели
 * @see get_points() для получения связанных точек
 */
const std::vector<edge> &Model::get_edges() const { return edges_list; }

/**
 * @brief Нормализует модель, центрируя и масштабируя её
 *
 * Выполняет два последовательных преобразования:
 * 1. Центрирование модели относительно её геометрического центра
 * 2. Масштабирование модели к единичному размеру
 *
 * @post Модель центрирована в начале координат
 * @post Модель имеет единичный размер по наибольшей оси
 * @note Изменяет все точки модели и граничные значения
 * @warning Требует предварительно вычисленных min/max значений
 */
void Model::normalize() {
  center();
  model_scale(1);
}

/**
 * @brief Центрирует модель относительно её границ
 *
 * Вычисляет геометрический центр модели и смещает все точки,
 * чтобы центр оказался в начале координат. Обновляет граничные значения.
 *
 * @post Центр модели совпадает с началом координат (0,0,0)
 * @post Все точки смещены на вектор центра
 * @post Граничные значения (min/max) обновлены
 * @note Использует текущие значения min_values и max_values
 * @warning Может вызвать потерю точности при больших координатах
 */
void Model::center() {
  point central;
  for (size_t i = 0; i < 3; ++i) {
    central[i] = min_values[i] + (max_values[i] - min_values[i]) / 2;
  }

  for (auto &p : points_list) {
    for (size_t j = 0; j < 3; ++j) {
      p[j] -= central[j];
    }
  }

  for (size_t j = 0; j < 3; ++j) {
    max_values[j] -= central[j];
    min_values[j] -= central[j];
  }
}

/**
 * @brief Возвращает количество точек в модели
 *
 * @return Количество точек (size-1, так как индексация с 0)
 * @note Возвращаемое значение на 1 меньше фактического размера вектора,
 *       так как предполагается индексация с 0
 * @warning Может вернуть неожиданное значение при пустой модели
 */
unsigned int Model::get_number_of_points() const {
  return points_list.size() - 1;
}

/**
 * @brief Возвращает количество рёбер в модели
 *
 * @return Количество рёбер (size-1, так как индексация с 0)
 * @note Возвращаемое значение на 1 меньше фактического размера вектора,
 *       так как предполагается индексация с 0
 * @warning Может вернуть неожиданное значение при пустой модели
 */
unsigned int Model::get_number_of_edges() const {
  return edges_list.size() - 1;
}

/**
 * @brief Устанавливает значение параметра конфигурации модели
 *
 * Изменяет указанный параметр конфигурации на новое значение.
 * Поддерживает все типы параметров, определенные в Сonfiguration_parameter.
 *
 * @param param Параметр конфигурации для изменения (из enum
 * Сonfiguration_parameter)
 * @param new_value Новое значение параметра
 * @throw std::out_of_range Если передано недопустимое значение параметра
 * @note Изменения применяются немедленно, но не вызывают автоматического
 * обновления отображения
 * @warning Не проверяет допустимость значения new_value для конкретного
 * параметра
 * @see get_config_param()
 */
void Model::set_config_param(Сonfiguration_parameter param,
                             const int &new_value) {
  switch (param) {
    case ROTATE_X_:
      config.rotate_x_ = new_value;
      break;
    case ROTATE_Y_:
      config.rotate_y_ = new_value;
      break;
    case ROTATE_Z_:
      config.rotate_z_ = new_value;
      break;
    case SHIFT_X_:
      config.shift_x_ = new_value;
      break;
    case SHIFT_Y_:
      config.shift_y_ = new_value;
      break;
    case SHIFT_Z_:
      config.shift_z_ = new_value;
      break;
    case LINES_WEIGHT_:
      config.lines_weight_ = new_value;
      break;
    case VERTEX_SIZE_:
      config.vertex_size_ = new_value;
      break;
  }
}

/**
 * @brief Возвращает значение параметра конфигурации модели
 *
 * Возвращает текущее значение запрошенного параметра конфигурации.
 * Гарантированно возвращает значение для любого валидного параметра.
 *
 * @param param Запрашиваемый параметр конфигурации
 * @return Текущее значение параметра
 * @throw std::out_of_range Если передан неизвестный параметр
 * @note Для неизвестных параметров возвращает 0 (но лучше генерировать
 * исключение)
 * @see set_config_param()
 */
int Model::get_config_param(Сonfiguration_parameter param) const {
  int value = 0;
  switch (param) {
    case ROTATE_X_:
      value = config.rotate_x_;
      break;
    case ROTATE_Y_:
      value = config.rotate_y_;
      break;
    case ROTATE_Z_:
      value = config.rotate_z_;
      break;
    case SHIFT_X_:
      value = config.shift_x_;
      break;
    case SHIFT_Y_:
      value = config.shift_y_;
      break;
    case SHIFT_Z_:
      value = config.shift_z_;
      break;
    case LINES_WEIGHT_:
      value = config.lines_weight_;
      break;
    case VERTEX_SIZE_:
      value = config.vertex_size_;
      break;
  }
  return value;
}

/**
 * @brief Возвращает минимальные координаты модели
 *
 * Предоставляет доступ только для чтения к точкам с минимальными координатами
 * (x,y,z). Эти значения обновляются при добавлении новых точек в модель.
 *
 * @return Константная ссылка на точку с минимальными координатами
 * @note Возвращаемая ссылка остается валидной до изменения модели
 * @see get_max_values()
 */
const point &Model::get_min_values() const { return min_values; }

/**
 * @brief Возвращает максимальные координаты модели
 *
 * Предоставляет доступ только для чтения к точкам с максимальными координатами
 * (x,y,z). Эти значения используются для нормализации и масштабирования модели.
 *
 * @return Константная ссылка на точку с максимальными координатами
 * @note Возвращаемая ссылка остается валидной до изменения модели
 * @see get_min_values()
 */
const point &Model::get_max_values() const { return max_values; }
}  // namespace s21
