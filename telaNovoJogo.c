#include "raylib.h"
#include "telaNovoJogo.h"
#include "movimentacao.h"
#include "arquivo.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


// inicializando as variaveis pra armazenar a string do nome digitado
char nome[MAX_CARACTERES + 1]= "\0";
int contadorLetras = 0;
// flag pra saber se o usuario terminou de digitar o nome
int nomeInserido = 0;
// tamanho padrão das letras da mensagens
const int tamanhoLetra = 28;
const int espacamento = 4;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// TELA DO NOME //////////////////////////////////////////////////////////////////////


void desenhaNome(char *nome, Font *fontMario){

    // mensagem digite seu nome
    const char msgDigiteSeuNome[20] = "Digite seu nome:";
    // posição e tamanho da caixa de texto para inserir o nome
    Rectangle caixaNome = {200, 300, 800, 80};
    // posição da mensagem digite seu nome
    Vector2 posiDigiteSeuNome = {caixaNome.x+226, caixaNome.y-35};


    // desenhando a mensagem digite seu nome
    DrawTextEx(*fontMario, msgDigiteSeuNome, posiDigiteSeuNome, tamanhoLetra, espacamento, WHITE);
    // desenhando a caixa
    DrawRectangleRounded(caixaNome, 0.8, 0, LIGHTGRAY);
    DrawRectangleRoundedLines(caixaNome, 0.8, 0, 2, BLUE);
    // desenhando o nome escrito no array com as teclas digitadas pelo usuário
    DrawText(nome, caixaNome.x + 200, caixaNome.y + 14, 50, BLACK);

    // desenhando o _, para ficar com animação de digitação
    if (contadorLetras < MAX_CARACTERES){
        DrawText("_", caixaNome.x + 210 + MeasureText(nome, 50), caixaNome.y + 19, 50, BLACK);
    }
}

