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

typedef char  BYTE; // 1 Bytes
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
    WORD  biPlanes;         //Nº de planos da imagem, deve ser 1
    WORD  biBitCount;       //Números de bits por pixel
    DWORD biCompression;    //Compressão usada
    DWORD biSizeImage;      //Tamanho de dados da imagem
    DWORD biXPelsPerMeter;  //Resolução horizontal pixel/m
    DWORD biYPelsPerMeter;  //Resolução vertical pixel/m
    DWORD biClrUsed;        //Nº de cores usadas
    DWORD biClrImportant;   //Nº de cores importantes
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

    //Alocando memória para os ponteiros
    ptrheader = (struct bmpheader*) malloc(sizeof(struct bmpheader));
    ptrinfo   = (struct bmpinfoheader*) malloc(sizeof(struct bmpinfoheader));

    puts("Por favor insira o nome do arquivo");
    gets(nome);
    strcat(nome, ".bmp");
    printf("Procurando arquivo %s...\n", nome);

    FILE *filePtr;
    filePtr = fopen(nome,"r+b");

    // Caso o programa não conseguir abrir a imagem:
    if (filePtr == 0)
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};
        system("cls");
        main();
    }
    else puts("Sucesso!");

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
                flag = 1;
                printf("\n");
                headerreader(filePtr,ptrheader,ptrinfo);
                    break;
            case 2:
                if(flag != 1) break;
                buscacor(filePtr,ptrheader,ptrinfo);
                break;
            case 3:
                if(flag != 1) break;
                break;
            case 4:
                if(flag != 1) break;
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
    fseek(adr, 0, SEEK_SET);                                //Garantindo que começa do começo
    fread(&ptrheader->bfType, sizeof(WORD), 1, adr);        //Lendo assinatura do arquivo
    printf("\n%d %x", ptrheader->bfType, ptrheader->bfType);
    if(ptrheader->bfType != 0x4d42)                         //Checa assinatura
    {
        puts("O arquivo não é .bmp!");
        return 0;
    }
    fread(&ptrheader->bfSize, sizeof(DWORD), 1, adr);       //Lendo Qtd de bytes do cabeçalho
    fread(&ptrheader->bfReserved1, sizeof(WORD), 1, adr);   //Lendo Byte Reservado 1
    fread(&ptrheader->bfReserved2, sizeof(WORD), 1, adr);   //Lendo Byte Reservado 2
    fread(&ptrheader->bfOffBits, sizeof(DWORD), 1, adr);    //Lendo BfOffSetBits
    /// bmpinfoheader
    fread(&ptrinfo->biSize, sizeof(DWORD), 1, adr);         //Lendo Ttamanho do arquivo
    fread(&ptrinfo->biWidth, sizeof(DWORD), 1, adr);        //Lendo Largura
    fread(&ptrinfo->biHeight, sizeof(DWORD), 1, adr);       //Lendo Altura
    fread(&ptrinfo->biPlanes, sizeof(WORD), 1, adr);        //Lendo Nº de planos da imagem
    fread(&ptrinfo->biBitCount, sizeof(WORD), 1, adr);      //Lendo Quantidade de bits por pixel
    fread(&ptrinfo->biCompression, sizeof(DWORD), 1, adr);  //Lendo Compressão usada
    fread(&ptrinfo->biSizeImage, sizeof(DWORD), 1, adr);    //Lendo Tamanho de dados da imagem
    fread(&ptrinfo->biXPelsPerMeter, sizeof(DWORD), 1, adr);//Lendo Resolução horizontal pixel/m
    fread(&ptrinfo->biYPelsPerMeter, sizeof(DWORD), 1, adr);//Lendo Resolução vertical pixel/m
    fread(&ptrinfo->biClrUsed, sizeof(DWORD), 1, adr);      //Lendo Nº de cores usadas
    fread(&ptrinfo->biClrImportant, sizeof(DWORD), 1, adr); //Lendo Nº de cores importantes


    printf("\nAssinatura: %c%c",ptrheader->bfType%0x100,ptrheader->bfType/0x100);
    printf("\nO tamanho do arquivo é %x Bytes",ptrheader->bfSize);
    printf("\nO deslocamento do cabeçalho até o início do arquivo é %d Bytes",ptrheader->bfOffBits );

    printf("\nO arquivo tem %d Bytes por pixel",ptrinfo->biBitCount);
    printf("\nO tamanho do cabeçalho é %x Bytes",ptrinfo->biSize);
    printf("\nA largura do arquivo é %d pixels",ptrinfo->biWidth);
    printf("\nA altura do arquivo é %d pixels",ptrinfo->biHeight);
    printf("\nO arquivo possui %d bits por pixel",ptrinfo->biBitCount);

    return 0;
}

