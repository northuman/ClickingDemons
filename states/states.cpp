#include "states.hpp"

//Funcion auxiliar
// Interpolación lineal entre a y b, usando t (entre 0.0 y 1.0)
float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

States::States(){
    currentScreen = MENU;
}

void States::GameMenu(Manentity_type& GE){
     // Bucle principal
    //menuBackground = LoadTexture("resources/backgrounds/PNG/Battleground1/Bright/Battleground1.png");

    auto& a1 = GE.createEntity_withCMPS<CmpRender>();
	GE.defineCMP<CmpRender>(a1, CmpRender{400,550});
    auto& a2 = GE.createEntity_withCMPS<CmpRender>();
	GE.defineCMP<CmpRender>(a2, CmpRender{700,650});
    auto& a3 = GE.createEntity_withCMPS<CmpRender>();
	GE.defineCMP<CmpRender>(a3, CmpRender{900,750});
    
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        BeginDrawing();
        //Dibujar fondo del menu
        ClearBackground(BLACK);
        float scaleX = static_cast<float>(GetScreenWidth()) / menuBackground.width;
        float scaleY = static_cast<float>(GetScreenHeight()) / menuBackground.height;
        DrawTextureEx(menuBackground, { 0, 0 }, 0.0f, std::max(scaleX, scaleY), WHITE);
        DrawBackground();
        GE.getEntityCMP<CmpRender>(a1).DrawShape();
        GE.getEntityCMP<CmpRender>(a2).DrawShape();
        GE.getEntityCMP<CmpRender>(a3).DrawShape();
        if (currentScreen == MENU) {
            MenuInicial(mouse);
        }else if (currentScreen == GAME) {
            Juego(GE);
        }else if(currentScreen == SETTINGS){
            Ajustes();
        }else if(currentScreen == BYE){
            break; // Cierra la ventana
        }
        cursor.DrawCursor();
        EndDrawing();
    }
    UnloadTexture(menuBackground); //Borramos de la memoria el fondo del menu
    CloseWindow(); // Cierra la ventana
}

void States::MenuInicial(Vector2& mouse) {
    const int titleFontSize = 70;
    const int buttonFontSize = 45;
    const int spacing = 50;
    const int marginX = 50;
    const int marginTop = 50;

    // Título con sombra para simular negrita
    DrawText("Proyecto X", marginX + 3, marginTop + 3, titleFontSize, GRAY);
    DrawText("Proyecto X", marginX, marginTop, titleFontSize, BLACK);

    int totalHeight = buttonCount * buttonFontSize + (buttonCount - 1) * spacing;
    int startY = (GetScreenHeight() * 3 / 4) - totalHeight / 2;

    float fadeSpeed = GetFrameTime() * 2.0f;
    float lineSpeed = GetFrameTime() * 6.0f;  // Más rápido para la línea
    float moveSpeed = GetFrameTime() * 6.0f;  // Más rápido para desplazamiento

    for (int i = 0; i < buttonCount; i++) {
        const char* label = (i == 0) ? "Jugar" :
                            (i == 1) ? "Ajustes" : "Salir";

        int posY = startY + i * (buttonFontSize + spacing);
        int textWidth = MeasureText(label, buttonFontSize);

        Rectangle textBounds = {
            static_cast<float>(marginX),
            static_cast<float>(posY),
            static_cast<float>(textWidth + 10),
            static_cast<float>(buttonFontSize)
        };

        bool hovered = CheckCollisionPointRec(mouse, textBounds);

        // Animaciones
        hoverAlphas[i] = hovered 
            ? Lerp(hoverAlphas[i], 1.0f, fadeSpeed)
            : Lerp(hoverAlphas[i], 0.0f, fadeSpeed);

        hoverLineProgress[i] = hovered
            ? Lerp(hoverLineProgress[i], 1.0f, lineSpeed)
            : Lerp(hoverLineProgress[i], 0.0f, lineSpeed);

        hoverOffsetX[i] = hovered
            ? Lerp(hoverOffsetX[i], 10.0f, moveSpeed)
            : Lerp(hoverOffsetX[i], 0.0f, moveSpeed);

        int drawX = marginX + static_cast<int>(hoverOffsetX[i]);

        // Sombra azul (3px desplazamiento)
        if (hoverAlphas[i] > 0.01f) {
            Color blueShadow = Fade(WHITE, hoverAlphas[i]);
            for (int offset = 1; offset <= 4; offset++) {
                DrawText(label, drawX + offset, posY + offset, buttonFontSize, blueShadow);
            }
        }

        // Negrita simulada
        DrawText(label, drawX + 2, posY + 2, buttonFontSize, GRAY);
        DrawText(label, drawX, posY, buttonFontSize, BLACK);

        // Línea azul animada
        if (hoverLineProgress[i] > 0.01f) {
            float lineWidth = textWidth * hoverLineProgress[i];
            DrawRectangle(drawX, posY + buttonFontSize + 5, static_cast<int>(lineWidth), 4, WHITE);
        }

        // Acción al hacer clic
        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (i == 0) currentScreen = GAME;
            else if (i == 1) currentScreen = SETTINGS;
            else if (i == 2) currentScreen = BYE;
        }
    }
}

