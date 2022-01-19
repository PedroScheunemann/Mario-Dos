#include "raylib.h"
#include "telaNovoJogo.h"
#include "movimentacao.h"
#include <stdio.h>


// inicializando retangulos de colisao
Rectangle boxCollisionPlat = {0};
Rectangle boxCollisionCano = {0};
Rectangle boxCollisionPower = {0};
Rectangle boxCollisionInimigo = {0};
Rectangle boxCollisionMario = {0};

// inicializando temporizadores
int tempoParado = 0, tempoNoCeu = 0, chegouEmCima = 0, tempoCabecada = 0;

// checa se teve colisao do retangulo movel e algum cano
void checaColisaoCano(OBSTACULO *canos, int numCanos, Rectangle *movel, int *teveColisao, int *canoColidido){

    int colisao;

    // inicializando o retangulo de colisao e a variavel que armazena o cano colidido
    boxCollisionCano.x = 0;
    boxCollisionCano.y = 0;
    boxCollisionCano.width = 0;
    boxCollisionCano.height = 0;
    *teveColisao = 0;
    *canoColidido = 0;

    for (int j = 0; j < numCanos; j++){
        colisao = CheckCollisionRecs(*movel, canos[j].rect);
        if (colisao){
            boxCollisionCano = GetCollisionRec(*movel, canos[j].rect);   // armazena a parte do cano que colidiu com o rect movel (em retangulo)
            *teveColisao = 1;                                            // joga em nível alto a flag que indica que teve colisao
            *canoColidido = j;                                           // armazena o indice do cano no array de canos
        }
    }

}

// checa se teve colisao do retangulo movel e alguma plataforma
void checaColisaoPlat(OBSTACULO *plataformas, int numPlataformas, Rectangle *movel, int *teveColisao){

    int colisao;

    // inicializando o retangulo de colisao e a variavel que armazena o estado de colisao
    boxCollisionPlat.x = 0;
    boxCollisionPlat.y = 0;
    boxCollisionPlat.width = 0;
    boxCollisionPlat.height = 0;
    *teveColisao = 0;

    for (int i = 0; i <= numPlataformas; i++){
        colisao = CheckCollisionRecs(*movel, plataformas[i].rect);
        if (colisao){
            boxCollisionPlat = GetCollisionRec(*movel, plataformas[i].rect);   // armazena a parte da plataforma que colidiu com o rect movel (em retangulo)
            *teveColisao = 1;                                                  // joga em nível alto a flag que indica que teve colisao
            break;
        }
    }
}

// checa se teve colisao do retangulo movel e algum inimigo
void checaColisaoInimigos(INIMIGO *tartarugas, INIMIGO *caranguejos, int numTartarugasAtivas, int numCaranguejosAtivos, Rectangle *movel, int *teveColisao, int *inimigoColidido, int *tipoInimigo){

    int colisao;

    // inicializando o retangulo de colisao e a variavel que armazena o estado de colisao
    boxCollisionInimigo.x = 0;
    boxCollisionInimigo.y = 0;
    boxCollisionInimigo.width = 0;
    boxCollisionInimigo.height = 0;
    *teveColisao = 0;
    *inimigoColidido = 0;

    for (int i = 0; i < numTartarugasAtivas; i++){
        if (movel->x != tartarugas[i].rect.x){              // como o próprio inimigo está no array também, testa só pros inimigos diferentes de si mesmo
            colisao = CheckCollisionRecs(*movel, tartarugas[i].rect);
            if (colisao){
                boxCollisionInimigo = GetCollisionRec(*movel, tartarugas[i].rect);  // armazena a parte do inimigo que colidiu com o rect movel (em retangulo)
                *teveColisao = 1;                                                   // joga em nível alto a flag que indica que teve colisao
                *inimigoColidido = i;                                               // armazena o indice do inimigo no array de inimigos
                *tipoInimigo = tartaruga;                                           // armazena o tipo do inimigo colidido
                break;
            }
        }
    }
    for (int i = 0; i < numCaranguejosAtivos; i++){
        if (movel->x != caranguejos[i].rect.x){
            colisao = CheckCollisionRecs(*movel, caranguejos[i].rect);
            if (colisao){
                boxCollisionInimigo = GetCollisionRec(*movel, caranguejos[i].rect);
                *teveColisao = 1;
                *inimigoColidido = i;
                *tipoInimigo = caranguejo;
                break;
            }
        }
    }
}

// checa se teve colisao do retangulo movel com o mario
void checaColisaoMario(MARIO *mario, Rectangle *movel, int *teveColisao){

    int colisao;

    // inicializando o retangulo de colisao e a variavel que armazena o estado de colisao
    boxCollisionMario.x = 0;
    boxCollisionMario.y = 0;
    boxCollisionMario.width = 0;
    boxCollisionMario.height = 0;
    *teveColisao = 0;

    colisao = CheckCollisionRecs(*movel, mario->rect);
    if (colisao){
        boxCollisionMario = GetCollisionRec(*movel, mario->rect);   // armazena a parte do mario que colidiu com o rect movel (em retangulo)
        *teveColisao = 1;                                           // joga em nível alto a flag que indica que teve colisao
    }
}


