#ifndef TELANOVOJOGO_H_INCLUDED
#define TELANOVOJOGO_H_INCLUDED

#include "estruturas.h"

void atualizaNome(char *nome, int tamString, int *contadorLetras, int *inserido, MIDIA *allMidias);
void desenhaNome(char *nome, Font *fontMario);

void atualizaVoltar(MENU *menuJogo);
void desenhaVoltar(Font *fontMario);

void atualizaTelaNome(char *nome, MENU *menuJogo, MIDIA *allMidias);
void desenhaTelaNome(char *nome, Font *fontMario);

void construirTelaNome(char *nome, MENU *menuJogo,  MIDIA *allMidias);

void loopingGameplay(int fase, int *pontuacaoMario, int vidasMario, int usosPower, MENU *menuJogo, MIDIA *allMidias);

void atualizaTelaGameplay(FASE *infoFase, MIDIA *allMidias, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, Texture2D *caixaMarioDos);
void desenhaTelaGameplay(FASE infoFase, MIDIA *allMidias, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower);

void construirTelaGameplay(MENU *menuJogo, MIDIA *allMidias);

void desenhaTelaGameOver(MIDIA *allMidias, int framesCounter, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, POWER *botaoPower, int numPlataformas, int numCanos, int numRetornosCanos);
void desenhaTelaGameEnd(MIDIA *allMidias, int framesCounter);
void desenhaTelaPause(FASE infoFase, MIDIA *allMidias, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower);

void desenhaMario(Texture2D *caixaMarioDos, MARIO *mario);
void desenhaPontuacao(MIDIA *allMidias,  MARIO *mario);
void desenhaCanos(Texture2D *caixaMarioDos, OBSTACULO *canos, OBSTACULO *retornosCanos, int numCanos, int numRetornosCanos);
void desenhaPlataformas(Texture2D *caixaMarioDos, OBSTACULO *plataformas, int numPlataformas);
void desenhaBotaoPower(Texture2D *caixaMarioDos, POWER *botaoPower);
void desenhaInimigos(Texture2D *caixaMarioDos, INIMIGO *tartarugas, INIMIGO *caranguejos, int numTartarugas, int numCaranguejos);
void desenhaMoedas(Texture2D *caixaMarioDos, MOEDA *moeda, int numMoedasAtivas);
void desenhaNroFase(Font *fonteMario, int fase);

void construirTelaNovoJogo(MENU *menuJogo,  MIDIA *allMidias);

int matouTodosInimigos(INIMIGO *tartarugas, INIMIGO *caranguejos, FASE infoFase);


#endif // TELANOVOJOGO_H_INCLUDED
