#ifndef TETRIS_FIGURES_H_
#define TETRIS_FIGURES_H_

#include <cstdlib>
#include <map>
#include <vector>

namespace tetris {

typedef bool CellType;
typedef std::vector<std::vector<CellType>> FigureShape;
typedef enum { I, SQUARE, L, T, SKEW_RIGHT, SKEW_LEFT, NUM_TYPES } FigureType;

struct Figure {
  const FigureType type;
  const FigureShape& shape;
  int orientation;

  Figure(FigureType type, const FigureShape& shape, int orientation)
      : type(type), shape(shape), orientation(orientation) {}

  int Height() const { return shape.size(); }
  int Width() const { return shape[0].size(); }
};

// Creates and Rotates Figures in an efficient way.
//
// FigureManager must outlive Figures that were returned by it.
class FigureManager {
 public:
  FigureManager();

  // Returns a figure with the given type and orientation.
  Figure MakeFigure(const FigureType type, const int orientation) {
    return Figure{type, shapes_.at(orientation).at(type), orientation};
  }

  // Returns a random figure with up orientation.
  inline Figure GetRandomUpFigure() const {
    auto type = static_cast<FigureType>(std::rand() % NUM_TYPES);
    return Figure{type, shapes_.at(0).at(type), 0};
  }

  // Rotates the given figure clockwise
  Figure GetRotated(const Figure& f) const {
    int new_orientation = (f.orientation + 1) % 4;
    return Figure{f.type, shapes_.at(new_orientation).at(f.type),
                  new_orientation};
  }

 private:
  FigureShape Rotate(const FigureShape& shape);

  std::vector<std::map<FigureType, FigureShape>> shapes_;
};
}
#endif
