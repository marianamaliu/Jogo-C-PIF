#include "tela_inical.h"
#include "config.h"
#include <raylib.h>

void MostrarTelaInicial(Texture2D fundo){
    Rectangle src  = { 0, 0, (float)fundo.width, (float)fundo.height };
    Rectangle dest = { 0, 0, (float)JANELA_LARGURA, (float)JANELA_ALTURA };
    Vector2 origin = { 0, 0 };

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER)) break;

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(fundo, src, dest, origin, 0.0f, WHITE);
            DrawText("PACMAN", JANELA_LARGURA/2 - 80, JANELA_ALTURA/3, 60, YELLOW);
            DrawText("Pressione ENTER para jogar", JANELA_LARGURA/2 - 140, JANELA_ALTURA/2, 20, RAYWHITE);
        EndDrawing();
    }
}
