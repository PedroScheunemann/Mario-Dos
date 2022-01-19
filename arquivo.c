#include "raylib.h"
#include "arquivo.h"
#include "telaNovoJogo.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


int lerArquivo(char *nomeArquivo, FASE *infoFase){

    FILE *fp;
    int linhas = 0;
    char caracter;
    int sumPlats = 0, sumCanos = 0, sumRetornos = 0;


    fp = fopen(nomeArquivo, "r");

    if (fp != NULL){

        while (linhas < 28){

            caracter = getc(fp);                                // lendo caractere por caractere do arquivo

            if (tolower(caracter) == 'p'){                      // contando o nro de plataformas a partir das letras 'p'
                sumPlats++;
                while (caracter == 'p') caracter = getc(fp);
            }
            else if (isdigit(caracter)) sumRetornos++;           // os números de 1-9 são o retorno dos canos // onde entram
            else if (tolower(caracter) == 'c') sumCanos++;       // as letras 'c' são os canos                // onde saem


            if (caracter == '\n') linhas++;                     // contando o nro de linhas
        }

        if (linhas == 28){
            fscanf(fp, "%d %d %d", &infoFase->numMaxTartarugas, &infoFase->numMaxCaranguejos, &infoFase->delayInimigos);  // pegando as informações da última linha do arquivo
        }

        fclose(fp);


    }else return 0;      // retorna 0 caso dê erro na leitura do arquivo

    infoFase->numPlataformas = sumPlats;
    infoFase->numCanos = sumCanos;
    infoFase->numRetornosCanos = sumRetornos;
    infoFase->numMaxMoedas = 10;

    return 1;           // se tudo der certo retorna 1

}


