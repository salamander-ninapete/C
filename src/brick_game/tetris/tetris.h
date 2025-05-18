#ifndef TETRIS_H

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 10   // Ширина игрового поля
#define HEIGHT 20  // Высота игрового поля
#define CENTR 3  // Поправка для центровки отображения фигуры
#define LVL_MAX 10       // Максимальный уровень
#define LVL_CURRENT 600  // Разница между уровнями в очках
#define LVL_STEP 40  // разница между уровнями в скорости
#define LVL_START 1      // начальный уровень
#define START_SCORE 0    // начальный счёт
#define START_SPEED 485  // начальная скорость
#define FINISH_SPEED 85  // итоговая сторость при макс уровне
#define COUNT_POINTS_FOR_1 100   // кол-во очков за 1 строку
#define COUNT_POINTS_FOR_2 300   // кол-во очков за 2 строки
#define COUNT_POINTS_FOR_3 700   // кол-во очков за 3 строки
#define COUNT_POINTS_FOR_4 1500  // кол-во очков за 4 строки
#define COORDINATE_NEXT_FIG 5  // высота отображения след. фигуры (5)
// следующее менять не стоит
#define NUM_FIGURES 7   // Кол-во фигур
#define FIGURE_STATE 4  // Кол-во положений фигуры
#define BLOCK_SIZE 4    // Размер фигуры 4x4
#define SDWIG 1  // сдвиг игры по вертикали (дебаг)
#define QUIT_IN_MENU -1  // значения опыта при выходе из стартового меню
#define INF -1  // значения для поддержания беск. цыкла

typedef struct {
  int **field;     // Игровое поле
  int **next;      // Следующий блок
  int next_fig;    // Следующая фигура
  int score;       // Текущий счет
  int high_score;  // Максимальный счет
  int level;       // Уровень игры
  int speed;       // Скорость игры
  bool pause;      // Статус паузы
} GameInfo_t;

// Перечисление действий пользователя
typedef enum {
  Start,      // Старт / снять с паузы
  Pause,      // Пауза / снять с паузы
  Terminate,  // Выход
  Left,       // Движение блока в лево
  Right,      // Движение блока в право
  Up,         // Постоянное движение вниз
  Down,       // Ускоренное движение вниз
  Action      // Поворот фигуры
} UserAction_t;

typedef enum {
  zer_row,    // ноль строк
  one_row,    // одна строка
  two_row,    // две строки
  three_row,  // три строки
  four_row,   // четыре строки
} NumRows;

typedef struct {
  int blocks[FIGURE_STATE][BLOCK_SIZE]
            [BLOCK_SIZE];  // [положение фигуры] [Блоки фигуры][Блоки фигуры]
  int currentState;  // Текущее  положение фигуры (0-3)
  int x, y;          // Координаты на игровом поле
  int size;          // Размер фигуры
} TetFigure;

extern TetFigure allFigures[NUM_FIGURES];

// Прототипы функций

// заведение первичных переменных
void initializeGame(GameInfo_t *game, const char *filename);
// выделение памяти на создание поля
int **createField();
// выделение памяти на создание фигуры
int **createNextFigure();
// генерация рандомной* фигуры
int generateFig();
// создание новой фигуры на поле
TetFigure placeFigure(GameInfo_t *game);
// получение информации от игрока для дфижения фигуры
int moveFigure(bool *running, TetFigure *figure, GameInfo_t *game);
// конечный автомат или обработка логики ввода пользователя
bool userInput(UserAction_t action, TetFigure *figure, GameInfo_t *game);
// закрепление фигуры на поле при успешном "падении"
void placeFigureOnField(GameInfo_t *game, TetFigure *figure);
// удаление строки / строк и подсчет очков
void clearFullRows(GameInfo_t *game);
// рассчет скорости для определенного уровня
GameInfo_t *leveling(GameInfo_t *game);
// обработка клавишь для выхода из меню
bool handleMenuInput(GameInfo_t *game);
// обработка клавишь для выхода из окна окончания игры
int gameOverCase();
// считывание с фйла лучшего счета и создание файла
int readHighScore(const char *filename);
// запись лучшего счета в файддл
void writeHighScore(const char *filename, int score);
// отрисовка фигуры
void drawFigure(GameInfo_t *game, TetFigure *figure);
// стирание "следа" фигуры для корректого движения
void washTraceFigure(GameInfo_t *game, TetFigure *figure);

// движение фигуры взниз
bool moveFigureDown(GameInfo_t *game, TetFigure *figure);
// смещениия вправо
TetFigure *moveRight(GameInfo_t *game, TetFigure *figure);
// смещение влево
TetFigure *moveLeft(GameInfo_t *game, TetFigure *figure);
// повороты @
TetFigure *handleRotation(GameInfo_t *game, TetFigure *figure);
// проверка на возможность поворота
bool canRotate(GameInfo_t game, TetFigure figure);
// проверка на соприкосновение со стенами, полом, фигурами
bool checkCollision(GameInfo_t game, TetFigure figure, int dx, int dy);
// действия при остановке фигуры
bool stopFigure(GameInfo_t *game, TetFigure *figure, bool val);

// отрисовка поля и окошка, а также все связанные с визуалм статичные части
void renderGame(GameInfo_t *game);
// отрисовка следующей фигуры
void renderNextFigure(GameInfo_t *game);
// отображение вступительного меню
void showMenu();
// отображение окошка окончаня игры
void showGameOver(GameInfo_t *game);
// отображение окошка альтернативного окончания игры
void showGameOver2();

// чистка памяти выделенной на игру
void cleanupGame(GameInfo_t *game);
#endif  // TETRIS_H