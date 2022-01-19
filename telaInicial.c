#include "raylib.h"
#include "telaInicial.h"
#include "telaNovoJogo.h"
#include "telaCarregarMapaRankingContinuar.h"
#include "telaSobreAjuda.h"

// inicializando o temporizador da tela inicial
int temporizadorTI = 0;

void desenhaTelaInicial(MENU *menuJogo, MIDIA *allMidias){

    // váriaveis pra cortar a sprite do MarioDos
    float frameWidth = allMidias->caixaMarioDos.width/2;
    float frameHeight =  allMidias->caixaMarioDos.height/4;
    Rectangle frameRec = { 20, 9, frameWidth, frameHeight};
    float larguraCaixa = 646;
    Rectangle frameDest = {(screenWidth/2)-(larguraCaixa/2), 70, 645, 236};
    Vector2 position = { 0, 0 };
    // retangulo do gold edition
    Rectangle goldEdition = {30, 30, screenWidth-60, screenHeight-60};
    Rectangle goldEdition2 = {38, 38, screenWidth-76, screenHeight-76};
    // mensagens na tela inicial
    const char msgNovoJogo[10] = "Novo jogo";
    const char msgContinuar[10] = "Continuar";
    const char msgCarregarMapa[14] = "Carregar Mapa";
    const char msgRanking[8] = "Ranking";
    const char msgAjuda[6] = "Ajuda";
    const char msgSobre[6] = "Sobre";
    const char msgSair[5] = "Sair";
    const char msgAutor[100] = "remake by Pedro Arejano Scheunemann";
    const char msgGoldEdition[13] = "GOLD EDITION";
    // tamanho das letras das mensagens
    const int tamanhoLetra = 25;
    const int espacamento = 4;
    // pontos X e Y origem de construção das mensagens
    const int YorigemTexto = 355;
    // vetores com as posições de cada mensagem na tela inicial
    Vector2 posiNovoJogo = {(screenWidth/2)-(MeasureText(msgNovoJogo, tamanhoLetra)-50/2), YorigemTexto};
    Vector2 posiContinuar = {(screenWidth/2)-(MeasureText(msgContinuar, tamanhoLetra)-60/2), YorigemTexto+40};
    Vector2 posiCarregarMapa = {(screenWidth/2)-(MeasureText(msgCarregarMapa, tamanhoLetra)-110/2), YorigemTexto+80};
    Vector2 posiRanking = {(screenWidth/2)-(MeasureText(msgRanking, tamanhoLetra)-50/2), YorigemTexto+120};
    Vector2 posiAjuda = {(screenWidth/2)-(MeasureText(msgAjuda, tamanhoLetra)-27/2), YorigemTexto+160};
    Vector2 posiSobre = {(screenWidth/2)-(MeasureText(msgSobre, tamanhoLetra)-40/2), YorigemTexto+200};
    Vector2 posiSair = {(screenWidth/2)-(MeasureText(msgSair, tamanhoLetra)-18/2), YorigemTexto+240};
    Vector2 posiGoldEdition = {978, 675};


    BeginDrawing();
    ClearBackground(BLACK);

    // desenhando a caixa do MarioDos
    DrawTextureTiled(allMidias->caixaMarioDos, frameRec, frameDest, position, 0.0f, 3.2, WHITE);
    // desenhando todas as opções em branco
    DrawTextEx(allMidias->fonteMario, msgNovoJogo, posiNovoJogo, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgContinuar, posiContinuar, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgCarregarMapa, posiCarregarMapa, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgRanking, posiRanking, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgAjuda, posiAjuda, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgSobre, posiSobre, tamanhoLetra, espacamento, WHITE);
    DrawTextEx(allMidias->fonteMario, msgSair, posiSair, tamanhoLetra, espacamento, WHITE);

    // fazendo a animação de mudar de opção
    switch(menuJogo->optionSelect){
        case NovoJogo:      DrawTextEx(allMidias->fonteMario, msgNovoJogo, posiNovoJogo, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case Continuar:     DrawTextEx(allMidias->fonteMario, msgContinuar, posiContinuar, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case CarregarMapa:  DrawTextEx(allMidias->fonteMario, msgCarregarMapa, posiCarregarMapa, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case Ranking:       DrawTextEx(allMidias->fonteMario, msgRanking, posiRanking, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case Ajuda:         DrawTextEx(allMidias->fonteMario, msgAjuda, posiAjuda, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case Sobre:         DrawTextEx(allMidias->fonteMario, msgSobre, posiSobre, tamanhoLetra, espacamento, SKYBLUE);
                            break;
        case Sair:          DrawTextEx(allMidias->fonteMario, msgSair, posiSair, tamanhoLetra, espacamento, SKYBLUE);
                            break;
    }

    // desenhando o retangulo amarelo piscante
    if ((temporizadorTI >= 0 && temporizadorTI < 200) || (temporizadorTI >= 210 && temporizadorTI < 220) || temporizadorTI >= 230) DrawRectangleLinesEx(goldEdition, 3, YELLOW);
    // desenhando a mensagem de gold edition e o retangulo amarelo
    if (temporizadorTI >= 230){
        DrawTextEx(allMidias->fonteGold, msgGoldEdition, posiGoldEdition, tamanhoLetra-6, espacamento, WHITE);
        DrawRectangleLinesEx(goldEdition2, 4, YELLOW);
    }
    // depois de desenhar a mensagem do autor
    else DrawText(TextSubtext(msgAutor, 0, temporizadorTI/5), 830, 674, 18, WHITE);

    EndDrawing();

    // atualizando a contagem do tempo apenas até chegar em 230
    if (temporizadorTI <= 230) temporizadorTI++;
}

void atualizaTelaInicial(MENU *menuJogo, MIDIA *allMidias){

    if (temporizadorTI == 1) PlaySound(allMidias->startGame);  // som da tela inicial
    // fazendo a lógica de mudança de opção e opção selecionada
    switch(menuJogo->optionSelect){
        case NovoJogo:      if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = Sair;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = Continuar;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case Continuar:     if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = NovoJogo;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = CarregarMapa;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case CarregarMapa:  if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = Continuar;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = Ranking;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case Ranking:       if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = CarregarMapa;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = Ajuda;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case Ajuda:         if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = Ranking;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = Sobre;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case Sobre:         if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = Ajuda;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = Sair;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;

        case Sair:          if(IsKeyPressed(KEY_UP)) menuJogo->optionSelect = Sobre;
                            else if(IsKeyPressed(KEY_DOWN)) menuJogo->optionSelect = NovoJogo;
                            else if(IsKeyPressed(KEY_ENTER)) menuJogo->optionSelected = 1;
                            break;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) PlaySound(allMidias->setaMenu);
        if (IsKeyPressed(KEY_ENTER)) PlaySound(allMidias->enter);

}

void construirTelaInicial(MENU *menuJogo, MIDIA *allMidias){

    atualizaTelaInicial(menuJogo, allMidias);
    desenhaTelaInicial(menuJogo, allMidias);

}

void construirJogo(MENU *menuJogo, MIDIA *allMidias){

    // se o usuário não escolheu nenhuma opção, atualiza e desenha a tela inicial
    if (!menuJogo->optionSelected) construirTelaInicial(menuJogo, allMidias);
    // senão, atualiza e desenha a tela que ele escolheu
    else{
        switch(menuJogo->optionSelect){
            case NovoJogo:      construirTelaNovoJogo(menuJogo, allMidias);
                                break;

            case Continuar:     construirTelaContinuar(menuJogo, allMidias);
                                break;

            case CarregarMapa:  construirTelaCarregarMapa(menuJogo, allMidias);
                                break;

            case Ranking:       construirTelaRanking(menuJogo, allMidias);
                                break;

            case Ajuda:         construirTelaAjuda(menuJogo, allMidias);
                                break;

            case Sobre:         construirTelaSobre(menuJogo, allMidias);
                                break;

        }
    }
}





