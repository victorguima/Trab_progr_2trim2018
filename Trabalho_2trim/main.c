/**
@Author: Victor Guimarães
@Author: Gabriel Machado
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <time.h>

void menu(int *escolha);

int main()
{
    system("color 0A");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando língua como Português
    int option = 0;
    int flag = 0;
    do
    {
        menu(&option);
        switch(option)
        {
            case 1:
                flag = 1;
                printf("\n");
                break;
            case 2:
                if(!flag) break;
                break;
            case 3:
                if(!flag) break;
                break;
            case 4:
                if(!flag) break;
                break;
            case 5:
                flag = 2;
                break;
        }
    }
    while(flag != 2);
}

void menu(int *escolha)
{
    printf("\n");
    printf("Selecione a opção:\n");
    printf("1. Abrir imagem e exibir informações       \n");
    printf("2. Criar imagens com figuras individuais   \n");
    printf("3. Encontrar uma figura colorida           \n");
    printf("4. Converter a imagem para escala de cinza \n");
    printf("5. Encerrar o programa                     \n");
    *escolha = ( getch()-'0' );
}
