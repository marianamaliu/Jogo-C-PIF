#include "pacman.h"

void desenhar_pacman(Pacman pacman){
    //converte as coordenadas da matriz em coodenadas de pixel
    float pixelX = (float)(pacman.x *TILE_SIZE);
    float pixelY = (float)(pacman.y *TILE_SIZE);

    Vector2 center={pixelX +TILE_SIZE/2, pixelY +TILE_SIZE/2};
    float raio = TILE_SIZE/2-1;

    float startAngle = 0.0f;
    float endAngle = 360.0f;
    float angulo_central = 0.0f;

    if(pacman.dx==1){
        angulo_central=0.0f; //direita
    }else if(pacman.dx==-1){
        angulo_central=180.0f; //esquerda
    }else if(pacman.dy==1){
        angulo_central=90.0f; //baixo
    }else if(pacman.dy==-1){
        angulo_central=270.0f; //cima
    }

    float maxAbertura = 50.0f; //boca mais aberta
    float minAbertura = 1.0f;//boca meio fechada

    float t = GetTime() * 20.0f; //velocidade da mastigação
    float seno = sinf(t);
    float fator = (seno + 1.0f) * 0.5f;
    float abertura_boca = minAbertura + fator * (maxAbertura - minAbertura);

    if (pacman.dx == 0 && pacman.dy == 0) {
        abertura_boca = 0.0f;
    }
    //cacula o angulo da abertura
    startAngle = angulo_central - abertura_boca;
    endAngle = angulo_central + abertura_boca;

    DrawCircleSector(center, raio, endAngle, startAngle + 360, 0, YELLOW);
}

//atualizar movimentação
void atualizar_pacman(Jogo *jogo){
    Pacman *pacman = &jogo->pacman;
    
    if(IsKeyDown(KEY_RIGHT)){
        pacman->dx=1;
        pacman->dy=0;
    }else if(IsKeyDown(KEY_LEFT)){
        pacman->dx=-1;
        pacman->dy=0;
    }else if(IsKeyDown(KEY_UP)){
        pacman->dx=0;
        pacman->dy=-1;
    }else if(IsKeyDown(KEY_DOWN)){
        pacman->dx=0;
        pacman->dy=1;
    }
}

void colisao(Jogo *jogo){ 
    Pacman *pacman=&jogo->pacman;
    Comida *comida=&jogo->comida;
    Fruta *fruta=&jogo->fruta;

    int proximoX = pacman->x + pacman->dx;
    int proximoY = pacman->y + pacman->dy;

    //detectar parede
    if(proximoX >=0 && proximoX < LARGURA && proximoY>=0 && proximoY<ALTURA &&
    jogo->mapa.mapa[proximoY][proximoX]!='#'){
        pacman->x = proximoX;
        pacman->y = proximoY;
    }

    //detectar bolinhas menores
    if(jogo->mapa.mapa[proximoY][proximoX]=='.'){
        jogo->mapa.mapa[proximoY][proximoX]=' '; //remove a bolinha
        jogo->pontuacao_atual+=5;
        jogo->bolinhas_comidas++;
    }
    
    //detectar comida
    if(proximoX==comida->x && proximoY==comida->y){
        jogo->pontuacao_atual+=10;
        jogo->pontoComida+=10;
        jogo->bolinhas_comidas++;

        if(!jogo->inicializarFruta && jogo->pontoComida>=jogo->maxComida){
            gerar_fruta(jogo); //se pegar 5 bolinhas (50 pontos)
            jogo->pontoComida=0;
        }
        gerar_comida(jogo);
    }

    //detectar fruta
    if(jogo->inicializarFruta && proximoX==fruta->x && proximoY==fruta->y){
        jogo->pontuacao_atual+=50;
        jogo->inicializarFruta=false;
    }

    //detectar fantasma 
    //for para garantir que a colisao seja detectada em cada fantasma
    for(int i=0; i<jogo->qntd_fantasmas; i++){
        Fantasma *fantasma = &jogo->fantasmas[i]; 
        
        //posição onde o pacman acabou de chegar
        if(pacman->x==fantasma->x && pacman->y==fantasma->y || 
            proximoX==fantasma->x && proximoY==fantasma->y){
            jogo->jogo_ativo=false;
        }
    }
}