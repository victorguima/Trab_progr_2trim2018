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
void infoimg(FILE *endereco);

int main()
{
    system("color 0A");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando língua como Português

    int option = 0,
        flag   = 0;

    typedef struct bmpheader {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
    } cabecalho;

    FILE *filePtr;
    ibagem = fopen("TesteBmp.bmp","r+b");

    /* Caso o programa não conseguir abrir a imagem: */
     if (filePtr == 0)
        puts("Deu ruim");
        return 0;

    do
    {
        menu(&option);
        switch(option)
        {
            case 1:
                printf("\n");
                infimg(filePtr);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                flag++;
                break;
        }
    }
    while(flag == 0);
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

void infoimg(FILE *endereco)
{

}