void lerTeclado(MARIO *mario){   // le o teclado e ve se o jogador pode pular ou ir pro lado (de acordo com os comandos)

    mario->canGoLeft = 0;
    mario->canGoRight = 0;
    mario->canJump = 0;

    if (IsKeyDown(KEY_LEFT)) mario->canGoLeft = 1;
    else if (IsKeyDown(KEY_RIGHT)) mario->canGoRight = 1;
    if (IsKeyDown(KEY_D) && !(mario->subindo || mario->descendo)) mario->canJump = 1;

}


void atualizaPosiMario(MIDIA *allMidias, FASE *infoFase, MARIO *mario, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, MOEDA *moedas, POWER *botaoPower){


    int teveColisao = 0, inimigoColidido, tipoInimigoColidido;

    // se o mario não foi atingido por algum inimigo não vulneravel
    if (!mario->atingido){

        // caso queira pular, inicializa as váriaveis para começar o pulo
        if (mario->canJump){
            mario->subindo = 1;
            mario->speed = 3.75;
            PlaySound(allMidias->puloMario); // play no som do pulo do mario
        }
        // senão pela lógica o mario sempre tem que estar caindo ou pisando em alguma plataforma
        else mario->descendo = 1;


        // atualizando a posição no movimento de subida
        if (mario->subindo){

            mario->descendo = 0;
            mario->rect.y -= 3*mario->speed;                            // atualizando a posição y do mario pra cima
            mario->speed -= 0.14;                                       // decrementa a velocidade (imitação da gravidade)

            // checa se a velocidade é menor ou igual a 0, pois significa que chegou ao ponto mais alto da subida
            if (mario->speed <= 0){
                mario->subindo = 0;                                     // o movimento de descida deve começar
                mario->descendo = 1;
            }

            // checa se teve colisao com alguma plataforma nessa nova posição y
            checaColisaoPlat(plataformas, infoFase->numPlataformas, &mario->rect, &teveColisao);
            if (teveColisao){                                            // se teve colisao nessa nova posicao y pra cima, significa que bateu de cabeça na plataforma
                mario->subindo = 0;
                mario->descendo = 1;
                mario->rect.y += boxCollisionPlat.height;                // reajusta a posicao y do mario para ele nao invadir a plataforma (utilizando a altura do retangulo de colisao)
                mario->speed = 0;                                        // e seta a velocidade dele em 0 (para começar o movimento de descer)
                mario->bateuCabeca = 1;
                PlaySound(allMidias->cabeceada);                         // som da cabeceada

                // checa se em cima da plataforma tinha algum inimigo
                Rectangle cabecadaMario = {boxCollisionPlat.x-5, (boxCollisionPlat.y+boxCollisionPlat.height)-plataformas[1].rect.height-10, boxCollisionPlat.width+10, 10};
                checaColisaoInimigos(tartarugas, caranguejos, infoFase->numTartarugasAtivas, infoFase->numCaranguejosAtivos, &cabecadaMario, &teveColisao, &inimigoColidido, &tipoInimigoColidido);
                if (teveColisao){
                    // se teve colisão com tartaruga e o estado dela não era vulneravel
                    if (tipoInimigoColidido == tartaruga && tartarugas[inimigoColidido].estado != vulneravel){
                        tartarugas[inimigoColidido].estado = vulneravel;       // seta as variaveis pra ela ficar vulneravel
                        tartarugas[inimigoColidido].frameRect.x = 145;
                        tartarugas[inimigoColidido].speed.y = 4;
                        tartarugas[inimigoColidido].descendo = 1;
                    }
                    // se teve colisão com caranguejo e o estado dele não era vulneravel
                    else if (tipoInimigoColidido == caranguejo && caranguejos[inimigoColidido].estado > vulneravel){
                        caranguejos[inimigoColidido].estado--;                // seta as variaveis pra ele decrementar o estado (de normal pra bravo || de bravo pra vulneravel)
                        caranguejos[inimigoColidido].speed.y = 4;
                        caranguejos[inimigoColidido].descendo = 1;
                        if (caranguejos[inimigoColidido].estado == normal){
                                if (caranguejos[inimigoColidido].sentidoDesloc == 1) caranguejos[inimigoColidido].frameRect.x = 105;
                                else caranguejos[inimigoColidido].frameRect.x = 240;
                        }
                        else if (caranguejos[inimigoColidido].estado == bravo) caranguejos[inimigoColidido].frameRect.x = 55;
                    }
                // mario ganha 10 pontos por cabecear o inimigo
                mario->pontuacao += 10;
                }
            }

            // checa se teve colisao com o botao power nessa nova posição y
            if (CheckCollisionRecs(mario->rect, botaoPower->rect)){
                mario->subindo = 0;
                mario->descendo = 1;
                boxCollisionPower = GetCollisionRec(mario->rect, botaoPower->rect);
                mario->rect.y += boxCollisionPower.height;               // reajusta a posicao y do mario para ele nao invadir o botão (utilizando a altura do retangulo de colisao)
                mario->speed = 0;

                if (botaoPower->usos > 0){

                    PlaySound(allMidias->hitPower); // som de bater no botao power

                    // mata os inimigos vivos e aumenta a pontuação do mario
                    for (int i = 0; i < infoFase->numTartarugasAtivas; i++){
                        if (!tartarugas[i].morto)  mario->pontuacao += 800;
                        tartarugas[i].morrendo = 1;
                        tartarugas[i].descendo = 0;
                        tartarugas[i].speed.x = 0;
                    }
                    for (int j = 0; j < infoFase->numCaranguejosAtivos; j++){
                        if (!caranguejos[j].morto)  mario->pontuacao += 800;
                        caranguejos[j].morrendo = 1;
                        caranguejos[j].descendo = 0;
                        caranguejos[j].speed.x = 0;
                    }
                    // coleta as moedas ativas
                    for (int y = 0; y < infoFase->numMoedasAtivas; y++){
                        if (!moedas[y].sumiu){
                            mario->pontuacao += 800;
                            PlaySound(allMidias->hitMoeda);  // som de coletar moeda
                        }
                        moedas[y].sumiu = 1;
                        moedas[y].speed = 0;
                    }
                    // diminui o numero de usos do botao
                    botaoPower->usos--;
                }
            }
        }

        // atualizando a posicao no movimento de descida
        if (mario->descendo){

            if (mario->bateuCabeca) tempoCabecada++;
            if (tempoCabecada > 8){                                     // fazendo a animação de ficar um tempo parado no ar quando da a cabeçada na plataforma
                tempoCabecada = 0;
                mario->bateuCabeca = 0;
            }

            if (!mario->bateuCabeca){

                mario->rect.y -= 3*mario->speed;                            // atualizando a posição y do mario pra baixo
                mario->speed -= 0.14;                                       // decrementa a velocidade (imitação da gravidade)

                // checa se teve colisao com alguma plataforma nessa nova posição y
                checaColisaoPlat(plataformas, infoFase->numPlataformas, &mario->rect, &teveColisao);
                if (teveColisao){                                           // se teve colisao nessa nova posicao y, significa que pisou em alguma plataforma
                    mario->descendo = 0;                                    // reseta as váriaveis do pulo
                    mario->rect.y -= boxCollisionPlat.height;               // reajusta a posicao y utilizando a altura do retangulo de colisao
                    mario->speed = -0.1;                                    // seta a váriavel speed como -0,1, pois se for 0 no próximo ciclo a mario->descendo vira e continua 1, pois a posi Y do mario nao muda nada com speed = 0
                }                                                           // dessa forma o mario continua colidindo com a plat e a váriavel mario->descendo segue 0 (corretamente)

                // checa se colidiu com o botao power nessa nova posição y
                if (CheckCollisionRecs(mario->rect, botaoPower->rect)){
                    mario->descendo = 0;
                    boxCollisionPower = GetCollisionRec(mario->rect, botaoPower->rect);
                    mario->rect.y -= boxCollisionPower.height;
                    mario->speed = -0.1;
                }
            }
        }

        // atualizando a posicao x do mario quando o jogador aperta pra esquerda
        if (mario->canGoLeft){

            mario->rect.x -= 4;    // atualiza a posicao x do mario pra esquerda

            // checa se teve colisão com alguma plataforma nessa nova posicao x
            checaColisaoPlat(plataformas, infoFase->numPlataformas, &mario->rect, &teveColisao);
            if (teveColisao) mario->rect.x += boxCollisionPlat.width; // se teve, reajusta a posicao x

            // checa se colidiu com o botao power
            if (CheckCollisionRecs(mario->rect, botaoPower->rect)){
                boxCollisionPower = GetCollisionRec(mario->rect, botaoPower->rect);
                mario->rect.x += boxCollisionPower.width;
            }

            // checa se o mario saiu da tela pela esquerda, se teve reajusta posicao pro lado oposto
            if (mario->rect.x + (mario->rect.width/2) < 0) mario->rect.x = screenWidth - mario->rect.width/2;
        }

        // atualizando a posicao x do mario quando o jogador aperta pra direita
        if (mario->canGoRight){

            mario->rect.x += 4;   // atualiza a posicao x do mario pra direita

            // checa se teve colisão com alguma plataforma nessa nova posicao x
            checaColisaoPlat(plataformas, infoFase->numPlataformas, &mario->rect, &teveColisao);
            if (teveColisao) mario->rect.x -= boxCollisionPlat.width;  // se teve, reajusta a posicao x

            // checa se colidiu com o botao power
            if (CheckCollisionRecs(mario->rect, botaoPower->rect)){
                boxCollisionPower = GetCollisionRec(mario->rect, botaoPower->rect);
                mario->rect.x -= boxCollisionPower.width;
            }

            // checa se o mario saiu da tela pela direita, se teve reajusta posicao pro lado oposto
            if (mario->rect.x + (mario->rect.width/2) > screenWidth) mario->rect.x = -mario->rect.width/2;
        }
    }
    else{ // se o mario foi atingido por algum inimigo

        if (!mario->chegouNoCeu){            // se, depois de ser atingido, o mario não chegou no ceu
            if (!mario->descendo){           // e não está descendo, então está parado
                tempoParado++;
                if (tempoParado == 1) PlaySound(allMidias->hitInimigo); // som de bater no inimigo
                if (tempoParado > 50){       // depois de algum tempo parado, seta as variaveis pra pular e descer
                    tempoParado = 0;
                    mario->descendo = 1;
                    mario->speed = 3;
                    PlaySound(allMidias->danoMario);  // som do mario tomando dano
                }
            }
            else{
                mario->rect.y -= 2*mario->speed;      // mario pulando e descendo
                mario->speed -= 0.2;
                if (mario->rect.y > screenHeight){    // quando passa da parte de baixo da tela, volta pela parte de cima
                    if (mario->lifes > 1){            // seta as coisas pra ele sair na parte de cima da tela caso tenha mais de 1 vida
                        mario->rect.y = 0;
                        mario->rect.x = screenWidth/2 - mario->rect.width/2;
                        mario->chegouNoCeu = 1;
                        mario->descendo = 0;
                        mario->speed = 0;
                    }
                    mario->lifes--;                           // diminui uma vida do mario, já que foi atingido pelo inimigo
                    if (mario->lifes == 0) PlaySound(allMidias->marioMorreu); // som do mario morto
                }
            }
        }
        if (mario->chegouNoCeu){                             // depois de chegar em cima, desce parado em uma plataforma
            mario->parado = 1;
            if (tempoNoCeu < 20){
                mario->rect.y += 2;
                tempoNoCeu++;
            }
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)){  // fica em cima da plataforma até o jogador apertar pra cair,
                mario->parado = 0;                                  // então cai normal para voltar ao jogo
                mario->atingido = 0;
                mario->chegouNoCeu = 0;
                tempoNoCeu = 0;
            }
        }
    }
}