void atualizarEstruturasJogo(char *nomeArquivo, FASE *infoFase, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, int *pontuacaoMario, int *vidasMario, int *usosPower, Texture2D *caixaMarioDos){

        FILE *fp;
        int linhas = 0, colunas = 0, contadorPlats = 1, contadorCanos = 0, contadorRetornosCanos = 0;
        char caracter;
        char numRetorno[9]="123456789";
        float frameWidth = caixaMarioDos->width;
        float frameHeight = caixaMarioDos->height;

        const int larguraCanos = 215;
        const int comprimentoCanos = 150;
        const int larguraRetornosCanos = 150;
        const int comprimentoRetornosCanos = 86;

        // inserindo a plataforma de baixo, a qual é sempre fixa (não é lida no arquivo) na primeira posição do array de plataformas
        Rectangle plataformaDeBaixo = {0, screenHeight-76, screenWidth, 87};
        plataformas[0].rect = plataformaDeBaixo;


        fp = fopen(nomeArquivo, "r");

        while (linhas < 28){

            // pegando caractere por caractere, linha por linha
            caracter = getc(fp);

            if (tolower(caracter) == 'p'){                              // setando a posição da plataforma de acordo com o que foi lido no arquivo
                plataformas[contadorPlats].rect.x = 10 * colunas;       // o primeiro p seta a origem da plataforma
                plataformas[contadorPlats].rect.y = 25 * linhas;
                plataformas[contadorPlats].rect.height = 25;
                plataformas[contadorPlats].rect.width = 0;
                while (caracter == 'p'){
                    plataformas[contadorPlats].rect.width += 10;       // a cada p lido a largura da plataforma cresce
                    caracter = getc(fp);
                    colunas++;
                }
                contadorPlats++;
            }
            else if (tolower(caracter) == 'm'){                        // setando as características iniciais do mario

                Rectangle frameInicialMario = {3, 602.8, frameWidth, frameHeight};

                mario->frameRect = frameInicialMario;
                mario->rect.height = 84;
                mario->rect.width = 66;
                mario->rect.x = (10 * colunas) - mario->rect.width/2;
                mario->rect.y = (25 * linhas) - mario->rect.height/2;
                mario->lifes = *vidasMario;
                mario->pontuacao = *pontuacaoMario;
            }

            else if (isdigit(caracter)){                                // setando as posições e caracteristicas iniciais de cada retorno de cano
                                                                        // (de acordo com o que foi lido no arquivo)
                for(int j = 0; j < 9; j++){
                    if(caracter == numRetorno[j]){
                        retornosCanos[j].rect.x = 10 * colunas;
                        retornosCanos[j].rect.y = (25 * linhas) - comprimentoRetornosCanos/2;
                        retornosCanos[j].rect.width = larguraRetornosCanos;
                        retornosCanos[j].rect.height = comprimentoRetornosCanos;

                        // reajustando a posicao x do cano caso ele esteja na esquerda ou na direita da tela
                        if (retornosCanos[j].rect.x < screenWidth/2) retornosCanos[j].rect.x -= larguraRetornosCanos;
                        else retornosCanos[j].rect.x += 10;
                    }
                }
                contadorRetornosCanos++;
            }
            else if (tolower(caracter) == 'c'){                                        // setando a posição e caracteristicas iniciais dos canos
                                                                                       // (de acordo com o que foi lido no arquivo)
                canos[contadorCanos].rect.x = 10 * colunas;
                canos[contadorCanos].rect.y = (25 * linhas) - comprimentoCanos/2;
                canos[contadorCanos].rect.width = larguraCanos;
                canos[contadorCanos].rect.height = comprimentoCanos;

                // reajustando a posicao x do cano caso ele esteja na esquerda ou direita da tela
                if (canos[contadorCanos].rect.x < screenWidth/2) canos[contadorCanos].rect.x -= larguraCanos;
                else canos[contadorCanos].rect.x += 10;

                contadorCanos++;
            }
            else if (tolower(caracter) == 'b'){                                       // setando a posição e caracteristicas iniciais do botao power
                                                                                      // (de acordo com o que foi lido no arquivo)
                botaoPower->rect.width = 60;
                botaoPower->rect.height = 60;
                botaoPower->rect.x = (10 * colunas) - botaoPower->rect.width/2;
                botaoPower->rect.y = (25 * linhas) - comprimentoCanos/2;
                botaoPower->usos = *usosPower;

            }
            colunas++;

            if (caracter == '\n'){          // a cada \n lido, acrescenta o nro de linhas e zera o de colunas para a lógica seguir dando certo
                linhas++;
                colunas = 0;
            }

        }

        fclose(fp);

        float larguraTartaruga = 52;
        float alturaTartaruga = 48;

        Rectangle posiInicialTartarugaEsq = { (canos[0].rect.x+canos[0].rect.width), (canos[0].rect.y + canos[0].rect.height/2 - alturaTartaruga/2), larguraTartaruga, alturaTartaruga};
        Rectangle posiInicialTartarugaDir = { (canos[1].rect.x-alturaTartaruga), (canos[1].rect.y + canos[1].rect.height/2 - alturaTartaruga/2), larguraTartaruga, alturaTartaruga};

        float larguraCaranguejo = 52;
        float alturaCaranguejo = 43;

        Rectangle posiInicialCaranguejoEsq = { (canos[0].rect.x+canos[0].rect.width), (canos[0].rect.y + canos[0].rect.height/2 - alturaCaranguejo/2), larguraCaranguejo, alturaCaranguejo};
        Rectangle posiInicialCaranguejoDir = { (canos[1].rect.x-alturaCaranguejo), (canos[1].rect.y + canos[1].rect.height/2 - alturaCaranguejo/2), larguraCaranguejo, alturaCaranguejo};


        Rectangle frameInicialTartarugaEsq = {6, 353, frameWidth, frameHeight};
        Rectangle frameInicialTartarugaDir = {6, 332, frameWidth, frameHeight};

        Rectangle frameInicialCaranguejo = {5, 420.5, frameWidth, frameHeight};

        for (int i = 0; i < infoFase->numMaxTartarugas; i++){          // setando as posições das tartarugas de acordo com seu indice no array de tartarugas
            if (i % 2 == 0){
                tartarugas[i].rect = posiInicialTartarugaDir;
                tartarugas[i].frameRect = frameInicialTartarugaEsq;    // se for par, sai em um cano da direita da tela, com sentido de desloc pra esquerda
                tartarugas[i].sentidoDesloc = esquerda;
            }
            else{
                tartarugas[i].rect = posiInicialTartarugaEsq;
                tartarugas[i].frameRect = frameInicialTartarugaDir;    // se for ímpar, sai em um cano da esquerda da tela, com sentido de desloc pra direita
                tartarugas[i].sentidoDesloc = direita;
            }
            tartarugas[i].speed.x = 1.4;                               // iniciando as características iniciais das tartarugas
            tartarugas[i].speed.y = 0;
            tartarugas[i].estado = normal;
            tartarugas[i].framesCounter = 0;
            tartarugas[i].colidindo = 0;
            tartarugas[i].morto = 0;
            tartarugas[i].morrendo = 0;
        }

        for (int j = 0; j < infoFase->numMaxCaranguejos; j++){         // setando as características iniciais dos caranguejos e atualizando suas posições
            if (j % 2 == 0){
                caranguejos[j].rect = posiInicialCaranguejoDir;
                caranguejos[j].frameRect = frameInicialCaranguejo;
                caranguejos[j].sentidoDesloc = esquerda;
            }
            else{
                caranguejos[j].rect = posiInicialCaranguejoEsq;
                caranguejos[j].frameRect = frameInicialCaranguejo;
                caranguejos[j].sentidoDesloc = direita;
            }
            caranguejos[j].speed.x = 1.4;
            caranguejos[j].speed.y = 0;
            caranguejos[j].estado = normal;
            caranguejos[j].framesCounter = 0;
            caranguejos[j].colidindo = 0;
            caranguejos[j].morto = 0;
            caranguejos[j].morrendo = 0;
        }

        float larguraMoeda = 20;
        float alturaMoeda = 40;

        Rectangle posiInicialMoedaEsq = { (canos[0].rect.x+canos[0].rect.width), (canos[0].rect.y + canos[0].rect.height/2 - alturaMoeda/2), larguraMoeda, alturaMoeda};
        Rectangle posiInicialMoedaDir = { (canos[1].rect.x-larguraMoeda), (canos[1].rect.y + canos[1].rect.height/2 - alturaMoeda/2), larguraMoeda, alturaMoeda};

        Rectangle frameInicialMoeda = {148, 137, frameWidth, frameHeight};

        for (int y = 0; y < infoFase->numMaxMoedas; y++){              // setando as características iniciais das moedas e atualizando suas posições
            if (y % 2 == 0){
                moedas[y].rect = posiInicialMoedaDir;
                moedas[y].sentidoDesloc = esquerda;
            }
            else{
                moedas[y].rect =  posiInicialMoedaEsq;
                moedas[y].sentidoDesloc = direita;
            }
            moedas[y].frameRect = frameInicialMoeda;
            moedas[y].framesCounter = 0;
            moedas[y].speed = 0;
            moedas[y].sumiu = 0;
            moedas[y].colidindo = 0;
            moedas[y].descendo = 0;
        }
}


