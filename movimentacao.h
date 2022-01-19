#ifndef MOVIMENTACAO_H_INCLUDED
#define MOVIMENTACAO_H_INCLUDED

#include "estruturas.h"

void checaColisaoPlat(OBSTACULO *plataformas, int numPlataformas, Rectangle *movel, int *teveColisao);
void checaColisaoCano(OBSTACULO *canos, int numCanos, Rectangle *movel, int *teveColisao, int *canoColidido);
void checaColisaoInimigos(INIMIGO *tartarugas, INIMIGO *caranguejos, int numTartarugasAtivas, int numCaranguejosAtivos, Rectangle *movel, int *teveColisao, int *inimigoColidido, int *tipoInimigo);

void atualizaFrameRectMario(MARIO *mario);
void atualizaFrameRectTartaruga(INIMIGO *inimigo);
void atualizaFrameRectCaranguejo(INIMIGO *inimigo);
void atualizaFrameMoeda(MOEDA *moeda);

void atualizaPosiMoeda(MIDIA *allMidias, FASE infoFase, MOEDA *moeda, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, MARIO *mario);
void atualizaPosiInimigo(MIDIA *allMidias, FASE infoFase, int tipoInimigo, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, INIMIGO *inimigo, MARIO *mario, int fase);
void atualizaPosiMario(MIDIA *allMidias, FASE *infoFase, MARIO *mario, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, MOEDA *moedas, POWER *botaoPower);
void lerTeclado(MARIO *mario);

#endif // MOVIMENTACAO_H_INCLUDED