void atualizaPosiInimigo(MIDIA *allMidias, FASE infoFase, int tipoInimigo, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, INIMIGO *inimigo, MARIO *mario, int fase){


    int teveColisao = 0, canoColidido, inimigoColidido, tipoInimigoColidido;

    // seta uma velocidade maior que a normal pra caso o caranguejo esteja no modo bravo
    if (tipoInimigo == caranguejo){
        if (inimigo->estado == bravo) inimigo->speed.x = 2.5;
    }

    // se o inimigo nao ta morrendo
    if (!inimigo->morrendo){

        // se o inimigo não estiver no modo vulneravel
        if (inimigo->estado > vulneravel){

            inimigo->descendo = 1;

            // indo pra baixo
            inimigo->rect.y -= 1.1*inimigo->speed.y;                    // atualizando a posição y do inimigo pra baixo
            inimigo->speed.y -= 0.2;                                    // decrementa a velocidade (imitação da gravidade)

            // checa se teve colisao com alguma plataforma indo pra baixo
            checaColisaoPlat(plataformas, infoFase.numPlataformas, &inimigo->rect, &teveColisao);
            if (teveColisao){                                           // se teve colisao nessa nova posicao y, significa que pisou em alguma plataforma
                inimigo->rect.y -= boxCollisionPlat.height;             // reajusta a posicao y do inimigo
                inimigo->speed.y = 0;
                inimigo->descendo = 0;
            }

            // checa se teve colisao com o mario indo pra baixo
            checaColisaoMario(mario, &inimigo->rect, &teveColisao);
            if (teveColisao && !mario->atingido){
                inimigo->sentidoDesloc *= -1;                           // troca o sentido de deslocamento do inimigo
                mario->atingido = 1;
                mario->descendo = 0;
            }


            // indo pra algum lado, de acordo com o sentido de desloc
            inimigo->rect.x -= 1.4 * inimigo->speed.x * inimigo->sentidoDesloc;

            // checa se teve colisão com alguma plataforma nessa nova posicao x pra esquerda ou direita
            checaColisaoPlat(plataformas, infoFase.numPlataformas, &inimigo->rect, &teveColisao);
            if (teveColisao){
                if (inimigo->sentidoDesloc == esquerda) inimigo->rect.x += boxCollisionPlat.width;  // reajusta a posicao x, se foi colisao indo pra esquerda
                else inimigo->rect.x -= boxCollisionPlat.width;                                     // ou se foi colisao indo pra direita
            }

            // checa se teve colisão com algum retorno, se teve reajusta a posição
            checaColisaoCano(retornosCanos, infoFase.numRetornosCanos, &inimigo->rect, &teveColisao, &canoColidido);
            if (teveColisao){
                inimigo->rect.y = canos[canoColidido].rect.y + canos[canoColidido].rect.height/2 - inimigo->rect.height/2;

                if(canos[canoColidido].rect.x > screenWidth/2){       // se vai sair no cano da direita, o sentido de desloc deve ser pra esquerda
                    if (inimigo->sentidoDesloc == direita) inimigo->sentidoDesloc = esquerda;
                    inimigo->rect.x = canos[canoColidido].rect.x - inimigo->rect.width;
                }
                else{                                                 // se vai sair no cano da esquerda, o sentido de desloc deve ser pra direita
                    if (inimigo->sentidoDesloc == esquerda) inimigo->sentidoDesloc = direita;
                    inimigo->rect.x = canos[canoColidido].rect.x + canos[canoColidido].rect.width;
                }
            }

            // checa se o inimigo saiu da tela pela direita, se teve reajusta posicao pro lado oposto
            if (inimigo->rect.x + (inimigo->rect.width/2) < 0) inimigo->rect.x = screenWidth - inimigo->rect.width/2;
            else if (inimigo->rect.x + (inimigo->rect.width/2)  > screenWidth) inimigo->rect.x = -inimigo->rect.width/2;

            // checa se teve colisao com o mario indo pra algum lado
            checaColisaoMario(mario, &inimigo->rect, &teveColisao);
            if (teveColisao && !mario->atingido){
                if (!inimigo->descendo){
                    if (inimigo->sentidoDesloc == esquerda) inimigo->rect.x += boxCollisionMario.width;  // fazendo o efeito de "bater" no mario e inverter sentido de desloc
                    else if (inimigo->sentidoDesloc == direita) inimigo->rect.x -= boxCollisionMario.width;
                }
                inimigo->sentidoDesloc *= -1;
                mario->atingido = 1;
                mario->descendo = 0;
            }

            // checa se teve colisao com algum inimigo indo pro lado
            checaColisaoInimigos(tartarugas, caranguejos, infoFase.numTartarugasAtivas, infoFase.numCaranguejosAtivos, &inimigo->rect, &teveColisao, &inimigoColidido, &tipoInimigoColidido);
            if (teveColisao && !inimigo->colidindo){
                if (tipoInimigoColidido == tartaruga && !tartarugas[inimigoColidido].colidindo){
                    // se tão em direções contrárias ou os dois estão no chão e colidiram, inverte o sentido de desloc dos dois
                    if (inimigo->sentidoDesloc != tartarugas[inimigoColidido].sentidoDesloc || (!inimigo->descendo && !tartarugas[inimigoColidido].descendo)){
                        inimigo->sentidoDesloc *= -1;
                        tartarugas[inimigoColidido].sentidoDesloc *= -1;
                    }
                    else{
                    // se tão em direçoes iguais
                        if (inimigo->sentidoDesloc == direita){   // se os dois tao indo pra direita, quem ta mais na esquerda vai pra esquerda
                            if (inimigo->rect.x < tartarugas[inimigoColidido].rect.x) inimigo->sentidoDesloc = esquerda;
                            else tartarugas[inimigoColidido].sentidoDesloc = esquerda;
                        }
                        else{                                     // se os dois tao indo pra esquerda, quem ta mais na direita vai pra direita
                            if (inimigo->rect.x > tartarugas[inimigoColidido].rect.x) inimigo->sentidoDesloc = direita;
                            else tartarugas[inimigoColidido].sentidoDesloc = direita;
                        }
                    }
                    tartarugas[inimigoColidido].colidindo = 1;
                }
                if (tipoInimigoColidido == caranguejo && !caranguejos[inimigoColidido].colidindo){
                    // se tão em direções contrárias ou os dois estão no chão e colidiram, inverte o sentido de desloc dos dois
                    if (inimigo->sentidoDesloc != caranguejos[inimigoColidido].sentidoDesloc || (!inimigo->descendo && !caranguejos[inimigoColidido].descendo)){
                        inimigo->sentidoDesloc *= -1;
                        caranguejos[inimigoColidido].sentidoDesloc *= -1;
                    }
                    else{
                    // se tão em direçoes iguais
                        if (inimigo->sentidoDesloc == direita){   // se os dois tao indo pra direita, quem ta mais na esquerda vai pra esquerda
                            if (inimigo->rect.x < caranguejos[inimigoColidido].rect.x) inimigo->sentidoDesloc = esquerda;
                            else caranguejos[inimigoColidido].sentidoDesloc = esquerda;
                        }
                        else{                                     // se os dois tao indo pra esquerda, quem ta mais na direita vai pra direita
                            if (inimigo->rect.x > caranguejos[inimigoColidido].rect.x) inimigo->sentidoDesloc = direita;
                            else caranguejos[inimigoColidido].sentidoDesloc = direita;
                        }
                    }
                    caranguejos[inimigoColidido].colidindo = 1;
                }
                inimigo->colidindo = 1;
            }
            else if (!teveColisao) inimigo->colidindo = 0;
        }

        // se o inimigo está vulneravel
        if (inimigo->estado == vulneravel){

            if (inimigo->descendo){  // se o inimigo está caindo (aqui é a animação de quando o inimigo fica em modo vulneravel)

                // alterando o y para fazer a animação do pulo
                inimigo->rect.y -= 3*inimigo->speed.y;
                inimigo->speed.y -= 0.2;

                // testando se colidiu com alguma plataforma nesse novo y
                checaColisaoPlat(plataformas, infoFase.numPlataformas, &inimigo->rect, &teveColisao);
                if (teveColisao){
                    // checa se teve colisao com alguma plataforma indo pra baixo
                    if(inimigo->speed.y < 0){
                        inimigo->rect.y -= boxCollisionPlat.height;
                        inimigo->descendo = 0;
                    }
                    // checa se teve colisao com alguma plataforma subindo
                    else{
                        inimigo->rect.y += boxCollisionPlat.height;
                        inimigo->speed.y = 0;  // seta a velocidade igual a 0, para começar o movimento de descida
                    }
                }

                // indo pro lado
                inimigo->rect.x -= 1.4 * inimigo->speed.x * inimigo->sentidoDesloc;

                // testando se colidiu com alguma plataforma nesse novo x
                checaColisaoPlat(plataformas, infoFase.numPlataformas, &inimigo->rect, &teveColisao);
                if (teveColisao){
                    if(inimigo->sentidoDesloc == direita) inimigo->rect.x -= boxCollisionPlat.width;  // ajusta o x de acordo com o sentido de desloc
                    else inimigo->rect.x += boxCollisionPlat.width;
                    inimigo->speed.x = 0;                                                             // seta a veloc em x como 0 para ele não se deslocar mais lateralmente
                }
            }
            else{ // se o inimigo já está no chão
                  // checa se teve colisao com o mario, estando vulneravel
                checaColisaoMario(mario, &inimigo->rect, &teveColisao);
                if (teveColisao){
                    inimigo->morrendo = 1;
                    inimigo->descendo = 0;
                    inimigo->speed.x = 5;
                    mario->pontuacao += 800;
                    PlaySound(allMidias->chuteInimigo);  // som de chutar o inimigo
                }
            }
        }
    }
    // se o inimigo morreu, faz toda a animação
    else{

        // primeiro seta as variaveis para ele começar a cair da tela
        if (!inimigo->descendo){
            if (!mario->subindo && !mario->descendo){
                // se o mario nao tiver subindo nem descendo, o inimigo é chutado pra direção que o mario ta andando
                if (mario->canGoLeft) inimigo->sentidoDesloc = esquerda;
                else inimigo->sentidoDesloc = direita;
            }
            else{
                // se o mario bateu de cabeça no inimigo ou caiu nele, o inimigo é chutado pra direção onde a menor parte do corpo do mario está
                if (inimigo->rect.x + inimigo->rect.width/2 > mario->rect.x) inimigo->sentidoDesloc = direita;
                else inimigo ->sentidoDesloc = esquerda;
            }
            inimigo->descendo = 1;
            inimigo->speed.y = 0;
            if (tipoInimigo == tartaruga) inimigo->frameRect.x = 283;
            else if (tipoInimigo == caranguejo) inimigo->frameRect.x = 206;
        }
        // altera a posição do inimigo enquanto ele não tiver saído dos limites da tela
        if(inimigo->rect.y <= screenHeight){

            inimigo->colidindo = 1;  // pra não bater em outro inimigo enquanto estiver caindo

            inimigo->rect.x -= 2.4 * inimigo->speed.x * inimigo->sentidoDesloc; // animação de ir pro lado
            if (inimigo->speed.x > 0.1) inimigo->speed.x -= 0.1;

            inimigo->rect.y -= 3*inimigo->speed.y;  // animação de cair
            inimigo->speed.y -= 0.2;

        }
        // quando sai dos limites dá tela, dá como inimigo morto
        else inimigo->morto = 1;
    }

}