void buscarDirArquivoFase(int level, char *dirArquivo){

    char dirArquivoFase[80] = "/Users/MAURO/Desktop/MarioDos/fases/fase",  numeroEmChar[5], extensaoArquivo[5] = ".txt";

    sprintf(numeroEmChar, "%d", level);

    // inserindo o numero em char no final da string do diretorio
    for (int i = strlen(dirArquivoFase), j = 0; i < (strlen(dirArquivoFase) + strlen(numeroEmChar)) && j < strlen(numeroEmChar); i++, j++){
        dirArquivoFase[i] = numeroEmChar[j];
    }
    // e logo após inserindo a extensão .txt no final da nova string
    for (int i = strlen(dirArquivoFase), j = 0; i < (strlen(dirArquivoFase) + strlen(extensaoArquivo)) && j < strlen(extensaoArquivo); i++, j++){
        dirArquivoFase[i] = extensaoArquivo[j];
    }
    // inserindo o delimitador da string
    dirArquivoFase[strlen(dirArquivoFase)] = '\0';

    strcpy(dirArquivo, dirArquivoFase);

}

void lerArquivoBinRanking(int *arquivoBinLido, int *pontuacaoPrimeiro, int *pontuacaoSegundo, int *pontuacaoTerceiro, int *pontuacaQuarto, int *pontuacaoQuinto, char *nome1, char *nome2, char *nome3, char *nome4, char *nome5){


    FILE *fp;

    fp = fopen("/Users/MAURO/Desktop/MarioDos/highscores.bin", "rb");

    if (fp != NULL){

        // lendo uma string e um inteiro por vez, 5 vezes (nome e pontuacao de cada um dos 5 colocados no ranking)
        // e armazenando nas devidas variaveis
        fread(nome1, 1, MAX_CARACTERES+1, fp);
        fread(pontuacaoPrimeiro, 4, 1, fp);

        fread(nome2, 1, MAX_CARACTERES+1, fp);
        fread(pontuacaoSegundo, 4, 1, fp);

        fread(nome3, 1, MAX_CARACTERES+1, fp);
        fread(pontuacaoTerceiro, 4, 1, fp);

        fread(nome4, 1, MAX_CARACTERES+1, fp);
        fread(pontuacaQuarto, 4, 1, fp);

        fread(nome5, 1, MAX_CARACTERES+1, fp);
        fread(pontuacaoQuinto, 4, 1, fp);

        fclose(fp);
    }

    *arquivoBinLido = 1;

}

