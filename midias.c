#include "raylib.h"
#include "midias.h"

void inicializarMidias(MIDIA *allMidias){

    // load das fontes do MarioDos
    Font fonteMario1 = LoadFontEx("C:/Users/MAURO/Desktop/MarioDos/fontes_texto/Mario1.ttf", 32, 0, 250);
    // load das fontes gold edition
    Font fonteGold = LoadFontEx("C:/Users/MAURO/Desktop/MarioDos/fontes_texto/Nesatho.ttf", 32, 0, 250);
    // load da caixa MarioDos
    Texture2D caixaMarioDos = LoadTexture("C:/Users/MAURO/Desktop/MarioDos/imagens/sprite_geral.png");
    // load do som do pulo do mario
    Sound puloMario = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/pulo.wav");
    // load do som de coletar moeda
    Sound hitMoeda = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/hit_coin.wav");
    // load do som de virar inimigo
    Sound hitInimigo = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/hit_enemy.wav");
    // load do som de chutar o inimigo
    Sound chuteInimigo = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/chute.wav");
    // load do som de mario tomar dano
    Sound danoMario = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/dano.wav");
    // load do som de mario perder as 3 vidas
    Sound marioMorreu = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/mariodie.wav");
    // load do som de game over
    Sound gameOver = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/gameover.wav");
    // load do som de inicio
    Sound inicio = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/inicio.wav");
    // load do som de bater no botao power
    Sound hitPower = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/power.mp3");
    // load do som do seletor menu
    Sound setaMenu = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/seta_menu.wav");
    // load do som da cabeceada
    Sound cabeceada = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/cabeceada.wav");
    // load do som do Game End
    Sound gameEnd = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/game_end.wav");
    // load do som do Start do Game
    Sound startGame = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/start_game.wav");
    // load do som do enter
    Sound enter = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/enter.wav");
    // load do som de tecla
    Sound keyPress = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/key_press.wav");
    // load do som da tecla backspace
    Sound backspace = LoadSound("C:/Users/MAURO/Desktop/MarioDos/som/backspace.wav");
    // load da imagem de fogos
    Image fogosImagem = LoadImage("C:/Users/MAURO/Desktop/MarioDos/imagens/fogos.png");
    Texture2D fogos = LoadTextureFromImage(fogosImagem);

    allMidias->fonteMario = fonteMario1;
    allMidias->caixaMarioDos = caixaMarioDos;
    allMidias->fonteGold = fonteGold;
    allMidias->puloMario = puloMario;
    allMidias->hitMoeda = hitMoeda;
    allMidias->hitInimigo = hitInimigo;
    allMidias->chuteInimigo = chuteInimigo;
    allMidias->danoMario = danoMario;
    allMidias->marioMorreu = marioMorreu;
    allMidias->gameOver = gameOver;
    allMidias->inicio = inicio;
    allMidias->hitPower = hitPower;
    allMidias->setaMenu = setaMenu;
    allMidias->cabeceada = cabeceada;
    allMidias->gameEnd = gameEnd;
    allMidias->startGame = startGame;
    allMidias->enter = enter;
    allMidias->keyPress = keyPress;
    allMidias->backspace = backspace;
    allMidias->fogos = fogos;
    allMidias->fogosImagem = fogosImagem;

}

void fecharMidias(MIDIA *allMidias){

    UnloadTexture(allMidias->caixaMarioDos);
    UnloadTexture(allMidias->fogos);

    UnloadImage(allMidias->fogosImagem);

    UnloadFont(allMidias->fonteMario);
    UnloadFont(allMidias->fonteGold);

    UnloadSound(allMidias->puloMario);
    UnloadSound(allMidias->hitMoeda);
    UnloadSound(allMidias->hitInimigo);
    UnloadSound(allMidias->chuteInimigo);
    UnloadSound(allMidias->danoMario);
    UnloadSound(allMidias->marioMorreu);
    UnloadSound(allMidias->gameOver);
    UnloadSound(allMidias->inicio);
    UnloadSound(allMidias->hitPower);
    UnloadSound(allMidias->setaMenu);
    UnloadSound(allMidias->cabeceada);
    UnloadSound(allMidias->gameEnd);
    UnloadSound(allMidias->startGame);
    UnloadSound(allMidias->enter);
    UnloadSound(allMidias->keyPress);
    UnloadSound(allMidias->backspace);
}
