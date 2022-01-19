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
    menuJogo.optionSelect = NovoJogo;  // o seletor começa na opção de Novo Jogo

    SetTargetFPS(60);

    // enquanto o usuário não clicar ESC na tela do menu, ou a opção de fechar janela, ou a opção "Sair" do menu, constrói o jogo
    while (!WindowShouldClose() && !(menuJogo.optionSelected && menuJogo.optionSelect == Sair)) construirJogo(&menuJogo, &allMidias);

    fecharMidias(&allMidias);          // fechando as mídias com o encerramento do jogo

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

