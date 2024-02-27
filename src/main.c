#include <stdio.h>
#include <intelfpgaup/accel.h>
#include <intelfpgaup/KEY.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
=================================
Parte dos defines
=================================
*/

#define LARGURA_BLOCO 32
#define ALTURA_BLOCO 10
#define NUM_BLOCOS_X 10
#define NUM_BLOCOS_Y 6
#define ESPACAMENTO 2
#define MARGEM_ESQUERDA 11
#define MARGEM_SUPERIOR 35
#define RAIO_BOLA 5

/**
* =================================
* Prototipação de funções
* =================================
*/

void fechar_hardwares();
int iniciar_hardwares();
void printTelaInicial();
int telaInicial();
void limparTela();
void printTelaParabens();
void gerar_bordas();
void moverRaquete();
void inicializarBlocos();
void palavra_score(int scoreInt, int shift_X, int shift_Y);
void sairPause();
void informacao_pause(int boolean);
void moverBola(int colisao);
void printTelaPerdeu();
void detectarColisao();
void detectarColisaoRaquete();
int calcularScore();
void desenhar_bola(int x, int y);
void sairTelaPerder(int histScore);
void numeroOne(int x, int y, short cor);
void numeroTwo(int x, int y, short cor);
void numeroThree(int x, int y, short cor);
void numeroFour(int x, int y, short cor);
void numeroFive(int x, int y, short cor);
void numeroSix(int x, int y, short cor);
void numeroSeven(int x, int y, short cor);
void numeroEight(int x, int y, short cor);
void numeroNine(int x, int y, short cor);
void numeroZero(int x, int y, short cor);
void printValorScore(int Score, int desloc_X, int desloc_Y);
int blocosAtivos();
void sairTelaParabens();
void printBlocosInferiores();
void renderizarBlocos();

/** 
 * --------------------------------------------------------------
 * @brief Estrutura para permitir criar os blocos que serão destruidos
 * 
 * Estrutura de dado para salvar as informações a respeito dos blocos
 * desenhandos na tela, que serão destruidos ao decorrer do jogo.
 * A estrutura contem:
 * @param X: valor para indicar o inicio de sua localização no eixo X
 * @param Y: valor para indicar o inicio de sua localização no eixo Y
 * @param largura: valor para indicar a largura que o bloco deve ter
 * @param altura: valor para indicar a altura que o bloco deve ter
 * @param destruido: inteiro que guarda a informação se o bloco ja foi destruido
 * ou não, sendo 0 para nao destruido e 1 para destruido
 * @param cor: valor para indicar qual a cor que esse bloco dever ter ao ser renderizado
 * 
 * --------------------------------------------------------------
 */
typedef struct
{
    int x;
    int y;
    int largura;
    int altura;
    int destruido; // 0 se não estiver destruído, 1 se estiver destruído
    short cor;
} Bloco;

/// @brief Lista dos blocos que serão apresentados e destruidos durante o jogo
Bloco blocos[NUM_BLOCOS_X * NUM_BLOCOS_Y];

/// @brief Variavel que indica o valor de onde a bola deve iniciar em X
int xInicialBola = 155;
/// @brief Variavel que indica o valor de onde a bola deve iniciar em Y
int yInicialBola = 135;
/// @brief Variavel que indica o valor de onde a raquete deve iniciar em X
int posicaoRaqueteX = 155;
/// @brief Variavel que indica o velocidade padrao de movimentação da bola no eixo X
int velocidadePadraoBolaX = 1;
/// @brief Variavel que indica o velocidade padrao de movimentação da bola no eixo Y
int velocidadePadraoBolaY = 1;
/// @brief Variavel que indica a largura da tela a se trabalhar
int screen_x = 320;
/// @brief Variavel que indica a altura da tela a se trabalhar
int screen_y = 240;
/// @brief Variavel que indica quantas colunas de texto o video tem disponivel
int quantColunasTexto = 0;
/// @brief Variavel que indica quantas linhas de texto o video tem disponivel
int quantLinhasTexto = 0;
/// @brief Variavel para inicializar o valor da pontuação em 0
int scoreInt = 0;
/// @brief Variavel confirmar com Naila
int velocidadeX = 0;


/** 
 * --------------------------------------------------------------
 * @brief Main do programa
 * 
 * Aqui onde todo o fluxo do jogo acontece, realizando todo os procedimentos
 * para que o jogo possa acontecer.
 * 
 * --------------------------------------------------------------
 */
int main()
{
    char *pause = "paused";
    int p, a;
    int btn_data = 0;

    //momento da inicialização do hardwares, caso ocorra erro o programa encerra
    if (!iniciar_hardwares()){
        printf("Não foi possivel iniciar os dispositivos de hardware");
        return -1; //caso em que nao foi possivel iniciar alguns dos 3 hardwares
    }
    //realiza a limpeza de tela e vai apresentar a tela inicial
    limparTela();
    if (!telaInicial()){
        return 0; //caso em que o usuario pressionou o botão de QUIT
    }

    /*
    realiza a limpeza da tela e inicialização dos blocos para inicio da logica
    do jogo, e assim poder jogar
    */
    limparTela();
    inicializarBlocos();

    /*
    o codigo permanece nesse loop enquanto o botão de saida geral não for pressioado
    */
    while (btn_data != 0b0010){
        //realiza a limpeza da tela, e leitura do botão
        limparTela();
        KEY_read(&btn_data);

        //inicio do desenho dos elementos, na ordem de procedencia junto
        //a chamadas para detectar colisão, e realizar movimentações dentro do jogo
        gerar_bordas();
        palavra_score(calcularScore(), 0, 0);
        moverRaquete();
        detectarColisao();
        detectarColisaoRaquete();
        moverBola(0);
        renderizarBlocos();


        //verificação para se o botão de pause tiver sido pressionado
        if (btn_data == 0b0100){
            // video_erase();
            informacao_pause(1);
            sairPause();
        }
        //serve para informar a função de pause, que não esta em situação de pause
        informacao_pause(0);

        /*
        após todos os elementos terem sido desenhados na tela, é feita a chamada para troca
        do buffer da tela, e logo a exibição dos elementos na tela
        */
        video_show();

        //verificação para se há bocos ativos, e caso não tenha, vai para a pagina de parabens
        if (blocosAtivos()){
            sairTelaParabens();
        }

        //caso a bola tenha ultrapassado a borda inferior, logo houve perda
        if (yInicialBola > 230){
            int scoreHist = calcularScore();
            sairTelaPerder(scoreHist);

        }
    }
    //limpa a tela, antes de ser encerrado o programa
    limparTela();

    // ROTINA DE QUANDO GANHA
    if (blocosAtivos()){
        limparTela();
        printTelaParabens();
        video_show();
    }
    //realiza a troca do buffer da tela, para apresentar a tela limpa
    video_show();
    //realiza o fechamento das portas de hardware ativas
    fechar_hardwares();
    return 0;
}


/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função faz a atribuição dos valores iniciais a cada um dos blocos da lista
 * em que estão guardados, colocando então aonde devem iniciar na posição X e Y,
 * além do seu tamanho, tanto em largura como em altura, a sua informação de que não
 * esta destruido, e por fim a cor daquele bloco, que é atribuida seguindo algoritmo
 * a fim de gerar um degrade de cores entre os blocos
 * 
 * --------------------------------------------------------------
 */
void inicializarBlocos()
{
    int indiceBloco = 0;
    // int larguraDisponivel = (screen_x - ((NUM_BLOCOS_X) * ESPACAMENTO));

    int larguraBloco = 28;
    int linha, coluna;

    for (linha = 0; linha < NUM_BLOCOS_Y; linha++)
    {
        for (coluna = 0; coluna < NUM_BLOCOS_X; coluna++)
        {
            blocos[indiceBloco].x = coluna * (larguraBloco + ESPACAMENTO) + MARGEM_ESQUERDA;
            blocos[indiceBloco].y = linha * (ALTURA_BLOCO + ESPACAMENTO) + MARGEM_SUPERIOR;
            blocos[indiceBloco].largura = larguraBloco;
            blocos[indiceBloco].altura = ALTURA_BLOCO;
            blocos[indiceBloco].destruido = 0;


            int cor1 = 0xF81F;
            int cor2 = 0xF809;
            int cor3 = 0xD0C0;

            float percentual = (float)coluna / NUM_BLOCOS_X;
            int corIntermediaria;
            if (percentual < 0.5)
            {
                corIntermediaria = (int)((0.5 - percentual) * 2 * cor1 + (percentual * 2) * cor2);
            }
            else
            {
                corIntermediaria = (int)((1.0 - percentual) * 2 * cor2 + (percentual * 2 - 1.0) * cor3);
            }

            blocos[indiceBloco].cor = corIntermediaria;

            indiceBloco++;

            if (indiceBloco >= NUM_BLOCOS_X * NUM_BLOCOS_Y)
            {
                break;
            }
        }

        if (indiceBloco >= NUM_BLOCOS_X * NUM_BLOCOS_Y)
        {
            break;
        }
    }
}



