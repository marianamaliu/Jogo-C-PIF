// fantasma.c
#include "fantasma.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* tempo de acumulo para mover fantasmas (usa GetFrameTime() do raylib no .c principal) */
static float tempo_acumulado_fantasma = 0.0f;
#define TEMPO_BASE_MOVIMENTO 0.1f

/* direções (dx,dy) */
static const int DIR_X[4] = { 1, -1, 0, 0 }; /* direita, esquerda, baixo, cima */
static const int DIR_Y[4] = { 0, 0, 1, -1 };

static inline bool dentro_mapa(int x, int y) {
    return x >= 0 && x < LARGURA && y >= 0 && y < ALTURA;
}

/* eh_parede considera apenas '#' como parede.
   'G' (porta/ghost house) é tratado como livre conforme sua escolha (3). */
static inline bool eh_parede(Jogo *j, int x, int y) {
    if (!dentro_mapa(x, y)) return true;
    return j->mapa.mapa[y][x] == '#';
}

/* --- garante seed do rand() na primeira chamada --- */
static void garantir_seed() {
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = true;
    }
}

/* --- inicializa uma direcao valida para o fantasma (não aponta para parede) --- */
static void inicializar_direcao_valida(Fantasma *f, Jogo *j) {
    garantir_seed();

    for (int t = 0; t < 50; t++) {
        int d = rand() % 4;
        int nx = f->x + DIR_X[d];
        int ny = f->y + DIR_Y[d];
        if (!eh_parede(j, nx, ny)) {
            f->dx = DIR_X[d];
            f->dy = DIR_Y[d];
            return;
        }
    }
    /* se não achou, mantém parado */
    f->dx = 0; f->dy = 0;
}

/* --- BFS para retornar o PRÓXIMO tile do start até target
   Preenche next_x/next_y com o primeiro passo após (sx,sy).
   Retorna true se achou caminho. */
static bool bfs_proximo_passo(Jogo *j, int sx, int sy, int tx, int ty, int *next_x, int *next_y) {
    if (!dentro_mapa(sx, sy) || !dentro_mapa(tx, ty)) return false;
    if (eh_parede(j, tx, ty)) return false;

    typedef struct { int x, y; } Node;
    Node fila[ALTURA * LARGURA];
    int head = 0, tail = 0;

    int px[ALTURA][LARGURA];
    int py[ALTURA][LARGURA];
    for (int y = 0; y < ALTURA; y++)
        for (int x = 0; x < LARGURA; x++)
            px[y][x] = py[y][x] = -1;

    fila[tail++] = (Node){ sx, sy };
    px[sy][sx] = sx;
    py[sy][sx] = sy;

    bool found = false;
    while (head < tail) {
        Node cur = fila[head++];
        if (cur.x == tx && cur.y == ty) { found = true; break; }

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + DIR_X[i];
            int ny = cur.y + DIR_Y[i];
            if (!dentro_mapa(nx, ny)) continue;
            if (eh_parede(j, nx, ny)) continue;
            if (px[ny][nx] != -1) continue; /* já visitado */

            px[ny][nx] = cur.x;
            py[ny][nx] = cur.y;
            fila[tail++] = (Node){ nx, ny };
        }
    }

    if (!found) return false;

    /* retroceder do target até achar o filho direto do start */
    int cx = tx, cy = ty;
    while (!(px[cy][cx] == sx && py[cy][cx] == sy)) {
        int tx2 = px[cy][cx];
        int ty2 = py[cy][cx];
        if (tx2 == -1 && ty2 == -1) break; /* proteção */
        cx = tx2; cy = ty2;
    }

    *next_x = cx;
    *next_y = cy;
    return true;
}

/* --- alvos estilo clássico --- */
static void alvo_blinky(Jogo *j, Fantasma *f, int *ax, int *ay) {
    *ax = j->pacman.x;
    *ay = j->pacman.y;
}

static void alvo_pinky(Jogo *j, Fantasma *f, int *ax, int *ay) {
    /* 4 tiles à frente do pacman. Se pacman parado, alvo = pacman */
    int tx = j->pacman.x + j->pacman.dx * 4;
    int ty = j->pacman.y + j->pacman.dy * 4;
    if (!dentro_mapa(tx, ty)) {
        *ax = j->pacman.x;
        *ay = j->pacman.y;
    } else {
        *ax = tx; *ay = ty;
    }
}

static void alvo_inky(Jogo *j, Fantasma *f, int *ax, int *ay) {
    /* Inky: vetor do Blinky para 2 tiles à frente do Pacman, dobrado. */
    int p2x = j->pacman.x + j->pacman.dx * 2;
    int p2y = j->pacman.y + j->pacman.dy * 2;

    /* garante que exista blinky (id 0) */
    int blx = j->fantasmas[0].x;
    int bly = j->fantasmas[0].y;
    if (j->qntd_fantasmas > 0) {
        blx = j->fantasmas[0].x;
        bly = j->fantasmas[0].y;
    }

    int vx = p2x - blx;
    int vy = p2y - bly;

    int alvox = blx + vx * 2;
    int alvoy = bly + vy * 2;

    if (!dentro_mapa(alvox, alvoy)) {
        *ax = p2x; *ay = p2y;
    } else {
        *ax = alvox; *ay = alvoy;
    }
}

static void alvo_clyde(Jogo *j, Fantasma *f, int *ax, int *ay) {
    int dx = f->x - j->pacman.x;
    int dy = f->y - j->pacman.y;
    int dist2 = dx*dx + dy*dy;
    if (dist2 > (8*8)) {
        *ax = j->pacman.x; *ay = j->pacman.y;
    } else {
        *ax = 1; *ay = ALTURA - 2; /* scatter corner */
    }
}