void atualizaPosiMoeda(MIDIA *allMidias, FASE infoFase, MOEDA *moeda, OBSTACULO *canos, OBSTACULO *retornosCanos, OBSTACULO *plataformas, INIMIGO *tartarugas, INIMIGO *caranguejos, MARIO *mario){

    int teveColisao = 0, canoColidido, inimigoColidido, tipoInimigoColidido;

    if (!moeda->sumiu){

        moeda->descendo = 1;

        // indo pra baixo
        moeda->rect.y -= 1.1 * moeda->speed;                      // atualizando a posição y da moeda pra baixo
        moeda->speed -= 0.2;                                      // decrementa a velocidade (imitação da gravidade)

        // checa se teve colisao com alguma plataforma indo pra baixo
        checaColisaoPlat(plataformas,infoFase.numPlataformas, &moeda->rect, &teveColisao);
        if (teveColisao){                                         // se teve colisao nessa nova posicao y, significa que pisou em alguma plataforma
            moeda->rect.y -= boxCollisionPlat.height;             // reajusta a posicao y da moeda
            moeda->speed = 0;
            moeda->descendo = 0;
        }


        // indo pra algum lado, de acordo com o sentido de desloc
        moeda->rect.x -= 2 * moeda->sentidoDesloc;

        // checa se teve colisão com alguma plataforma nessa nova posicao x pra esquerda ou direita
        checaColisaoPlat(plataformas, infoFase.numPlataformas, &moeda->rect, &teveColisao);
        if (teveColisao){
            if (moeda->sentidoDesloc == esquerda) moeda->rect.x += boxCollisionPlat.width;  // reajusta a posicao x, se foi colisao indo pra esquerda
            else moeda->rect.x -= boxCollisionPlat.width;                                     // ou se foi colisao indo pra direita
        }

        // checa se teve colisão com algum retorno, se teve reajusta a posição
        checaColisaoCano(retornosCanos, infoFase.numRetornosCanos, &moeda->rect, &teveColisao, &canoColidido);
        if (teveColisao){
            moeda->rect.y = canos[canoColidido].rect.y + canos[canoColidido].rect.height/2 - moeda->rect.height/2;

            if(canos[canoColidido].rect.x > screenWidth/2){   // se vai sair no cano da direita, o sentido de desloc deve ser pra esquerda
                if (moeda->sentidoDesloc == direita) moeda->sentidoDesloc = esquerda;
                moeda->rect.x = canos[canoColidido].rect.x - moeda->rect.width;
            }
            else{                                                 // se vai sair no cano da esquerda, o sentido de desloc deve ser pra direita
                if (moeda->sentidoDesloc == esquerda) moeda->sentidoDesloc = direita;
                moeda->rect.x = canos[canoColidido].rect.x + canos[canoColidido].rect.width;
            }
        }

        // checa se a moeda saiu da tela pela direita, se teve reajusta posicao pro lado oposto
        if (moeda->rect.x + (moeda->rect.width/2) < 0) moeda->rect.x = screenWidth - moeda->rect.width/2;
        else if (moeda->rect.x + (moeda->rect.width/2)  > screenWidth) moeda->rect.x = -moeda->rect.width/2;


        // checa se teve colisao com algum inimigo indo pro lado
        checaColisaoInimigos(tartarugas, caranguejos, infoFase.numTartarugasAtivas, infoFase.numCaranguejosAtivos, &moeda->rect, &teveColisao, &inimigoColidido, &tipoInimigoColidido);
        if (teveColisao && !moeda->colidindo){
            if (tipoInimigoColidido == tartaruga && !tartarugas[inimigoColidido].colidindo){
                // se tão em direções contrárias ou os dois estão no chão e colidiram, inverte o sentido de desloc dos dois
                if (moeda->sentidoDesloc != tartarugas[inimigoColidido].sentidoDesloc || (!moeda->descendo && !tartarugas[inimigoColidido].descendo)){
                    moeda->sentidoDesloc *= -1;
                    tartarugas[inimigoColidido].sentidoDesloc *= -1;
                }
                else{
                // se tão em direçoes iguais
                    if (moeda->sentidoDesloc == direita){   // se os dois tao indo pra direita, quem ta mais na esquerda vai pra esquerda
                        if (moeda->rect.x < tartarugas[inimigoColidido].rect.x) moeda->sentidoDesloc = esquerda;
                        else tartarugas[inimigoColidido].sentidoDesloc = esquerda;
                    }
                    else{                                     // se os dois tao indo pra esquerda, quem ta mais na direita vai pra direita
                        if (moeda->rect.x > tartarugas[inimigoColidido].rect.x) moeda->sentidoDesloc = direita;
                        else tartarugas[inimigoColidido].sentidoDesloc = direita;
                    }
                }
                tartarugas[inimigoColidido].colidindo = 1;
            }
            if (tipoInimigoColidido == caranguejo && !caranguejos[inimigoColidido].colidindo){
                // se tão em direções contrárias ou os dois estão no chão e colidiram, inverte o sentido de desloc dos dois
                if (moeda->sentidoDesloc != caranguejos[inimigoColidido].sentidoDesloc || (!moeda->descendo && !caranguejos[inimigoColidido].descendo)){
                    moeda->sentidoDesloc *= -1;
                    caranguejos[inimigoColidido].sentidoDesloc *= -1;
                }
                else{
                // se tão em direçoes iguais
                    if (moeda->sentidoDesloc == direita){   // se os dois tao indo pra direita, quem ta mais na esquerda vai pra esquerda
                        if (moeda->rect.x < caranguejos[inimigoColidido].rect.x) moeda->sentidoDesloc = esquerda;
                        else caranguejos[inimigoColidido].sentidoDesloc = esquerda;
                    }
                    else{                                     // se os dois tao indo pra esquerda, quem ta mais na direita vai pra direita
                        if (moeda->rect.x > caranguejos[inimigoColidido].rect.x) moeda->sentidoDesloc = direita;
                        else caranguejos[inimigoColidido].sentidoDesloc = direita;
                    }
                }
                caranguejos[inimigoColidido].colidindo = 1;
            }
            moeda->colidindo = 1;
        }
        else if (!teveColisao) moeda->colidindo = 0;


        // checa se teve colisao com o mario nessa nova posição
        checaColisaoMario(mario, &moeda->rect, &teveColisao);
        if (teveColisao){
            moeda->sumiu = 1;
            mario->pontuacao += 800;
            PlaySound(allMidias->hitMoeda);  // som de coletar a moeda
        }
    }
    else{
        moeda->rect.y = -500;  // tira a moeda da tela
        moeda->rect.x = -500;
    }

}

