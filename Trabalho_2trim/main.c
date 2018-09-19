/**
@Author: Victor Guimar�es
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
void headerreader(FILE *endereco);

int main()
{
    system("color 0A");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando l�ngua como Portugu�s

    int option = 0,
        flag   = 0;

    FILE *filePtr;
    filePtr = fopen("TesteBmp.bmp","r+b");

    // Caso o programa n�o conseguir abrir a imagem: //
     if (filePtr == 0)
     {
        puts("Deu ruim");
        return 0;
    }

    do
    {
        menu(&option);
        switch(option)
        {
            case 1:
                printf("\n");
                headerreader(filePtr);
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
    printf("Selecione a op��o:\n");
    printf("1. Abrir imagem e exibir informa��es       \n");
    printf("2. Criar imagens com figuras individuais   \n");
    printf("3. Encontrar uma figura colorida           \n");
    printf("4. Converter a imagem para escala de cinza \n");
    printf("5. Encerrar o programa                     \n");
    *escolha = ( getche()-'0' );
}

void headerreader(FILE *endereco)
{
    typedef unsigned short WORD;
    typedef unsigned int DWORD;
    typedef unsigned short BYTE;
    typedef long LONG;

    struct bmpheader {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
    };

    struct bmpinfoheader{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
    };

    struct bmpinfoheader  cabecalho_info;
    struct bmpheader cabecalho_bmp;

    fread(&cabecalho_bmp.bfType, sizeof(WORD), 1, endereco);
    printf("%x",cabecalho_bmp.bfType);


}