int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
    int i, aux,
        altura  = ptrinfo->biHeight,
        largura = ptrinfo->biWidth;

    if(largura%4 != 0)
    {
       largura += (4-largura%4);
    }

    int tamanho = altura*largura;
    printf("\n%d", tamanho);

    FILE *redptr;
    redptr = fopen("RED.bmp","w+b");

    //Passando cabeçalho para novo arquivo
    fwrite(&ptrheader->bfType, sizeof(WORD), 1, redptr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, redptr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, redptr);

    fseek(redptr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    BYTE cor[3];
    BYTE white[3]   = {0xff, 0xff, 0xff};
    BYTE red[3]     = {0x00, 0x00, 0xff};
    BYTE blue[3]    = {0xff, 0x00, 0x00};
    BYTE green[3]   = {0x00, 0xff, 0x00};
    BYTE nulo  = 0;

    puts(" ");

    //for(i = 0; i <=ptrheader->bfSize; i++)
    for(i = 0; i < tamanho; i++)
    {
        if(feof(adr))
        {
            printf("EITA ");
            fwrite(&nulo, 1, 1, redptr);
            continue;
        }
        aux = fread(cor, 1, 1, adr);
        if(feof(adr))
        {
            printf("EITA ");
            fwrite(&nulo, 1, 1, redptr);
            continue;
        }
        if(!aux)
        {
            printf("\ndeu ruim1 i=%d\n", i);
            cor[0] = white[0];
        }
        aux = fread(&cor[1], 1, 1, adr);
        if(feof(adr))
        {
            printf("EITA ");
            fwrite(&nulo, 1, 1, redptr);
            continue;
        }
        if(!aux)
        {
            printf("\ndeu ruim2 i=%d\n", i);
            cor[1] = white[1];
        }
        aux = fread(&cor[2], 1, 1, adr);
        if(feof(adr))
        {
            printf("EITA ");
            fwrite(&nulo, 1, 1, redptr);
            continue;
        }
        if(!aux)
        {
            printf("\ndeu ruim3 i=%d\n", i);
            cor[2] = white[2];
        }

        /*printf("%x", cor[0]);
        printf("%x", cor[1]);
        printf("%x\t", cor[2]);*/

        //printf("%x%x%x\t", cor, cor+1, cor+2);

        if(aux)
        {
            if(cor[0] == blue[0] && cor[1] == blue[1] && cor[2] == blue[2])
            {
                fwrite(&white,   1, 1, redptr);
                fwrite(&white[1], 1, 1, redptr);
                fwrite(&white[2], 1, 1, redptr);
                continue;
            }
            if(cor[0] == green[0] && cor[1] == green[1] && cor[2] == green[2])
            {
                fwrite(&white,   1, 1, redptr);
                fwrite(&white[1], 1, 1, redptr);
                fwrite(&white[2], 1, 1, redptr);
                continue;
            }
            if(cor[0] == red[0] && cor[1] == red[1] && cor[2] == red[2])
            {
                fwrite(&red,   1, 1, redptr);
                fwrite(&red[1], 1, 1, redptr);
                fwrite(&red[2], 1, 1, redptr);
                continue;
            }
            else
            {
                fwrite(&cor,   1, 1, redptr);
                fwrite(&cor[1], 1, 1, redptr);
                fwrite(&cor[2], 1, 1, redptr);
                continue;
            }
        }
    }

    return 0;
}
