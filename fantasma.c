#include "fantasma.h"
#include <math.h>
#include <stdlib.h>

// ---- Parâmetros (ajuste se quiser) ----
static const float TEMPO_SCATTER = 7.0f;
static const float TEMPO_CHASE   = 20.0f;
#define TEMPO_ASSUSTADO 6.0f

// Helper: verifica se a célula é parede
static bool eh_parede(Jogo *jogo, int x, int y) {
    if (x < 0 || x >= LARGURA || y < 0 || y >= ALTURA) return true;
    return (jogo->mapa.mapa[y][x] == '#');
}

// ------------------ Desenhar fantasma (seu estilo) ------------------
void desenhar_fantasma(Fantasma fantasma, Color CorFantasma){
    float pixelX = (float)(fantasma.x * TILE_SIZE);
    float pixelY = (float)(fantasma.y * TILE_SIZE);
    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raio = TILE_SIZE/2 - 2;

    DrawRectangle((int)pixelX + 1, (int)pixelY + raio, TILE_SIZE - 2, raio, CorFantasma);
    DrawCircle(centroX, (int)pixelY + raio, raio, CorFantasma);

    for(int i = 0; i < 4; i++){
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / 4)) + (TILE_SIZE / 8),
                       pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4) };
        DrawTriangle(p1, p2, p3, CorFantasma);
    }

    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
}

// ------------------ Alvos clássicos ------------------
// Blinky: diretamente no Pac-Man
static void set_alvo_blinky(Jogo *jogo, Fantasma *f, int *ax, int *ay) {
    *ax = jogo->pacman.x;
    *ay = jogo->pacman.y;
}

// Pinky: 4 tiles à frente (com o "bug" original para cima)
static void set_alvo_pinky(Jogo *jogo, Fantasma *f, int *ax, int *ay) {
    int dx = jogo->pacman.dx;
    int dy = jogo->pacman.dy;
    *ax = jogo->pacman.x + dx * 4;
    *ay = jogo->pacman.y + dy * 4;

    // Bug clássico: quando Pac-Man está apontando para cima, o alvo desloca 4 para a esquerda também.
    if (dx == 0 && dy == -1) {
        *ax = jogo->pacman.x - 4;
        *ay = jogo->pacman.y - 4;
    }

    if (*ax < 0) *ax = 0; if (*ax >= LARGURA) *ax = LARGURA-1;
    if (*ay < 0) *ay = 0; if (*ay >= ALTURA) *ay = ALTURA-1;
}

// Inky: usa Blinky + ponto 2 tiles à frente do Pac-Man
static void set_alvo_inky(Jogo *jogo, Fantasma *f, Fantasma *blinky, int *ax, int *ay) {
    int px = jogo->pacman.x + jogo->pacman.dx * 2;
    int py = jogo->pacman.y + jogo->pacman.dy * 2;

    int vx = px - blinky->x;
    int vy = py - blinky->y;

    *ax = blinky->x + 2 * vx;
    *ay = blinky->y + 2 * vy;

    if (*ax < 0) *ax = 0; if (*ax >= LARGURA) *ax = LARGURA-1;
    if (*ay < 0) *ay = 0; if (*ay >= ALTURA) *ay = ALTURA-1;
}

// Clyde: se longe > 8 tiles persegue, se perto (<=8) vai para cantinho (scatter)
static void set_alvo_clyde(Jogo *jogo, Fantasma *f, int *ax, int *ay) {
    int dx = jogo->pacman.x - f->x;
    int dy = jogo->pacman.y - f->y;
    int dist2 = dx*dx + dy*dy;
    if (dist2 > 64) { // > 8 tiles (8^2 = 64)
        *ax = jogo->pacman.x; *ay = jogo->pacman.y;
    } else {
        *ax = 1; *ay = ALTURA - 2; // canto inferior esquerdo
    }
}

// ------------------ Escolher direção (minimiza distância quadrada) ------------------
static void escolher_direcao(Fantasma *f, Jogo *jogo, int alvoX, int alvoY) {
    int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int opx = -f->dx, opy = -f->dy; // direção oposta (evitar 180)
    float melhor = 1e9f;
    int melhor_dx = 0, melhor_dy = 0;
    bool achou = false;

    for (int i = 0; i < 4; i++) {
        int dx = dirs[i][0], dy = dirs[i][1];
        if (dx == opx && dy == opy) continue; // evita 180º
        int nx = f->x + dx, ny = f->y + dy;
        if (nx < 0 || nx >= LARGURA || ny < 0 || ny >= ALTURA) continue;
        if (eh_parede(jogo, nx, ny)) continue;
        float dist = (alvoX - nx)*(alvoX - nx) + (alvoY - ny)*(alvoY - ny);
        if (dist < melhor) {
            melhor = dist;
            melhor_dx = dx; melhor_dy = dy;
            achou = true;
        }
    }

    if (!achou) {
        // permite reversão se não há outra opção
        for (int i = 0; i < 4; i++) {
            int dx = dirs[i][0], dy = dirs[i][1];
            int nx = f->x + dx, ny = f->y + dy;
            if (nx < 0 || nx >= LARGURA || ny < 0 || ny >= ALTURA) continue;
            if (eh_parede(jogo, nx, ny)) continue;
            melhor_dx = dx; melhor_dy = dy;
            break;
        }
    }

    f->dx = melhor_dx; f->dy = melhor_dy;
}

