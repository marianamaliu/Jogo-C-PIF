#include "pacman.h"
#include "fantasma.h"
#include "itens.h"
#include <math.h>

// desenhar pacman (com boca animada)
void desenhar_pacman(Pacman pacman){
    float pixelX = (float)(pacman.x * TILE_SIZE);
    float pixelY = (float)(pacman.y * TILE_SIZE);
    Vector2 center = { pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2 };
    float raio = TILE_SIZE/2 - 1;

    float angulo_central = 0.0f;
    if (pacman.dx == 1) angulo_central = 0.0f;
    else if (pacman.dx == -1) angulo_central = 180.0f;
    else if (pacman.dy == 1) angulo_central = 90.0f;
    else if (pacman.dy == -1) angulo_central = 270.0f;

    float maxAbertura = 50.0f;
    float minAbertura = 1.0f;
    float t = GetTime() * 8.0f;
    float seno = sinf(t);
    float fator = (seno + 1.0f) * 0.5f;
    float abertura_boca = minAbertura + fator * (maxAbertura - minAbertura);
    if (pacman.dx == 0 && pacman.dy == 0) abertura_boca = 0.0f;

    float startAngle = angulo_central - abertura_boca;
    float endAngle = angulo_central + abertura_boca;

    DrawCircleSector(center, raio, endAngle, startAngle + 360, 0, YELLOW);
}

// atualizar direção do Pac-Man (apenas mudança de direção até próximo tile)
void atualizar_pacman(Jogo *jogo){
    Pacman *pacman = &jogo->pacman;
    if (IsKeyDown(KEY_RIGHT)) { pacman->dx = 1; pacman->dy = 0; }
    else if (IsKeyDown(KEY_LEFT)) { pacman->dx = -1; pacman->dy = 0; }
    else if (IsKeyDown(KEY_UP)) { pacman->dx = 0; pacman->dy = -1; }
    else if (IsKeyDown(KEY_DOWN)) { pacman->dx = 0; pacman->dy = 1; }
}

// colisões e interações (chamada no loop principal após atualizar movimento)
void colisao(Jogo *jogo){
    Pacman *pacman = &jogo->pacman;
    Comida *comida = &jogo->comida;
    Fruta *fruta = &jogo->fruta;

    int proximoX = pacman->x + pacman->dx;
    int proximoY = pacman->y + pacman->dy;

    // mover se não for parede
    if (proximoX >= 0 && proximoX < LARGURA && proximoY >= 0 && proximoY < ALTURA &&
        jogo->mapa.mapa[proximoY][proximoX] != '#') {
        pacman->x = proximoX;
        pacman->y = proximoY;

        // coletar bolinha comum
        if (jogo->mapa.mapa[proximoY][proximoX] == '.') {
            jogo->mapa.mapa[proximoY][proximoX] = ' ';
            jogo->pontuacao_atual += 5;
            jogo->pontoComida += 5;
        }
    }

    // detectar comida especial (comida é item que você gera)
    if (proximoX == comida->x && proximoY == comida->y) {
        jogo->pontuacao_atual += 10;
        jogo->pontoComida += 10;

        // ativa modo assustado nos fantasmas
        ativar_modo_assustado(jogo);

        if (!jogo->inicializarFruta && jogo->pontoComida >= jogo->maxComida) {
            gerar_fruta(jogo);
            jogo->pontoComida = 0;
        }
        gerar_comida(jogo);
    }

    // detectar fruta
    if (jogo->inicializarFruta && proximoX == fruta->x && proximoY == fruta->y) {
        jogo->pontuacao_atual += 50;
        jogo->inicializarFruta = false;
    }

    // colisão com fantasmas:
    // posição anterior do pacman
    int prevPacX = pacman->x - pacman->dx;
    int prevPacY = pacman->y - pacman->dy;

    for (int i = 0; i < jogo->qntd_fantasmas; i++) {
        Fantasma *f = &jogo->fantasmas[i];
        int fx = f->x, fy = f->y;
        // mesma célula
        if (pacman->x == fx && pacman->y == fy) {
            jogo->jogo_ativo = false;
            return;
        }
        // colisão cruzada (troca de posições)
        int prevFantX = fx - f->dx;
        int prevFantY = fy - f->dy;
        if (prevPacX == fx && prevPacY == fy && prevFantX == pacman->x && prevFantY == pacman->y) {
            jogo->jogo_ativo = false;
            return;
        }
    }
}