/** 
 * --------------------------------------------------------------
 * @brief Função para detectar a colisão entre a bola e algum dos blocos
 * 
 * Esta função aqui é para verificar se a bola ta colidindo com algum bloco no jogo.
 * Para cada bloco não destruído, verifica se a próxima posição da bola
 * irá colidir com as bordas do bloco. Se houver colisão:
 *   - Marca o bloco como destruído
 *   - Inverte a direção da bola dependendo de onde ela colidiu:
 *        - Se a bola estava indo para cima, inverte a direção Y para baixo
 *        - Se a bola estava indo para baixo, inverte a direção Y para cima
 *        - Se a bola estava indo para a esquerda, inverte a direção X para a direita
 *        - Se a bola estava indo para a direita, inverte a direção X para a esquerda
 *   - Indica que ocorreu uma colisão e sai do loop
 * Se não houver colisão, atualiza a posição da bola para a próxima posição.
 * 
 * --------------------------------------------------------------
 */
void detectarColisao()
{
    int i;
    int raio = 5;
    int colisao = 0; 

    // Calcula a próxima posição da bola
    int proximoX = xInicialBola + velocidadePadraoBolaX;
    int proximoY = yInicialBola + velocidadePadraoBolaY;

    // Verifica colisão com cada bloco
    for (i = 0; i < NUM_BLOCOS_X * NUM_BLOCOS_Y; i++)
    {
        if (!blocos[i].destruido)
        {
            int bordaEsquerda = blocos[i].x;
            int bordaDireita = blocos[i].x + blocos[i].largura;
            int bordaCima = blocos[i].y;
            int bordaBaixo = blocos[i].y + blocos[i].altura;

           
            if (proximoX + raio >= bordaEsquerda && proximoX - raio <= bordaDireita &&
                proximoY + raio >= bordaCima && proximoY - raio <= bordaBaixo)
            {
                // bloco como destruído
                blocos[i].destruido = 1;

                
                if (velocidadePadraoBolaY < 0) //cima
                {
                    velocidadePadraoBolaY = -velocidadePadraoBolaY; 
                }
                else if (velocidadePadraoBolaY > 0) // baixo
                {
                    velocidadePadraoBolaY = -velocidadePadraoBolaY; 
                }
                else if (velocidadePadraoBolaX < 0) //  esquerda
                {
                    velocidadePadraoBolaX = -velocidadePadraoBolaX; 
                }
                else if (velocidadePadraoBolaX > 0) //direita
                {
                    velocidadePadraoBolaX = -velocidadePadraoBolaX; // Inverter direção X da bola
                }

                colisao = 1; // Indica que ocorreu uma colisão 
                break;       // Sai do loop após uma colisão
            }
        }
    }

    // Se não houve colisão atualiza a posição da bola normalmente
    if (!colisao)
    {
        xInicialBola = proximoX;
        yInicialBola = proximoY;
    }
}


/** 
 * --------------------------------------------------------------
 * 
 * @brief Função para detectar colisão com a raquete
 * 
 * Esta função verifica se a bola ta batendo com a raquete, se tiver
 * realiza mudança na direção e velocidade da bola de acordo na 
 * posição em que a colisão ocorreu.
 * 
 * --------------------------------------------------------------
 */
void detectarColisaoRaquete()
{
    int raqueteEsquerda = posicaoRaqueteX;
    int raqueteDireita = posicaoRaqueteX + 40;
    int raqueteTopo = 240 - 5;
    int raqueteBase = 240;
    int bolaEsquerda = xInicialBola;
    int bolaDireita = xInicialBola + 10;
    int bolaTopo = yInicialBola;
    int bolaBase = yInicialBola + 10;
    int deslocamento = 0;

    // aqui acontece as verificações para olhar se a bola ta colindindo com a raquete.
    if ((bolaDireita >= raqueteEsquerda && bolaEsquerda <= raqueteDireita) &&
        (bolaBase >= raqueteTopo && bolaTopo <= raqueteBase))
    {

        // centro da raquete
        int centroRaquete = posicaoRaqueteX + 20;

        // deslocamento relacionado ao centro da bola
        deslocamento = (xInicialBola + 5) - centroRaquete;

        // Ajusta a velocidade x
        velocidadeX = deslocamento / 8;

        // Limita a velocidade x
        if (velocidadeX > 2)
        {
            velocidadeX = 2;
        }
        if (velocidadeX < -2)
        {
            velocidadeX = -2;
        }
        if (velocidadeX == 0)
        {
            velocidadeX = 2;
        }

        // Inverte a direção 
        velocidadePadraoBolaY = -velocidadePadraoBolaY;

        // Atribui a velocidade x
        velocidadePadraoBolaX = velocidadeX;
    }
}



/** 
 * --------------------------------------------------------------
 * @brief Função para realizar a movimentação da bola
 * 
 * Esta função serve para poder realizar a detecção de colisão da
 * bola com as bordas superior, e laterais do jogo. Ao detectar a
 * a colisão é feita a inversão do sentido da bola. Além disso ela
 * realiza a movimentação da bola, alterando a sua posição, e por fim
 * realiza a chamada da função, passando o valor de X e de Y, para
 * desenhar a bola na tela.
 * @param Colisão: valor do tipo inteiro para informar a ocorrencia
 * de uma colisão ou não, os valores aceitos são 0 ou 1, para afirmar
 * ou negar a informação.
 * 
 * --------------------------------------------------------------
 */
void moverBola(int colisao)
{
    // velocidade atual
    xInicialBola += velocidadePadraoBolaX;
    yInicialBola += velocidadePadraoBolaY;

    // CORRIGIR COLISAO COM AS LATERAIS

    if (xInicialBola - 5 < 7 || xInicialBola + 5 > 313)
    {
        velocidadePadraoBolaX = -velocidadePadraoBolaX; // Inverte a direção horizontal
    }

    // Verifica se a bola atingiu as bordas superior e inferior da tela
    if (yInicialBola - 5 < 31 || yInicialBola + 5 > 300)
    {
        velocidadePadraoBolaY = -velocidadePadraoBolaY; // Inverte a direção vertical
    }

    if (colisao)
    {
        velocidadePadraoBolaY = -velocidadePadraoBolaY;
    }

    desenhar_bola(xInicialBola, yInicialBola);
}



/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para realizar a movimentação da raquete de acordo
 * com o valor lido pelo acelerometro, movimentando ela seja para o lado
 * direito, seja para o lado esquerdo. Para realizar isso ela requisita a
 * leitura do valor capturado pelo acelerometro, esse valor afim de melhor
 * jogabilidade é feita a divisão por 10, para usar apenas 10% do valor lido,
 * e então é somado com o valor da posição da raquete, e logo após é gerada ela
 * utilizando para isso a função "video_box".
 * Há verificação para que a raquete não ultrapasse os limites da tela, tanto
 * o esquero como o direito.
 * 
 * --------------------------------------------------------------
 */
void moverRaquete()
{

    int y1 = 240;
    int ptr_x;
    int ptr_y;
    int ptr_z;
    int ptr_ready;
    int ptr_tap;
    int ptr_dtap;
    int ptr_msg;

    // Não sei exatamente se pode colocar null, caso não possa é só
    // adicionar as outras variaveis indicadas.
    accel_read(&ptr_ready, &ptr_tap, &ptr_dtap, &ptr_x, &ptr_y, &ptr_z, &ptr_msg);

    // melhorar essa questão do y1 para melhorar os valores
    video_box(posicaoRaqueteX, y1 - 10, posicaoRaqueteX + 40, y1 - 7, 0xFFFF);
    posicaoRaqueteX += ptr_x / 10;

    /*os valores tem que considerar que ta falando da posição inicial, e
    ainda vai ser somado os 40px que é para fazer a largura da raquete
    */
    /*
    tela tem largura de 320px e raquete largura de 40px, logo
    posicaoRaqueteX so pode ir ate 319(tamanho util da tela) - 40 - 5(largura da borda)
    e valor minimo da raquete tem de ser 1(inicio da tela) + 5 (tam da borda) + 2 (espaçamento)
    */
    if (posicaoRaqueteX > 272)
    {
        posicaoRaqueteX = 272;
    }
    else if (posicaoRaqueteX < 8)
    {
        posicaoRaqueteX = 8;
    }
}


/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para iniciar uma rotina de pause do jogo, em que
 * é iniciado um loop condicional, para esperar pela mudança do valor
 * obtido da leitura dos botões, usando a função "KEY_read", ou seja
 * quando o botão de pause/despause for pressionado, ele sai do loop,
 * e retorna para o fluxo normal do jogo novamente.
 * 
 * --------------------------------------------------------------
 */
void sairPause()
{
    int btn_data = 0;
    while (btn_data != 0b0100)
    {
        informacao_pause(1);
        KEY_read(&btn_data);
    }
}



/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para realizar a rotina de apresentar para o
 * jogador que ele perdeu, apresentando a tela de "Game Over".
 * A partir desse estado é possivel retornar e jogar novamente, para
 * isso alguns valores de variaveis são reiniciados ao seu valor inicial
 * para que possa ser jogado novamente.
 * Nesse estado para voltar ao jogo é aguardado o pressionar do botão, para
 * poder retornar.
 * @param histScore: uma variavel do tipo int que irá servir para poder
 * apresentar na tela o valor de pontuação feito naquele momento em que perdeu
 * 
 * --------------------------------------------------------------
 */
