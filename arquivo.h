#ifndef ARQUIVO_H_INCLUDED
#define ARQUIVO_H_INCLUDED

#include "estruturas.h"

void atualizarEstruturasJogo(char *nomeArquivo, FASE *infoFase, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, int *pontuacaoMario, int *vidasMario, int *usosPower, Texture2D *caixaMarioDos);
void buscarDirArquivoFase(int level, char *dirArquivo);
int lerArquivo(char *nomeArquivo, FASE *infoFase);

void escreverArquivoBinRanking(char *nome, int pontuacaoMario);
void lerArquivoBinRanking(int *arquivoBinLido, int *pontuacaoPrimeiro, int *pontuacaoSegundo, int *pontuacaoTerceiro, int *pontuacaQuarto, int *pontuacaoQuinto, char *nome1, char *nome2, char *nome3, char *nome4, char *nome5);

void salvarArquivoContinuarPT1(FASE *infoFase, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, char *nome);
void salvarArquivoContinuarPT2(MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, FASE infoFase);

void lerArquivoContinuarPT1(FASE *infoFase, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, char *nome);
void lerArquivoContinuarPT2(MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, FASE infoFase);

#endif // ARQUIVO_H_INCLUDED
