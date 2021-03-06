#ifndef TETRIS_BOARD_H_
#define TETRIS_BOARD_H_

#include <array>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>

#include "figures.h"

namespace tetris {

// Directions we can move figures in
typedef enum { LEFT, RIGHT, DOWN } Direction;

struct BoardFigure : Figure {
  int top_left_row;
  int top_left_column;

  BoardFigure(FigureType type, int orientation, int top_left_row,
              int top_left_column)
      : Figure(type, orientation),
        top_left_row(top_left_row),
        top_left_column(top_left_column) {}
};

class RandomFigureGenerator {
 public:
  RandomFigureGenerator() { std::srand(std::time(0)); }

  virtual BoardFigure Get() {
    auto type = static_cast<FigureType>(std::rand() % NUM_TYPES);
    BoardFigure f(type, 0, 0, 0);
    return f;
  };
};

// TODO add tests
class Board {
 public:
  static const int height = 12;
  static const int width = 8;

 public:
  Board(std::unique_ptr<RandomFigureGenerator> r =
            std::make_unique<RandomFigureGenerator>())
      : figure_gen_(std::move(r)) {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        cells_[i][j] = false;
      }
    }
  }

  // Should be called periodically to move the current figure down
  bool CallBack();

  // Should be called when player wants to move the current figure
  // If there is no current figure, nothing will happen.
  bool Move(Direction d);

  // Should be called when player wants to rotate the current figure
  // If there is no current figure, nothing will happen.
  bool Rotate();

  // Returns whether there is a figure at the given location
  CellType At(int row, int column) const { return cells_[row][column]; }

  // private:
  void ClearFullRows();

  // Returns true if we can move the given figure to the given direction
  bool MoveIfPossible(BoardFigure& figure, Direction d);
  // Returns treu if we can move the given figure down
  bool MoveDownIfPossible(BoardFigure& figure) {
    return MoveIfPossible(figure, DOWN);
  }

  bool CanPlace(const FigureShape& figure, int row, int column) const;
  bool CanPlace(const BoardFigure& figure, int row, int column) const {
    return CanPlace(figure.shape, row, column);
  }
  bool CanPlace(const BoardFigure& figure) const {
    return CanPlace(figure.shape, figure.top_left_row, figure.top_left_column);
  }

  void AddFigure(const BoardFigure& figure);

  void AddToCells(const BoardFigure& figure);
  void RemoveFromCells(const BoardFigure& figure);
  void SetCells(const BoardFigure& figure, CellType value);

  std::unique_ptr<RandomFigureGenerator> figure_gen_;
  std::array<std::array<CellType, width>, height> cells_;
  std::vector<BoardFigure> figures_;
};
}

#endif