void sairTelaPerder(int histScore)
{
    int btn_data = 0;
    int contador = 0;
    char *info = "press button 1 to return";
    xInicialBola = 155;
    yInicialBola = 135;
    posicaoRaqueteX = 155;
    // velocidadeX = 0;
    velocidadePadraoBolaY = 1;
    velocidadePadraoBolaX = 1;
    limparTela();
    printTelaPerdeu();
    palavra_score(histScore, 82, 166);
    video_show();
    inicializarBlocos();
    while (btn_data != 0b1000)
    {
        KEY_read(&btn_data);
        if (contador < 1000){
            contador++;
        }
        else{
            video_text(28, 30, info);
        }
    }
}


/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para apresentar a tela de que o jogador ganhou
 * ao ter quebrado todos os blocos, desejando parabens a ele. Além]
 * disso a partir desse estado é possivel retornar e jogar novamente,
 * para isso alguns valores de variaveis são reiniciados ao seu valor
 * inicial para que possa ser jogado novamente.
 * 
 * --------------------------------------------------------------
 */
void sairTelaParabens(){
    limparTela();
    printTelaParabens();
    video_show();
    inicializarBlocos();
    xInicialBola = 155;
    yInicialBola = 135;
    posicaoRaqueteX = 155;
    int btn_data = 0;
    int contador = 0;
    char *info = "press button 1 to return";
    while (btn_data != 0b1000){
        KEY_read(&btn_data);
        if (contador < 1000){
            contador++;
        }
        else{
            video_text(28, 30, info);
        }
    }
}


/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para informar com um texto na tela, se o jogo esta pausado
 * caso não esteja nenhuma informação é apresentada.
 * @param boolean: uma variavel do tipo int que irá servir para poder
 * para indicar se o jogo está em pause ou não, usando ela em verificação num "if"
 * logo, seus valores podem ser 1(True) ou 0(False)
 * 
 * --------------------------------------------------------------
 */
void informacao_pause(int boolean)
{
    char *pause = "pause";
    char *play = "play";

    if (boolean)
    {
        video_text(quantColunasTexto - 6, 3, pause);
    }

}


/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para desenhar na tela uma bola especial. Para
 * isso é utilizada uma estrutura de duplo "for", e desenhado pixel
 * a pixel da bola, a fim de apresentar cores diversas na bola.
 * @param centroX: variavel do tipo "int", que serve para indicar a posição
 * X inicial em que a bola deverá iniciar a ser renderizada.
 * @param centroY: variavel do tipo "int", que serve para indicar a posição
 * Y inicial em que a bola deverá iniciar a ser renderizada. 
 * 
 * --------------------------------------------------------------
 */
void desenhar_bola(int centroX, int centroY)
{
    int raio = 5; // Defina o raio da bola
    int x, y;

    for (y = -raio; y <= raio; y++)
    {
        for (x = -raio; x <= raio; x++)
        {

            if (x * x + y * y <= raio * raio)
            {
                int pixelX = centroX + x;
                int pixelY = centroY + y;

                int distSquared = x * x + y * y;

                float aproxSqrt = 1.0 - (float)distSquared / (raio * raio);

                if (aproxSqrt < 0)
                {
                    aproxSqrt = 0;
                }
                else if (aproxSqrt > 1)
                {
                    aproxSqrt = 1;
                }

                int cor = (int)(aproxSqrt * 0xFFFF); // Branco diferente

                // Desenhar o pixel com a cor
                if (pixelX >= 0 && pixelX < screen_x && pixelY >= 0 && pixelY < screen_y)
                {
                    video_pixel(pixelX, pixelY, cor);
                }
            }
        }
    }
}



/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos blocos
 * 
 * Esta função serve para simplificar o procedimento de fechar a porta
 * dos hardwares utilizados no jogo. Logo com a chamada de apenas 1 função
 * já fecha os 3 hardwares, o do acelerometro, o do video, e o dos botões.
 * 
 * --------------------------------------------------------------
 */
void fechar_hardwares(){
    accel_close();
    video_close();
    KEY_close();
}



/** 
 * --------------------------------------------------------------
 * @brief Função para inicialização dos dispositivos de hardware
 * 
 * Esta função serve para abrir a porta dos hardwares que serão utilizados
 * no funcionamento do jogo, sendo eles o acelerometro, o video, e o os botões.
 * Caso durante a abertura de um desses elementos haja o retorno de erro, é
 * apresentado na tela do terminal um print informando o erro.
 * Ainda é acertado alguns valores do acelerometro, como feita a sua calibração,
 * e formato de leitura.
 * @return Inteiro informando que não possivel iniciar algum dos hardwares, caso
 * isso aconteça
 * 
 * --------------------------------------------------------------
 */