void escreverArquivoBinRanking(char *nome, int pontuacaoMario){

    FILE *fp;

    int pontuacaoPrimeiro, pontuacaoSegundo, pontuacaoTerceiro, pontuacaoQuarto, pontuacaoQuinto, arquivoBinLido;
    char nome1[MAX_CARACTERES + 1], nome2[MAX_CARACTERES + 1], nome3[MAX_CARACTERES + 1], nome4[MAX_CARACTERES + 1], nome5[MAX_CARACTERES + 1];

    // lendo os colocados do ranking
    lerArquivoBinRanking(&arquivoBinLido, &pontuacaoPrimeiro, &pontuacaoSegundo, &pontuacaoTerceiro, &pontuacaoQuarto, &pontuacaoQuinto, nome1, nome2, nome3, nome4, nome5);

    // testando se a pontuacao atual é maior que alguma que está no ranking, senão nada muda no ranking
    if(pontuacaoMario > pontuacaoPrimeiro ||  pontuacaoMario > pontuacaoSegundo || pontuacaoMario > pontuacaoTerceiro || pontuacaoMario > pontuacaoQuarto || pontuacaoMario > pontuacaoQuinto){

        // trocando os colocados de lugar pra cada um dos casos
        if (pontuacaoMario >= pontuacaoPrimeiro){
            strcpy(nome5, nome4);
            strcpy(nome4, nome3);
            strcpy(nome3, nome2);
            strcpy(nome2, nome1);
            strcpy(nome1, nome);
            pontuacaoQuinto = pontuacaoQuarto;
            pontuacaoQuarto = pontuacaoTerceiro;
            pontuacaoTerceiro = pontuacaoSegundo;
            pontuacaoSegundo = pontuacaoPrimeiro;
            pontuacaoPrimeiro = pontuacaoMario;
        }
        else if(pontuacaoMario >= pontuacaoSegundo){
            strcpy(nome5, nome4);
            strcpy(nome4, nome3);
            strcpy(nome3, nome2);
            strcpy(nome2, nome);
            pontuacaoQuinto = pontuacaoQuarto;
            pontuacaoQuarto = pontuacaoTerceiro;
            pontuacaoTerceiro = pontuacaoSegundo;
            pontuacaoSegundo = pontuacaoMario;
        }
        else if(pontuacaoMario >= pontuacaoTerceiro){
            strcpy(nome5, nome4);
            strcpy(nome4, nome3);
            strcpy(nome3, nome);
            pontuacaoQuinto = pontuacaoQuarto;
            pontuacaoQuarto = pontuacaoTerceiro;
            pontuacaoTerceiro = pontuacaoMario;
        }
        else if(pontuacaoMario >= pontuacaoQuarto){
            strcpy(nome5, nome4);
            strcpy(nome4, nome);
            pontuacaoQuinto = pontuacaoQuarto;
            pontuacaoQuarto = pontuacaoMario;
        }
        else if(pontuacaoMario >= pontuacaoQuinto){
            strcpy(nome5, nome);
            pontuacaoQuinto = pontuacaoMario;
        }

        fp = fopen("/Users/MAURO/Desktop/MarioDos/highscores.bin", "wb");

        // escrevendo no arquivo o ranking atualizado
        if (fp != NULL){

            fwrite(nome1, 1, MAX_CARACTERES+1, fp);
            fwrite(&pontuacaoPrimeiro, 4, 1, fp);

            fwrite(nome2, 1, MAX_CARACTERES+1, fp);
            fwrite(&pontuacaoSegundo, 4, 1, fp);

            fwrite(nome3, 1, MAX_CARACTERES+1, fp);
            fwrite(&pontuacaoTerceiro, 4, 1, fp);

            fwrite(nome4, 1, MAX_CARACTERES+1, fp);
            fwrite(&pontuacaoQuarto, 4, 1, fp);

            fwrite(nome5, 1, MAX_CARACTERES+1, fp);
            fwrite(&pontuacaoQuinto, 4, 1, fp);

            fclose(fp);
        }
    }
}

