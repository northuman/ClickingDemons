#include "CmpRender.hpp"
CmpRender::CmpRender(){
    sprite = GenerateShape(Vector2{500,500});
}
CmpRender::CmpRender(float x, float y){
    sprite = GenerateShape(Vector2{x,y});
}

// Función para generar un color neón aleatorio
Color GenerateNeonColor(std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(100, 255); // Colores brillantes
    int r = dist(rng);
    int g = dist(rng);
    int b = dist(rng);
    return (Color){r, g, b, 255}; // Color con alto brillo
}

// Función para generar una forma aleatoria
Shape CmpRender::GenerateShape(Vector2 position) {
    // Generar tipo aleatorio de forma
    std::random_device rd;
    std::mt19937 rng(rd()); // Generador de números aleatorios
    ShapeType type = static_cast<ShapeType>(rng() % 8); // Genera un valor entre 0 y 7

    Shape shape;
    shape.type = type;
    shape.position = position;
    shape.color = GenerateNeonColor(rng); // Color de la forma (neón)
    shape.size = static_cast<float>(rng() % 50 + 50); // Tamaño aleatorio entre 50 y 100
    shape.hasBorder = rng() % 2 == 0; // Aleatoriamente decide si tiene borde
    shape.borderColor = GenerateNeonColor(rng); // Color de borde
    shape.hasInnerShape = rng() % 2 == 0; // Aleatoriamente decide si tiene forma interna
    shape.innerColor = GenerateNeonColor(rng); // Color de forma interna

    return shape;
}

// Funciones para dibujar las formas

void DrawCircleShape(Shape& shape) {
    DrawCircleV(shape.position, shape.size, shape.color); // Dibuja el círculo

    if (shape.hasBorder) {
        DrawCircleLines(shape.position.x, shape.position.y, shape.size, shape.borderColor); // Borde de círculo
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.6f; // Tamaño más pequeño para forma interna
        DrawCircleV(shape.position, innerSize, shape.innerColor); // Forma interna
    }
}

void DrawRectangleShape(Shape& shape) {
    // Dibuja el rectángulo
    DrawRectangleV(shape.position, (Vector2){shape.size, shape.size * 2}, shape.color);

    if (shape.hasBorder) {
        DrawRectangleLines(shape.position.x, shape.position.y, shape.size, shape.size * 2, shape.borderColor); // Borde
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.5f; // Tamaño más pequeño para forma interna
        DrawRectangleV((Vector2){shape.position.x + (shape.size * 0.25f), shape.position.y + (shape.size * 0.25f)}, (Vector2){innerSize, innerSize * 2}, shape.innerColor); // Rectángulo interno
    }
}

void DrawTriangleShape(Shape& shape) {
    Vector2 p1 = {shape.position.x, shape.position.y - shape.size};
    Vector2 p2 = {shape.position.x - shape.size, shape.position.y + shape.size};
    Vector2 p3 = {shape.position.x + shape.size, shape.position.y + shape.size};
    DrawTriangle(p1, p2, p3, shape.color); // Dibuja el triángulo

    if (shape.hasBorder) {
        DrawTriangleLines(p1, p2, p3, shape.borderColor); // Borde del triángulo
    }

    if (shape.hasInnerShape) {
        Vector2 p4 = {shape.position.x, shape.position.y};
        DrawTriangle(p1, p2, p4, shape.innerColor); // Forma interna triangular
    }
}

void DrawPolygonShape(Shape& shape, int sides) {
    Vector2 points[sides];
    for (int i = 0; i < sides; ++i) {
        points[i] = (Vector2){
            shape.position.x + shape.size * cos(i * 2 * PI / sides),
            shape.position.y + shape.size * sin(i * 2 * PI / sides)
        };
    }
    DrawPoly(shape.position, sides, shape.size,0, shape.color); // Dibuja el polígono

    if (shape.hasBorder) {
        DrawPolyLines(shape.position, sides, shape.size,0, shape.borderColor); // Borde del polígono
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.6f; // Tamaño más pequeño para forma interna
        DrawPoly(shape.position, sides, innerSize,0, shape.innerColor); // Forma interna
    }
}

void DrawStarShape(Shape& shape) {
    Vector2 points[10];
    for (int i = 0; i < 10; ++i) {
        float radius = (i % 2 == 0) ? shape.size : shape.size / 2;
        points[i] = (Vector2){
            shape.position.x + radius * cos(i * 2 * PI / 10),
            shape.position.y + radius * sin(i * 2 * PI / 10)
        };
    }
    DrawPoly(shape.position, 10, shape.size,0, shape.color); // Dibuja la estrella

    if (shape.hasBorder) {
        DrawPolyLines(shape.position, 10, shape.size,0, shape.borderColor); // Borde de la estrella
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.5f; // Tamaño más pequeño para forma interna
        DrawPoly(shape.position, 10, innerSize,0, shape.innerColor); // Forma interna
    }
}

// Función para dibujar una forma
void CmpRender::DrawShape() {
    switch (sprite.type) {
        case ShapeType::CIRCLE:
            DrawCircleShape(sprite);
            break;
        case ShapeType::RECTANGLE:
            DrawRectangleShape(sprite);
            break;
        case ShapeType::TRIANGLE:
            DrawTriangleShape(sprite);
            break;
        case ShapeType::PENTAGON:
            DrawPolygonShape(sprite, 5); // 5 lados para el pentágono
            break;
        case ShapeType::HEXAGON:
            DrawPolygonShape(sprite, 6); // 6 lados para el hexágono
            break;
        case ShapeType::OCTAGON:
            DrawPolygonShape(sprite, 8); // 8 lados para el octágono
            break;
        case ShapeType::STAR:
            DrawStarShape(sprite);
            break;
    }
}