#include "viver.h"

namespace s21 {

/**
 * @brief Масштабирует модель до заданного размера
 *
 * Производит равномерное масштабирование всех точек модели таким образом,
 * чтобы наибольший габарит модели (по любой из осей) соответствовал заданному
 * значению. Автоматически обрабатывает пограничные случаи и корректирует
 * входные параметры.
 *
 * @param desired_scale Желаемый размер наибольшей оси модели (должен быть > 0)
 * @note Алгоритм работы:
 * 1. Вычисляет текущие размеры модели по осям
 * 2. Находит максимальный габарит
 * 3. Корректирует входной масштаб в допустимых пределах
 * 4. Вычисляет и применяет масштабный коэффициент
 * 5. Обновляет все точки и граничные значения
 *
 * @post Все точки модели равномерно масштабированы
 * @post Граничные значения (min_values/max_values) обновлены
 * @warning При нулевых размерах модели выводит предупреждение и прерывает
 * операцию
 * @warning Корректирует входной масштаб в пределах [MINIMUM_SCALE,
 * MAXIMUM_SCALE]
 * @warning Применяет минимальный масштабный коэффициент MINIMUM_SCALE_FACTOR
 *
 * @see normalize()
 * @see center()
 */
void Model::model_scale(double desired_scale) {
  point len_by_coords;
  for (size_t i = 0; i < 3; ++i) {
    len_by_coords[i] = max_values[i] - min_values[i];
  }

  double max_length = *std::max_element(len_by_coords.coords.begin(),
                                        len_by_coords.coords.end());
  if (max_length == 0) {
    std::cerr << "Фигура имеет длянну 0 в каких-то координатах" << std::endl;
    return;
  }
  if (desired_scale < MINIMUM_SCALE) {
    desired_scale = MINIMUM_SCALE;
  }
  if (desired_scale > MAXIMUM_SCALE) {
    desired_scale = MAXIMUM_SCALE;
  }

  double scale_factor = desired_scale / max_length;

  if (scale_factor < MINIMUM_SCALE_FACTOR) {
    scale_factor = MINIMUM_SCALE_FACTOR;
  }

  for (auto &p : points_list) {
    for (size_t j = 0; j < 3; ++j) {
      p[j] *= scale_factor;
    }
  }

  for (size_t i = 0; i < 3; ++i) {
    max_values[i] *= scale_factor;
    min_values[i] *= scale_factor;
  }
}

/**
 * @brief Вращает модель вокруг заданной оси на указанный угол
 *
 * Применяет матрицу поворота ко всем точкам модели вокруг выбранной оси (X, Y
 * или Z). Автоматически проверяет корректность входных параметров.
 *
 * @param axis_type Ось вращения (0-X, 1-Y, 2-Z)
 * @param angle Угол поворота в радианах
 * @note Алгоритм работы:
 * 1. Проверяет корректность выбранной оси
 * 2. Для каждой точки вычисляет новые координаты после поворота
 * 3. Применяет классическую матрицу поворота для 3D-преобразования
 *
 * @post Все точки модели повернуты на заданный угол вокруг выбранной оси
 * @warning Прерывает выполнение при некорректном значении axis_type
 * @warning Не обновляет граничные значения (min_values/max_values) после
 * поворота
 *
 * @see shift_model()
 * @see model_scale()
 */
void Model::rotate_model(size_t axis_type, double angle) {
  if (axis_type > Axis_type::Z || axis_type < Axis_type::X) {
    std::cerr << "Incorrect axis type" << std::endl;
    return;
  }

  double tmp_axis_value_1 = 0;
  double tmp_axis_value_2 = 0;

  for (auto &pnt : points_list) {
    if (axis_type == 0) {
      tmp_axis_value_1 = pnt[1];
      tmp_axis_value_2 = pnt[2];
      pnt[1] = tmp_axis_value_1 * cos(angle) - tmp_axis_value_2 * sin(angle);
      pnt[2] = tmp_axis_value_1 * sin(angle) + tmp_axis_value_2 * cos(angle);
    } else if (axis_type == 1) {
      tmp_axis_value_1 = pnt[0];
      tmp_axis_value_2 = pnt[2];
      pnt[0] = tmp_axis_value_1 * cos(angle) + tmp_axis_value_2 * sin(angle);
      pnt[2] = -(tmp_axis_value_1 * sin(angle)) + tmp_axis_value_2 * cos(angle);
    } else if (axis_type == 2) {
      tmp_axis_value_1 = pnt[0];
      tmp_axis_value_2 = pnt[1];
      pnt[0] = tmp_axis_value_1 * cos(angle) - tmp_axis_value_2 * sin(angle);
      pnt[1] = tmp_axis_value_1 * sin(angle) + tmp_axis_value_2 * cos(angle);
    }
  }
}

/**
 * @brief Сдвигает модель вдоль заданной оси на указанное значение
 *
 * Производит параллельный перенос всех точек модели по выбранной оси (X, Y или
 * Z). Автоматически обновляет граничные значения модели.
 *
 * @param axis_type Ось сдвига (0-X, 1-Y, 2-Z)
 * @param shift_value Величина сдвига
 * @note Алгоритм работы:
 * 1. Проверяет корректность выбранной оси
 * 2. Сдвигает каждую точку модели вдоль выбранной оси
 * 3. Обновляет min_values и max_values для сдвинутой оси
 *
 * @post Все точки модели сдвинуты на заданное значение
 * @post Граничные значения (min_values/max_values) обновлены для сдвинутой оси
 * @warning Прерывает выполнение при некорректном значении axis_type
 *
 * @see rotate_model()
 * @see center()
 */
void Model::shift_model(size_t axis_type, double shift_value) {
  if (axis_type > Axis_type::Z) {
    std::cerr << "Incorrect axis type" << std::endl;
    return;
  }

  for (auto &pnt : points_list) {
    pnt[axis_type] += shift_value;
  }

  min_values[axis_type] += shift_value;
  max_values[axis_type] += shift_value;
}

}  // namespace s21
