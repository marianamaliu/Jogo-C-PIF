#include "fantasma.h"

void desenhar_fantasma(Fantasma fantasma, Color CorFantasma){
    float pixelX=(float)(fantasma.x*TILE_SIZE);
    float pixelY=(float)(fantasma.y*TILE_SIZE);

    int centroX=pixelX + TILE_SIZE/2;
    int centroY= pixelY + TILE_SIZE/2;
    int raiocorpo = TILE_SIZE/2-2;

    //"tronco" do fantasma
    DrawRectangle(pixelX + 1, pixelY +raiocorpo, TILE_SIZE-2, raiocorpo, CorFantasma);
    //cabeça do fantasma
    DrawCircle(centroX, pixelY + raiocorpo, raiocorpo, CorFantasma);
    //ondulação do fantasma
    for (int i = 0; i < 4; i++) {
        // Triângulo que forma a parte inferior ondulada 
        //Vector2 = vértices do triângulo
        Vector2 p1 = { pixelX + (i * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p2 = { pixelX + ((i + 1) * (TILE_SIZE / 4)), pixelY + TILE_SIZE - 2 };
        Vector2 p3 = { pixelX + (i * (TILE_SIZE / 4)) + (TILE_SIZE / 8), pixelY + TILE_SIZE - 2 - (TILE_SIZE / 4) };

        DrawTriangle(p1, p2, p3, CorFantasma);
    }
    //olho Esquerdo
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX - TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
    //olho Direito
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 8, WHITE);
    DrawCircle(centroX + TILE_SIZE / 6, centroY - TILE_SIZE / 4, TILE_SIZE / 16, BLACK);
}

//atualizar movimentação
//fantasma sem seguir
void atualizar_fantasma(Jogo *jogo){
    for(int i = 0; i < jogo->qntd_fantasmas; i++){
        Fantasma *fantasma = &jogo->fantasmas[i];

        int proximoX = fantasma->x + fantasma->dx;
        int proximoY = fantasma->y + fantasma->dy;
        
        //colisao c a parede
        bool colisao_parede = (proximoY < 0 || proximoY >= ALTURA || proximoX < 0 || proximoX >= LARGURA || jogo->mapa.mapa[proximoY][proximoX] == '#');

        if (colisao_parede || (rand() % 5 == 0)) { //esse rand faz ele alterar tambem caso nao tenha uma parede, ele tem 1 chance em 5 de alterar
            int direcao_valida = 0; 
            
            while (direcao_valida==0) {
                int sorteio_direcao = rand() % 4; 
                //sorteia um num de 0 a 3 (cima, baixo, direita e esquerda)
                
                fantasma->dx = 0; //isso serve para resetar a direcao atual antes de sortear a nova
                fantasma->dy = 0;
                
                switch (sorteio_direcao) {
                    case 0: fantasma->dx = 1; break;  // direita
                    case 1: fantasma->dx = -1; break; // esquerda
                    case 2: fantasma->dy = 1; break;  // baixo
                    case 3: fantasma->dy = -1; break; // cima
                }

                //recalcular a prox direcao depois de trocar de direcao
                proximoX = fantasma->x + fantasma->dx;
                proximoY = fantasma->y + fantasma->dy;

                if (proximoY >= 0 && proximoY < ALTURA && proximoX >= 0 && proximoX < LARGURA && 
                    jogo->mapa.mapa[proximoY][proximoX] != '#') {
                    direcao_valida = 1; //o loop so para qnd ele acha uma nova direcao
                }
            }
        } 
        fantasma->x = fantasma->x + fantasma->dx;
        fantasma->y = fantasma->y + fantasma->dy; 
    }
}