int iniciar_hardwares(){
    int larguraTela, alturaTela, char_x, char_y;
    int resolucao = 1;
    int range = 16;

    if (!accel_open() || !video_open() || !KEY_open())
    {
        return 0;
    }
    accel_init();
    video_read(&larguraTela, &alturaTela, &char_x, &char_y); // get screen & text size
    accel_format(resolucao, range);
    accel_calibrate();
    quantColunasTexto = char_x;
    quantLinhasTexto = char_y;
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar a tela inicial
 * 
 * Esta função serve para imprimir na tela a tela inicial, a tela com
 * o menu do jogo, contendo o nome do jogo e opção para play e quit.
 * 
 * --------------------------------------------------------------
 */
void printTelaInicial(){

    /*
    24 blocos para letras
    8 letras
    3 blocos por letra
    7 blocos de espaçamento
    */
    int corL1;
    int corL2;
    int corL3;
    int corL4;
    int corL5;

    /*
    0xf810
    oxb1db
    0xd016
    0xb01a
    0x901f


    */

    corL1 = 0xf810;
    corL2 = 0xe014;
    corL3 = 0xd016;
    corL4 = 0xb01a;
    corL5 = 0x901f;

    // x1, y1, x2, y2
    // começa x
    // começa y
    // termina x
    // termina y

    // NOME BREAKOUT DA TELA QUE AMANDA FEZ A FOTO
    // PRIMEIRA LINHA 15 BLOCOS #ff004a
    video_box(15, 5, 23, 11, corL1); // 1
    video_box(25, 5, 33, 11, corL1); // 2
    // video_box(35, 5, 43, 11, corL1); //3
    // video_box(45, 5, 53, 11, corL1); //4
    video_box(55, 5, 63, 11, corL1); // 5
    video_box(65, 5, 73, 11, corL1); // 6
    // video_box(75, 5, 83, 11, corL1); //7
    // video_box(85, 5, 93, 11, corL1); //8
    // video_box(95, 5, 103, 11, corL1); //9
    video_box(105, 5, 113, 11, corL1); // 10
    video_box(115, 5, 123, 11, corL1); // 11
    // video_box(125, 5, 133 11, corL1); //12
    // video_box(135, 5, 143, 11, corL1); //13
    video_box(145, 5, 153, 11, corL1); // 14
    // video_box(155, 5, 163, 11, corL1); //15
    // video_box(165, 5, 173, 11, corL1); //16
    video_box(175, 5, 183, 11, corL1); // 17
    // video_box(185, 5, 193, 11, corL1); //18
    video_box(195, 5, 203, 11, corL1); // 19
    // video_box(205, 5, 213, 11, corL1); //20
    // video_box(215, 5, 223, 11, corL1); //21
    video_box(225, 5, 233, 11, corL1); // 22
    // video_box(235, 5, 243, 11, corL1); //23
    // video_box(245, 5, 253, 11, corL1); //24
    video_box(255, 5, 263, 11, corL1); // 25
    // video_box(265, 5, 273, 11, corL1); //26
    video_box(275, 5, 283, 11, corL1); // 27
    video_box(285, 5, 293, 11, corL1); // 28
    video_box(295, 5, 303, 11, corL1); // 29
    video_box(305, 5, 313, 11, corL1); // 30

    // SEGUNDA LINHA 14 BLOCOS LARANJAS
    video_box(15, 13, 23, 19, corL2); // 1
    // video_box(25, 13, 33, 19, 0xF809); //2
    video_box(35, 13, 43, 19, corL2); // 3
    // video_box(45, 13, 53, 19, 0xF809); //4
    video_box(55, 13, 63, 19, corL2); // 5
    // video_box(65, 13, 73, 19, 0xF809); //6
    video_box(75, 13, 83, 19, corL2); // 7
    // video_box(85, 13, 93, 19, 0xF809); //8
    video_box(95, 13, 103, 19, corL2); // 9
    // video_box(105, 13, 113, 19, 0xF809); //10
    // video_box(115, 13, 123, 19, 0xF809); //11
    // video_box(125, 13, 133 19, 0xF809); //12
    video_box(135, 13, 143, 19, corL2); // 13
    // video_box(145, 13, 153, 19, 0xF809); //14
    video_box(155, 13, 163, 19, corL2); // 15
    // video_box(165, 13, 173, 19, 0xF809); //16
    video_box(175, 13, 183, 19, corL2); // 17
    video_box(188, 13, 196, 19, corL2); // 18 - deslocar para esquerda - V
    // video_box(195, 13, 203, 19, 0xF809); //19
    // video_box(205, 13, 213, 19, 0xF809); //20
    video_box(215, 13, 223, 19, corL2); // 21
    // video_box(225, 13, 233, 19, 0xF809); //22
    video_box(235, 13, 243, 19, corL2); // 23
    // video_box(245, 13, 253, 19, 0xF809); //24
    video_box(255, 13, 263, 19, corL2); // 25
    // video_box(265, 13, 273, 19, 0xF809); //26
    video_box(275, 13, 283, 19, corL2); // 27
    // video_box(285, 13, 293, 19, 0xF809); //28
    video_box(295, 13, 303, 19, corL2); // 29
    // video_box(305, 13, 313, 19, 0xF809); //30

    // TERCEIRA LINHA 15 BLOCOS AMARELOS
    video_box(15, 21, 23, 27, corL3); // 1
    video_box(25, 21, 33, 27, corL3); // 2
    // video_box(35, 21, 43, 27, 0xF809); //3
    // video_box(45, 21, 53, 27, 0xF809); //4
    video_box(55, 21, 63, 27, corL3); // 5
    video_box(65, 21, 73, 27, corL3); // 6
    // video_box(75, 21, 83, 27, 0xF809); //7
    // video_box(85, 21, 93, 27, 0xF809); //8
    video_box(95, 21, 103, 27, corL3);  // 9
    video_box(105, 21, 113, 27, corL3); // 10
    // video_box(115, 21, 123, 27, 0xF809); //11
    // video_box(125, 21, 133 27, 0xF809); //12
    video_box(135, 21, 143, 27, corL3); // 13
    video_box(145, 21, 153, 27, corL3); // 14
    video_box(155, 21, 163, 27, corL3); // 15
    // video_box(165, 21, 173, 27, 0xF809); //16
    video_box(175, 21, 183, 27, corL3); // 17
    video_box(185, 21, 193, 27, corL3); // 18
    // video_box(195, 21, 203, 27, 0xF809); //19
    // video_box(205, 21, 213, 27, 0xF809); //20
    video_box(215, 21, 223, 27, corL3); // 21
    // video_box(225, 21, 233, 27, 0xF809); //22
    video_box(235, 21, 243, 27, corL3); // 23
    // video_box(245, 21, 253, 27, 0xF809); //24
    video_box(255, 21, 263, 27, corL3); // 25
    // video_box(265, 21, 273, 27, 0xF809); //26
    video_box(275, 21, 283, 27, corL3); // 27
    // video_box(285, 21, 293, 27, 0xF809); //28
    video_box(295, 21, 303, 27, corL3); // 29
    // video_box(305, 21, 313, 27, 0xF809); //30

    // QUARTA LINHA 14 BLOCOS VERMELHOS
    video_box(15, 29, 23, 35, corL4); // 1
    // video_box(25, 29, 33, 35, corL4); //2
    video_box(35, 29, 43, 35, corL4); // 3
    // video_box(45, 29, 53, 35, corL4); //4
    video_box(55, 29, 63, 35, corL4); // 5
    // video_box(65, 29, 73, 35, corL4); //6
    video_box(75, 29, 83, 35, corL4); // 7
    // video_box(85, 29, 93, 35, corL4); //8
    video_box(95, 29, 103, 35, corL4); // 9
    // video_box(105, 29, 113, 35, corL4); //10
    // video_box(115, 29, 123, 35, corL4); //11
    // video_box(125, 29, 133, 35, corL4); //12
    video_box(135, 29, 143, 35, corL4); // 13
    // video_box(145, 29, 153, 35, corL4); //14
    video_box(155, 29, 163, 35, corL4); // 15
    // video_box(165, 29, 173, 35, corL4); //16
    video_box(175, 29, 183, 35, corL4); // 17
    video_box(188, 29, 196, 35, corL4); // 18 - deslocar a esquerda - V
    // video_box(195, 29, 203, 35, corL4); //19
    // video_box(205, 29, 213, 35, corL4); //20
    video_box(215, 29, 223, 35, corL4); // 21
    // video_box(225, 29, 233, 35, corL4); //22
    video_box(235, 29, 243, 35, corL4); // 23
    // video_box(245, 29, 253, 35, corL4); //24
    video_box(255, 29, 263, 35, corL4); // 25
    // video_box(265, 29, 273, 35, corL4); //26
    video_box(275, 29, 283, 35, corL4); // 27
    // video_box(285, 29, 293, 35, corL4); //28
    video_box(295, 29, 303, 35, corL4); // 29
    // video_box(305, 29, 313, 35, corL4); //30

    // QUINTA LINHA 15 BLOCOS VERMELHOS
    video_box(15, 37, 23, 43, corL5); // 1
    video_box(25, 37, 33, 43, corL5); // 2
    // video_box(35, 37, 43, 43, corL5); //3
    // video_box(45, 37, 53, 43, corL5); //4
    video_box(55, 37, 63, 43, corL5); // 5
    // video_box(65, 37, 73, 43, corL5); //6
    video_box(75, 37, 83, 43, corL5); // 7
    // video_box(85, 37, 93, 43, corL5); //8
    // video_box(95, 37, 103, 43, corL5); //9
    video_box(105, 37, 113, 43, corL5); // 10
    video_box(115, 37, 123, 43, corL5); // 11
    // video_box(125, 37, 133, 43, corL5); //12
    video_box(135, 37, 143, 43, corL5); // 13
    // video_box(145, 37, 153, 43, corL5); //14
    video_box(155, 37, 163, 43, corL5); // 15
    // video_box(165, 37, 173, 43, corL5); //16
    video_box(175, 37, 183, 43, corL5); // 17
    // video_box(185, 37, 193, 43, corL5); //18
    video_box(195, 37, 203, 43, corL5); // 19
    // video_box(205, 37, 213, 43, corL5); //20
    // video_box(215, 37, 223, 43, corL5); //21
    video_box(225, 37, 233, 43, corL5); // 22
    // video_box(235, 37, 243, 43, corL5); //23
    // video_box(245, 37, 253, 43, corL5); //24
    // video_box(255, 37, 263, 43, corL5); //25
    video_box(265, 37, 273, 43, corL5); // 26
    // video_box(275, 37, 283, 43, corL5); //27
    // video_box(285, 37, 293, 43, corL5); //28
    video_box(295, 37, 303, 43, corL5); // 29
    // video_box(305, 37, 313, 43, corL5); //30

    // PALAVRAS PLAY E QUIT
    // x1, y1, x2, y2
    // começa x
    // começa y
    // termina x
    // termina y
    int x_center = 30;
    // LETRA P
    video_box(x_center + 93, 130, x_center + 95, 144, 0xFFFF); // traço de cima do P
    video_box(x_center + 96, 130, x_center + 104, 133, 0xFFFF);
    video_box(x_center + 105, 130, x_center + 107, 139, 0xFFFF);
    video_box(x_center + 96, 137, x_center + 104, 139, 0xFFFF);

    // LETRA L
    video_box(x_center + 110, 130, x_center + 112, 144, 0xFFFF);
    video_box(x_center + 112, 142, x_center + 124, 144, 0xFFFF);

    // LETRA A
    video_box(x_center + 127, 130, x_center + 129, 144, 0xFFFF); // traço de cima do P
    video_box(x_center + 130, 130, x_center + 138, 133, 0xFFFF);
    video_box(x_center + 139, 130, x_center + 141, 144, 0xFFFF);
    video_box(x_center + 130, 137, x_center + 138, 139, 0xFFFF);

    // LETRA Y
    video_box(x_center + 144, 130, x_center + 146, 132, 0xFFFF); // traço de cima do P
    video_box(x_center + 146, 133, x_center + 148, 135, 0xFFFF);
    video_box(x_center + 148, 136, x_center + 154, 138, 0xFFFF);
    video_box(x_center + 154, 133, x_center + 156, 135, 0xFFFF);
    video_box(x_center + 156, 130, x_center + 158, 132, 0xFFFF);
    video_box(x_center + 150, 139, x_center + 152, 144, 0xFFFF);

    // LETRA Q
    video_box(x_center + 101, 152, x_center + 103, 166, 0xFFFF); // traço deitado pequeno do
    video_box(x_center + 104, 152, x_center + 112, 154, 0xFFFF);
    video_box(x_center + 113, 152, x_center + 115, 166, 0xFFFF);
    video_box(x_center + 104, 164, x_center + 112, 166, 0xFFFF);
    video_box(x_center + 107, 167, x_center + 109, 168, 0xFFFF);
    video_box(x_center + 110, 169, x_center + 113, 170, 0xFFFF);

    // LETRA U
    video_box(x_center + 118, 152, x_center + 120, 166, 0xFFFF);
    video_box(x_center + 121, 164, x_center + 129, 166, 0xFFFF);
    video_box(x_center + 130, 152, x_center + 132, 166, 0xFFFF);

    // LETRA I
    video_box(x_center + 135, 152, x_center + 137, 166, 0xFFFF);

    // LETRA T
    video_box(x_center + 140, 152, x_center + 150, 154, 0xFFFF);
    video_box(x_center + 144, 155, x_center + 146, 166, 0xFFFF);

    printBlocosInferiores();
}



/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar a tela de Parabens
 * 
 * Esta função serve para desenhar na tela de parabens, que surge logo
 * após o jogador conseguir quebrar todos os blocos.
 * Ela desenha a palavra Parabens, assim como faz chamada para a função
 * que desenha alguns blocos decorativos ao final da tela.
 * 
 * --------------------------------------------------------------
 */
void printTelaParabens()
{

   int y_desloc =30;
   int x_desloc = 20;
    int cor1 = 0x81d6;
    int cor2 = 0xb96f;
    int cor3 = 0xd92c;
    int cor4 = 0xf8e9;
    int cor5 = 0xf8ab;
    int cor6 = 0xf86d;
    int cor7 = 0xf84e;
    int cor8 = 0xf810;
    video_box(x_desloc + 6, y_desloc + 1, x_desloc + 10, y_desloc + 30, cor1);
    video_box(x_desloc + 11, y_desloc + 1, x_desloc + 35, y_desloc + 5, cor1);
    video_box(x_desloc + 11, y_desloc + 26, x_desloc + 35, y_desloc + 30, cor1);


    video_box(x_desloc + 38, y_desloc + 1, x_desloc + 42, y_desloc + 30, cor2);
    video_box(x_desloc + 43, y_desloc + 1, x_desloc + 63, y_desloc + 5, cor2);
    video_box(x_desloc + 43, y_desloc + 26, x_desloc + 63, y_desloc + 30, cor2);
    video_box(x_desloc + 64, y_desloc + 1, x_desloc + 68, y_desloc + 30, cor2);

    video_box(x_desloc + 71, y_desloc + 1, x_desloc + 75, y_desloc + 30, cor3);
    video_box(x_desloc + 76, y_desloc + 2, x_desloc + 82, y_desloc + 11, cor3);
    video_box(x_desloc + 83, y_desloc + 12, x_desloc + 89, y_desloc + 21, cor3);
    video_box(x_desloc + 89, y_desloc + 22, x_desloc + 96, y_desloc + 28, cor3);
    video_box(x_desloc + 97, y_desloc + 1, x_desloc + 101, y_desloc + 30, cor3);

    video_box(x_desloc + 104, y_desloc + 1, x_desloc + 108, y_desloc + 30, cor4);
    video_box(x_desloc + 109, y_desloc + 1, x_desloc + 129, y_desloc + 5, cor4);
    video_box(x_desloc + 109, y_desloc + 26, x_desloc + 129, y_desloc + 30, cor4);
    video_box(x_desloc + 121, y_desloc + 15, x_desloc + 129, y_desloc + 19, cor4);
    video_box(x_desloc + 130, y_desloc + 15, x_desloc + 134, y_desloc + 30, cor4);

    video_box(x_desloc + 137, y_desloc + 1, x_desloc + 141, y_desloc + 30, cor5);
    video_box(x_desloc + 142, y_desloc + 1, x_desloc + 162, y_desloc + 5, cor5);
    video_box(x_desloc + 163, y_desloc + 1, x_desloc + 167, y_desloc + 18, cor5);
    video_box(x_desloc + 142, y_desloc + 14, x_desloc + 162, y_desloc + 18, cor5);
    video_box(x_desloc + 142, y_desloc + 19, x_desloc + 151, y_desloc + 22, cor5);
    video_box(x_desloc + 151, y_desloc + 23, x_desloc + 160, y_desloc + 26, cor5);
    video_box(x_desloc + 161, y_desloc + 27, x_desloc + 167, y_desloc + 30, cor5);

    video_box(x_desloc + 170, y_desloc + 1, x_desloc + 174, y_desloc + 30, cor6);
    video_box(x_desloc + 175, y_desloc + 1, x_desloc + 195, y_desloc + 5, cor6);
    video_box(x_desloc + 196, y_desloc + 1, x_desloc + 200, y_desloc + 30, cor6);
    video_box(x_desloc + 175, y_desloc + 19, x_desloc + 195, y_desloc + 23, cor6);

    video_box(x_desloc + 203, y_desloc + 1, x_desloc + 233, y_desloc + 5, cor7);
    video_box(x_desloc + 216, y_desloc + 6, x_desloc + 220, y_desloc + 30, cor7);

    video_box(x_desloc + 236, y_desloc + 1, x_desloc + 266, y_desloc + 5, cor8);
    video_box(x_desloc + 236, y_desloc + 6, x_desloc + 240, y_desloc + 12, cor8);
    video_box(x_desloc + 236, y_desloc + 13, x_desloc + 266, y_desloc + 17, cor8);
    video_box(x_desloc + 262, y_desloc + 18, x_desloc + 266, y_desloc + 25, cor8);
    video_box(x_desloc + 236, y_desloc + 26, x_desloc + 266, y_desloc + 30, cor8);

    video_box(x_desloc + 269, y_desloc + 1, x_desloc + 274, y_desloc + 24, cor8);
    video_box(x_desloc + 269, y_desloc + 27, x_desloc + 274, y_desloc + 30, cor8);

    printBlocosInferiores();
}


/** 
 * --------------------------------------------------------------
 * @brief Função para funcionamento do "menu inicial"
 * 
 * Esta função serve para apresentar a tela inicial do jogo, e fazendo
 * a logica de funcionamento, fazendo a chamada de função para desenhar
 * a tela inicial, e aguardando em loop o pressionar do botão para saber
 * qual a proxima operação a ser realizada, seja ela sair do jogo, ou
 * ir para o jogo.
 * 
 * @return Retorno: variavel do tipo inteiro que indica se jogador apertou
 * para sair do jogo, ou para jogar, ou seja se deu play, ou quit
 * 
 * --------------------------------------------------------------
 */
int telaInicial()
{
    int aux_while = 1;
    int btn_data, retorno;
    while (aux_while){
        KEY_read(&btn_data);
        limparTela();

        // construir tela de inicio para apresentar antes de ir ao jogo
        printTelaInicial();
        video_show(); // swap Front/Back to display the line

        switch (btn_data){
        case 0b1000:       // botão de PLAY pressionado, o botão mais a esquerda
            aux_while = 0; // para poder sair do while e seguir para o jogo
            retorno = 1;
            break;
        case 0b0001: // botao de SAIR pressionado, o botão mais a direita
            aux_while = 0;
            retorno = 0;
            break;

        default:
            aux_while = 1;
            break;
        }
    }
    return retorno;
}



/** 
 * --------------------------------------------------------------
 * @brief Função para limpar a tela
 * 
 * Esta função serve para simplificar a limpeza de informações na tela
 * possibilitando com apenas uma chamada de função limpar tanto textos na tela
 * como limpar os outros pixels desenhados na tela.
 * 
 * --------------------------------------------------------------
 */
void limparTela()
{
    video_erase(); // erase any text on the screen
    video_clear(); // clear current VGA Back buffer
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar as bordas do jogo
 * 
 * Esta função serve para desenhar na tela as bordas do jogo, usando a logica
 * de criar uma caixa e logo após criar outra caixa menor, deixando a primeira
 * aparecer apenas uma parte, que é a borda.
 * 
 * --------------------------------------------------------------
 */
void gerar_bordas()
{
    int larguraTela, alturaTela, char_x, char_y;
    video_read(&larguraTela, &alturaTela, &char_x, &char_y);

    video_box(1, 25, larguraTela - 1, alturaTela - 1, 0x8430);
    video_box(6, 31, larguraTela - 6, alturaTela - 6, 0x0000);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar na tela a informação sobre a pontuação
 * 
 * Esta função serve para escrever na tela a palavra "SCORE" de forma
 * personalizada, além de chamar outra função que apresenta o valor do
 * score na mesma formatação utilizada na palavra "SCORE".
 * Para melhor funcionamento e reuso, a função contém variaveis para 
 * permitir o deslocamento dos pixels para onde desejar dentro da tela.
 * 
 * @param scoreInt: parametro do tipo inteiro que contém o valor do score obtido
 * ate o momento pelo jogador. Esse valor irá ser passado para a função que
 * escreve o valor numerico na tela.
 * @param shift_X: parametro do tipo inteiro que contém o valor de quando o pixel
 * deve ser deslocado para ser apresentado na tela no eixo X, quando não for
 * necessario o deslocamento, usa-se o valor 0;
 * @param shift_Y: parametro do tipo inteiro que contém o valor de quando o pixel
 * deve ser deslocado para ser apresentado na tela no eixo Y, quando não for
 * necessario o deslocamento, usa-se o valor 0;
 * 
 * --------------------------------------------------------------
 */
void palavra_score(int scoreInt, int shift_X, int shift_Y)
{
    // x1, y1, x2, y2
    // começa x
    // começa y
    // termina x
    // termina y

    // S
    video_box(8 + shift_X, 3 + shift_Y, 22 + shift_X, 5 + shift_Y, 0xFFFF);    // perna alta do S
    video_box(8 + shift_X, 6 + shift_Y, 10 + shift_X, 8 + shift_Y, 0xFFFF);    // perna comprima do S
    video_box(8 + shift_X, 9 + shift_Y, 22 + shift_X, 11 + shift_Y, 0xFFFF);   // perna meio do S
    video_box(20 + shift_X, 12 + shift_Y, 22 + shift_X, 14 + shift_Y, 0xFFFF); // perna comprima do S
    video_box(8 + shift_X, 15 + shift_Y, 22 + shift_X, 17 + shift_Y, 0xFFFF);  // perna alta do S

    // C
    video_box(25 + shift_X, 3 + shift_Y, 39 + shift_X, 5 + shift_Y, 0xFFFF);   // lateral superior do C
    video_box(25 + shift_X, 6 + shift_Y, 27 + shift_X, 14 + shift_Y, 0xFFFF);  // perna comprida do C
    video_box(25 + shift_X, 15 + shift_Y, 39 + shift_X, 17 + shift_Y, 0xFFFF); // lateral inferior do C

    // O
    video_box(42 + shift_X, 3 + shift_Y, 56 + shift_X, 5 + shift_Y, 0xFFFF);   // lateral superior do O
    video_box(42 + shift_X, 6 + shift_Y, 44 + shift_X, 14 + shift_Y, 0xFFFF);  // lateral direita do O
    video_box(54 + shift_X, 6 + shift_Y, 56 + shift_X, 14 + shift_Y, 0xFFFF);  // lateral esquerda do O
    video_box(42 + shift_X, 15 + shift_Y, 56 + shift_X, 17 + shift_Y, 0xFFFF); // lateral inferior do O

    // R
    video_box(59 + shift_X, 3 + shift_Y, 73 + shift_X, 5 + shift_Y, 0xFFFF);   // parte de cima do R
    video_box(59 + shift_X, 6 + shift_Y, 61 + shift_X, 17 + shift_Y, 0xFFFF);  // lateral direita do R
    video_box(71 + shift_X, 6 + shift_Y, 73 + shift_X, 9 + shift_Y, 0xFFFF);   // parte de cima do R
    video_box(62 + shift_X, 10 + shift_Y, 73 + shift_X, 12 + shift_Y, 0xFFFF); // parte de cima do R
    video_box(62 + shift_X, 13 + shift_Y, 67 + shift_X, 15 + shift_Y, 0xFFFF);
    video_box(68 + shift_X, 15 + shift_Y, 73 + shift_X, 17 + shift_Y, 0xFFFF);

    // E
    video_box(76 + shift_X, 3 + shift_Y, 90 + shift_X, 5 + shift_Y, 0xFFFF);   // lateral superior do E
    video_box(76 + shift_X, 6 + shift_Y, 78 + shift_X, 14 + shift_Y, 0xFFFF);  // lateral meio do E
    video_box(76 + shift_X, 15 + shift_Y, 90 + shift_X, 17 + shift_Y, 0xFFFF); // lateral inferior do E
    video_box(79 + shift_X, 9 + shift_Y, 87 + shift_X, 11 + shift_Y, 0xFFFF);  // lateral direita do E

    // dois pontos
    video_box(93 + shift_X, 4 + shift_Y, 95 + shift_X, 7 + shift_Y, 0xFFFF);   // ponto superior
    video_box(93 + shift_X, 13 + shift_Y, 95 + shift_X, 16 + shift_Y, 0xFFFF); // ponto inferior

    printValorScore(scoreInt, shift_X, (shift_Y - 1));

}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar a tela de "GAME OVER"
 * 
 * Esta função serve para desenhar na tela a palavra "GAME OVER", indicando
 * que o jogador perdeu, além disso também faz chamada da função para desenhar
 * blocos decorativos na parte inferior da tela
 * 
 * --------------------------------------------------------------
 */
void printTelaPerdeu()
{
    int corG;
    int corA;
    int corM;
    int corE;
    int corO;
    int corV;
    int corE2;
    int corR;

    int cor1;
    int cor2;
    int cor3;
    int cor4;
    int cor5;

    cor1 = 0xf810;
    cor2 = 0xe014;
    cor3 = 0xd016;
    cor4 = 0xb01a;
    cor5 = 0x901f;

    corG = cor1;
    corA = cor2;
    corM = cor3;
    corE = cor5;
    corO = cor4;
    corV = cor3;
    corE2 = cor2;
    corR = cor1;

    // PRIMEIRA LINHA 15 BLOCOS VERMELHOS
    // video_box(15, 5, 23, 11, corG); //1
    video_box(25, 5, 33, 11, corG); // 2
    video_box(35, 5, 43, 11, corG); // 3
    video_box(45, 5, 53, 11, corG); // 4
    video_box(55, 5, 63, 11, corG); // 5

    video_box(65, 5, 73, 11, corA); // 6
    video_box(75, 5, 83, 11, corA); // 7
    video_box(85, 5, 93, 11, corA); // 8

    video_box(95, 5, 103, 11, corM); // 9
    // video_box(105, 5, 113, 11, corM); //10
    // video_box(115, 5, 123, 11, corM); //11
    // video_box(125, 5, 133 11, corM); //12
    video_box(135, 5, 143, 11, corM); // 13

    video_box(145, 5, 153, 11, corE); // 14
    video_box(155, 5, 163, 11, corE); // 15
    video_box(165, 5, 173, 11, corE); // 16

    // video_box(175, 5, 183, 11, cor5); //17

    video_box(185, 5, 193, 11, corO); // 18
    video_box(195, 5, 203, 11, corO); // 19
    video_box(205, 5, 213, 11, corO); // 20

    video_box(215, 5, 223, 11, corV); // 21
    // video_box(225, 5, 233, 11, corV); //22
    video_box(235, 5, 243, 11, corV); // 23

    video_box(245, 5, 253, 11, corE2); // 24
    video_box(255, 5, 263, 11, corE2); // 25
    video_box(265, 5, 273, 11, corE2); // 26

    video_box(275, 5, 283, 11, corR); // 27
    video_box(285, 5, 293, 11, corR); // 28
    video_box(295, 5, 303, 11, corR); // 29
    // video_box(305, 5, 313, 11, corR); //30

    // SEGUNDA LINHA 14 BLOCOS LARANJAS
    // video_box(15, 13, 23, 19, corG); //1
    video_box(25, 13, 33, 19, corG); // 2
    // video_box(35, 13, 43, 19, corG); //3
    // video_box(45, 13, 53, 19, corG); //4
    // video_box(55, 13, 63, 19, corG); //5

    video_box(65, 13, 73, 19, corA); // 6
    // video_box(75, 13, 83, 19, corA); //7
    video_box(85, 13, 93, 19, corA); // 8

    video_box(95, 13, 103, 19, corM);  // 9
    video_box(105, 13, 113, 19, corM); // 10
    // video_box(115, 13, 123, 19, corM); //11
    video_box(125, 13, 133, 19, corM); // 12
    video_box(135, 13, 143, 19, corM); // 13

    video_box(145, 13, 153, 19, corE); // 14
    // video_box(155, 13, 163, 19, corE); //15
    // video_box(165, 13, 173, 19, corE); //16

    // video_box(175, 13, 183, 19, corO); //17
    video_box(185, 13, 193, 19, corO); // 18 - deslocar para esquerda - V
    // video_box(195, 13, 203, 19, corO); //19
    video_box(205, 13, 213, 19, corO); // 20

    video_box(215, 13, 223, 19, corV); // 21
    // video_box(225, 13, 233, 19, corV); //22
    video_box(235, 13, 243, 19, corV); // 23

    video_box(245, 13, 253, 19, corE2); // 24
    // video_box(255, 13, 263, 19, corE2); //25
    // video_box(265, 13, 273, 19, corE2); //26

    video_box(275, 13, 283, 19, corR); // 27
    // video_box(285, 13, 293, 19, corR); //28
    video_box(295, 13, 303, 19, corR); // 29
    // video_box(305, 13, 313, 19, corR); //30

    // TERCEIRA LINHA 15 BLOCOS AMARELOS
    // video_box(15, 21, 23, 27, corG); //1
    video_box(25, 21, 33, 27, corG); // 2
    // video_box(35, 21, 43, 27, corG); //3
    video_box(45, 21, 53, 27, corG); // 4
    video_box(55, 21, 63, 27, corG); // 5

    video_box(65, 21, 73, 27, corA); // 6
    // video_box(75, 21, 83, 27, corA); //7
    video_box(85, 21, 93, 27, corA); // 8

    video_box(95, 21, 103, 27, corM); // 9
    // video_box(105, 21, 113, 27, corM); //10
    video_box(115, 21, 123, 27, corM); // 11
    // video_box(125, 21, 133 27, corM); //12
    video_box(135, 21, 143, 27, corM); // 13

    video_box(145, 21, 153, 27, corE); // 14
    video_box(155, 21, 163, 27, corE); // 15
    // video_box(165, 21, 173, 27, corE); //16

    // video_box(175, 21, 183, 27, corO); //17
    video_box(185, 21, 193, 27, corO); // 18
    // video_box(195, 21, 203, 27, corO); //19
    video_box(205, 21, 213, 27, corO); // 20

    video_box(215, 21, 223, 27, corV); // 21
    // video_box(225, 21, 233, 27, corV); //22
    video_box(235, 21, 243, 27, corV); // 23

    video_box(245, 21, 253, 27, corE2); // 24
    video_box(255, 21, 263, 27, corE2); // 25
    // video_box(265, 21, 273, 27, corE2); //26

    video_box(275, 21, 283, 27, corR); // 27
    video_box(285, 21, 293, 27, corR); // 28
    video_box(295, 21, 303, 27, corR); // 29
    // video_box(305, 21, 313, 27, corR); //30

    // QUARTA LINHA 14 BLOCOS VERMELHOS
    // video_box(15,29, 23, 35, corG); //1
    video_box(25, 29, 33, 35, corG); // 2
    // video_box(35,29, 43, 35, corG); //3
    // video_box(45,29, 53, 35, corG); //4
    video_box(55, 29, 63, 35, corG); // 5

    video_box(65, 29, 73, 35, corA); // 6
    video_box(75, 29, 83, 35, corA); // 7
    video_box(85, 29, 93, 35, corA); // 8

    video_box(95, 29, 103, 35, corM); // 9
    // video_box(105,29, 113, 35, corM); //10
    // video_box(115,29, 123, 35, corM); //11
    // video_box(125,29, 133 35, corM); //12
    video_box(135, 29, 143, 35, corM); // 13

    video_box(145, 29, 153, 35, corE); // 14
    // video_box(155,29, 163, 35, corE); //15
    // video_box(165,29, 173, 35, corE); //16

    // video_box(175,29, 183, 35, corO); //17
    video_box(185, 29, 193, 35, corO); // 18 - deslocar a esquerda - V
    // video_box(195,29, 203, 35, corO); //19
    video_box(205, 29, 213, 35, corO); // 20

    video_box(215, 29, 223, 35, corV); // 21
    // video_box(225,29, 233, 35, corV); //22
    video_box(235, 29, 243, 35, corV); // 23

    video_box(245, 29, 253, 35, corE2); // 24
    // video_box(255,29, 263, 35, corE2); //25
    // video_box(265,29, 273, 35, corE2); //26

    video_box(275, 29, 283, 35, corR); // 27
    video_box(285, 29, 293, 35, corR); // 28
    // video_box(295,29, 303, 35, corR); //29
    // video_box(305,29, 313, 35, corR); //30

    // QUINTA LINHA 15 BLOCOS VERMELHOS
    // video_box(15, 37, 23, 43 corG); //1
    video_box(25, 37, 33, 43, corG); // 2
    video_box(35, 37, 43, 43, corG); // 3
    video_box(45, 37, 53, 43, corG); // 4
    video_box(55, 37, 63, 43, corG); // 5

    video_box(65, 37, 73, 43, corA); // 6
    // video_box(75, 37, 83, 43, corA); //7
    video_box(85, 37, 93, 43, corA); // 8

    video_box(95, 37, 103, 43, corM); // 9
    // video_box(105, 37, 113,43, corM); //10
    // video_box(115, 37, 123,43, corM); //11
    // video_box(125, 37, 133 43 corM); //12
    video_box(135, 37, 143, 43, corM); // 13

    video_box(145, 37, 153, 43, corE); // 14
    video_box(155, 37, 163, 43, corE); // 15
    video_box(165, 37, 173, 43, corE); // 16

    // video_box(175, 37, 183,43, corO); //17
    video_box(185, 37, 193, 43, corO); // 18
    video_box(195, 37, 203, 43, corO); // 19
    video_box(205, 37, 213, 43, corO); // 20

    // video_box(215, 37, 223,43, corV); //21
    video_box(225, 37, 233, 43, corV); // 22
    // video_box(235, 37, 243,43, corV); //23

    video_box(245, 37, 253, 43, corE2); // 24
    video_box(255, 37, 263, 43, corE2); // 25
    video_box(265, 37, 273, 43, corE2); // 26

    video_box(275, 37, 283, 43, corR); // 27
    // video_box(285, 37, 293,43, corR); //28
    video_box(295, 37, 303, 43, corR); // 29
    // video_box(305, 37, 313,43, corR); //30
    printBlocosInferiores();
}


/** 
 * --------------------------------------------------------------
 * @brief Função para calcular a pontuação
 * 
 * Esta função serve para fazer o calculo da pontuação, baseando-se na
 * quantidade de blocos que ja foram destruidos. Cada bloco equivale a
 * 10 pontos.
 * @return contador: uma variavel do tipo Int interna a função e que contabiliza
 *  o valor da pontuação
 * 
 * --------------------------------------------------------------
 */
int calcularScore()
{
    int contador = 0;
    int i = 0;
    for (i = 0; i < NUM_BLOCOS_X * NUM_BLOCOS_Y; i++)
    {
        if (blocos[i].destruido)
        {
            contador += 10;
        }
    }
    return contador;
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar na tela um conjunto de blocos
 * 
 * Esta função faz um desenho de um conjunto de blocos na tela, 
 * colocando cores para cada linha de bloco, afim de formar um degrade
 * de cores, indo de um roxo mais escuro, até um rosa.
 * 
 * --------------------------------------------------------------
 */
void printBlocosInferiores()
{
    // ULTIMA LINHA
    int cor1;
    int cor2;
    int cor3;
    int cor4;
    int cor5;

    cor5 = 0xf810;
    cor4 = 0xe014;
    cor3 = 0xd016;
    cor2 = 0xb01a;
    cor1 = 0x901f;

    video_box(4, 226, 27, 239, cor1);
    video_box(31, 226, 58, 239, cor1);
    video_box(62, 226, 89, 239, cor1);
    video_box(93, 226, 120, 239, cor1);
    video_box(124, 226, 151, 239, cor1);
    video_box(155, 226, 182, 239, cor1);
    video_box(186, 226, 213, 239, cor1);
    video_box(217, 226, 244, 239, cor1);
    video_box(248, 226, 275, 239, cor1);
    video_box(279, 226, 306, 239, cor1);

    // PENULTIMA LINHA
    video_box(4, 211, 27, 224, cor2);   // 1
    video_box(31, 211, 58, 224, cor2);  // 2
    video_box(62, 211, 89, 224, cor2);  // 3
    video_box(93, 211, 120, 224, cor2); // 4

    video_box(186, 211, 213, 224, cor2); // 7
    video_box(217, 211, 244, 224, cor2); // 8
    video_box(248, 211, 275, 224, cor2); // 9
    video_box(279, 211, 306, 224, cor2); // 10

    // ANTEPENULTIMA LINHA
    video_box(4, 196, 27, 209, cor3);  // 1
    video_box(31, 196, 58, 209, cor3); // 2
    video_box(62, 196, 89, 209, cor3); // 3

    video_box(217, 196, 244, 209, cor3); // 8
    video_box(248, 196, 275, 209, cor3); // 9
    video_box(279, 196, 306, 209, cor3); // 10

    // ANTEANTEPENULTIMA LINHA
    video_box(4, 181, 27, 194, cor4);  // 1
    video_box(31, 181, 58, 194, cor4); // 2

    video_box(248, 181, 275, 194, cor4); // 9
    video_box(279, 181, 306, 194, cor4); // 10

    // PRIMEIRA LINHA
    video_box(4, 166, 27, 179, cor5); // 1

    video_box(279, 166, 306, 179, cor5); // 10
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 1
 * 
 * Esta função para desenhar na tela o numero 1
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroOne(int x, int y, short cor)
{
    video_box(1 + x, 7 + y, 3 + x, 9 + y, cor);
    video_box(4 + x, 4 + y, 6 + x, 6 + y, cor);
    video_box(7 + x, 1 + y, 9 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 2
 * 
 * Esta função para desenhar na tela o numero 2
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroTwo(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 11 + x, 3 + y, cor);
    video_box(9 + x, 4 + y, 11 + x, 6 + y, cor);
    video_box(4 + x, 7 + y, 8 + x, 9 + y, cor);
    video_box(1 + x, 10 + y, 3 + x, 12 + y, cor);
    video_box(1 + x, 13 + y, 11 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 3
 * 
 * Esta função para desenhar na tela o numero 3
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroThree(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 11 + x, 3 + y, cor);
    video_box(9 + x, 1 + y, 11 + x, 15 + y, cor);
    video_box(3 + x, 7 + y, 11 + x, 9 + y, cor);
    video_box(1 + x, 13 + y, 11 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 4
 * 
 * Esta função para desenhar na tela o numero 4
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroFour(int x, int y, short cor)
{
    video_box(9 + x, 1 + y, 11 + x, 15 + y, cor);
    video_box(7 + x, 2 + y, 8 + x, 3 + y, cor);
    video_box(4 + x, 4 + y, 6 + x, 6 + y, cor);
    video_box(1 + x, 7 + y, 3 + x, 12 + y, cor);
    video_box(4 + x, 10 + y, 8 + x, 12 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 5
 * 
 * Esta função para desenhar na tela o numero 5
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroFive(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 11 + x, 3 + y, cor);
    video_box(1 + x, 4 + y, 3 + x, 6 + y, cor);
    video_box(1 + x, 7 + y, 11 + x, 9 + y, cor);
    video_box(9 + x, 10 + y, 11 + x, 12 + y, cor);
    video_box(1 + x, 13 + y, 11 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 6
 * 
 * Esta função para desenhar na tela o numero 6
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroSix(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 11 + x, 3 + y, cor);
    video_box(1 + x, 4 + y, 3 + x, 15 + y, cor);
    video_box(4 + x, 7 + y, 11 + x, 9 + y, cor);
    video_box(9 + x, 10 + y, 11 + x, 12 + y, cor);
    video_box(4 + x, 13 + y, 11 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 7
 * 
 * Esta função para desenhar na tela o numero 7
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroSeven(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 11 + x, 3 + y, cor);
    video_box(8 + x, 4 + y, 10 + x, 8 + y, cor);
    video_box(5 + x, 8 + y, 7 + x, 11 + y, cor);
    video_box(2 + x, 11 + y, 4 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 8
 * 
 * Esta função para desenhar na tela o numero 8
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroEight(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 3 + x, 15 + y, cor);
    video_box(4 + x, 1 + y, 8 + x, 3 + y, cor);
    video_box(9 + x, 1 + y, 11 + x, 15 + y, cor);
    video_box(4 + x, 8 + y, 8 + x, 9 + y, cor);
    video_box(4 + x, 13 + y, 8 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 9
 * 
 * Esta função para desenhar na tela o numero 9
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroNine(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 3 + x, 9 + y, cor);
    video_box(4 + x, 1 + y, 8 + x, 3 + y, cor);
    video_box(4 + x, 7 + y, 8 + x, 9 + y, cor);
    video_box(9 + x, 1 + y, 11 + x, 15 + y, cor);
    video_box(1 + x, 13 + y, 8 + x, 15 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar numero 0
 * 
 * Esta função para desenhar na tela o numero 0
 * @param X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param cor: variavel para indicar de qual cor será desenhado o numero na tela
 * 
 * --------------------------------------------------------------
 */
void numeroZero(int x, int y, short cor)
{
    video_box(1 + x, 1 + y, 3 + x, 15 + y, cor);
    video_box(9 + x, 1 + y, 11 + x, 15 + y, cor);
    video_box(4 + x, 1 + y, 8 + x, 3 + y, cor);
    video_box(4 + x, 13 + y, 8 + x, 15 + y, cor);
    video_box(4 + x, 9 + y, 5 + x, 10 + y, cor);
    video_box(5 + x, 7 + y, 6 + x, 8 + y, cor);
    video_box(7 + x, 5 + y, 8 + x, 6 + y, cor);
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar o valor da pontuação
 * 
 * Esta função para desenhar na tela os 3 digitos do valor da pontuação
 * fazendo o ordenamento, e deslocamentos necessarios para desenhar
 * cada digito na sua posição correta, assim como o digito correto
 * 
 * @param desloc_X: valor inteiro que serve para indicar o deslocamento no eixo
 * X, podendo mudar a posição do mesmo nesse eixo
 * @param desloc_Y: valor inteiro que serve para indicar o deslocamento no eixo
 * Y, podendo mudar a posição do mesmo nesse eixo
 * @param Score: variavel inteira que contém o valor da pontuação que deverá ser
 * escrito na tela
 * 
 * --------------------------------------------------------------
 */
void printValorScore(int Score, int desloc_X, int desloc_Y)
{
    int cor1;
    int cor2;
    int cor3;
    int cor4;
    int cor5;

    cor1 = 0xf810;
    cor2 = 0xe014;
    cor3 = 0xd016;
    cor4 = 0xb01a;
    cor5 = 0x901f;

    int digito1 = 0;
    int digito2 = 0;
    int digito3 = 0;
    short cor = 0xFFFF;
    
    digito1 = Score / 100;
    digito2 = (Score % 100) / 10;
    digito3 = (Score % 100) % 10;
    
    //score entre 100 e 200
    if (Score >= 100 && Score < 200)
    {
        cor = 0xff00;
    } //score entre 200 e 300
    else if (Score >= 200 && Score < 300)
    {
        cor = 0xd4e0;
    } //score entre 300 e 400
    else if (Score >= 300 && Score < 400)
    {
        cor = 0xe300;
    } //score entre 400 e 500
    else if (Score >= 400 && Score < 500)
    {
        cor = 0xe9a0;
    } //score maior que 500
    else if (Score >= 500)
    {
        cor = 0xf800;
    } //score menor que 100
    else if (Score < 100)
    {
        cor = 0xefe0;
    }

    switch (digito1)
    {
    case 0:
        numeroZero(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 1:
        numeroOne(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 2:
        numeroTwo(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 3:
        numeroThree(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 4:
        numeroFour(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 5:
        numeroFive(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 6:
        numeroSix(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 7:
        numeroSeven(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 8:
        numeroEight(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 9:
        numeroNine(98 + desloc_X, 3 + desloc_Y, cor);
        break;
    default:
        break;
    }

    switch (digito2)
    {
    case 0:
        numeroZero(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 1:
        numeroOne(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 2:
        numeroTwo(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 3:
        numeroThree(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 4:
        numeroFour(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 5:
        numeroFive(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 6:
        numeroSix(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 7:
        numeroSeven(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 8:
        numeroEight(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 9:
        numeroNine(111 + desloc_X, 3 + desloc_Y, cor);
        break;
    default:
        break;
    }

    switch (digito3)
    {
    case 0:
        numeroZero(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 1:
        numeroOne(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 2:
        numeroTwo(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 3:
        numeroThree(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 4:
        numeroFour(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 5:
        numeroFive(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 6:
        numeroSix(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 7:
        numeroSeven(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 8:
        numeroEight(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    case 9:
        numeroNine(124 + desloc_X, 3 + desloc_Y, cor);
        break;
    default:
        break;
    }
}


/** 
 * --------------------------------------------------------------
 * @brief Função para desenhar os blocos na tela
 * 
 * Esta função eh usada para percorrer o array de blocos e desenha cada bloco
 * na tela, se ele nao tiver destruído. Ela utiliza a função
 * video_box para desenhar um retângulo para cada bloco.

 * Primeiro a função percorre todos os blocos do jogo, representados pelo
 * array 'blocos', verificando se estão destruídos ou não. Se um
 * bloco não estiver destruído, ele é desenhado na tela como um
 * retângulo preenchido.
 * 
 * As coordenadas de cada bloco são feitas da soma das posições
 * x e y do canto esquerdo, e do tamanho largura e
 * altura de cada bloco.
 * 
 * Antes de desenhar, a função olha se os bloco
 * estão dentro dos limites da tela para evitar desenhos fora da área
 * visível e problemas na hora de mostrar.
 * 
 * --------------------------------------------------------------
 */
void renderizarBlocos()
{
    int x1;
    int y1;
    int x2;
    int y2;
    int p, a;
    for (p = 0; p <= NUM_BLOCOS_X; p++)
    {
        for (a = 0; a <= NUM_BLOCOS_Y; a++)
        {
            if (!blocos[p * NUM_BLOCOS_Y + a].destruido)
            {
                int x1 = blocos[p * NUM_BLOCOS_Y + a].x;
                int y1 = blocos[p * NUM_BLOCOS_Y + a].y;
                int x2 = x1 + blocos[p * NUM_BLOCOS_Y + a].largura;
                int y2 = y1 + blocos[p * NUM_BLOCOS_Y + a].altura;

                // Para olhar se os blocos estão dentro dos limites da tela
                if (x1 >= 0 && x2 <= screen_x && y1 >= 0 && y2 <= screen_y)
                {
                    video_box(x1, y1, x2, y2, blocos[p * NUM_BLOCOS_Y + a].cor);
                }
            }
        }
    }
}

/** 
 * --------------------------------------------------------------
 * @brief Função para verificar existencia de blocos ativos
 * 
 * Esta função serve para poder verificar se ainda existem blocos ativos, 
 * e então realizar o retorno informando se existem ou não
 * @return inteiro para informar a existencia ou ausencia de blocos que ainda
 * não foram destruidos
 * 
 * --------------------------------------------------------------
 */
int blocosAtivos()
{
    int contador = 0;
    int i = 0;
    for (i = 0; i < NUM_BLOCOS_X * NUM_BLOCOS_Y; i++)
    {
        if (blocos[i].destruido)
        {
            contador++;
        }
    }
    if (contador == ((NUM_BLOCOS_X) * (NUM_BLOCOS_Y)))
    {
        return 1; // sem blocos ativos
    }
    else
    {
        return 0; // tem blocos ativos
    }
}


