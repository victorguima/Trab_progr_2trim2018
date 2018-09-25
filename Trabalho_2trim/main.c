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
void headerreader(FILE *adr);

int main()
{
    system("color 0A");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando língua como Português

    int option = 0,
        flag   = 0;

    FILE *filePtr;
    filePtr = fopen("TesteBmp.bmp","r+b");

    // Caso o programa não conseguir abrir a imagem: //
     if (filePtr == 0)
     {
        puts("Deu ruim");
        return 0;
    }

    printf("\n");
    printf("Selecione a opção:\n");
    printf("1. Abrir imagem e exibir informações       \n");
    printf("2. Criar imagens com figuras individuais   \n");
    printf("3. Encontrar uma figura colorida           \n");
    printf("4. Converter a imagem para escala de cinza \n");
    printf("5. Encerrar o programa                     \n");

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
                flag = 2;
                break;
        }
    }
    while(flag != 2);
}

void menu(int *escolha)
{
    printf("\n");
    *escolha = ( getche()-'0' );
    if(*escolha < 1 || *escolha > 5)
    {
        printf("\nColoca um valor de 1 a 5 caralho\n"); //Hoje eu to pro crime
        *escolha = 0;
    }
}

void headerreader(FILE *adr)
{
    typedef unsigned short BYTE; // 1 Byte
    typedef unsigned short WORD; // 2 Bytes
    typedef unsigned int   DWORD;// 4 Bytes

    struct bmpheader {
	WORD	bfType; //Assinatura do arquivo (BM)
	DWORD	bfSize; //Tamanho do arquivo
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits; //Numero de bytes do cabeçalho até o começo do arquivo
    };

    struct bmpinfoheader{
    DWORD biSize;  //specifies the number of bytes required by the struct
    DWORD biWidth;  //specifies width in pixels
    DWORD biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    DWORD biXPelsPerMeter;  //number of pixels per meter in x axis
    DWORD biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
    };

    struct bmpinfoheader  cabecalho_info;
    struct bmpheader      cabecalho_bmp;

    //Lendo assinatura do arquivo
    fread(&cabecalho_bmp.bfType, sizeof(WORD), 1, adr);
    printf("\nAssinatura: %c%c",cabecalho_bmp.bfType%0x100,cabecalho_bmp.bfType/0x100);

    //Movendo para próximo dado
    fseek(adr, 2, SEEK_SET);

    //Lendo tamanho do arquivo
    fread(&cabecalho_bmp.bfSize, sizeof(DWORD), 1, adr);
    printf("\nO tamanho do arquivo é %x Bytes \n",cabecalho_bmp.bfSize);

    //Movendo para próximo dado
    fseek(adr, 8, SEEK_SET);

    //Lendo BfOffSetBits
    fread(&cabecalho_bmp.bfOffBits, sizeof(DWORD), 1, adr);
    printf("\n %d \t %x", cabecalho_bmp.bfOffBits, cabecalho_bmp.bfOffBits);
    printf("\nO tamanho do cabeçalho é %d Bytes \n",
        (cabecalho_bmp.bfOffBits>>16) );


}
