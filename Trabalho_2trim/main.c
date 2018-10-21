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
int separacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo);
int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo);
int grayscale(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo);

int main()
{
    system("color 0E");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando língua como Português

    int     option = 0,
            flag   = 0;

    char    nome[20],
            nomebmp[20];


    struct bmpheader *ptrheader;
    struct bmpinfoheader *ptrinfo;

    FILE *filePtr;

    //Alocando memória para os ponteiros
    ptrheader = (struct bmpheader*) malloc(sizeof(struct bmpheader));
    ptrinfo   = (struct bmpinfoheader*) malloc(sizeof(struct bmpinfoheader));

    puts("Por favor insira o nome do arquivo");
    gets(nome);
    strcpy(nomebmp, nome);
    strcat(nomebmp, ".bmp");
    printf("Procurando arquivo %s...\n", nomebmp);


    filePtr = fopen(nomebmp,"r+b");

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
                separacor(filePtr,ptrheader,ptrinfo,nome);
                break;
            case 3:
                if(flag != 1) break;
                buscacor(filePtr,ptrheader,ptrinfo,nome);
                break;
            case 4:
                if(flag != 1) break;
                grayscale(filePtr,ptrheader,ptrinfo,nome);
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
    printf("\nO arquivo possui %d bits por pixel \n",ptrinfo->biBitCount);

    return 0;
}

int separacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo)
{
    int     i, j,
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth;

    DWORD   option,
            black = 0x000000,
            white = 0xffffff,
            red   = 0xff0000,
            blue  = 0x0000ff,
            green = 0x00ff00;

    char    *nome,
            cor[3];

    FILE    *newFilePtr;

    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;
    nome = malloc(sizeof(arquivo));

    strcpy(nome, arquivo);

    puts("\n\nEscolha a cor que desejas separar:");
    puts("1. Vermelho");
    puts("2. Verde");
    puts("3. Azul");

    //Leitura da tecla pressionada
    do
    {
        option = ( getche()-'0' );
        if(option < 1 || option > 3) printf("\nColoca um valor de 1 a 3 porra\n");
    }while(option < 1 || option > 3);
    puts(" ");

    switch(option)
    {
        case 1:
            option = red;
            strcat(nome, "_R");
            break;
        case 2:
            option = green;
            strcat(nome, "_G");
            break;
        case 3:
            option = blue;
            strcat(nome, "_B");
            break;
    }
    strcat(nome, ".bmp");

    newFilePtr = fopen(nome,"w+b");
    if (newFilePtr == 0)
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};
        system("cls");
        main();
    }
    else
    {
        printf("O arquivo %s foi criado com sucesso!", nome);
    }

    //Passando cabeçalho para novo arquivo
    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(i = 0; i < altura; i++)
    {
        for(j = 0; j < largura; j++)
        {
            fread(&cor[0], 1, 1, adr);  //Blue
            fread(&cor[1], 1, 1, adr);  //Green
            fread(&cor[2], 1, 1, adr);  //Red

            if( (cor[0] + cor[1] + cor[2]) < (0xFF * 3) ) //Branco == FF*3
            {
                if(option == red)
                {
                    //if( (cor[2] - cor[0] < 30) || (cor[2] - cor[1] < 30) )
                    if(cor[2] > 40)
                    {
                        if( (cor[0] < 30) && (cor[1] < 30) )
                        {
                            fwrite(&cor[0], 1, 1, newFilePtr);
                            fwrite(&cor[1], 1, 1, newFilePtr);
                            fwrite(&cor[2], 1, 1, newFilePtr);
                        }
                        else fwrite(&white, 3, 1, newFilePtr);
                    }
                    else fwrite(&white, 3, 1, newFilePtr);
                }
                if(option == green)
                {
                    //if( (cor[1] - cor[0] < 30) || (cor[1] - cor[2] < 30) )
                    if(cor[1] > 40)
                    {
                        if( (cor[0] < 30) && (cor[2] < 30) )
                        {
                            fwrite(&cor[0], 1, 1, newFilePtr);
                            fwrite(&cor[1], 1, 1, newFilePtr);
                            fwrite(&cor[2], 1, 1, newFilePtr);
                        }
                        else fwrite(&white, 3, 1, newFilePtr);
                    }
                    else fwrite(&white, 3, 1, newFilePtr);
                }
                if(option == blue)
                {
                    //if( (cor[0] - cor[1] < 30) || (cor[0] - cor[2] < 30) )
                    if(cor[0] > 40)
                    {
                        if( (cor[1] < 30) && (cor[2] < 30) )
                        {
                            fwrite(&cor[0], 1, 1, newFilePtr);
                            fwrite(&cor[1], 1, 1, newFilePtr);
                            fwrite(&cor[2], 1, 1, newFilePtr);
                        }
                        else fwrite(&white, 3, 1, newFilePtr);
                    }
                    else fwrite(&white, 3, 1, newFilePtr);
                }
            }
            else
            {
                fwrite(&white, 3, 1, newFilePtr);
            }

        }
        for(j = 0; j < (largura%4); j++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            fseek(adr, 1, SEEK_CUR);
        }
    }
    return 0;
}