void States::Juego(Manentity_type& GE){
    DrawText("¡Juego en progreso!", 200, 200, 30, GREEN);
    DrawText("Presiona B para volver", 200, 250, 20, GRAY);

    if (IsKeyPressed(KEY_B)) {
        currentScreen = MENU;
    }
}

void States::Ajustes() {
    const int titleFontSize = 60;
    const int labelFontSize = 35;
    const int optionFontSize = 30;
    const int marginX = 50;
    const int spacingY = 100;
    const int optionSpacingX = 20;
    const int controlStartX = 400;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título
    DrawText("AJUSTES", marginX + 2, 20 + 2, titleFontSize, GRAY);
    DrawText("AJUSTES", marginX, 20, titleFontSize, BLACK);

    int startY = 120;
    Vector2 mouse = GetMousePosition();
    float fadeSpeed = GetFrameTime() * 10.0f;

    // MONITOR
    {
        DrawText("Monitor", marginX + 1, startY + 1, labelFontSize, GRAY);
        DrawText("Monitor", marginX, startY, labelFontSize, BLACK);

        int x = controlStartX;
        int monitorCount = GetMonitorCount();
        for (int i = 0; i < monitorCount; i++) {
            const char* label = GetMonitorName(i);
            int textW = MeasureText(label, optionFontSize);
            Rectangle optionRect = { (float)x, (float)(startY), (float)textW, (float)optionFontSize };

            bool hovered = CheckCollisionPointRec(mouse, optionRect);
            static float hoverAnim[8] = { 0 };
            hoverAnim[i] = hovered ? Lerp(hoverAnim[i], 1.0f, fadeSpeed) : Lerp(hoverAnim[i], 0.0f, fadeSpeed);

            if (hoverAnim[i] > 0.01f)
                DrawText(label, x + 2, startY + 2, optionFontSize, Fade(WHITE, hoverAnim[i]));

            if (i == selectedMonitor) {
                DrawRectangle(x - 5, startY + optionFontSize + 5, textW + 10, 3, WHITE);
            }

            DrawText(label, x + 1, startY + 1, optionFontSize, GRAY);
            DrawText(label, x, startY, optionFontSize, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedMonitor = i;
            }

            x += textW + optionSpacingX;
        }

        startY += spacingY;
    }

    // MODO DE PANTALLA
    {
        DrawText("Modo Pantalla", marginX + 1, startY + 1, labelFontSize, GRAY);
        DrawText("Modo Pantalla", marginX, startY, labelFontSize, BLACK);

        const char* modos[2] = { "Pantalla Completa", "Ventana" };
        int x = controlStartX;
        for (int i = 0; i < 2; i++) {
            const char* label = modos[i];
            int textW = MeasureText(label, optionFontSize);
            Rectangle optionRect = { (float)x, (float)(startY), (float)textW, (float)optionFontSize };

            bool hovered = CheckCollisionPointRec(mouse, optionRect);
            static float hoverAnim[2] = { 0 };
            hoverAnim[i] = hovered ? Lerp(hoverAnim[i], 1.0f, fadeSpeed) : Lerp(hoverAnim[i], 0.0f, fadeSpeed);

            if (hoverAnim[i] > 0.01f)
                DrawText(label, x + 2, startY + 2, optionFontSize, Fade(WHITE, hoverAnim[i]));

            if (i == selectedDisplayMode) {
                DrawRectangle(x - 5, startY + optionFontSize + 5, textW + 10, 3, WHITE);
            }

            DrawText(label, x + 1, startY + 1, optionFontSize, GRAY);
            DrawText(label, x, startY, optionFontSize, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedDisplayMode = i;
            }

            x += textW + optionSpacingX;
        }

        startY += spacingY;
    }

    // RESOLUCION
    {
        DrawText("Resolución", marginX + 1, startY + 1, labelFontSize, GRAY);
        DrawText("Resolución", marginX, startY, labelFontSize, BLACK);

        int x = controlStartX;
        for (int i = 0; i < resolutionsCount; i++) {
            const char* label = resolutions[i].c_str();
            int textW = MeasureText(label, optionFontSize);
            Rectangle optionRect = { (float)x, (float)(startY), (float)textW, (float)optionFontSize };

            bool hovered = CheckCollisionPointRec(mouse, optionRect);
            static float hoverAnim[8] = { 0 };
            hoverAnim[i] = hovered ? Lerp(hoverAnim[i], 1.0f, fadeSpeed) : Lerp(hoverAnim[i], 0.0f, fadeSpeed);

            if (hoverAnim[i] > 0.01f)
                DrawText(label, x + 2, startY + 2, optionFontSize, Fade(WHITE, hoverAnim[i]));

            if (i == selectedResolution) {
                DrawRectangle(x - 5, startY + optionFontSize + 5, textW + 10, 3, WHITE);
            }

            DrawText(label, x + 1, startY + 1, optionFontSize, GRAY);
            DrawText(label, x, startY, optionFontSize, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedResolution = i;
            }

            x += textW + optionSpacingX;
        }

        startY += spacingY;
    }

    // BOTONES: Aplicar y Volver
    {
        const char* aplicarLabel = "Aplicar";
        const char* volverLabel = "Volver";
        int buttonFontSize = 40;

        static float aplicarHover = 0.0f;
        static float volverHover = 0.0f;

        int aplicarX = screenWidth - marginX - MeasureText(aplicarLabel, buttonFontSize);
        int aplicarY = screenHeight - 60;
        int volverX = marginX;
        int volverY = aplicarY;

        Rectangle aplicarRect = { (float)aplicarX, (float)aplicarY, (float)MeasureText(aplicarLabel, buttonFontSize), (float)buttonFontSize };
        Rectangle volverRect = { (float)volverX, (float)volverY, (float)MeasureText(volverLabel, buttonFontSize), (float)buttonFontSize };

        bool aplicarOver = CheckCollisionPointRec(mouse, aplicarRect);
        bool volverOver = CheckCollisionPointRec(mouse, volverRect);

        aplicarHover = aplicarOver ? Lerp(aplicarHover, 1.0f, fadeSpeed) : Lerp(aplicarHover, 0.0f, fadeSpeed);
        volverHover = volverOver ? Lerp(volverHover, 1.0f, fadeSpeed) : Lerp(volverHover, 0.0f, fadeSpeed);

        if (aplicarHover > 0.01f)
            DrawText(aplicarLabel, aplicarX + 2, aplicarY + 2, buttonFontSize, Fade(WHITE, aplicarHover));
        if (volverHover > 0.01f)
            DrawText(volverLabel, volverX + 2, volverY + 2, buttonFontSize, Fade(WHITE, volverHover));

        DrawText(aplicarLabel, aplicarX + 1, aplicarY + 1, buttonFontSize, GRAY);
        DrawText(aplicarLabel, aplicarX, aplicarY, buttonFontSize, BLACK);

        DrawText(volverLabel, volverX + 1, volverY + 1, buttonFontSize, GRAY);
        DrawText(volverLabel, volverX, volverY, buttonFontSize, BLACK);

        // ACCIONES
        if (aplicarOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            size_t xPos = resolutions[selectedResolution].find('x');
            std::string anchoStr = resolutions[selectedResolution].substr(0, xPos);
            std::string altoStr = resolutions[selectedResolution].substr(xPos + 1);
            int width = std::stoi(anchoStr);
            int height = std::stoi(altoStr);

            CloseWindow();
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
            InitWindow(width, height, "ProyectoX");
            ClearWindowState(FLAG_WINDOW_RESIZABLE);
            GuiLoadStyleDefault();

            Font customFont = LoadFont("resources/fonts/ARIAL.ttf");
            GuiSetFont(customFont);

            SetWindowMonitor(selectedMonitor);
            if (IsWindowFullscreen() && selectedDisplayMode == 1) ToggleFullscreen();
            else if (!IsWindowFullscreen() && selectedDisplayMode == 0) ToggleFullscreen();
        
            HideCursor();
            currentScreen = SETTINGS;
        }

        if (volverOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentScreen = MENU;
        }
    }
}

void States::DrawBackground() {
    if (!backgroundInitialized) {
        for (int i = 0; i < maxSquares; i++) {
            squares[i].position = { 
                static_cast<float>(GetRandomValue(0, GetScreenWidth())),
                static_cast<float>(GetRandomValue(0, GetScreenHeight()))
            };
            squares[i].size = GetRandomValue(5, 20);
            squares[i].speed = GetRandomValue(50, 200) / 100.0f;
            int alpha = GetRandomValue(50, 120);
            squares[i].color = (Color){ 100, 100, 255, static_cast<unsigned char>(alpha) }; // Azul con transparencia
        }
        backgroundInitialized = true;
    }

    for (int i = 0; i < maxSquares; i++) {
        squares[i].position.y += squares[i].speed;

        if (squares[i].position.y > GetScreenHeight()) {
            squares[i].position.y = -squares[i].size;
            squares[i].position.x = static_cast<float>(GetRandomValue(0, GetScreenWidth()));
        }

        DrawRectangleV(squares[i].position, { squares[i].size, squares[i].size }, squares[i].color);
    }
}