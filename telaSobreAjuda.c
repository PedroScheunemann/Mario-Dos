#include "raylib.h"
#include "telaSobreAjuda.h"
#include <locale.h>
#include "midias.h"
#include "telaNovoJogo.h"


// origem (x,y) da construção das mensagens na tela
float origemY = 84;
float origemX = 314;

// inicia temporizador da tela Sobre
int temporizadorS = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////  TELA SOBRE  //////////////////////////////////////////////////////////////////////////////////


void desenhaTelaSobre(MENU *menuJogo, MIDIA *allMidias){

    // mensagens na tela inicial
    const char msg1[60] = "inspirado no Mario Bros de 1983";
    const char msg2[80] = "o jogo foi escrito na linguagem C";
    const char msg3[60] = "desenvolvido a partir da biblioteca raylib";
    const char msg4[60] = "Autor: Pedro Arejano Scheunemann";
    const char msg5[60] = "Graduando em Ciencia da Computacao - UFRGS";
    const char msg6[60] = "@pedroarejanosch";

    // tamanho das letras das mensagens
    const int tamanhoLetra = 16;
    const int espacamento = 4;

    // vetores com as posições de cada mensagem na tela
    Vector2 posi1 = {origemX+78, origemY+250};
    Vector2 posi2 = {origemX+65, origemY+275};
    Vector2 posi3 = {origemX+10, origemY+300};
    Vector2 posi4 = {origemX+65, origemY+325};
    Vector2 posi5 = {origemX-5,  origemY+350};
    Vector2 posi6 = {origemX+185,origemY+390};

    // váriaveis pra cortar a sprite do MarioDos com a caixa
    float frameWidth = allMidias->caixaMarioDos.width/2;
    float frameHeight =  allMidias->caixaMarioDos.height/4;
    Rectangle frameRec = { 20, 9, frameWidth, frameHeight};
    float larguraCaixa = 646;
    Rectangle frameDest = {(screenWidth/2)-(larguraCaixa/2), origemY-14, 645, 236};
    Vector2 position = {0, 0};


    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextureTiled(allMidias->caixaMarioDos, frameRec, frameDest, position, 0, 3.2, WHITE);

    // desenhando todas as mensagens em branco
    DrawTextEx(allMidias->fonteMario, msg1, posi1, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg2, posi2, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg3, posi3, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg4, posi4, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg5, posi5, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg6, posi6, tamanhoLetra-4, espacamento, WHITE);

    desenhaVoltar(&allMidias->fonteMario);

    EndDrawing();

    if (temporizadorS > 200){
        if (origemY > screenHeight) origemY = -365;
        else origemY++;
    }
    else temporizadorS++;

}

void atualizaTelaSobre(MENU *menuJogo){

    atualizaVoltar(menuJogo);
    if (menuJogo->optionSelected == 0){
        origemY = 84;                                 // reinicializa a origem Y da construção da imagem
        temporizadorS = 0;                             // e o temporizador da construção
    }
}

void construirTelaSobre(MENU *menuJogo, MIDIA *allMidias){

    atualizaTelaSobre(menuJogo);
    desenhaTelaSobre(menuJogo, allMidias);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////// TELA AJUDA //////////////////////////////////////////////////////////////////////////////////////



void desenhaTelaAjuda(MENU *menuJogo, MIDIA *allMidias){

    // mensagens na tela inicial
    const char msg1[8] = "GERAL";
    const char msg2[80] = "Pressione 'Delete' para voltar de qualquer tela";
    const char msg3[80] = "Pressione 'ESC' ou o icone de fechar para sair do jogo";
    const char msg4[8] = "MENU";
    const char msg5[80] = "Pressione as setas 'Up' e 'Down' para alterar o seletor";
    const char msg6[70] = "Pressione 'Enter' para selecionar a opcao";
    const char msg7[10] = "GAMEPLAY";
    const char msg8[80] = "Pressione as setas 'Left' e 'Right' para movimentar o Mario para os lados";
    const char msg9[40] = "Pressione 'D' para o Mario pular";
    const char msg10[40] = "Pressione 'Delete' para pausar o jogo";

    // tamanho das letras das mensagens
    const int tamanhoLetra = 16;
    const int espacamento = 4;

    // vetores com as posições de cada mensagem na tela
    Vector2 posi1 = {60, screenHeight/12};
    Vector2 posi2 = {60, (screenHeight*2/12)};
    Vector2 posi3 = {60, (screenHeight*3/12)};
    Vector2 posi4 = {60, (screenHeight*4/12)};
    Vector2 posi5 = {60,  (screenHeight*5/12)};
    Vector2 posi6 = {60, (screenHeight*6/12)};
    Vector2 posi7 = {60, (screenHeight*7/12)};
    Vector2 posi8 = {60, (screenHeight*8/12)};
    Vector2 posi9 = {60, (screenHeight*9/12)};
    Vector2 posi10 = {60, (screenHeight*10/12)};

    Rectangle frame1 = {40,  posi1.y-20, screenWidth-80,posi4.y-posi1.y};
    Rectangle frame2 = {40, posi4.y-20, screenWidth-80, posi7.y-posi4.y};
    Rectangle frame3 = {40,  posi7.y-20, screenWidth-80, (screenHeight*11/12)-posi7.y};


    BeginDrawing();
    ClearBackground(BLACK);

    // desenhando todas as mensagens em branco
    DrawTextEx(allMidias->fonteMario, msg1, posi1, tamanhoLetra+20, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg2, posi2, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg3, posi3, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg4, posi4, tamanhoLetra+20, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg5, posi5, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg6, posi6, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg7, posi7, tamanhoLetra+20, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg8, posi8, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg9, posi9, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msg10, posi10, tamanhoLetra, espacamento, WHITE);
    // retangulos em volta
    DrawRectangleLinesEx(frame1, 5, GREEN);
    DrawRectangleLinesEx(frame2, 5, BLUE);
    DrawRectangleLinesEx(frame3, 5, WHITE);

    desenhaVoltar(&allMidias->fonteMario);

    EndDrawing();
}


void construirTelaAjuda(MENU *menuJogo, MIDIA *allMidias){

    atualizaVoltar(menuJogo);
    desenhaTelaAjuda(menuJogo, allMidias);

}












