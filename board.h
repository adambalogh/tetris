#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>

typedef enum { LEFT, RIGHT, DOWN } Direction;

typedef std::vector<std::vector<bool>> Figure;

const std::vector<Figure> FIGURES{
    {{1, 1, 1, 1}},
    {{1, 1, 1}, {1, 0, 0}},
    {{1, 1}, {1, 1}},
    {{1, 1, 1}, {0, 1, 0}},
    {{0, 1, 1}, {1, 1, 0}},
    {{1, 1, 0}, {0, 1, 1}},
};

struct BoardFigure {
  Figure figure;
  int top_left_row;
  int top_left_column;

  BoardFigure(Figure f, int top_left_row, int top_left_column) {
    this->figure = f;
    this->top_left_row = top_left_row;
    this->top_left_column = top_left_column;
  }

  int height() const { return figure.size(); }
  int width() const { return figure[0].size(); }
};

class Board {
 public:
  Board() {
    std::srand(std::time(0));
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        cells_[i][j] = false;
      }
    }
  }

  const int height = 15;
  const int width = 8;

  bool CallBack();
  bool Move(Direction d);

  bool At(int row, int column) const {
    std::lock_guard<std::mutex> lock(mu_);
    return cells_[row][column];
  }

 private:
  mutable std::mutex mu_;

  bool CanMoveTo(const BoardFigure& figure, Direction d);
  bool CanMoveDown(const BoardFigure& figure) {
    return CanMoveTo(figure, DOWN);
  }

  void MoveTo(BoardFigure& figure, Direction d);
  void MoveDown(BoardFigure& figure) { return MoveTo(figure, DOWN); }

  bool CanPlace(const Figure& figure, int row, int column);
  bool CanPlace(const BoardFigure& figure, int row, int column) {
    return CanPlace(figure.figure, row, column);
  }
  bool CanPlace(const BoardFigure& figure) {
    return CanPlace(figure.figure, figure.top_left_row, figure.top_left_column);
  }

  void Add(const BoardFigure& figure);
  void AddToCells(const BoardFigure& figure);

  void RemoveFromCells(const BoardFigure& figure);

  Figure GetRandomFigure() {
    int index = std::rand() % FIGURES.size();
    return FIGURES[index];
  }

  bool cells_[15][8];
  bool moving = false;
  std::vector<BoardFigure> figures_;
};