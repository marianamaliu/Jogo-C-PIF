#include "itens.h"

//desenhar
void desenhar_comida(Comida comida){
    float pixelX=(float)(comida.x *TILE_SIZE);
    float pixelY=(float)(comida.y *TILE_SIZE);

    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/2-4, YELLOW);

}

void desenhar_cereja(int centroX, int centroY, int raio_base) {
    // caule 1
    DrawLine(centroX, centroY-raio_base*2, centroX-raio_base, centroY+raio_base-raio_base/2, GREEN);
    // caule 2
    DrawLine(centroX, centroY-raio_base*2, centroX+raio_base, centroY+raio_base-raio_base/2, GREEN);
    // cereja 1
    DrawCircle(centroX-raio_base, centroY+raio_base, raio_base, RED);
    // cereja 2
    DrawCircle(centroX+raio_base, centroY+raio_base, raio_base, RED);
    // brilho da cereja
    DrawCircle(centroX - raio_base - raio_base/4, centroY + raio_base - raio_base/4, raio_base/4, WHITE);
    DrawCircle(centroX + raio_base - raio_base/4, centroY + raio_base - raio_base/4, raio_base/4, WHITE);
}

void desenhar_fruta(Jogo *jogo){
    // É mais fácil acessar o tipo de fruta do jogo
    Fruta fruta = jogo->fruta;
    
    float pixelX=(float)(fruta.x*TILE_SIZE);
    float pixelY=(float)(fruta.y*TILE_SIZE);

    int centroX=pixelX+TILE_SIZE/2;
    int centroY=pixelY+TILE_SIZE/2;
    int raio_base=TILE_SIZE/4; // Novo nome para a variável de raio

    desenhar_cereja(centroX, centroY, raio_base);
}

bool existe_bolinha(Jogo *jogo){
    for(int y=0; y<ALTURA; y++){
        for(int x=0; x<LARGURA; x++){
            if(jogo->mapa.mapa[y][x] == '.'){
                return true;
            }
        }
    }
    return false;
}

//gerar
void gerar_comida(Jogo *jogo){
    Comida *comida = &jogo->comida;

    if(!existe_bolinha(jogo)){
        return; //nao gerar comida quando não tem mais '.'
    }

    int novoX, novoY;
    do {
        novoX = rand() % (LARGURA-2) + 1;
        novoY = rand() % (ALTURA-2) + 1;
    } while(jogo->mapa.mapa[novoY][novoX] != '.');

    comida->x = novoX;
    comida->y = novoY;
}

void gerar_fruta(Jogo *jogo){
    if(!existe_bolinha(jogo)){
        return; 
    }

    Fruta *fruta = &jogo->fruta;
    Comida *comida = &jogo->comida;

    int novoX, novoY;
    
    do{
        novoX = rand() % (LARGURA-2) + 1;
        novoY = rand() % (ALTURA-2) + 1;
    } while(jogo->mapa.mapa[novoY][novoX] != '.' ||(novoX == comida->x && novoY == comida->y));

    jogo->inicializarFruta = true;
    fruta->x = novoX;
    fruta->y = novoY;
    fruta->temporizador=100;
}