int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo)
{
    int     x, y,
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth,
            up      = 0,
            down    = altura,
            left    = largura,
            right   = 0;

    DWORD   option,
            red   = 0xff0000,
            blue  = 0x0000ff,
            green = 0x00ff00,
            black = 0x000000;

    char   *nome,
            cor[3];

    FILE    *newFilePtr;

    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;
    nome = malloc(sizeof(arquivo));

    /*
    up[0][0] = 0;
    up[0][1] = 0;
    down[0][0] = 0;
    down[0][1] = altura;
    left[0][0] = largura;
    left[0][1] = 0;
    right[0][0] = 0;
    right[0][1] = 0;
    */

    strcpy(nome, arquivo);

    puts("\n\nEscolha a cor que estais a procurar:");
    puts("1. Vermelho");
    puts("2. Verde");
    puts("3. Azul");

    do
    {
        option = ( getche()-'0' );
        if(option < 1 || option > 3) printf("\nColoca um valor de 1 a 3 porra\n");
    }while(option < 1 || option > 3);
    puts(" ");

    switch(option)
    {
        case 1:
            option = red;
            strcat(nome, "_R_achei");
            break;
        case 2:
            option = green;
            strcat(nome, "_G_achei");
            break;
        case 3:
            option = blue;
            strcat(nome, "_B_achei");
            break;
    }
    strcat(nome, ".bmp");

    newFilePtr = fopen(nome,"w+b");
    if (newFilePtr == 0)
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};
        system("cls");
        main();
    }
    else
    {
        printf("O arquivo %s foi criado com sucesso!", nome);
    }

    //Passando cabeçalho para novo arquivo
    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(y = 0; y < altura; y++)
    {
        for(x = 0; x < largura; x++)
        {
            fread(&cor[0], 1, 1, adr);  //Blue
            fread(&cor[1], 1, 1, adr);  //Green
            fread(&cor[2], 1, 1, adr);  //Red

            //if(cor == option)
            {
                if(altura  > up)     up   = altura;
                if(altura  < down)   down = altura;
                if(largura > left)   left = largura;
                if(largura < right)  right = largura;
                /*if(y > up[0][1])
                {
                    up[0][0] = x;
                    up[0][1] = y;
                }
                if(x < left[0][0])
                {
                    left[0][0] = x;
                    left[0][1] = y;
                }
                if(y < down[0][1])
                {
                    down[0][0] = x;
                    down[0][1] = y;
                }
                if(x > right[0][0])
                {
                    right[0][0] = x;
                    right[0][1] = y;
                }*/
            }
        }
    }
    /*up[0][1]    += 2;
    left[0][0]  -= 2;
    down[0][1]  -= 2;
    right[0][0] += 2;*/

    up      += 2;
    down    -= 2;
    right   += 2;
    left    -= 2;


    for(y = 0; y < altura; y++)
    {
        for(x = 0; x < largura; x++)
        {
            /*if(altura == up[0][1] && largura <= right[0][0] && largura >= left[0][0] )
            {
                fwrite(&black, 3, 1, newFilePtr);
                continue;
            }*/

            if(altura == up)
            {
                if(largura >= left && largura <= right)
                {
                    fwrite(&black, 3, 1, newFilePtr);
                    fseek(adr, 3, SEEK_CUR);
                }
                else
                {
                    fread(&cor, 3, 1, adr);
                    fwrite(&cor, 3, 1, newFilePtr);
                }
                continue;
            }

            fread(&cor, 3, 1, adr);
            fwrite(&cor, 3, 1, newFilePtr);

        }
        for(x = 0; x < (largura%4); x++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            fseek(adr, 1, SEEK_CUR);
        }
    }

    return 0;
}

int grayscale(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo)
{
    int     i, j,
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth;

    DWORD   black = 0x000000,
            white = 0xffffff,
            gray  = 0;

    char    *nome,
            cor[3];

    FILE    *newFilePtr;

    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;
    nome = malloc(sizeof(arquivo));

    strcpy(nome, arquivo);
    strcat(nome, "_gs.bmp");

    newFilePtr = fopen(nome,"w+b");
    if (newFilePtr == 0)
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};
        system("cls");
        main();
    }
    else
    {
        printf("\nO arquivo %s foi criado com sucesso!", nome);
    }

    //Passando cabeçalho para novo arquivo
    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(i = 0; i < altura; i++)
    {
        for(j = 0; j < largura; j++)
        {
            fread(&cor[0], 1, 1, adr);
            fread(&cor[1], 1, 1, adr);
            fread(&cor[2], 1, 1, adr);

            if( (cor[0] + cor[1] + cor[2]) < (0xFF * 3) ) //Branco == FF*3
            {
                cor[0]  *=  0.3;
                cor[1]  *=  0.59;
                cor[2]  *=  0.11;

                gray = ( (cor[0] + cor[1] + cor[2]));

                fwrite(&gray, 1, 1, newFilePtr);
                fwrite(&gray, 1, 1, newFilePtr);
                fwrite(&gray, 1, 1, newFilePtr);

            }
            else
            {
                fwrite(&white, 3, 1, newFilePtr);
            }

        }
        for(j = 0; j < (largura%4); j++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            fseek(adr, 1, SEEK_CUR);
        }
    }
    return 0;
}