void atualizaNome(char *nome, int tamMaxString, int *contadorLetras, int *inserido, MIDIA *allMidias){

    int key = GetCharPressed(), contador = *contadorLetras;

    while (key > 0) {
        // o caractere digitado deve ser um numero, letra do alfabeto ou pontuação
        // e o contador de letras deve ser menor que o máximo de caracteres no array
        if ((key >= 32) && (key <= 125) && (contador < tamMaxString)) {
            nome[contador] = (char)key;     // armazena o caracter digitado no array
            contador++;                     // acrescenta o contador de letras
            PlaySound(allMidias->keyPress); // som de tecla
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {          // se pressionou backspace, decrescenta o contador de letras
        contador--;
        if (contador < 0) contador = 0;         // se o contador for menor que 0, atribui zero a ele para não ter contador negativo
        nome[contador] = '\0';                  // e substitui a ultima letra armazenada por "\0", excluindo-a do array
        PlaySound(allMidias->backspace);        // som do backspace
    }
    else if (IsKeyPressed(KEY_ENTER) && contador > 0){ *inserido = 1; PlaySound(allMidias->enter);} // som do enter //se pressionou enter e escreveu mais de uma letra, aciona a flag pra passar pra proxima tela
    else if (IsKeyPressed(KEY_SPACE)){ StopSound(allMidias->keyPress); PlaySound(allMidias->backspace);} // som do space

    *contadorLetras = contador;
}


void desenhaVoltar(Font *fontMario){

    // mensagem voltar
    const char msgVoltar[20] = "Voltar";
    // posição da mensagem voltar
    Vector2 posiVoltar= {screenWidth - 130, screenHeight - 40};
    // retangulo da mensagem voltar
    Rectangle rectVoltar = {screenWidth - 130, screenHeight - 40, 100, tamanhoLetra - 10};

    // se passou o mouse desenha a mensagem em azul, senão em branco
    if (CheckCollisionPointRec(GetMousePosition(), rectVoltar)) DrawTextEx(*fontMario, msgVoltar, posiVoltar, tamanhoLetra-10, espacamento, SKYBLUE);
    else DrawTextEx(*fontMario, msgVoltar, posiVoltar, tamanhoLetra-10, espacamento, WHITE);
}

void atualizaVoltar(MENU *menuJogo){

    // retangulo da mensagem voltar
    Rectangle rectVoltar = {screenWidth - 130, screenHeight - 40, 100, tamanhoLetra - 10};

    // se clicou na mensagem de Voltar ou clicou Backspace, volta
    if ((CheckCollisionPointRec(GetMousePosition(), rectVoltar) &&  IsMouseButtonPressed(0)) || IsKeyPressed(KEY_DELETE)) menuJogo->optionSelected = 0;  // voltando pro menu

}

void desenhaTelaNome(char *nome, Font *fontMario){

    BeginDrawing();
    ClearBackground(BLACK);

    desenhaNome(nome, fontMario);
    desenhaVoltar(fontMario);

    EndDrawing();
}


void atualizaTelaNome(char *nome, MENU *menuJogo, MIDIA *allMidias){

    atualizaNome(nome, MAX_CARACTERES, &contadorLetras, &nomeInserido, allMidias);
    atualizaVoltar(menuJogo);
    if (menuJogo->optionSelected == 0){
        for (int i = contadorLetras; i >= 0; i--) nome[i] = '\0';  // reinicializando o array com o nome escrito pra voltar pro menu
        contadorLetras = 0;                                        // zerando o contador de letras também
    }
}

void construirTelaNome(char *nome, MENU *menuJogo,  MIDIA *allMidias){

    atualizaTelaNome(nome, menuJogo, allMidias);
    desenhaTelaNome(nome, &allMidias->fonteMario);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// TELA DA GAMEPLAY /////////////////////////////////////////////////////////////////////


void desenhaPlataformas(Texture2D *caixaMarioDos, OBSTACULO *plataformas, int numPlataformas){

    float frameWidth = caixaMarioDos->width;
    float frameHeight = caixaMarioDos->height;
    // váriaveis pra cortar a sprite dos tijolos
    Rectangle frameTijolos = {10, 292, frameWidth, frameHeight};
    Vector2 positionPlat = { 0, 4 };

    // desenha a plataforma de tijolos
    DrawTextureTiled(*caixaMarioDos, frameTijolos, plataformas[0].rect, positionPlat, 0, 5, WHITE);
     // desenha as plataformas azuis
    for (int i = 1; i < numPlataformas+1; i++){
        DrawRectangleRec(plataformas[i].rect, BLUE);
    }
}

void desenhaCanos(Texture2D *caixaMarioDos, OBSTACULO *canos, OBSTACULO *retornosCanos, int numCanos, int numRetornosCanos){

    float frameWidth = caixaMarioDos->width;
    float frameHeight = caixaMarioDos->height;
    // váriaveis pra cortar a sprite do cano da esquerda cima
    Rectangle frameCanoEsq = {51.5, 244, frameWidth, frameHeight};
    Vector2 positionCanos = {0,0};
    // váriaveis pra cortar a sprite do cano da direita cima
    Rectangle frameCanoDir = {51, 201, frameWidth, frameHeight};
    // váriaveis pra cortar a sprite do cano da direita baixo
    Rectangle frameRetornoCanoDir = {10.7, 257.5, frameWidth, frameHeight};
    // váriaveis pra cortar a sprite do cano da esquerda baixo
    Rectangle frameRetornoCanoEsq = {10, 214.5, frameWidth, frameHeight};

    // desenha os canos de acordo com sua posição na tela (direita ou esquerda)
    for (int i = 0; i < numCanos; i++){
        if (canos[i].rect.x < screenWidth/2) DrawTextureTiled(*caixaMarioDos, frameCanoEsq, canos[i].rect, positionCanos, 0, 4.5, WHITE);
        else DrawTextureTiled(*caixaMarioDos, frameCanoDir, canos[i].rect, positionCanos, 0, 4.5, WHITE);
    }
    // desenha os retornos dos canos
    for (int j = 0; j < numRetornosCanos; j++){
        if (retornosCanos[j].rect.x < screenWidth/2) DrawTextureTiled(*caixaMarioDos, frameRetornoCanoEsq, retornosCanos[j].rect, positionCanos, 0, 4.5, WHITE);
        else DrawTextureTiled(*caixaMarioDos, frameRetornoCanoDir, retornosCanos[j].rect, positionCanos, 0, 4.5, WHITE);
    }
}


void desenhaPontuacao(MIDIA *allMidias, MARIO *mario){

    float frameWidth = allMidias->caixaMarioDos.width;
    float frameHeight = allMidias->caixaMarioDos.height;
    // váriaveis pra cortar a sprite da cabeça do mario
    Rectangle frameCabecaMario = {2, 590, frameWidth, frameHeight};
    Vector2 positionVida1 = { 0, 0 };
    Vector2 positionVida2 = { -40, 0 };
    Vector2 positionVida3 = { -80, 0 };
    Rectangle rectVidas = {300, 10, 200, 50};
    // variaveis pra desenhar a pontuacao
    Vector2 posiPontuacao = {145, 18};
    char pontuacaoEmChar[5];

    // transformando a pontuacao de inteiro pra char
    sprintf(pontuacaoEmChar, "%d", mario->pontuacao);

    DrawText("I ", 120, 14.5, 39, SKYBLUE);
    DrawCircle(135.5, 31.8, 3, SKYBLUE);
    DrawTextEx(allMidias->fonteMario, pontuacaoEmChar, posiPontuacao, 36, 2, WHITE);

    // desenha as vidas com a cabeça do mario
    switch(mario->lifes){
        case 3: DrawTextureTiled(allMidias->caixaMarioDos, frameCabecaMario, rectVidas, positionVida3, 0, 4, WHITE);
        case 2: DrawTextureTiled(allMidias->caixaMarioDos, frameCabecaMario, rectVidas, positionVida2, 0, 4, WHITE);
        case 1: DrawTextureTiled(allMidias->caixaMarioDos, frameCabecaMario, rectVidas, positionVida1, 0, 4, WHITE);
                break;
    }
}

void desenhaBotaoPower(Texture2D *caixaMarioDos, POWER *botaoPower){

    float frameWidth = caixaMarioDos->width;
    float frameHeight = caixaMarioDos->height;
    // váriaveis pra cortar a sprite da cabeça do mario
    Rectangle frameBotaoPower = {133, 243, frameWidth, frameHeight};
    Vector2 position = {0, 0};

    // desenha o botao power
    DrawTextureTiled(*caixaMarioDos, frameBotaoPower, botaoPower->rect, position, 0, 3.7, WHITE);
}

void desenhaMario(Texture2D *caixaMarioDos, MARIO *mario){

    float frameWidth = caixaMarioDos->width;
    float frameHeight = caixaMarioDos->height;
    Vector2 position = {0, 0};
    // váriaveis pra cortar a sprite dos tijolos
    Rectangle frameTijolos = {208.5, 277.5, frameWidth, frameHeight};
    Rectangle tijolos = {mario->rect.x - 5, mario->rect.y + mario->rect.height, mario->rect.width + 10, 40};

    // desenha o mario
    DrawTextureTiled(*caixaMarioDos, mario->frameRect, mario->rect, position, 0, 4, WHITE);
    // desenhando a plataforma quando o mario ta caindo do céu
    if (mario->atingido && mario->parado) DrawTextureTiled(*caixaMarioDos, frameTijolos, tijolos, position, 0, 4.0, WHITE);

}


void desenhaInimigos(Texture2D *caixaMarioDos, INIMIGO *tartarugas, INIMIGO *caranguejos, int numTartarugasAtivas, int numCaranguejosAtivos){

    Vector2 position = {0,0};

    // desenha os inimigos de acordo o nro de inimigos ativos, dessa forma respeitando o delay
    for (int i = 0; i < numTartarugasAtivas; i++){
        DrawTextureTiled(*caixaMarioDos, tartarugas[i].frameRect, tartarugas[i].rect, position, 0, 3, WHITE);
    }
    for (int j = 0; j < numCaranguejosAtivos; j++){
        DrawTextureTiled(*caixaMarioDos, caranguejos[j].frameRect, caranguejos[j].rect, position, 0, 3, WHITE);
    }

}

void desenhaMoedas(Texture2D *caixaMarioDos, MOEDA *moedas, int numMoedasAtivas){

    Vector2 position = {0,0};
    // desenha as moedas uma por uma de acordo com o nro de moedas ativas
    for (int i = 0; i < numMoedasAtivas; i++){
        DrawTextureTiled(*caixaMarioDos, moedas[i].frameRect, moedas[i].rect, position, 0, 3, WHITE);
    }

}

void desenhaNroFase(Font *fonteMario, int fase){

    char msgFase[10] = "FASE", faseNroEmChar[5];
    Vector2 posiFase = {525, 500};
    Vector2 posiNro = {posiFase.x+120, posiFase.y};

    // transformando a fase de inteiro pra char
    sprintf(faseNroEmChar, "%d", fase);
    // desenha o texto de fase
    DrawTextEx(*fonteMario, msgFase, posiFase, 32, 4, WHITE);
    DrawTextEx(*fonteMario, faseNroEmChar, posiNro, 32, 4, WHITE);

}

void desenhaTelaGameOver(MIDIA *allMidias, int framesCounter, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, POWER *botaoPower, int numPlataformas, int numCanos, int numRetornosCanos){

    const char msgGameOver[10] = "GAME OVER";
    Vector2 posiGameOver = {screenWidth/2 - 135 ,  screenHeight/3+ 30};

    BeginDrawing();
    ClearBackground(BLACK);

    // desenha o mapa
    desenhaCanos(&allMidias->caixaMarioDos, canos, retornosCanos, numCanos, numRetornosCanos);
    desenhaPlataformas(&allMidias->caixaMarioDos, plataformas, numPlataformas);
    desenhaBotaoPower(&allMidias->caixaMarioDos, botaoPower);
    // e a mensagem de game over
    if (framesCounter < 550 || (framesCounter >= 560 && framesCounter < 570) || (framesCounter >= 580 && framesCounter <= 600)){
        DrawTextEx(allMidias->fonteMario, msgGameOver, posiGameOver, 38, 4, WHITE);
    }

    EndDrawing();

}

void desenhaTelaPause(FASE infoFase, MIDIA *allMidias, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower){

    const char msgPause[10] = "|| Pause";
    const char msgMenu[50] = "Pressione a tecla 'Enter' para voltar ao jogo";
    const char msgVoltar[50] = "Pressione a tecla 'Delete' para voltar ao menu";
    const char msgSalvar[50] = "Pressione a tecla 'S' para salvar o jogo";

    Vector2 posiPause = {screenWidth/2 - 80 ,  screenHeight/3+ 30};
    Vector2 posiMenu = {screenWidth/2 - 340 ,  screenHeight-200};
    Vector2 posiVoltar = {screenWidth/2 - 350 ,  screenHeight-170};
    Vector2 posiSalvar = {screenWidth/2 - 300 ,  screenHeight-140};

    Rectangle frameMsg = {screenWidth/2 - 390 ,  screenHeight-215, 770, 100};
    Rectangle frameMsg2 = {screenWidth/2 - 395 ,  screenHeight-220, 780, 110};


    BeginDrawing();
    ClearBackground(BLACK);

    // desenha o mapa, os inimigos e o mario
    desenhaCanos(&allMidias->caixaMarioDos, canos, retornosCanos, infoFase.numCanos, infoFase.numRetornosCanos);
    desenhaPlataformas(&allMidias->caixaMarioDos, plataformas, infoFase.numPlataformas);
    desenhaMoedas(&allMidias->caixaMarioDos, moedas, infoFase.numMoedasAtivas);
    desenhaPontuacao(allMidias, mario);
    desenhaBotaoPower(&allMidias->caixaMarioDos, botaoPower);
    desenhaInimigos(&allMidias->caixaMarioDos, tartarugas, caranguejos, infoFase.numTartarugasAtivas, infoFase.numCaranguejosAtivos);
    desenhaMario(&allMidias->caixaMarioDos, mario);
    // efeito cinza
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.8));
    // desenha o pause
    DrawTextEx(allMidias->fonteMario, msgPause, posiPause, 34, 4, WHITE);
    // desenha as mensagens de voltar e salvar jogo
    DrawTextEx(allMidias->fonteMario, msgMenu, posiMenu, 18, 4, WHITE);
    DrawTextEx(allMidias->fonteMario, msgVoltar, posiVoltar, 18, 4, WHITE);
    DrawTextEx(allMidias->fonteMario, msgSalvar, posiSalvar, 18, 4, WHITE);
    // retangulo em volta
    DrawRectangleLinesEx(frameMsg, 5, YELLOW);
    DrawRectangleLinesEx(frameMsg2, 5, BLACK);

    EndDrawing();

}


void desenhaTelaGameEnd(MIDIA *allMidias, int framesCounter){

    const char msgGameEnd[10] = "GAME END!";
    Vector2 posiGameEnd = {screenWidth/2 - 145 ,  screenHeight/3+ 10};

    Rectangle fundo = {posiGameEnd.x-10, posiGameEnd.y-10, 320, 55};

    float frameWidth = allMidias->caixaMarioDos.width;
    float frameHeight = allMidias->caixaMarioDos.height;

    // váriaveis pra cortar a sprite dos tijolos
    Rectangle frameTijolos = {10, 292, frameWidth, frameHeight};
    Vector2 positionPlat = { 0, 4 };
    Rectangle plataformaDeBaixo = {0, screenHeight-76, screenWidth, 87};

    // váriaveis pras posições dos marios e o frame
    Rectangle frameMario = {92, 602.8, frameWidth, frameHeight};           // pra esquerda 602.8 e pra direita 622  // subindo 92
    Rectangle lugarMario1 = {posiGameEnd.x, posiGameEnd.y+60, 66, 84};
    Rectangle lugarMario2 = {lugarMario1.x+120, posiGameEnd.y+60, 66, 84};
    Rectangle lugarMario3 = {lugarMario2.x+120, posiGameEnd.y+60, 66, 84};
    Vector2 position = {0, 0};

    // variáveis pra imagem dos fogos
    float frameFogosWidth = allMidias->fogos.width;
    float frameFogosHeight = allMidias->fogos.height;
    Rectangle frameRec = {0, 0, frameFogosWidth, frameFogosHeight};
    Rectangle frameDest = {0, 100, screenWidth, 390};


    BeginDrawing();
    ClearBackground(BLACK);

    // desenha os fogos
    DrawTextureTiled(allMidias->fogos, frameRec, frameDest, position, 0, 0.6, WHITE);
    // desenha a plataforma de tijolos
    DrawTextureTiled(allMidias->caixaMarioDos, frameTijolos, plataformaDeBaixo, positionPlat, 0, 5, WHITE);
    // desenha os marios
    DrawTextureTiled(allMidias->caixaMarioDos, frameMario, lugarMario1, position, 0, 4, WHITE);
    DrawTextureTiled(allMidias->caixaMarioDos, frameMario, lugarMario2, position, 0, 4, WHITE);
    DrawTextureTiled(allMidias->caixaMarioDos, frameMario, lugarMario3, position, 0, 4, WHITE);
    // desenha mensagem de game end num retangulo
    if (framesCounter < 300 || (framesCounter >= 310 && framesCounter < 320) || (framesCounter >= 330 && framesCounter <= 350)){
        DrawRectangleRec(fundo, WHITE);
        DrawTextEx(allMidias->fonteMario, msgGameEnd, posiGameEnd, 48, 4, BLACK);
    }

    EndDrawing();

}

void desenhaTelaGameplay(FASE infoFase, MIDIA *allMidias, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower){

    BeginDrawing();
    ClearBackground(BLACK);

    // desenha o mapa, os inimigos e o mario
    desenhaCanos(&allMidias->caixaMarioDos, canos, retornosCanos, infoFase.numCanos, infoFase.numRetornosCanos);
    desenhaPlataformas(&allMidias->caixaMarioDos, plataformas, infoFase.numPlataformas);
    desenhaMoedas(&allMidias->caixaMarioDos, moedas, infoFase.numMoedasAtivas);
    desenhaPontuacao(allMidias, mario);
    desenhaBotaoPower(&allMidias->caixaMarioDos, botaoPower);
    desenhaInimigos(&allMidias->caixaMarioDos, tartarugas, caranguejos, infoFase.numTartarugasAtivas, infoFase.numCaranguejosAtivos);
    desenhaMario(&allMidias->caixaMarioDos, mario);

    EndDrawing();

}


void atualizaTelaGameplay(FASE *infoFase, MIDIA *allMidias, int *fase, int *pontuacaoMario, int *vidasMario, int *usosPower, float *framesCounter, float *relogioMoedas, MARIO *mario, INIMIGO *tartarugas, INIMIGO *caranguejos, OBSTACULO *plataformas, OBSTACULO *canos, OBSTACULO *retornosCanos, MOEDA *moedas, POWER *botaoPower, Texture2D *caixaMarioDos){

    int tartarugasAtivas = infoFase->numTartarugasAtivas, caranguejosAtivos = infoFase->numCaranguejosAtivos, moedasAtivas = infoFase->numMoedasAtivas;
    int framesCount = *framesCounter, relogio = *relogioMoedas;

    // le o teclado, atualiza as posições do mario e o frame do mario
    lerTeclado(mario);
    atualizaPosiMario(allMidias, infoFase, mario, canos, retornosCanos, plataformas, tartarugas, caranguejos, moedas, botaoPower);
    atualizaFrameRectMario(mario);


    // "ativando" o primeiro inimigo depois de 3 segundos
    if (tartarugasAtivas == 0 && caranguejosAtivos == 0){
        framesCount++;
        if(framesCount/60 > 3){
            if (infoFase->numMaxTartarugas == 0) caranguejosAtivos++;
            else if (infoFase->numMaxCaranguejos == 0) tartarugasAtivas++;
            else tartarugasAtivas++;
            framesCount = 0;
        }
    }
    else{
        // acrescenta o nro de inimigos ativos de acordo com o delay lido do arquivo
        if (tartarugasAtivas < infoFase->numMaxTartarugas || caranguejosAtivos < infoFase->numMaxCaranguejos){
            framesCount++;
            if (framesCount/60 >= infoFase->delayInimigos){
                if (tartarugasAtivas < infoFase->numMaxTartarugas) tartarugasAtivas++;
                else caranguejosAtivos++;
                framesCount = 0;
            }
        }
    }
    // atualiza as tartarugas ativas, tanto a posição como seus frames
    for (int i = 0; i < tartarugasAtivas; i++){
        atualizaPosiInimigo(allMidias, *infoFase, tartaruga, canos, retornosCanos, plataformas, tartarugas, caranguejos, &tartarugas[i], mario, *fase);
        atualizaFrameRectTartaruga(&tartarugas[i]);
    }
    // atualiza os caranguejos ativas, tanto a posição como seus frames
    for (int j = 0; j < caranguejosAtivos; j++){
        atualizaPosiInimigo(allMidias, *infoFase, caranguejo, canos, retornosCanos, plataformas, tartarugas, caranguejos, &caranguejos[j], mario, *fase);
        atualizaFrameRectCaranguejo(&caranguejos[j]);
    }

    // solta as moedas e atualiza o frame das ativas
    relogio++;
    if (relogio > 800 && moedasAtivas < infoFase->numMaxMoedas){
        moedasAtivas++;
        relogio = 0;
    }
    for (int y = 0; y < moedasAtivas; y++){
        atualizaPosiMoeda(allMidias, *infoFase, &moedas[y], canos, retornosCanos, plataformas, tartarugas, caranguejos, mario);
        atualizaFrameMoeda(&moedas[y]);
    }

    infoFase->numCaranguejosAtivos = caranguejosAtivos;
    infoFase->numTartarugasAtivas = tartarugasAtivas;
    infoFase->numMoedasAtivas = moedasAtivas;

    // atualizando as váriaveis que devem ser mantidas nos próximos loopings
    *pontuacaoMario = mario->pontuacao;
    *vidasMario = mario->lifes;
    *usosPower = botaoPower->usos;
    *framesCounter = framesCount;
    *relogioMoedas = relogio;

}

int matouTodosInimigos(INIMIGO *tartarugas, INIMIGO *caranguejos, FASE infoFase){

    int numCaranguejosMortos = 0, numTartarugasMortas = 0;

    // confere quantos dos inimigos ativos estão mortos
    for (int i = 0; i < infoFase.numTartarugasAtivas; i++){
        if (tartarugas[i].morto) numTartarugasMortas++;
    }
    for (int j = 0; j < infoFase.numCaranguejosAtivos; j++){
        if (caranguejos[j].morto) numCaranguejosMortos++;
    }
    // testa se todos inimigos da fase estão mortos
    if (numTartarugasMortas == infoFase.numMaxTartarugas && numCaranguejosMortos == infoFase.numMaxCaranguejos) return 1;
    else return 0;

}

void loopingGameplay(int fase, int *pontuacaoMario, int vidasMario, int usosPower, MENU *menuJogo, MIDIA *allMidias){

    // inicializando as informações da fase
    FASE infoFase = {0};
    // inicializando o diretorio da fase
    char dirArquivoFase[80];
    // inicializando o contador de frames e o temporizador das moedas
    float framesCounter, framesCounterNroFase, relogioMoedas;


    if (IsSoundPlaying(allMidias->startGame)) StopSound(allMidias->startGame);

    // looping da gameplay
    do{
        // reinicializando os temporizadores a cada fase
        framesCounter = 0;
        framesCounterNroFase = 0;
        relogioMoedas = 0;

        // atualiza o diretorio do arquivo da fase para a fase atual
        buscarDirArquivoFase(fase, dirArquivoFase);

        // primeiro lê o arquivo da fase
        // se der erro pra abrir o arquivo ou não tem mais fases, desenha o endgame e depois volta pro menu
        if (!lerArquivo(dirArquivoFase, &infoFase)){
            do{
                desenhaTelaGameEnd(allMidias, framesCounter);
                framesCounter++;
                if (framesCounter == 1) PlaySound(allMidias->gameEnd);
                if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
                if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
            }while(framesCounter < 350);
            if (framesCounter >= 350) menuJogo->optionSelected = 0;
            framesCounter = 0;
            break;
        }
        else PlaySound(allMidias->inicio); // senão começa a fase com o som

        // cria as estruturas de acordo com o arquivo lido
        INIMIGO tartarugas[infoFase.numMaxTartarugas], caranguejos[infoFase.numMaxCaranguejos];
        OBSTACULO plataformas[infoFase.numPlataformas+1], canos[infoFase.numCanos], retornosCanos[infoFase.numRetornosCanos];
        MOEDA moedas[infoFase.numMaxMoedas];
        POWER botaoPower;
        MARIO mario = {0};
        infoFase.numTartarugasAtivas = 0;
        infoFase.numCaranguejosAtivos = 0;
        infoFase.numMoedasAtivas = 0;

        // atualiza as posições das estruturas criadas
        atualizarEstruturasJogo(dirArquivoFase, &infoFase, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, pontuacaoMario, &vidasMario, &usosPower, &allMidias->caixaMarioDos);

        // enquanto o mario tiver mais que 0 vidas,
        while(vidasMario > 0){

            // atualiza e desenha a gameplay
            atualizaTelaGameplay(&infoFase, allMidias, &fase, pontuacaoMario, &vidasMario, &usosPower, &framesCounter, &relogioMoedas, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, &allMidias->caixaMarioDos);
            desenhaTelaGameplay(infoFase, allMidias, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower);

            // desenhando o nro da fase no inicio de cada fase
            if (framesCounterNroFase < 180){
                desenhaNroFase(&allMidias->fonteMario, fase);
                framesCounterNroFase++;
            }

            // se matou todos inimigos,
            if (matouTodosInimigos(tartarugas, caranguejos, infoFase)){
                // se está na opção de Novo Jogo, pula de fase
                if (menuJogo->optionSelect == NovoJogo) fase++;
                // se está na opção de Carregar Mapa, volta pro menu e desenha o endgame
                else if (menuJogo->optionSelect == CarregarMapa){
                    do{
                        desenhaTelaGameEnd(allMidias, framesCounter);
                        framesCounter++;
                        if (framesCounter == 1) PlaySound(allMidias->gameEnd); // som do endgame
                        if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
                        if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
                    }while(framesCounter < 350);
                    if (framesCounter >= 350) menuJogo->optionSelected = 0;
                    framesCounter = 0;
                }
                break;
            }

            // se apertou Backspace pra voltar, abre o menu do pause
            if (IsKeyPressed(KEY_DELETE)){
                do{
                    desenhaTelaPause(infoFase, allMidias, &mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower);
                    // se clicou pra sair do jogo, seta tudo pra sair
                    if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
                    // se clicou "Backspace", seta pra voltar ao menu
                    if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
                    // se clicou "S" para salvar o jogo
                    if (IsKeyPressed(KEY_S)){
                        salvarArquivoContinuarPT1(&infoFase, &fase, pontuacaoMario, &vidasMario, &usosPower, &framesCounter, &relogioMoedas, nome);
                        salvarArquivoContinuarPT2(&mario, tartarugas, caranguejos, plataformas, canos, retornosCanos, moedas, &botaoPower, infoFase);
                    }
                }while(!IsKeyPressed(KEY_ENTER)); // se clicou enter pra voltar ao jogo, volta
            }

            // se clicou pra sair do jogo, seta a opção pra sair
            if (WindowShouldClose()) menuJogo->optionSelect = Sair;

            // se em algum momento clicou pra sair do jogo, sai
            if(menuJogo->optionSelect == Sair || !menuJogo->optionSelected) break;
        }

        // enquanto o mario tiver 0 vidas, desenha a tela de game over e depois volta pro menu
        while(vidasMario == 0){
            desenhaTelaGameOver(allMidias, framesCounter, plataformas, canos, retornosCanos, &botaoPower, infoFase.numPlataformas, infoFase.numCanos, infoFase.numRetornosCanos);
            framesCounter++;
            if (framesCounter == 380) PlaySound(allMidias->gameOver);  // som da tela de game over
            if (framesCounter > 600){ menuJogo->optionSelected = 0; framesCounter = 0; break; }
            // se clicou pra sair do jogo, sai
            if (WindowShouldClose()){ menuJogo->optionSelect = Sair; break; }
            // se clicou "Backspace", seta pra voltar ao menu
            if (IsKeyPressed(KEY_DELETE)){ menuJogo->optionSelected = 0; break; }
        }

    }while(vidasMario > 0 && menuJogo->optionSelected && menuJogo->optionSelect != Sair);

    if (IsSoundPlaying(allMidias->inicio)) StopSound(allMidias->inicio);           // se saiu da gameplay e a musica de inicio de fase está tocando, para
    if (IsSoundPlaying(allMidias->gameEnd)) StopSound(allMidias->gameEnd);         // ou a musica de gameEnd
    if (IsSoundPlaying(allMidias->gameOver)) StopSound(allMidias->gameOver);       // ou a musica de gameOver
    if (IsSoundPlaying(allMidias->marioMorreu)) StopSound(allMidias->marioMorreu); // ou a musica do mario morrer

}

void construirTelaGameplay(MENU *menuJogo, MIDIA *allMidias){

    // inicializando as variaveis para primeira fase que vai rodar
    int fase = 1, pontuacaoMario = 0, vidasMario = 3, usosPower = 3;

    // carregando o looping da gameplay
    loopingGameplay(fase, &pontuacaoMario, vidasMario, usosPower, menuJogo, allMidias);

    // salva as informações do jogador caso esteja no ranking de melhores pontuações
    escreverArquivoBinRanking(nome, pontuacaoMario);

    for (int i = contadorLetras; i >= 0; i--) nome[i] = '\0';  // reinicializando o array com o nome escrito pra voltar pro menu
    contadorLetras = 0;                                        // zerando o contador de letras
    nomeInserido = 0;                                          // reinicializando a flag pra passar pra tela de gameplay depois de inserir o nome
}


void construirTelaNovoJogo(MENU *menuJogo, MIDIA *allMidias){

    // se o nome não foi inserido, atualiza e desenha a tela de inserir o nome
    if (!nomeInserido) construirTelaNome(nome, menuJogo, allMidias);
    // senão, atualiza e desenha a tela do jogo;
    else construirTelaGameplay(menuJogo, allMidias);

}





