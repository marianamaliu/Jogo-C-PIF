// jogo.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>

#include "struct.h"
#include "config.h"
#include "tela_inical.h"
#include "mapa.h"
#include "pacman.h"
#include "fantasma.h"
#include "itens.h"
#include "colisao.h"

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

static void inicalizar(Jogo *jogo);
static void liberar(Jogo *jogo);
static void ativar_frightened(Jogo *jogo, float duracao_seconds);

int main(void) {
    srand((unsigned)time(NULL));
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Pacman com raylib em C");
    SetTargetFPS(10);

    Texture2D fundoMenu = LoadTexture("assets/menu.png");
    MostrarTelaInicial(fundoMenu);
    UnloadTexture(fundoMenu);

    Jogo jogo = {0};
    inicalizar(&jogo);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (IsKeyPressed(KEY_F)) ativar_frightened(&jogo, 6.0f);

        if (jogo.jogo_ativo) {
            atualizar_pacman(&jogo);
            atualizar_fantasma(&jogo);
            colisao(&jogo);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (jogo.jogo_ativo) {

                // 1 — MAPA (fundo)
                desenhar_mapa(&jogo);

                // 2 — COMIDA (item aleatório) e bolinhas do mapa
                desenhar_comida(jogo.comida);

                // bolinhas pequenas do mapa já são desenhadas dentro de desenhar_mapa (quando tile == '.')
                // 3 — FRUTA (se existir)
                if (jogo.inicializarFruta) desenhar_fruta(jogo.fruta);

                // 4 — FANTASMAS
                for (int i = 0; i < jogo.qntd_fantasmas; i++)
                    desenhar_fantasma(jogo.fantasmas[i], jogo.fantasmas[i].cor);

                // 5 — PACMAN (sempre por último para evitar artefatos visuais)
                desenhar_pacman(jogo.pacman);

                // HUD
                char buf[64];
                snprintf(buf, sizeof(buf), "Pontos: %d", jogo.pontuacao_atual);
                DrawText(buf, 8, 8, 18, RAYWHITE);

            } else {
                DrawText("GAME OVER!", JANELA_LARGURA/2 - 100, JANELA_ALTURA/2 - 20, 40, RED);
            }
        EndDrawing();
    }

    liberar(&jogo);
    CloseWindow();
    return 0;
}

static void inicalizar(Jogo *jogo) {
    jogo->mapa.largura = LARGURA;
    jogo->mapa.altura  = ALTURA;
    jogo->mapa.mapa = malloc(ALTURA * sizeof(char*));
    for (int i = 0; i < ALTURA; i++) {
        jogo->mapa.mapa[i] = malloc((LARGURA + 1) * sizeof(char));
        strcpy(jogo->mapa.mapa[i], MAPA[i]);
    }

    jogo->jogo_ativo = true;
    jogo->pontuacao_atual = 0;
    jogo->pontoComida = 0;
    jogo->maxComida = 50;
    jogo->inicializarFruta = false;

    jogo->pacman.x = 2; jogo->pacman.y = 1; jogo->pacman.dx = 1; jogo->pacman.dy = 0; jogo->pacman.vivo = true;

    jogo->qntd_fantasmas = 4;
    jogo->fantasmas = malloc(jogo->qntd_fantasmas * sizeof(Fantasma));
    Color cores[] = { RED, PINK, ORANGE, SKYBLUE };
    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        jogo->fantasmas[i].x = 20 + i;
        jogo->fantasmas[i].y = 8;
        jogo->fantasmas[i].dx = -1;
        jogo->fantasmas[i].dy = 0;
        jogo->fantasmas[i].spawnX = jogo->fantasmas[i].x;
        jogo->fantasmas[i].spawnY = jogo->fantasmas[i].y;
        jogo->fantasmas[i].modo = MODO_CHASE;
        jogo->fantasmas[i].tipo = (TipoFantasma)i;
        jogo->fantasmas[i].velocidade = 1.0f;
        jogo->fantasmas[i].timer_modo = 0.0f;
        jogo->fantasmas[i].targetX = jogo->fantasmas[i].x;
        jogo->fantasmas[i].targetY = jogo->fantasmas[i].y;
        jogo->fantasmas[i].cor = cores[i % 4];
    }

    gerar_comida(jogo);
}

static void ativar_frightened(Jogo *jogo, float duracao_seconds) {
    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];
        f->modo = MODO_FRIGHTENED;
        f->timer_modo = duracao_seconds;
        f->velocidade = 0.5f;
        f->cor = BLUE;
        f->dx = -f->dx; f->dy = -f->dy;
    }
}

static void liberar(Jogo *jogo) {
    if (jogo->mapa.mapa) {
        for (int i = 0; i < jogo->mapa.altura; i++) free(jogo->mapa.mapa[i]);
        free(jogo->mapa.mapa); jogo->mapa.mapa = NULL;
    }
    if (jogo->fantasmas) { free(jogo->fantasmas); jogo->fantasmas = NULL; }
}
