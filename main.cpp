#include "states/states.hpp"
#include "engine/entitymanager.hpp"
#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"
#include "engine/utils/types.hpp"
int main() {
    // Inicializa la ventana
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    Manentity_type GE{};
    States game{};
    InitWindow(screenWidth, screenHeight, "ProyectoX");
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
    ToggleFullscreen();
    GuiLoadStyleDefault();
    Font customFont = LoadFont("resources/fonts/ARIAL.ttf");
    GuiSetFont(customFont);
    SetTargetFPS(60);
    HideCursor();
    game.GameMenu(GE);

    return 0;
}