# Trab_progr_2trim2018
Trabalho de programação da disciplina de Elementos de Programação sobre manipulação de imagens
Especificações: 
Desenvolver um aplicativo para Windows, modo console, em linguagem C, que implemente as seguintes funcionalidades, selecionáveis através de menu:
 - Abrir um arquivo no formato BMP Ao abrir o arquivo, exibir as seguintes informações:
 - Tamanho da imagem (h x l)
 - Profundidade de cores
 - Se o arquivo não for BMP, exibir uma mensagem de erro.
 - Gravar canais de cores em arquivos separados Gravar 3 arquivos, um somente com os componentes em vermelho, outro em verde e o terceiro em azul, identificando os arquivos com um _R (ou _G ou _B) ao final do nome do arquivo.
 - Identificar na imagem objetos de uma cor especificada pelo usuário, gravando um arquivo cujo nome vai receber _achei no final. A identificação consiste em inscrever o objeto em um retângulo com as bordas pretas ou ainda, contornar o objeto com uma linha preta de largura 2 pixeis.
 - Converter o arquivo para o formato escala de cinza, gravando o arquivo com a adição _gs ao nome. No formato escala de cinza utiliza-se somente 1 byte por pixel, e este byte indica o nível de luminosidade do pixel. O cabeçalho do arquivo deve ser lido e manipulado através de uma struct. O programa não pode usar variáveis globais. Utilizar funções para implementar as funcionalidades do programa. Criar um tipo de dado para manipular a imagem dentro do programa. Arquivos gravados serão visualizados através de programas de edição de bmp (paint). 