// atualiza o frame da tartaruga de acordo com as medidas da sprite geral
void atualizaFrameRectTartaruga(INIMIGO *inimigo){

    // contando os frames da tartaruga até ela estar no frame de morta, aí não precisa mais atualizar
    if (inimigo->frameRect.x != 305) inimigo->framesCounter++;

    if (inimigo->sentidoDesloc == direita) inimigo->frameRect.y = 332;
    else inimigo->frameRect.y = 353;

    if (!inimigo->morrendo){
        if(inimigo->estado > vulneravel){               // estado normal
            if (inimigo->framesCounter > 5){
                inimigo->frameRect.x += 24;
                if (inimigo->frameRect.x == 126) inimigo->frameRect.x = 122;
                else if (inimigo->frameRect.x == 146) inimigo->frameRect.x = 6;
                inimigo->framesCounter = 0;
            }
        }
        else if(inimigo->estado == vulneravel){         // estado vulneravel
            if (inimigo->framesCounter > 3){
                inimigo->frameRect.x += 23;
                if (inimigo->frameRect.x == 283) inimigo->frameRect.x = 260;
                inimigo->framesCounter = 0;
            }
        }
    }
    else{                                               // morta
        if (inimigo->framesCounter > 35){
            inimigo->frameRect.x += 22;
            if (inimigo->frameRect.x == 327) inimigo->frameRect.x = 305;
        }
    }
}

