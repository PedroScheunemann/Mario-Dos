#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

#include "raylib.h"

// parametros da tela
#define screenWidth 1200
#define screenHeight 700

// número da optionSelect
#define NovoJogo 30
#define Continuar 31
#define CarregarMapa 32
#define Ranking 33
#define Ajuda 34
#define Sobre 35
#define Sair 36
// número de opções do menu
#define NROOPCOES 7

// maximo de caracteres a serem digitados pelo usuario
#define MAX_CARACTERES 13

// sentidos de deslocamento dos inimigos
#define direita -1
#define esquerda 1

// nível dos inimigos
#define vulneravel 20
#define bravo 21
#define normal 22

// tipos de inimigo
#define tartaruga 5
#define caranguejo 6


typedef struct{
    int optionSelect, optionSelected;
}MENU;

typedef struct{
    Font fonteMario, fonteGold;
    Texture2D caixaMarioDos, fogos;
    Image fogosImagem;
    Sound puloMario, hitMoeda, hitInimigo, chuteInimigo, danoMario, marioMorreu, gameOver, inicio, hitPower, setaMenu, cabeceada, gameEnd, startGame, enter, keyPress, backspace;
}MIDIA;

typedef struct{
    int numPlataformas, numCanos, numRetornosCanos, numMaxTartarugas, numMaxCaranguejos, numMaxMoedas, numTartarugasAtivas, numCaranguejosAtivos, numMoedasAtivas, delayInimigos;
}FASE;

typedef struct{
    Rectangle rect, frameRect;
    int lifes, pontuacao, canJump, canGoLeft, canGoRight, subindo, descendo, atingido, parado, bateuCabeca, chegouNoCeu, framesCounter;
    float speed;
}MARIO;

typedef struct{
    Rectangle rect, frameRect;
    Vector2 speed;
    int estado, sentidoDesloc, descendo, colidindo, morrendo, morto, framesCounter;
}INIMIGO;

typedef struct{
    Rectangle rect;
}OBSTACULO;  // representa plataformas, canos e retornos

typedef struct{
    Rectangle rect;
    int usos;
}POWER;

typedef struct{
    Rectangle rect, frameRect;
    int sentidoDesloc, framesCounter, descendo, colidindo, sumiu;
    float speed;
}MOEDA;

#endif // ESTRUTURAS_H_INCLUDED
