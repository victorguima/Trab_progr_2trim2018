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
    int     i, j,               // Variáveis de incremento do for
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth;
<<<<<<< HEAD

    DWORD   option,             // Variável da função escolhida pelo usuário
=======
    DWORD   option,
>>>>>>> parent of c763577... Mais algumas melhoras
            black = 0x000000,
            white = 0xffffff,
            red   = 0xff0000,
            blue  = 0x0000ff,
<<<<<<< HEAD
            green = 0x00ff00;

    char    *nome,              // Nome do arquivo a ser criado dentro dessa função
            cor[3];             // Cada posição corresponde ao valor de 0 a 255 de cada cor

    FILE    *newFilePtr;        // Ponteiro para o arquivo gerado
=======
            green = 0x00ff00,
            cor[3];
    char    *nome;
    FILE    *newFilePtr;
>>>>>>> parent of c763577... Mais algumas melhoras

    //Garantindo que o vetor está zerado (Previne erros)
    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;

    //Aloca memória para simular um vetor igual ao do ponteiro do nome recebido
    nome = malloc(sizeof(arquivo));

    strcpy(nome, arquivo); //Copiando a string do nome original para uma nova variavel

    puts("\n\nEscolha a cor que desejas separar:");
    puts("1. Vermelho");
    puts("2. Verde");
    puts("3. Azul");

    //Leitura da tecla pressionada
    do
    {
        option = ( getche() - '0' );
        if(option < 1 || option > 3) printf("\nColoca um valor de 1 a 3 porra\n"); // Mensagem cordial
    }while(option < 1 || option > 3);
    puts(" ");

    switch(option)
    {
        case 1:
            option = red;
            strcat(nome, "_R");     // Colocando o sufixo da cor separada
            break;
        case 2:
            option = green;
            strcat(nome, "_G");     // Colocando o sufixo da cor separada
            break;
        case 3:
            option = blue;
            strcat(nome, "_B");     // Colocando o sufixo da cor separada
            break;
    }
    strcat(nome, ".bmp");           // Colocando o .bmp ao fim do nome do arquivo

    newFilePtr = fopen(nome,"w+b"); // Crianco arquivo
    if (newFilePtr == 0)            // Em caso de erro
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};          // Esperando qualquer tecla ser pressionada
        system("cls");              // Limpa tela
        main();                     // Volta para o começo
    }
    else
    {
        printf("O arquivo %s foi criado com sucesso!", nome);
    }

    /** Passando cabeçalho para novo arquivo:
    *
    *   Tive que usar dois fwrite pra passar as informações
    *   contidas na struct ptrheader pois por algum motivo
    *   sempre acabava indo junto alguns bytes nulos
    *   e mesmo assim tive que subtrair 4 bytes em vez de
    *   2 (Tamanho do bfType passado no primeiro fwrite)
    *   porque ficavam sobrando dois bytes nulos no final
    *
    */

    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    // Pulando o ponteiro do arquivo pra o começo dos pixels pra garantir
    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(i = 0; i < altura; i++)
    {
        for(j = 0; j < largura; j++)
        {
            // Lendo cada valor RGB de cada pixel,
            // lembrando que no arquivo eles estão na sequência BGR
            fread(&cor[0], 1, 1, adr);  //Blue
            fread(&cor[1], 1, 1, adr);  //Green
            fread(&cor[2], 1, 1, adr);  //Red

            /// Testando se a soma dos 3 for menor do que a cor branca
            if( (cor[0] + cor[1] + cor[2]) < (0xFF * 3) ) //Branco == FF*3
            {
                if(option == red)
                {
                    if(cor[2] > 40) // Mínimo pra ser considerado vermelho
                    {
                        if( (cor[0] < 30) && (cor[1] < 30) ) //Máximos antes que deixe de ser vermelho
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
                    if(cor[1] > 40) // Mínimo pra ser considerado verde
                    {
                        if( (cor[0] < 30) && (cor[2] < 30) ) // Máximos antes que deixe de ser verde
                        {
                            fwrite(&cor[0], 1, 1, newFilePtr);
                            fwrite(&cor[1], 1, 1, newFilePtr);
                            fwrite(&cor[2], 1, 1, newFilePtr);
                        }
                        else fwrite(&white, 3, 1, newFilePtr);
                    }
                    else fwrite(&white, 3, 1, newFilePtr);
                }
                if(option == blue) // Mínimo pra ser considerado azul
                {
                    if(cor[0] > 40)
                    {
                        if( (cor[1] < 30) && (cor[2] < 30) ) // Máximos antes que deixe de ser azul
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
        ///Escrevendo bytes nulos no final dos arquivos cuja largura não for divisível por 4
        for(j = 0; j < (largura%4); j++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            //Movendo o ponteiro do arquivo original para ficar sincronizado com o outro
            fseek(adr, 1, SEEK_CUR);
        }
    }
    return 0;
}

int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo)
{
    int     x, y,               // Variáveis de incremento do for
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth,
<<<<<<< HEAD
            up      = 0,        // Representa a parte mais alta do objeto
            down    = altura,   // Representa a parte mais baixa do objeto
            left    = largura,  // Representa a parte mais à esquerda do objeto
            right   = 0;        // Representa a parte mais à direita do objeto
=======
            up      = 0,
            down    = altura,
            left    = largura,
            right   = 0;;
>>>>>>> parent of c763577... Mais algumas melhoras

    DWORD   option,             // Variável da função escolhida pelo usuário
            red   = 0xff0000,
            blue  = 0x0000ff,
            green = 0x00ff00,
<<<<<<< HEAD
            black = 0x000000;

    char    *nome,              // Nome do arquivo a ser criado dentro dessa função
            cor[3];             // Cada posição corresponde ao valor de 0 a 255 de cada cor

    FILE    *newFilePtr;        // Ponteiro para o arquivo gerado

    //Garantindo que o vetor está zerado (Previne erros)
    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;

    //Aloca memória para simular um vetor igual ao do ponteiro do nome recebido
    nome = malloc(sizeof(arquivo));

    strcpy(nome, arquivo); //Copiando a string do nome original para uma nova variavel

=======
            black = 0x000000,
            cor   = 0x00;
    FILE    *newFilePtr;
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

>>>>>>> parent of c763577... Mais algumas melhoras
    puts("\n\nEscolha a cor que estais a procurar:");
    puts("1. Vermelho");
    puts("2. Verde");
    puts("3. Azul");

    do
    {
        option = ( getche()-'0' );
        if(option < 1 || option > 3) printf("\nColoca um valor de 1 a 3 porra\n"); // Mensagem cordial
    }while(option < 1 || option > 3);
    puts(" ");

    switch(option)
    {
        case 1:
            option = red;
<<<<<<< HEAD
            strcat(nome, "_R_achei");     // Colocando o sufixo da cor encontrada
            break;
        case 2:
            option = green;
            strcat(nome, "_G_achei");     // Colocando o sufixo da cor encontrada
            break;
        case 3:
            option = blue;
            strcat(nome, "_B_achei");     // Colocando o sufixo da cor encontrada
            break;
    }
    strcat(nome, ".bmp");           // Colocando o .bmp ao fim do nome do arquivo

    newFilePtr = fopen(nome,"w+b"); // Crianco arquivo
    if (newFilePtr == 0)            // Em caso de erro
=======
            strcat(arquivo, "_R_achei");
            break;
        case 2:
            option = green;
            strcat(arquivo, "_G_achei");
            break;
        case 3:
            option = blue;
            strcat(arquivo, "_B_achei");
            break;
    }
    strcat(arquivo, ".bmp");

    newFilePtr = fopen(arquivo,"w+b");
    if (newFilePtr == 0)
>>>>>>> parent of c763577... Mais algumas melhoras
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};          // Esperando qualquer tecla ser pressionada
        system("cls");              // Limpa tela
        main();                     // Volta para o começo
    }
    else
    {
        printf("O arquivo %s foi criado com sucesso!", arquivo);
    }

    /** Passando cabeçalho para novo arquivo:
    *
    *   Tive que usar dois fwrite pra passar as informações
    *   contidas na struct ptrheader pois por algum motivo
    *   sempre acabava indo junto alguns bytes nulos
    *   e mesmo assim tive que subtrair 4 bytes em vez de
    *   2 (Tamanho do bfType passado no primeiro fwrite)
    *   porque ficavam sobrando dois bytes nulos no final
    *
    */

    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(y = 0; y < altura; y++)
    {
        for(x = 0; x < largura; x++)
        {
<<<<<<< HEAD
            // Lendo cada valor RGB de cada pixel,
            // lembrando que no arquivo eles estão na sequência BGR
            fread(&cor[0], 1, 1, adr);  //Blue
            fread(&cor[1], 1, 1, adr);  //Green
            fread(&cor[2], 1, 1, adr);  //Red

            /// Testando se a soma dos 3 for menor do que a cor branca
            if( (cor[0] + cor[1] + cor[2]) < (0xFF * 3) ) //Branco == FF*3
=======
            fread(&cor, 3, 1, adr);
            if(cor == option)
>>>>>>> parent of c763577... Mais algumas melhoras
            {
                if(option == red)
                {
                    if(cor[2] > 40) // Mínimo pra ser considerado vermelho
                    {
                        if( (cor[0] < 30) && (cor[1] < 30) ) //Máximos antes que deixe de ser vermelho
                        {
                            if(y  > up)     up      =   y;
                            if(y  < down)   down    =   y;
                            if(x  > left)   left    =   x;
                            if(x  < right)  right   =   x;
                        }
                    }
                }
                if(option == green)
                {
                    if(cor[1] > 40) // Mínimo pra ser considerado verde
                    {
                        if( (cor[0] < 30) && (cor[2] < 30) ) // Máximos antes que deixe de ser verde
                        {
                            if(y  > up)     up      =   y;
                            if(y  < down)   down    =   y;
                            if(x  > left)   left    =   x;
                            if(x  < right)  right   =   x;
                        }
                    }
                }
                if(option == blue) // Mínimo pra ser considerado azul
                {
                    if(cor[0] > 40)
                    {
                        if( (cor[1] < 30) && (cor[2] < 30) ) // Máximos antes que deixe de ser azul
                        {
                            if(y  > up)     up      =   y;
                            if(y  < down)   down    =   y;
                            if(x  > left)   left    =   x;
                            if(x  < right)  right   =   x;
                        }
                    }
                }
            }
        }
    }

    up      += 2;
    down    -= 2;
    right   += 2;
    left    -= 2;

    for(y = 0; y < altura; y++)
    {
        for(x = 0; x < largura; x++)
        {
            if(y == up)
            {
                if(x >= left && y <= right)
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
        ///Escrevendo bytes nulos no final dos arquivos cuja largura não for divisível por 4
        for(x = 0; x < (largura%4); x++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            //Movendo o ponteiro do arquivo original para ficar sincronizado com o outro
            fseek(adr, 1, SEEK_CUR);
        }
    }
    return 0;
}

int grayscale(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo, char *arquivo)
{
    int     i, j,                   // Variáveis de incremento do for
            altura  = ptrinfo->biHeight,
            largura = ptrinfo->biWidth;
    DWORD   black = 0x000000,
            white = 0xffffff,
<<<<<<< HEAD
            gray  = 0;

    char    *nome,                  // Nome do arquivo a ser criado dentro dessa função
            cor[3];                 // Cada posição corresponde ao valor de 0 a 255 de cada cor

=======
            gray  = 0,
            cor[3];
>>>>>>> parent of c763577... Mais algumas melhoras
    FILE    *newFilePtr;
    strcat(arquivo, "_gs.bmp");

    // Garantindo que o vetor está zerado (Previne erros)
    cor[0] = 0;
    cor[1] = 0;
    cor[2] = 0;
<<<<<<< HEAD

    //Aloca memória para simular um vetor igual ao do ponteiro do nome recebido
    nome = malloc(sizeof(arquivo));

    strcpy(nome, arquivo);          // Copiando a string do nome original para uma nova variavel

    strcat(nome, "_gs.bmp");        // Adicionando os sufixos necessários ao fim do nome do arquivo

    newFilePtr = fopen(nome,"w+b"); // Crianco arquivo
    if (newFilePtr == 0)            // Em caso de erro
=======

     newFilePtr = fopen(arquivo,"w+b");
    if (newFilePtr == 0)
>>>>>>> parent of c763577... Mais algumas melhoras
    {
        puts("Deu ruim");
        puts("Pressione qualquer tecla para continuar...");
        while(!kbhit()){};          // Esperando qualquer tecla ser pressionada
        system("cls");              // Limpa tela
        main();                     // Volta para o começo
    }
    else
    {
        printf("\nO arquivo %s foi criado com sucesso!", arquivo);
    }

    /** Passando cabeçalho para novo arquivo:
    *
    *   Tive que usar dois fwrite pra passar as informações
    *   contidas na struct ptrheader pois por algum motivo
    *   sempre acabava indo junto alguns bytes nulos
    *   e mesmo assim tive que subtrair 4 bytes em vez de
    *   2 (Tamanho do bfType passado no primeiro fwrite)
    *   porque ficavam sobrando dois bytes nulos no final
    *
    */

    fwrite(&ptrheader->bfType, sizeof(WORD), 1, newFilePtr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, newFilePtr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, newFilePtr);

    fseek(newFilePtr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);

    for(i = 0; i < altura; i++)
    {
        for(j = 0; j < largura; j++)
        {
            // Lendo cada valor RGB de cada pixel,
            // lembrando que no arquivo eles estão na sequência BGR
            fread(&cor[0], 1, 1, adr);  // Blue
            fread(&cor[1], 1, 1, adr);  // Green
            fread(&cor[2], 1, 1, adr);  // Red

            if( (cor[0] + cor[1] + cor[2]) < (0xFF * 3) ) //Branco == FF*3
            {
                // Cálculo aproximar os valores para o cinza
                cor[0]  =   cor[0] * 0.3;
                cor[1]  =   cor[1] * 0.59;
                cor[2]  =   cor[2] * 0.11;

                gray = ( (cor[0] + cor[1] + cor[2])/3);

                // Na escala de cinza, todos os valores para as tres cores são iguais
                fwrite(&gray, 1, 1, newFilePtr);
                fwrite(&gray, 1, 1, newFilePtr);
                fwrite(&gray, 1, 1, newFilePtr);
                // E sim, tem que escrever o comando três vezes se não não funciona
            }
            else
            {
                fwrite(&white, 3, 1, newFilePtr);
            }

        }
        ///Escrevendo bytes nulos no final dos arquivos cuja largura não for divisível por 4
        for(j = 0; j < (largura%4); j++)
        {
            fwrite(&black, 1, 1, newFilePtr);
            //Movendo o ponteiro do arquivo original para ficar sincronizado com o outro
            fseek(adr, 1, SEEK_CUR);
        }
    }
    return 0;
}
