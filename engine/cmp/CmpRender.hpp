#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <random>  // Para generación aleatoria mejorada

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE,
    PENTAGON,
    HEXAGON,
    OCTAGON,
    STAR
};

struct Shape {
    ShapeType type;
    Vector2 position;
    Color color;
    float size;
    bool hasBorder;
    Color borderColor;
    bool hasInnerShape;
    Color innerColor;
};

struct CmpRender{
    Shape sprite;
    CmpRender(float x, float y);
    CmpRender();
    Shape GenerateShape(Vector2 position);
    void DrawShape();
};