// atualiza o frame do caranguejo de acordo com as medidas da sprite geral
void atualizaFrameRectCaranguejo(INIMIGO *inimigo){

    // contando os frames do caranguejo até ele estar no frame de morto, aí não precisa mais atualizar
    if (inimigo->frameRect.x != 180) inimigo->framesCounter++;
    inimigo->framesCounter++;

    if (!inimigo->morrendo){
        if (inimigo->framesCounter > 5){
            if(inimigo->estado == normal){          // modo normal
                inimigo->frameRect.x += 25;
                if (inimigo->frameRect.x == 55) inimigo->frameRect.x = 5;
            }
            else if(inimigo->estado == bravo){       // modo bravo
                inimigo->frameRect.x += 25;
                if (inimigo->frameRect.x == 105) inimigo->frameRect.x = 55;
                inimigo->framesCounter = 0;
            }
            else if (inimigo->estado == vulneravel){ // modo vulneravel
                inimigo->frameRect.x += 25;
                if (inimigo->frameRect.x == 180) inimigo->frameRect.x = 155;
                if (inimigo->frameRect.x == 315) inimigo->frameRect.x = 290;
            }
            inimigo->framesCounter = 0;
        }
    }
    else{                                           // morto
        if (inimigo->framesCounter > 35){
            inimigo->frameRect.x -= 26;
            if (inimigo->frameRect.x == 154) inimigo->frameRect.x = 180;
        }
    }
}

