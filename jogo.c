#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>

#include "config.h"
#include "tela_inical.h"
#include "mapa.h"
#include "pacman.h"
#include "fantasma.h"
#include "itens.h"

// layout do mapa (mesmo que você usava)
static const char *MAPA[ALTURA] = {
    "########################################",
    "#...................#..................#",
    "#..####....######...#...######...####..#",
    "#..#  #....#    #...#...#    #...#  #..#",
    "#..####....######.......######...####..#",
    "#......................................#",
    "#..####....###.............###...####..#",
    "#..#  #....# ####.......#### #...#  #..#",
    "#..####....#    #.......#    #...####..#",
    "#..........# ####.......#### #.........#",
    "#..####....# #......#......# #...####..#",
    "#..........###......#......###.........#",
    "#...................#..................#",
    "#..####....######...#...######...####..#",
    "#..#  #....#    #...#...#    #...#  #..#",
    "#..####....######.......######...####..#",
    "#...................#..................#",
    "#..######...####....#...####...######..#",
    "#..#    #...#  #....#...#  #...#    #..#",
    "#..######...####........####...######..#",
    "#......................................#",
    "########################################"
};

void inicalizar(Jogo *jogo);
void liberar(Jogo *jogo);

int main(void){
    Jogo jogo = {0};

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Pacman com raylib em C");
    SetTargetFPS(10);
    srand((unsigned)time(NULL));

    Texture2D fundoMenu = LoadTexture("assets/menu.png");
    MostrarTelaInicial(fundoMenu);
    UnloadTexture(fundoMenu);

    inicalizar(&jogo);

    while (!WindowShouldClose()) {
        if (jogo.jogo_ativo) {
            atualizar_pacman(&jogo);
            atualizar_fantasma(&jogo);
            colisao(&jogo);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (jogo.jogo_ativo) {
                desenhar_mapa(&jogo);
                desenhar_pacman(jogo.pacman);
                desenhar_comida(jogo.comida);

                if (jogo.inicializarFruta) desenhar_fruta(jogo.fruta);

                for (int i = 0; i < jogo.qntd_fantasmas; i++) {
                    desenhar_fantasma(jogo.fantasmas[i], jogo.fantasmas[i].cor);
                }

                DrawText(TextFormat("Score: %i", jogo.pontuacao_atual), 8, JANELA_ALTURA - 24, 20, WHITE);
            } else {
                DrawText("GAME OVER!", JANELA_LARGURA/2 - 100, JANELA_ALTURA/2 - 20, 40, RED);
            }
        EndDrawing();
    }

    liberar(&jogo);
    CloseWindow();
    return 0;
}

void inicalizar(Jogo *jogo){
    jogo->mapa.largura = LARGURA;
    jogo->mapa.altura = ALTURA;
    jogo->mapa.mapa = (char**)malloc(ALTURA * sizeof(char*));
    for (int i = 0; i < ALTURA; i++) {
        jogo->mapa.mapa[i] = (char*)malloc((LARGURA + 1) * sizeof(char));
        strcpy(jogo->mapa.mapa[i], MAPA[i]);
    }

    jogo->jogo_ativo = true;
    jogo->pontuacao_atual = 0;
    jogo->pontoComida = 0;
    jogo->maxComida = 50;
    jogo->inicializarFruta = false;

    // Pacman
    jogo->pacman.x = 2;
    jogo->pacman.y = 1;
    jogo->pacman.dx = 1;
    jogo->pacman.dy = 0;

    // Fantasmas
    jogo->qntd_fantasmas = 4;
    jogo->fantasmas = (Fantasma*)malloc(jogo->qntd_fantasmas * sizeof(Fantasma));
    Color cor_fantasma[] = { RED, PINK, ORANGE, SKYBLUE };
    int total_cores = sizeof(cor_fantasma) / sizeof(cor_fantasma[0]);

    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        jogo->fantasmas[i].x = 20 + i;
        jogo->fantasmas[i].y = 8;
        // direções iniciais variadas (evitam que todos se movam igual)
        if (i == 0) { jogo->fantasmas[i].dx = 1; jogo->fantasmas[i].dy = 0; }
        else if (i == 1) { jogo->fantasmas[i].dx = 0; jogo->fantasmas[i].dy = -1; }
        else if (i == 2) { jogo->fantasmas[i].dx = -1; jogo->fantasmas[i].dy = 0; }
        else { jogo->fantasmas[i].dx = 0; jogo->fantasmas[i].dy = 1; }

        jogo->fantasmas[i].cor = cor_fantasma[i % total_cores];
        jogo->fantasmas[i].modo = PERSEGUIR;
        jogo->fantasmas[i].velocidade = 1.0f;
        jogo->fantasmas[i].timer_modo = 0.0f;
    }

    gerar_comida(jogo);
}

void liberar(Jogo *jogo){
    for (int i = 0; i < jogo->mapa.altura; i++) free(jogo->mapa.mapa[i]);
    free(jogo->mapa.mapa);
    jogo->mapa.mapa = NULL;

    if (jogo->fantasmas != NULL) { free(jogo->fantasmas); jogo->fantasmas = NULL; }
}
