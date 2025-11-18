#include "itens.h"

//desenhar
void desenhar_comida(Comida comida){
    float pixelX=(float)(comida.x *TILE_SIZE);
    float pixelY=(float)(comida.y *TILE_SIZE);

    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/2-4, YELLOW);

}

// void desenhar_fruta(Fruta fruta){
//     float pixelX=(float)(fruta.x*TILE_SIZE);
//     float pixelY=(float)(fruta.y*TILE_SIZE);

//     int centroX=pixelX+TILE_SIZE/2;
//     int centroY=pixelY+TILE_SIZE/2;
//     int raio_cereja=TILE_SIZE/4;

//     //caule 1
//     DrawLine(centroX, centroY-raio_cereja*2, centroX-raio_cereja, centroY+raio_cereja-raio_cereja/2, GREEN);
//     //caule 2
//     DrawLine(centroX, centroY-raio_cereja*2, centroX+raio_cereja, centroY+raio_cereja-raio_cereja/2, GREEN);
//     //cereja 1
//     DrawCircle(centroX-raio_cereja, centroY+raio_cereja, raio_cereja, RED);
//     //cereja 2
//     DrawCircle(centroX+raio_cereja, centroY+raio_cereja, raio_cereja, RED);
//     //brilho da cereja
//     DrawCircle(centroX - raio_cereja - raio_cereja/4, centroY + raio_cereja - raio_cereja/4, raio_cereja/4, WHITE);
//     DrawCircle(centroX + raio_cereja - raio_cereja/4, centroY + raio_cereja - raio_cereja/4, raio_cereja/4, WHITE);
// }

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

void desenhar_morango(int centroX, int centroY, int raio_base) {
    // Corpo do Morango (Forma de triângulo para simular ponta)
    Vector2 topo = {centroX, centroY - raio_base * 2};
    Vector2 canto_e = {centroX - raio_base * 2, centroY + raio_base * 1.5};
    Vector2 canto_d = {centroX + raio_base * 2, centroY + raio_base * 1.5};

    DrawTriangle(topo, canto_e, canto_d, RED);
    
    // Folha verde (topo)
    DrawRectangle(centroX - raio_base / 2, centroY - raio_base * 2, raio_base, raio_base, GREEN);
    
    // Sementes simplificadas (pontos amarelos)
    DrawCircle(centroX - raio_base, centroY, raio_base / 4, YELLOW);
    DrawCircle(centroX + raio_base, centroY, raio_base / 4, YELLOW);
}

void desenhar_laranja(int centroX, int centroY, int raio_base) {
    // Desenho da Laranja
    int raio_laranja = raio_base * 1.5;
    DrawCircle(centroX, centroY, raio_laranja, ORANGE);
    // Folha/Haste
    DrawRectangle(centroX, centroY - raio_laranja - raio_base / 2, raio_base, raio_base / 2, GREEN);
    // Brilho
    DrawCircle(centroX - raio_laranja / 2, centroY - raio_laranja / 2, raio_base / 2, WHITE);
}

void desenhar_fruta(Jogo *jogo){
    // É mais fácil acessar o tipo de fruta do jogo
    Fruta fruta = jogo->fruta;
    
    float pixelX=(float)(fruta.x*TILE_SIZE);
    float pixelY=(float)(fruta.y*TILE_SIZE);

    int centroX=pixelX+TILE_SIZE/2;
    int centroY=pixelY+TILE_SIZE/2;
    int raio_base=TILE_SIZE/4; // Novo nome para a variável de raio

    switch (fruta.tipo) {
        case CEREJA:
            desenhar_cereja(centroX, centroY, raio_base);
            break;
        case MORANGO:
            desenhar_morango(centroX, centroY, raio_base);
            break;
        case LARANJA:
            desenhar_laranja(centroX, centroY, raio_base);
            break;
        default:
            // Caso não tenha fruta
            break;
    }
}

//gerar
void gerar_comida(Jogo *jogo){
    Comida *comida=&jogo->comida;
    //aleatoriedade
    int novoX = rand() % (LARGURA-2) + 1; 
    int novoY = rand() % (ALTURA-2) + 1;

    //se for na parede ou se nao for onde tem '.', ele sorteia os valores de novo ate nao ser na parede
    if(jogo->mapa.mapa[novoY][novoX]=='#' || jogo->mapa.mapa[novoY][novoX]!='.'){ 
        gerar_comida(jogo);
        
    }else{
        comida->x=novoX;
        comida->y=novoY;
    }
}

void gerar_fruta(Jogo *jogo){
    Fruta *fruta=&jogo->fruta;
    Comida *comida=&jogo->comida;

    //aleatoriedade
    int novoX=rand()%(LARGURA-2)+1;
    int novoY=rand()%(ALTURA-1)+1;

    bool colisao_comida = (novoX == comida->x && novoY == comida->y);

    //se for parede ou se for onde ja tem comida ou se nao for no '.', ele gera de novo ate nao ser parede ou nao ter comida
    if(jogo->mapa.mapa[novoY][novoX]=='#' || colisao_comida || jogo->mapa.mapa[novoY][novoX]!='.'){
        gerar_fruta(jogo);
    }else{
        jogo->inicializarFruta=true;
        fruta->x=novoX;
        fruta->y=novoY;
        fruta->tipo = jogo->niveis[jogo->nivel_atual].fruta_nivel;
    }
}