/**
@Author: Victor Guimarães
@Author: Gabriel Machado
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <time.h>

typedef unsigned short WORD; // 2 Bytes
typedef unsigned int   DWORD;// 4 Bytes

struct bmpheader {
	WORD	bfType;         //Assinatura do arquivo (BM)
	DWORD	bfSize;         //Tamanho do arquivo
	WORD	bfReserved1;    //Reservado
	WORD	bfReserved2;    //Reservado
	DWORD	bfOffBits;      //Numero de bytes do cabeçalho até o começo do arquivo
    };

struct bmpinfoheader{
    DWORD biSize;           //Tamanho do cabeçalho, em bytes
    DWORD biWidth;          //Largura em pixels
    DWORD biHeight;         //Altura em pixels
    WORD biPlanes;          //specifies the number of color planes, must be 1
    WORD biBitCount;        //Números de bits por pixel
    DWORD biCompression;    //specifies the type of compression
    DWORD biSizeImage;      //size of image in bytes
    DWORD biXPelsPerMeter;  //number of pixels per meter in x axis
    DWORD biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;        //number of colors used by the bitmap
    DWORD biClrImportant;   //number of colors that are important
    };

void menu(int *escolha);
int headerreader(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo);
int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo);

int main()
{
    system("color 0E");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando língua como Português

    int option = 0,
        flag   = 0;

    char nome[20];

    struct bmpheader *ptrheader;
    struct bmpinfoheader *ptrinfo;

    //ptrheader = (struct bmpheader*) malloc(sizeof(struct bmpheader));
    //ptrinfo   = (struct bmpinfoheader*) malloc(sizeof(struct bmpinfoheader));

    puts("Por favor insira o nome do arquivo");
    gets(nome);
    strcat(nome, ".bmp");
    puts(nome);

    FILE *filePtr;
    filePtr = fopen(nome,"r+b");

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
                headerreader(filePtr,ptrheader,ptrinfo);
                break;
            case 2:
                buscacor(filePtr,ptrheader,ptrinfo);
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
    printf("\n");
    fclose(filePtr);
    return 0;
}

void menu(int *escolha)
{
    printf("\nOpção: ");
    *escolha = ( getche()-'0' );
    if(*escolha < 1 || *escolha > 5)
    {
        printf("\nColoca um valor de 1 a 5 caralho\n"); //Hoje eu to pro crime
        *escolha = 0;
    }
}

int headerreader(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
    /// bmpheader
    //Lendo assinatura do arquivo
    fseek(adr, 0, SEEK_SET); //Garantindo que começa do começo
    fread(&ptrheader->bfType, sizeof(WORD), 1, adr);
    //Checa assinatura
    if(ptrheader->bfType != 0x4d42)
    {
        puts("O arquivo não é .bmp!");
        return 0;
    }
    printf("\nAssinatura: %c%c",ptrheader->bfType%0x100,ptrheader->bfType/0x100);
    //Lendo quantidade de bytes do cabeçalho
    fread(&ptrheader->bfSize, sizeof(DWORD), 1, adr);
    printf("\nO tamanho do arquivo é %x Bytes",ptrheader->bfSize);
    //Pulando espaços reservados
    fseek(adr, 4, SEEK_CUR);
    //Lendo BfOffSetBits
    fread(&ptrheader->bfOffBits, sizeof(DWORD), 1, adr);
    printf("\nO deslocamento do cabeçalho até o início do arquivo é %d Bytes",ptrheader->bfOffBits );

    /// bmpinfoheader
    //Lendo tamanho do arquivo
    fread(&ptrinfo->biSize, sizeof(DWORD), 1, adr);
    printf("\nO tamanho do cabeçalho é %x Bytes",ptrinfo->biSize);
    //Lendo largura
    fread(&ptrinfo->biWidth, sizeof(DWORD), 1, adr);
    printf("\nA largura do arquivo é %d pixels",ptrinfo->biWidth);
    //Lendo altura
    fread(&ptrinfo->biHeight, sizeof(DWORD), 1, adr);
    printf("\nA altura do arquivo é %d pixels",ptrinfo->biHeight);
    //Pulando BiPlanes
    fseek(adr, 2, SEEK_CUR);
    //Lendo biBitCount
    fread(&ptrinfo->biBitCount, sizeof(WORD), 1, adr);
    printf("\nO arquivo possui %d bits por pixel",ptrinfo->biBitCount);

    printf("\n%d, %x  \n", ptrheader->bfOffBits, ptrheader->bfOffBits);

    return 0;
}

int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
     //int cor = sizeof(int);
    //printf("\n%d, %x \n",cor, cor);
    printf("\n%d, %x  \n", ptrheader->bfOffBits, ptrheader->bfOffBits);
    //fseek(adr, ptrheader->bfOffBits, SEEK_SET);
    //fread(&cor, sizeof(int), 1, adr);
    //printf("%d, %x \n",cor, cor);
    return 0;
}
