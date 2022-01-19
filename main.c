#include "raylib.h"
#include "telaInicial.h"
#include "midias.h"

int main(void)
{
    InitWindow(screenWidth, screenHeight, "MarioDos");
    InitAudioDevice();

    MIDIA allMidias = {0};             // inicializando as midias
    inicializarMidias(&allMidias);
    MENU menuJogo = {0};               // inicializando o menu do jogo
    menuJogo.optionSelect = NovoJogo;  // o seletor come�a na op��o de Novo Jogo

    SetTargetFPS(60);

    // enquanto o usu�rio n�o clicar ESC na tela do menu, ou a op��o de fechar janela, ou a op��o "Sair" do menu, constr�i o jogo
    while (!WindowShouldClose() && !(menuJogo.optionSelected && menuJogo.optionSelect == Sair)) construirJogo(&menuJogo, &allMidias);

    fecharMidias(&allMidias);          // fechando as m�dias com o encerramento do jogo

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

