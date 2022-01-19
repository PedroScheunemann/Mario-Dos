#ifndef TELACONTINUAR_H_INCLUDED
#define TELACONTINUAR_H_INCLUDED

#include "estruturas.h"

void desenhaNomeFase(char *nroFase, Font *fontMario);
void desenhaTelaFase(char *nroFase, Font *fontMario);

void atualizaTelaFase(char *nroFase, MENU *menuJogo, MIDIA *allMidias);
void construirTelaNroFase(char *nroFase, MENU *menuJogo, MIDIA *allMidias);

void carregarTelaGameplay(char *nroFase, MENU *menuJogo, MIDIA *allMidias);
void construirTelaCarregarMapa(MENU *menuJogo,  MIDIA *allMidias);

void atualizaTelaRanking(MENU *menuJogo);
void desenhaTelaRanking(MENU *menuJogo, MIDIA *allMidias, int pontuacaoPrimeiro, int pontuacaoSegundo, int pontuacaoTerceiro, int pontuacaQuarto, int pontuacaoQuinto, char *nome1, char *nome2, char *nome3, char *nome4, char *nome5);
void construirTelaRanking(MENU *menuJogo, MIDIA *allMidias);

void construirTelaContinuar(MENU *menuJogo,  MIDIA *allMidias);

#endif // TELACONTINUAR_H_INCLUDED