void salvarArquivoContinuarPT1(FASE *infoFase, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, char *nome){

    FILE *fp;

    fp = fopen("/Users/MAURO/Desktop/MarioDos/continuar1.bin", "wb");

    // salvando informações do jogo no arquivo
    if (fp != NULL){
        fwrite(nome, 1, MAX_CARACTERES+1, fp);
        fwrite(infoFase, sizeof(FASE), 1, fp);
        fwrite(fase, sizeof(fase), 1, fp);
        fwrite(pontuacaoMario, sizeof(pontuacaoMario), 1, fp);
        fwrite(vidasMario, sizeof(vidasMario), 1, fp);
        fwrite(usosPower, sizeof(usosPower), 1, fp);
        fwrite(framesCounter, sizeof(framesCounter),  1, fp);
        fwrite(relogioMoedas, sizeof(relogioMoedas), 1 , fp);
        fclose(fp);
    }
}

void salvarArquivoContinuarPT2(MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, FASE infoFase){

    FILE *fp;

    fp = fopen("/Users/MAURO/Desktop/MarioDos/continuar2.bin", "wb");

    // salvando estruturas complexas do jogo no arquivo
    if (fp != NULL){
        fwrite(mario, sizeof(MARIO), 1, fp);
        fwrite(tartarugas, sizeof(INIMIGO), infoFase.numMaxTartarugas, fp);
        fwrite(caranguejos, sizeof(INIMIGO), infoFase.numMaxCaranguejos, fp);
        fwrite(plataformas, sizeof(OBSTACULO), infoFase.numPlataformas+1, fp);
        fwrite(canos, sizeof(OBSTACULO), infoFase.numCanos, fp);
        fwrite(retornosCanos, sizeof(OBSTACULO), infoFase.numRetornosCanos, fp);
        fwrite(moedas, sizeof(MOEDA), infoFase.numMaxMoedas, fp);
        fwrite(botaoPower, sizeof(POWER), 1, fp);
        fclose(fp);
    }
}


void lerArquivoContinuarPT1(FASE *infoFase, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, char *nome){

    FILE *fp;

    fp = fopen("/Users/MAURO/Desktop/MarioDos/continuar1.bin", "rb");

    // lendo informações do jogo do arquivo
    if (fp != NULL){
        fread(nome, 1, MAX_CARACTERES+1, fp);
        fread(infoFase, sizeof(FASE), 1, fp);
        fread(fase, sizeof(fase), 1, fp);
        fread(pontuacaoMario, sizeof(pontuacaoMario), 1, fp);
        fread(vidasMario, sizeof(vidasMario), 1 , fp);
        fread(usosPower, sizeof(usosPower), 1 , fp);
        fread(framesCounter, sizeof(framesCounter),  1, fp);
        fread(relogioMoedas, sizeof(relogioMoedas), 1 , fp);
        fclose(fp);
    }

}

void lerArquivoContinuarPT2(MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, FASE infoFase){

    FILE *fp;

    fp = fopen("/Users/MAURO/Desktop/MarioDos/continuar2.bin", "rb");

    // lendo estruturas complexas do jogo do arquivo
    if (fp != NULL){
        fread(mario, sizeof(MARIO), 1, fp);
        fread(tartarugas, sizeof(INIMIGO), infoFase.numMaxTartarugas, fp);
        fread(caranguejos, sizeof(INIMIGO), infoFase.numMaxCaranguejos, fp);
        fread(plataformas, sizeof(OBSTACULO), infoFase.numPlataformas+1, fp);
        fread(canos, sizeof(OBSTACULO), infoFase.numCanos, fp);
        fread(retornosCanos, sizeof(OBSTACULO), infoFase.numRetornosCanos, fp);
        fread(moedas, sizeof(MOEDA), infoFase.numMaxMoedas, fp);
        fread(botaoPower, sizeof(POWER), 1, fp);
        fclose(fp);
    }
}




