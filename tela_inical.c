#include "tela_inical.h"

void MostrarTelaInicial(Texture2D fundo){

    int screenWidth  = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // retângulos pra esticar a imagem na tela inteira
    Rectangle src  = { 0, 0, (float)fundo.width, (float)fundo.height };
    Rectangle dest = { 0, 0, (float)JANELA_LARGURA, (float)JANELA_ALTURA };
    Vector2 origin = { 0, 0 };

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER)) {
            // Sai da tela inicial e volta pra main
            break;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(fundo, src, dest, origin, 0.0f, WHITE);

            const char *titulo = "PACMAN";
            int fontTitulo = 60;
            int larguraTitulo = MeasureText(titulo, fontTitulo);

            DrawText(titulo, GetScreenWidth()/2 - larguraTitulo/2, GetScreenHeight()/3, fontTitulo, YELLOW);
            
            const char *subtitulo = "Pressione ENTER para jogar";
            int fontSub = 20;
            int larguraSub = MeasureText(subtitulo, fontSub);

            DrawText(subtitulo, GetScreenWidth()/2 - larguraSub/2, GetScreenHeight()/2, fontSub, RAYWHITE);
        EndDrawing();
    }
}