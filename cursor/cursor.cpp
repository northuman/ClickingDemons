#include "cursor.hpp"

Cursor::Cursor(){
    HideCursor();
}

void Cursor::DrawCursor() {
    Vector2 mouse = GetMousePosition();
    float radius = 8.0f;
    
    // Sombra/borde azul translúcido
    DrawCircleLines(mouse.x, mouse.y, radius + 4.0f, Fade(BLUE, 0.4f));

    // Círculo blanco central
    DrawCircle(mouse.x, mouse.y, radius, WHITE);
}