// ------------------ Modo global alterna SCATTER / CHASE ------------------
static void obter_modo_global(float now, int *outModo) {
    // ciclo simples: SCATTER (7s) -> CHASE (20s) -> repete
    float ciclo = TEMPO_SCATTER + TEMPO_CHASE;
    float t = fmodf(now, ciclo);
    if (t < TEMPO_SCATTER) *outModo = ESPALHAR; else *outModo = PERSEGUIR;
}

// ------------------ Atualizar todos os fantasmas (IA clássica) ------------------
void atualizar_fantasma(Jogo *jogo) {
    float now = (float)GetTime();
    int modoGlobal = PERSEGUIR;
    obter_modo_global(now, &modoGlobal);

    Fantasma *blinky = NULL;
    if (jogo->qntd_fantasmas > 0) blinky = &jogo->fantasmas[0];

    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];

        // se fantasma está em modo ASSUSTADO -> movimento aleatório/assustado
        if (f->modo == ASSUSTADO) {
            // reduz velocidade (se usar velocidade, ajuste sua lógica de tempo)
            f->velocidade = 0.5f;
            // decrementa timer
            f->timer_modo -= GetFrameTime();
            if (f->timer_modo <= 0) {
                f->modo = PERSEGUIR;
                f->velocidade = 1.0f;
            }

            // movimento aleatório simples (evita paredes)
            int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
            for (int tentativa = 0; tentativa < 8; tentativa++) {
                int s = rand() % 4;
                int nx = f->x + dirs[s][0];
                int ny = f->y + dirs[s][1];
                if (nx >= 0 && nx < LARGURA && ny >= 0 && ny < ALTURA && !eh_parede(jogo, nx, ny)) {
                    f->dx = dirs[s][0]; f->dy = dirs[s][1];
                    break;
                }
            }
            int nx = f->x + f->dx;
            int ny = f->y + f->dy;
            if (!eh_parede(jogo, nx, ny)) { f->x = nx; f->y = ny; }
            continue;
        }

        // determinar alvo dependendo do modo global (SCATTER ou CHASE) e do fantasma
        int alvoX = f->x, alvoY = f->y;

        if (modoGlobal == ESPALHAR) {
            // Scatter corners (simplificado)
            if (i == 0) { alvoX = LARGURA - 2; alvoY = 1; }            // Blinky -> top-right
            else if (i == 1) { alvoX = 1; alvoY = 1; }                 // Pinky  -> top-left
            else if (i == 2) { alvoX = LARGURA - 2; alvoY = ALTURA - 2; } // Inky   -> bottom-right
            else if (i == 3) { alvoX = 1; alvoY = ALTURA - 2; }        // Clyde  -> bottom-left
            else { alvoX = jogo->pacman.x; alvoY = jogo->pacman.y; }
        } else {
            // CHASE mode: alvo por fantasma
            if (i == 0) { set_alvo_blinky(jogo, f, &alvoX, &alvoY); }
            else if (i == 1) { set_alvo_pinky(jogo, f, &alvoX, &alvoY); }
            else if (i == 2) { if (blinky) set_alvo_inky(jogo, f, blinky, &alvoX, &alvoY); else set_alvo_blinky(jogo, f, &alvoX, &alvoY); }
            else if (i == 3) { set_alvo_clyde(jogo, f, &alvoX, &alvoY); }
            else { alvoX = jogo->pacman.x; alvoY = jogo->pacman.y; }
        }

        // escolher direção ótima e mover (evita 180º)
        escolher_direcao(f, jogo, alvoX, alvoY);

        int nx = f->x + f->dx;
        int ny = f->y + f->dy;
        if (!eh_parede(jogo, nx, ny)) {
            f->x = nx; f->y = ny;
        } else {
            // fallback: tenta qualquer direção válida
            int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
            for (int d = 0; d < 4; d++) {
                int tx = f->x + dirs[d][0], ty = f->y + dirs[d][1];
                if (tx >= 0 && tx < LARGURA && ty >= 0 && ty < ALTURA && !eh_parede(jogo, tx, ty)) {
                    f->dx = dirs[d][0]; f->dy = dirs[d][1];
                    f->x = tx; f->y = ty;
                    break;
                }
            }
        }
    }
}

// ------------------ Ativar modo assustado (FRIGHTENED) ------------------
void ativar_modo_assustado(Jogo *jogo) {
    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];
        f->modo = ASSUSTADO;
        f->timer_modo = TEMPO_ASSUSTADO;
        f->velocidade = 0.5f;
        // inverte direção imediatamente
        f->dx = -f->dx;
        f->dy = -f->dy;
    }
}
