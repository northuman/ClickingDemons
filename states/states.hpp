#pragma once
#include <raylib.h>
#include "../cursor/cursor.hpp"
#include "../lib/raygui.h"
#include "../gui.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include "../engine/utils/types.hpp"
//Game screens
enum GameScreen { MENU, GAME, SETTINGS, BYE };

class States{
    public:
    States();    
    void GameMenu(Manentity_type& GE);

    private:
    void MenuInicial(Vector2& mouse);
    void Juego(Manentity_type& GE);
    void Ajustes();
    void DrawBackground();

    Cursor cursor{};
    GameScreen currentScreen;
    const int offsetX = 50;  // Mover todo hacia la derecha
    const int offsetY = 50;  // Mover todo hacia abajo
    int selectedMonitor = 0; // Monitor seleccionado
    int selectedResolution = 2; // Resolución seleccionada
    std::string resolutions[4] = { "1280x720", "1600x900", "1920x1080"}; // Listas de opciones de resoluciones
    const int resolutionsCount = sizeof(resolutions) / sizeof(resolutions[0]);        // Cantidad de resoluciones permitidas
    int selectedDisplayMode = 0; // 0 = Pantalla completa, 1 = Ventana
    bool displayModeDropdownActive = false;
    const char* displayModeList = "Pantalla completa;Ventana";
    Texture2D menuBackground;
    float hoverAlphas[buttonCount] = { 0.0f, 0.0f, 0.0f }; // Animación por botón
    float hoverLineProgress[3] = {0};   // Para la línea inferior del botón
    float hoverOffsetX[3] = {0};  // Para el desplazamiento del botón
    static const int maxSquares = 100; //Densidad de la lluvia
    struct Square {
        Vector2 position;
        float size;
        float speed;
        Color color;
    }; // Lluvia
    Square squares[maxSquares]; //Lluvia
    bool backgroundInitialized = false; //Control del fondo dinamico del menu
};