/* --- definir alvo considerando modo --- */
static void definir_alvo(Fantasma *f, Jogo *j) {
    /* Se na caixa / voltando -> alvo = caixa */
    if (f->modo == FANTASMA_NA_CAIXA || f->modo == FANTASMA_VOLTANDO_CAIXA) {
        Nivel *n = &j->niveis[j->nivel_atual];
        f->alvo_x = n->caixa_x;
        f->alvo_y = n->caixa_y;
        return;
    }

    /* Se cego -> alvo aleatório válido */
    if (f->modo == FANTASMA_CEGO) {
        garantir_seed();
        int tries = 0;
        int rx, ry;
        do {
            rx = rand() % LARGURA;
            ry = rand() % ALTURA;
            tries++;
        } while (eh_parede(j, rx, ry) && tries < 200);
        f->alvo_x = rx;
        f->alvo_y = ry;
        return;
    }

    /* Se espalhar -> corners por id */
    if (f->modo == FANTASMA_ESPALHAR) {
        switch (f->id % 4) {
            case 0: f->alvo_x = LARGURA - 2; f->alvo_y = 1; break;
            case 1: f->alvo_x = 1; f->alvo_y = 1; break;
            case 2: f->alvo_x = LARGURA - 2; f->alvo_y = ALTURA - 2; break;
            case 3: f->alvo_x = 1; f->alvo_y = ALTURA - 2; break;
        }
        return;
    }

    /* Modo perseguir: escolha por id */
    switch (f->id % 4) {
        case 0: alvo_blinky(j, f, &f->alvo_x, &f->alvo_y); break;
        case 1: alvo_pinky(j, f, &f->alvo_x, &f->alvo_y); break;
        case 2: alvo_inky(j, f, &f->alvo_x, &f->alvo_y); break;
        case 3: alvo_clyde(j, f, &f->alvo_x, &f->alvo_y); break;
        default: alvo_blinky(j, f, &f->alvo_x, &f->alvo_y); break;
    }
}

/* --- atualização principal (usa seu sistema de tempo) --- */
void atualizar_fantasma(Jogo *jogo) {
    garantir_seed();

    float dt = GetFrameTime();
    tempo_acumulado_fantasma += dt;

    float multiplicador = jogo->niveis[jogo->nivel_atual].velocidade;
    float tempo_movimento = TEMPO_BASE_MOVIMENTO / multiplicador;

    while (tempo_acumulado_fantasma >= tempo_movimento) {
        tempo_acumulado_fantasma -= tempo_movimento;

        for (int i = 0; i < jogo->qntd_fantasmas; i++) {
            Fantasma *f = &jogo->fantasmas[i];

            /* se temporizador da caixa ativo, diminui e continua */
            if (f->tempo_para_sair > 0) {
                f->tempo_para_sair--;
                continue;
            }

            /* garante direção inicial */
            if (f->dx == 0 && f->dy == 0) {
                inicializar_direcao_valida(f, jogo);
            }

            /* definir alvo (comportamento individual) */
            definir_alvo(f, jogo);

            int nextx = -1, nexty = -1;
            bool ok = bfs_proximo_passo(jogo, f->x, f->y, f->alvo_x, f->alvo_y, &nextx, &nexty);

            if (!ok) {
                /* fallback: escolhe direção aleatória válida */
                int tent = 0;
                int candx, candy;
                do {
                    int d = rand() % 4;
                    candx = f->x + DIR_X[d];
                    candy = f->y + DIR_Y[d];
                    tent++;
                } while (eh_parede(jogo, candx, candy) && tent < 20);
                nextx = candx; nexty = candy;
            }

            /* se ainda inválido, tenta manter direção antiga (se possível) */
            if (eh_parede(jogo, nextx, nexty)) {
                int candx = f->x + f->dx;
                int candy = f->y + f->dy;
                if (!eh_parede(jogo, candx, candy)) {
                    nextx = candx; nexty = candy;
                } else {
                    inicializar_direcao_valida(f, jogo);
                    nextx = f->x + f->dx;
                    nexty = f->y + f->dy;
                }
            }

            /* determina dx/dy relativos antes de atualizar posição */
            int ddx = nextx - f->x;
            int ddy = nexty - f->y;
            f->dx = ddx;
            f->dy = ddy;

            /* aplica movimento */
            if (!eh_parede(jogo, nextx, nexty)) {
                f->x = nextx;
                f->y = nexty;
            }
        }
    }
}

/* --- desenho (seu desenho original adaptado) --- */
void desenhar_fantasma(Fantasma fantasma, Color CorFantasma){
    float pixelX = (float)(fantasma.x * TILE_SIZE);
    float pixelY = (float)(fantasma.y * TILE_SIZE);

    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raiocorpo = TILE_SIZE/2 - 2;

    /* tronco */
    DrawRectangle((int)pixelX + 1, (int)pixelY + raiocorpo, TILE_SIZE - 2, raiocorpo, CorFantasma);
    /* cabeça */
    DrawCircle(centroX, (int)pixelY + raiocorpo, raiocorpo, CorFantasma);

    /* ondulação */
    for (int i = 0; i < 4; i++) {
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / 4.0f)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / 4.0f)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / 4.0f)) + (TILE_SIZE / 8.0f), pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4.0f) };
        DrawTriangle(p1, p2, p3, CorFantasma);
    }

    /* olhos */
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);

    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
}
