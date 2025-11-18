#include "pacman.h"
#include <raylib.h>
#include <math.h>

void atualizar_pacman(Jogo *jogo)
{
    Pacman *p = &jogo->pacman;

    if (IsKeyDown(KEY_UP))    { p->dx = 0;  p->dy = -1; }
    if (IsKeyDown(KEY_DOWN))  { p->dx = 0;  p->dy = 1; }
    if (IsKeyDown(KEY_LEFT))  { p->dx = -1; p->dy = 0; }
    if (IsKeyDown(KEY_RIGHT)) { p->dx = 1;  p->dy = 0; }

    int newX = p->x + p->dx;
    int newY = p->y + p->dy;

    if (newX >= 0 && newX < LARGURA && newY >= 0 && newY < ALTURA &&
        jogo->mapa.mapa[newY][newX] != '#') {
        p->x = newX; p->y = newY;
    }
}

void desenhar_pacman(Pacman p)
{
    float xc = p.x * TILE_SIZE + TILE_SIZE / 2;
    float yc = p.y * TILE_SIZE + TILE_SIZE / 2;
    float r  = TILE_SIZE / 2 - 1;

    float anim = (sinf(GetTime() * 10) + 1) / 2;
    float abertura = anim * 35 + 5;

    float angInicio = abertura;
    float angFim    = 360 - abertura;

    if (p.dx == -1) {
        angInicio = 180 + abertura; angFim = 180 - abertura;
    } else if (p.dy == -1) {
        angInicio = 270 + abertura; angFim = 270 - abertura;
    } else if (p.dy == 1) {
        angInicio = 90 + abertura; angFim = 90 - abertura;
    }

    DrawCircleSector((Vector2){xc, yc}, r, angInicio, angFim, 32, YELLOW);
}
