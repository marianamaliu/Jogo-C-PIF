#include "fantasma.h"
#include "struct.h"
#include <math.h>
#include <stdlib.h>

// helpers
static float distancia_int(int x1, int y1, int x2, int y2) {
    float dx = (float)(x1 - x2);
    float dy = (float)(y1 - y2);
    return sqrtf(dx*dx + dy*dy);
}
static bool dentro_mapa(int x, int y) {
    return (x >= 0 && x < LARGURA && y >= 0 && y < ALTURA);
}

// desenhar
void desenhar_fantasma(Fantasma fantasma, Color cor) {
    float pixelX = (float)(fantasma.x * TILE_SIZE);
    float pixelY = (float)(fantasma.y * TILE_SIZE);
    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raio = TILE_SIZE/2 - 2;

    DrawRectangle((int)pixelX + 1, (int)pixelY + raio, TILE_SIZE - 2, raio, cor);
    DrawCircle(centroX, (int)pixelY + raio, raio, cor);

    for (int i = 0; i < 4; i++) {
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / 4)) + (TILE_SIZE / 8),
                       pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4) };
        DrawTriangle(p1, p2, p3, cor);
    }

    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
}

// alvo por tipo
static void calcular_alvo(Fantasma *f, Jogo *j) {
    Pacman *p = &j->pacman;
    switch (f->tipo) {
        case BLINKY:
            f->targetX = p->x;
            f->targetY = p->y;
            break;
        case PINKY:
            f->targetX = p->x + p->dx * 4;
            f->targetY = p->y + p->dy * 4;
            if (f->targetX < 0) f->targetX = 0;
            if (f->targetX >= LARGURA) f->targetX = LARGURA-1;
            if (f->targetY < 0) f->targetY = 0;
            if (f->targetY >= ALTURA) f->targetY = ALTURA-1;
            break;
        case INKY: {
            int px = p->x + p->dx * 2;
            int py = p->y + p->dy * 2;
            Fantasma *blinky = &j->fantasmas[BLINKY];
            f->targetX = blinky->x + (px - blinky->x) * 2;
            f->targetY = blinky->y + (py - blinky->y) * 2;
            if (f->targetX < 0) f->targetX = 0;
            if (f->targetX >= LARGURA) f->targetX = LARGURA-1;
            if (f->targetY < 0) f->targetY = 0;
            if (f->targetY >= ALTURA) f->targetY = ALTURA-1;
        } break;
        case CLYDE:
            if (distancia_int(f->x, f->y, p->x, p->y) < 8.0f) {
                f->targetX = 1;
                f->targetY = ALTURA - 2;
            } else {
                f->targetX = p->x;
                f->targetY = p->y;
            }
            break;
        default:
            f->targetX = p->x;
            f->targetY = p->y;
    }
}

// escolhe direção que minimiza distancia (evita reversão se possível)
static void escolher_direcao(Fantasma *f, Jogo *j) {
    int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    int proibidoX = -f->dx, proibidoY = -f->dy;
    float melhor = 1e9f;
    int melhorDx = f->dx, melhorDy = f->dy;
    bool achou = false;

    for (int i = 0; i < 4; i++) {
        int dx = dirs[i][0], dy = dirs[i][1];
        if (dx == proibidoX && dy == proibidoY) continue;
        int nx = f->x + dx, ny = f->y + dy;
        if (!dentro_mapa(nx, ny)) continue;
        if (j->mapa.mapa[ny][nx] == '#') continue;
        float d = distancia_int(nx, ny, f->targetX, f->targetY);
        if (d < melhor) { melhor = d; melhorDx = dx; melhorDy = dy; achou = true; }
    }

    if (!achou) {
        for (int i = 0; i < 4; i++) {
            int dx = dirs[i][0], dy = dirs[i][1];
            int nx = f->x + dx, ny = f->y + dy;
            if (!dentro_mapa(nx, ny)) continue;
            if (j->mapa.mapa[ny][nx] == '#') continue;
            melhorDx = dx; melhorDy = dy;
            break;
        }
    }

    f->dx = melhorDx; f->dy = melhorDy;
}

static void atualizar_fantasma_um(Fantasma *f, Jogo *j) {
    if (f->modo == MODO_FRIGHTENED) {
        f->timer_modo -= GetFrameTime();
        if (f->timer_modo <= 0.0f) {
            f->modo = MODO_CHASE;
            f->velocidade = 1.0f;
            // restaurar cor se quiser (faz no chamador se preferir)
        }
        // movimento aleatório válido
        int tries = 0, dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
        while (tries < 8) {
            int r = rand() % 4;
            int nx = f->x + dirs[r][0], ny = f->y + dirs[r][1];
            if (dentro_mapa(nx, ny) && j->mapa.mapa[ny][nx] != '#') {
                f->dx = dirs[r][0]; f->dy = dirs[r][1]; break;
            }
            tries++;
        }
    } else {
        calcular_alvo(f, j);
        escolher_direcao(f, j);
    }

    int nx = f->x + f->dx, ny = f->y + f->dy;
    if (dentro_mapa(nx, ny) && j->mapa.mapa[ny][nx] != '#') {
        f->x = nx; f->y = ny;
    } else {
        // fallback: tenta outra direção válida
        int dirs_alt[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
        for (int i = 0; i < 4; i++) {
            int tx = f->x + dirs_alt[i][0], ty = f->y + dirs_alt[i][1];
            if (!dentro_mapa(tx, ty)) continue;
            if (j->mapa.mapa[ty][tx] == '#') continue;
            f->dx = dirs_alt[i][0]; f->dy = dirs_alt[i][1];
            f->x = tx; f->y = ty;
            break;
        }
    }
}

void atualizar_fantasma(Jogo *jogo) {
    if (!jogo || !jogo->fantasmas) return;
    for (int i = 0; i < jogo->qntd_fantasmas; i++) atualizar_fantasma_um(&jogo->fantasmas[i], jogo);
}

void ativar_modo_assustado(Jogo *jogo) {
    if (!jogo || !jogo->fantasmas) return;
    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];
        f->modo = MODO_FRIGHTENED;
        f->timer_modo = 6.0f; // segundos
        f->velocidade = 0.5f;
        // inverter direção ao entrar em frightened (opcional)
        f->dx = -f->dx; f->dy = -f->dy;
    }
}
