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

    float multiplicador = 0.8f;
    float tempo_movimento = TEMPO_BASE_MOVIMENTO / multiplicador;

    while (tempo_acumulado_fantasma >= tempo_movimento) {
        tempo_acumulado_fantasma -= tempo_movimento;

        // se tiver nesses 5 segundos cego
        if(jogo->tempo_cego > 0){
            jogo->tempo_cego--;
        }
        for (int i = 0; i < jogo->qntd_fantasmas; i++) {
            Fantasma *f = &jogo->fantasmas[i];

            if (f->tempo_para_sair > 0) { //p o temporizador ir dominuindo
                f->tempo_para_sair--;
                continue;
            }

            //garantir q o fantasma tem uma direção p andar
            if (f->dx == 0 && f->dy == 0) {
                inicializar_direcao_valida(f, jogo);
            }

            int nextx = -1, nexty = -1;

            if(jogo->tempo_cego > 0){
                // modo cego: andar aleatoriamente
                int tent = 0;
                do {
                    int d = rand() % 4;
                    nextx = f->x + DIR_X[d];
                    nexty = f->y + DIR_Y[d];
                    tent++;
                } while(eh_parede(jogo, nextx, nexty) && tent < 20);
            } else {
                //volta ao normal, perseguir Pac-Man
                definir_alvo(f, jogo);
                bool ok = bfs_proximo_passo(jogo, f->x, f->y, f->alvo_x, f->alvo_y, &nextx, &nexty);
                if(!ok){
                    inicializar_direcao_valida(f, jogo);
                    nextx = f->x + f->dx;
                    nexty = f->y + f->dy;
                }
            }

            //atualiza dx/dy e aplica movimento 
            f->dx = nextx - f->x;
            f->dy = nexty - f->y;

            if(!eh_parede(jogo, nextx, nexty)){
                f->x = nextx;
                f->y = nexty;
            }
        }
    }
}

void desenhar_fantasma(Fantasma fantasma, Color CorFantasma){
    float pixelX = (float)(fantasma.x * TILE_SIZE);
    float pixelY = (float)(fantasma.y * TILE_SIZE);

    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raiocorpo = TILE_SIZE/2 - 2;

    //corpo
    DrawRectangle((int)pixelX + 1, (int)pixelY + raiocorpo, TILE_SIZE - 2, raiocorpo, CorFantasma);
    //cabeca
    DrawCircle(centroX, (int)pixelY + raiocorpo, raiocorpo, CorFantasma);

    //ondulação
    for (int i = 0; i < 4; i++) {
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / 4.0f)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / 4.0f)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / 4.0f)) + (TILE_SIZE / 8.0f), pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4.0f) };
        DrawTriangle(p1, p2, p3, CorFantasma);
    }

    //olho
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);

    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
}

void desenhar_fantasma_cego(Fantasma fantasma){
    float pixelX = (float)(fantasma.x * TILE_SIZE);
    float pixelY = (float)(fantasma.y * TILE_SIZE);

    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raiocorpo = TILE_SIZE/2 - 2;

    Color cegoCor = BLUE; 

    //corpo e cabeça
    DrawRectangle((int)pixelX + 1, (int)pixelY + raiocorpo, TILE_SIZE - 2, raiocorpo, cegoCor);
    DrawCircle(centroX, (int)pixelY + raiocorpo, raiocorpo, cegoCor);

    //ondulaçao de baixo
    int nOndas = 4;
    for (int i = 0; i < nOndas; i++) {
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / (float)nOndas)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / (float)nOndas)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / (float)nOndas)) + (TILE_SIZE / (2.0f*nOndas)), pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4.0f) };
        DrawTriangle(p1, p2, p3, cegoCor);
    }

    // olho
    int raio_olho = TILE_SIZE / 10; 
    int offset_y = TILE_SIZE / 4;
    int offset_x = TILE_SIZE / 6;
    DrawCircle(centroX - offset_x, centroY - offset_y, raio_olho, WHITE);
    DrawCircle(centroX + offset_x, centroY - offset_y, raio_olho, WHITE);

    // boca ondulada 
    Color bocaCor = YELLOW;
    int boca_y = centroY + TILE_SIZE/8; 
    int nOnda = 5;
    float boca_largura = TILE_SIZE / 1.5f;

    for (int i = 0; i < nOnda; i++) {
        float x1 = centroX - boca_largura/2 + i * (boca_largura / nOnda);
        float x2 = centroX - boca_largura/2 + (i + 1) * (boca_largura / nOnda);
        float y1 = boca_y + ((i % 2 == 0) ? 2 : -2);
        float y2 = boca_y;

        DrawLine((int)x1, (int)y2, (int)((x1+x2)/2), (int)y1, bocaCor);
        DrawLine((int)((x1+x2)/2), (int)y1, (int)x2, (int)y2, bocaCor);
    }
}