// atualiza o frame do mario de acordo com as medidas da sprite geral
void atualizaFrameRectMario(MARIO *mario){

    mario->framesCounter++;

    if (mario->canGoLeft) mario->frameRect.y = 602.8;              // ajustando a altura dos frames de acordo com a direção do mario
    else if (mario->canGoRight) mario->frameRect.y = 633;


    if (mario->atingido){                                          // mario atingido por um inimigo sem estar no modo vulneravel
        if (mario->descendo) mario->frameRect.x = 128.9;           // mario caindo
        else if (mario->parado){                                   // mario parado
            mario->frameRect.x = 3;
        }
        else mario->frameRect.x = 112;                             // mario pulando depois de ser atingido
    }
    else{
        if (mario->subindo) mario->frameRect.x = 92;                   // mario pulando normal
        else if (mario->descendo) mario->frameRect.x = 72;
        else if (mario->canGoLeft || mario->canGoRight){               // indo pra esquerda ou direita
            if (mario->framesCounter > 5){                             // altera os frames com o passar do tempo (fazendo a animação de andar)
                mario->frameRect.x += 16;
                if (mario->frameRect.x == 51) mario->frameRect.x += 1;
                if (mario->frameRect.x > 67) mario->frameRect.x = 3;
                mario->framesCounter = 0;
            }
        }
        else mario->frameRect.x = 3;                                   // mario parado
    }
}

// atualiza o frame das moedas de acordo com as medidas da sprite geral
void atualizaFrameMoeda(MOEDA *moeda){

    moeda->framesCounter++;

    if (moeda->framesCounter > 5){
        moeda->frameRect.x += 12;
        if (moeda->frameRect.x == 184) moeda->frameRect.x = 148;
        moeda->framesCounter = 0;
    }

}









