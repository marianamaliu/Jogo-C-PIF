#include "colisao.h"
#include "fantasma.h"
#include <raylib.h>

// helper
static bool mesmaPos(int a, int b, int c, int d) { return a==c && b==d; }

void colisao(Jogo *jogo) {
    Pacman *p = &jogo->pacman;

    // comida no mapa ('.')
    if (jogo->mapa.mapa[p->y][p->x] == '.') {
        jogo->mapa.mapa[p->y][p->x] = ' ';
        jogo->pontuacao_atual += 10;
        jogo->pontoComida += 10;

        if (!jogo->inicializarFruta && jogo->pontoComida >= jogo->maxComida) {
            gerar_fruta(jogo);
            jogo->pontoComida = 0;
        }
    }

    // fruta (item)
    if (jogo->inicializarFruta && mesmaPos(p->x, p->y, jogo->fruta.x, jogo->fruta.y)) {
        jogo->pontuacao_atual += 100;
        jogo->inicializarFruta = false;
        ativar_modo_assustado(jogo);
    }

    // colisão com fantasmas
    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];
        if (mesmaPos(p->x, p->y, f->x, f->y)) {
            if (f->modo == MODO_FRIGHTENED) {
                jogo->pontuacao_atual += 200;
                // respawn fantasma
                f->x = f->spawnX; f->y = f->spawnY;
                f->modo = MODO_CHASE;
                f->velocidade = 1.0f;
            } else {
                // Pacman perde
                jogo->pacman.vivo = false;
                jogo->jogo_ativo = false;
            }
        }
    }
}
