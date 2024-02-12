#include <stdio.h>
#include <intelfpgaup/accel.h>
#include <intelfpgaup/video.h>
#include <intelfpgaup/KEY.h>

#define NUM_BLOCOS_X 10
#define NUM_BLOCOS_Y 4


void moverRaquete();
void gerar_bordas();
void gerar_bola_quadrada(int x, int y);
void inicializarBlocos();
void renderBlocos();

typedef struct {
    int x; 
    int y; 
    int destruido; // 0 se não tiver destruído, 1 para destruído
    short cor;
} Bloco;

// Variável global para os blocos
Bloco blocos[NUM_BLOCOS_X * NUM_BLOCOS_Y];

// dimensões da tela 
int larguraTela, alturaTela;

int posicaoRaqueteX;
int posicaoBolaX, posicaoBolaY;
int velocidadeBolaX, velocidadeBolaY;



int main() {
    if(!iniciar_hardwares()){
        printf("Não foi possivel iniciar os dispositivos de hardware");
        return -1;
    }

    limparTela();

    //serve para apresentar a tela inicial tipo o menu do jogo
    //no momento so da para iniciar ou para sair do jogo
    if(!telaInicial()){
        //vai para a função de sair do jogo, e la faz a operação que precisa
        return 0;

    }

    limparTela();
    gerar_bordas();
    inicializarBlocos();
    renderBlocos();
    video_show();

    int btn_data;
    //while em que o jogo vai ficar
    /*
    Enquanto tiver blocos disponiveis, e enquanto nao apertar o botao de sair
    geral (3º botão da esquerda para direita), ele permanece aqui
    Quando apertar o botão de pause tem uma rotina aqui dentro mesmo para ir ao pause
    esperar pelo pressionar do usuario novamente para poder sair do pause
    So vai ter que se ligar aqui para apresentar na tela o texto de pause ou nao
    para isso tem uma função "informacao_pause" que exibe na tela o status do jogo, mas tem que
    ver onde ela vai ser chamada para poder exibir na tela
    */
    while(blocosDisponiveis() && (btn_data == 0b0010)){
        KEY_data(&btn_data);
        if(btn_data == 0b0100){
            sleep(0.5);
            sairPause();
        }

        limparTela();
        gerar_bordas();
        renderBlocos();
        video_show();
        moverRaquete();
    }
    //caso tenha ganho com o fim dos blocos, ai vai printar que terminou
    if(!blocosDisponiveis()){
        printTelaParaabens();
        fechar_hardwares();
        return 0;
    } else{
        fechar_hardwares();
        return 0;
    }


    // while (1) {
    //     moverRaquete();
    // }

	
    return 0;
}


void moverRaquete() {
    int posicaoRaqueteX = 155;
    int y1 = 240;
    int ptr_x, posicaoRaqueteX;
    // Não sei exatamente se pode colocar null, caso não possa é só 
    // adicionar as outras variaveis indicadas.
    accel_read(NULL, NULL, NULL, &ptr_X, NULL, NULL, NULL);

    //melhorar essa questão do y1 para melhorar os valores
    video_box (posicaoRaqueteX, y1-10, posicaoRaqueteX +40, y1-7, SlateBlue3);
    posicaoRaqueteX += ptr_X;

    /*os valores tem que considerar que ta falando da posição inicial, e
    ainda vai ser somado os 40px que é para fazer a largura da raquete
    */
    /*
    tela tem largura de 320px e raquete largura de 40px, logo
    posicaoRaqueteX so pode ir ate 319(tamanho util da tela) - 40 - 5(largura da borda)
    e valor minimo da raquete tem de ser 1(inicio da tela) + 5 (tam da borda) + 2 (espaçamento)
    */
    if (posicaoRaqueteX > 272) {
        posicaoRaqueteX = 272;
    } else if (posicaoRaqueteX<8) {
        posicaoRaqueteX = 8;
    }
}

void gerar_bordas(){
    int larguraTela, alturaTela, char_x, char_y;
	video_read (&larguraTela, &alturaTela, &char_x, &char_y);
	
	video_box(1, 1,larguraTela-1, alturaTela-1, 0xFF00);
	video_box(6, 6,larguraTela-6, alturaTela-6, 0x0000);
}

/* 
* Aqui eu fiz o seguinte, para inicicializar os blocos vamos ter laços aninhados para percorrer
* as posições da matriz criada lá no inicio. Para atribuir a posição de x e y da struct bloco é feito a 
* multiplicação de i pela largura do bloco (larguraTela/num) e j pela altura do bloco que vai ser util para 
* definir a posição de cada bloco na tela. Logo depois, os blocos são definicados como não destruidos e é feito
* uma espécie de random com % para definir a cor de cada bloco. 
*/ 
void inicializarBlocos() {
    for (int i = 0; i < NUM_BLOCOS_X; i++) {
        for (int j = 0; j < NUM_BLOCOS_Y; j++) {
            blocos[i][j].x = i *( larguraTela / NUM_BLOCOS_X);
            blocos[i][j].y = j * 20;
            blocos[i][j].destruido = 0;
            if (j % 3 == 0) {
                blocos[i][j].cor = 0xF800;
            } else if (j % 3 == 1) {
                blocos[i][j].cor = 0x07E0;
            } else {
                blocos[i][j].cor =0x001F;
             }
        }
    }
}

/*
Essa aqui é para mostrar os blocos na tela do jogo. Ela percorre a matriz
de blocos usando dois laços aninhados para acessar cada bloco. Então, 
em cada bloquinho, ela verifica se ele está marcado como não destruído.
Se não estiver destruído, calcula as coordenadas x1, y1, x2 e y2 que 
representam os cantos do retângulo que representa o bloco na tela.
E dps usa video_box() para colocar o retângulo na tela com a cor correspondente ao bloco.
*/

void renderBlocos() {
    for (int i = 0; i < NUM_BLOCOS_X; i++) {
        for (int j = 0; j < NUM_BLOCOS_Y; j++) {
            if (!blocos[i][j].destruido) {
                int x1 = i * ( larguraTela / NUM_BLOCOS_X);
                int y1 = j * 20;
                int x2 = x1 + ( larguraTela / NUM_BLOCOS_X);
                int y2 = y1 + 20;
                video_box(x1, y1, x2, y2, blocos[i][j].cor);
            }
        }
    }
}


/*
Serve para poder escrever na tela a palavra score
fonte usada tem 20px de largura e 20px de altura
*/
void palavra_score(){
	//x1, y1, x2, y2
	//começa x
	//começa y
	//termina x
	//termina y

	//S
	video_box(8,3,20,7,0xFFFF); //perna alta do S
	video_box(4,7,8,11,0xFFFF); //perna comprima do S
	video_box(8,11,20,15,0xFFFF); //perna meio do S
	video_box(20,15,24,19,0xFFFF); //perna comprima do S
	video_box(4,19,20,23,0xFFFF); //perna alta do S

	//C
	video_box(22,3,42,7,0xFFFF); //lateral superior do C
	video_box(22,3,26,23,0xFFFF); //perna comprida do C
	video_box(22,19,42,23,0xFFFF); //lateral inferior do C

	//O
	video_box(48,3,62,7,0xFFFF); //lateral superior do O
	video_box(44,7,48,19,0xFFFF); //lateral direita do O
	video_box(62,7,66,19,0xFFFF); //lateral esquerda do O
	video_box(48,19,62,23,0xFFFF); //lateral inferior do O

	//R
	video_box(64,3,84,23,0xFFFF); //parte de cima do R
    video_box(64,3,68,23,0xFFFF); //lateral direita do R
    video_box(80,3,84,13,0xFFFF); //parte de cima do R
    video_box(64,9,84,13,0xFFFF); //parte de cima do R
    video_box(64,3,84,23,0xFFFF); //parte de cima do R
    video_line(64,13,84,23,0xFFFF); //linha lateral descendente do R

	//E
	video_box(86,3,106,7,0xFFFF); //lateral superior do E
	video_box(86,11,106,15,0xFFFF); //lateral meio do E
	video_box(86,19,106,23,0xFFFF); //lateral inferior do E
	video_box(86,3,90,23,0xFFFF); //lateral direita do E

    // dois pontos
    video_box(108,7,112,11,0xFFFF); //ponto superior
    video_box(108,15,112,19,0xFFFF); //ponto inferior
}

/*
Serve para poder apresentar na tela o texto de pause
ta iniciando apresentando na tela na posição:
    x: 200px 
    y: 3px
Parametros:
    Boolean - indicar se esta em pause ou nao e apresentar na tela ou nao
*/
void informacao_pause(int boolean){
	char pause = "paused";
	char play = "played";
	if(boolean){
		video_text(200, 3, pause);
	} else{
		video_text(200, 3, play);
	}
}

/*
vai apresentar na tela logo apos a palavra score o texto com o valor da pontuação
Parametro:
    Pontuação: um char que tem o valor da pontuação
*/
void informarScore(char potuacao){
    video_text(115, 3, potuacao);
}


/*
pensar em como fazer para fazer essa bola redonda
por enquanto ela é quadrada, e se identifica como redonda
*/
void gerar_bola_quadrada(int x, int y){
	video_box(x, y, x+10, y+10, 0xFFFFFF);
}


/*
fechar a porta dos hardwares que foram utilizados
*/
void fechar_hardwares(){
	accel_close();
	video_close();
    KEY_close();
}

/*
realizar abertura e configuração dos hardwares a serem utilizados
*/
int iniciar_hardwares(){
	int larguraTela, alturaTela, char_x, char_y;
	int resolucao = 1;
	int range = 16;

    
    if(!accel_open() || !video_open() || !KEY_open()){
        return 0;
    }
    accel_init();
    video_read (&larguraTela, &alturaTela, &char_x, &char_y); // get screen & text size
    accel_format(resolucao, range);
    accel_calibrate();
}

/*
função para simplificar a limpeza do video que aparece na tela
ela sozinha ja faz o clear de texto e de caixas e linhas
*/
void limparTela(){
    video_erase ( ); // erase any text on the screen
	video_clear ( ); // clear current VGA Back buffer
}




/*
colocar while com switch case dentro
ele vai pegar e ter uma variavel que controla a interação do while, e se for p sair troca esse valor dentro do switch
no switch ele vai verificar qual botão foi apertado, se for o que quer massa, ele pega e sai, se nao for ele pega e continua ali
são 4 botões que tem e vem no formato da data de 0b1111, sendo para cada botao 1 bit
*/
/*
função para lidar com botões e apresentar a tela inicial para o usuario
como se fosse um menu, e a depender do botão que ele aperta ele pega e 
ou sai do programa, ou vai para a tela do jogo
dentro da logica tem variavel auxiliar para controlar o fluxo do while e dizer
quando deve sair dele, alem de controle para quando sai da função e ver o retorno
*/
int telaInicial(){
    int aux_while=1;
    int btn_data, retorno;
    while(aux_while){
        KEY_data(&btn_data);
        limparTela();
        
        //construir tela de inicio para apresentar antes de ir ao jogo
        printTelaInicial();




        switch (btn_data){
            case 0b1000: //botão de PLAY pressionado, o botão mais a esquerda
                aux_while = 0; //para poder sair do while e seguir para o jogo
                retorno = 1;
                break;
            case 0b0001: //botao de SAIR pressionado, o botão mais a direita
                aux_while = 0;
                retorno =0;
                break;
            
            default:
                aux_while = 1;
                break;
        }
    }
    return retorno;
}

/*
visa verificar se existem blocos disponiveis ainda
caso nao existam blocos disponiveis ele muda o retorno para poder sair do while
*/
int blocosDisponiveis(){
    int contador=0;
    for(int i=0; i<=(NUM_BLOCOS_X*NUM_BLOCOS_Y); i++){
        if(blocos[i].destruido == 1){
            contador ++;
        }
    }
    if(contador == (NUM_BLOCOS_X*NUM_BLOCOS_Y)){
        return 0;
    }
    
    return 1;
}


/*
função para ficar no loop enquanto espera apertar o botão de pause novamente
para poder sair do pause e voltar ao inicio
*/
void sairPause(){
    int btn_data=0;
    while(btn_data != 0b0100){
        KEY_data(&btn_data);
    }
    
}

void printTelaInicial(){

        /*
        24 blocos para letras
        8 letras
        3 blocos por letra
        7 blocos de espaçamento
        */
        
        //x1, y1, x2, y2
        //começa x
        //começa y
        //termina x
        //termina y

        //NOME BREAKOUT DA TELA QUE AMANDA FEZ
        //PRIMEIRA LINHA 15 BLOCOS VERMELHOS
        video_box(1, 5, 10, 11, 0xF800); //1
        video_box(12, 5, 22, 11, 0xF800); //2
        video_box(44, 5, 54, 11, 0xF800); //3
        video_box(56, 5, 66, 11, 0xF800); //4
        video_box(90, 5, 100, 11, 0xF800); //5
        video_box(102, 5, 112, 11, 0xF800); //6
        video_box(134, 5, 144, 11, 0xF800); //7
        video_box(166, 5, 176, 11, 0xF800); //8
        video_box(188, 5, 198, 11, 0xF800); //9
        video_box(218, 5, 228, 11, 0xF800); //10
        video_box(250, 5, 260, 11, 0xF800); //11
        video_box(272, 5, 282, 11, 0xF800); //12
        video_box(292, 5, 302, 11, 0xF800); //13
        video_box(304, 5, 314, 11, 0xF800); //14
        video_box(314, 5, 319, 11, 0xF800); //15

        //SEGUNDA LINHA 14 BLOCOS LARANJAS
        video_box(1, 13, 10, 19, 0xFC00); //1
        video_box(20, 13, 30, 19, 0xFC00); //2
        video_box(40, 13, 50, 19, 0xFC00); //3
        video_box(60, 13, 70, 19, 0xFC00); //4
        video_box(80, 13, 90, 19, 0xFC00); //5
        video_box(120, 13, 130, 19, 0xFC00); //6
        video_box(140, 13, 150, 19, 0xFC00); //7
        video_box(160, 13, 170, 19, 0xFC00); //8
        video_box(175, 13, 185, 19, 0xFC00); //9
        video_box(200, 13, 210, 19, 0xFC00); //10
        video_box(220, 13, 230, 19, 0xFC00); //11
        video_box(240, 13, 250, 19, 0xFC00); //12
        video_box(260, 13, 270, 19, 0xFC00); //13
        video_box(290, 13, 300, 19, 0xFC00); //14
        

        //TERCEIRA LINHA 15 BLOCOS AMARELOS
        video_box(1, 21, 10, 27, 0xFFE0); //1
        video_box(12, 21, 22, 27, 0xFFE0); //2
        video_box(42, 21, 52, 27, 0xFFE0); //3
        video_box(54, 21, 64, 27, 0xFFE0); //4
        video_box(84, 21, 94, 27, 0xFFE0); //5
        video_box(96, 21, 106, 27, 0xFFE0); //6
        video_box(126, 21, 136, 27, 0xFFE0); //7
        video_box(138, 21, 148, 27, 0xFFE0); //8
        video_box(150, 21, 160, 27, 0xFFE0); //9
        video_box(170, 21, 180, 27, 0xFFE0); //10
        video_box(182, 21, 192, 27, 0xFFE0); //11
        video_box(212, 21, 222, 27, 0xFFE0); //12
        video_box(232, 21, 242, 27, 0xFFE0); //13
        video_box(252, 21, 262, 27, 0xFFE0); //14
        video_box(272, 21, 282, 27, 0xFFE0); //15
        video_box(292, 21, 302, 27, 0xFFE0); //16

        //QUARTA LINHA 14 BLOCOS VERMELHOS
        video_box(1, 29, 10, 35, 0x07E0); //1
        video_box(20, 29, 30, 35, 0x07E0); //2
        video_box(40, 29, 50, 35, 0x07E0); //3
        video_box(60, 29, 70, 35, 0x07E0); //4
        video_box(80, 29, 90, 35, 0x07E0); //5
        video_box(120, 29, 130, 35, 0x07E0); //6
        video_box(140, 29, 150, 35, 0x07E0); //7
        video_box(160, 29, 170, 35, 0x07E0); //8
        video_box(175, 29, 185, 35, 0x07E0); //9
        video_box(200, 29, 210, 35, 0x07E0); //10
        video_box(220, 29, 230, 35, 0x07E0); //11
        video_box(240, 29, 250, 35, 0x07E0); //12
        video_box(260, 29, 270, 35, 0x07E0); //13
        video_box(290, 29, 300, 35, 0x07E0); //14
        
        //QUINTA LINHA 15 BLOCOS VERMELHOS
        video_box(1, 37, 10, 43, 0x07E0); //1
        video_box(12, 37, 22, 43, 0x07E0); //2
        video_box(44, 37, 54, 43, 0x07E0); //3
        video_box(64, 37, 74, 43, 0x07E0); //4
        video_box(90, 37, 100, 43, 0x07E0); //5
        video_box(102, 37, 112, 43, 0x07E0); //6
        video_box(126, 37, 136, 43, 0x07E0); //7
        video_box(146, 37, 156, 43, 0x07E0); //8
        video_box(166, 37, 176, 43, 0x07E0); //9
        video_box(186, 37, 196, 43, 0x07E0); //10
        video_box(216, 37, 226, 43, 0x07E0); //11
        video_box(256, 37, 266, 43, 0x07E0); //12
        video_box(286, 37, 296, 43, 0x07E0); //13


}


void printTelaParaabens(){
    //x1, y1, x2, y2
    //começa x
    //começa y
    //termina x
    //termina y

    //LETRA P
    video_box(10, 20, 40, 24,0xFFFF); //traço de cima do P
	video_box(10,20,14,50,0xFFFF); //traço em pé esquerdo do P 
	video_box(36,20,40,29,0xFFFF); //traço em pé menor direito do P
	video_box(10,36,40,40,0xFFFF); //traço deitado pequeno do P

    //LETRA A
    video_box(42, 20, 72, 24,0xFFFF); //traço de cima do P
	video_box(42,20,46,50,0xFFFF); //traço em pé esquerdo do P 
	video_box(68,20,72,50,0xFFFF); //traço em pé esquerdo do P
	video_box(42,40,72,44,0xFFFF); //traço deitado pequeno do P

    //LETRA R
    video_box(74, 20, 104, 24,0xFFFF); //traço de cima do P
	video_box(74,20,14,50,0xFFFF); //traço em pé esquerdo do P 
	video_box(36,20,104,29,0xFFFF); //traço em pé menor direito do P
	video_box(74,36,104,40,0xFFFF); //traço deitado pequeno do P
    video_line(74,36,104,50,0xFFFF); //linha lateral descendente do R

    //LETRA A
    video_box(110, 20, 140, 24,0xFFFF); //traço de cima do P
	video_box(110,20,46,50,0xFFFF); //traço em pé esquerdo do P 
	video_box(138,20,140,50,0xFFFF); //traço em pé esquerdo do P
	video_box(110,40,140,44,0xFFFF); //traço deitado pequeno do P

    //LETRA B
    video_box(142, 20, 168, 24,0xFFFF); //traço de cima do P
	video_box(142,20,146,50,0xFFFF); //traço em pé esquerdo do P 
	video_box(168,24,172,33,0xFFFF); //traço em pé esquerdo do P
    video_box(168,37,172,46,0xFFFF); //traço em pé esquerdo do P
	video_box(142,33,168,37,0xFFFF); //traço deitado pequeno do P
    video_box(142, 46, 168, 50,0xFFFF); //traço de cima do P


	//LETRA E
	video_box(174,20,204,24,0xFFFF); //lateral superior do E
	video_box(174,33,200,37,0xFFFF); //lateral meio do E
	video_box(174,46,204,50,0xFFFF); //lateral inferior do E
	video_box(174,20,178,50,0xFFFF); //lateral direita do E

    //LETRA N
	video_box(206,20,210,50,0xFFFF); //lateral superior do E
	video_box(234,20,236,50,0xFFFF); //lateral meio do E
    video_line(210,20,234,50,0xFFFF); //linha lateral descendente do R

    //LETRA S
    video_box(236,20,266,24,0xFFFF); //perna alta do S
	video_box(236,20,240,35,0xFFFF); //perna comprima do S
	video_box(236,33,266,37,0xFFFF); //perna meio do S
	video_box(262,37,266,50,0xFFFF); //perna comprima do S
	video_box(236,46,266,50,0xFFFF); //perna alta do S
    

    //ULTIMA LINHA
    video_box(4, 220, 27, 239,0x07E0);
    video_box(31, 220, 58, 239,0x07E0);
    video_box(62, 220, 89, 239,0x07E0);
    video_box(93, 220, 120, 239,0x07E0);
    video_box(124, 220, 151, 239,0x07E0);
    video_box(155, 220, 182, 239,0x07E0);
    video_box(186, 220, 213, 239,0x07E0);
    video_box(217, 220, 244, 239,0x07E0);
    video_box(248, 220, 275, 239,0x07E0);
    video_box(279, 220, 306, 239,0x07E0);
    
    //PENULTIMA LINHA
    video_box(4, 220, 27, 239,0x07E0); //1
    video_box(31, 220, 58, 239,0x07E0); //2
    video_box(62, 220, 89, 239,0x07E0); //3
    video_box(93, 220, 120, 239,0x07E0); //4
    

    video_box(186, 220, 213, 239,0x07E0); //7
    video_box(217, 220, 244, 239,0x07E0); //8
    video_box(248, 220, 275, 239,0x07E0); //9
    video_box(279, 220, 306, 239,0x07E0); //10

    //ANTEPENULTIMA LINHA
    video_box(4, 220, 27, 239,0xFFE0); //1
    video_box(31, 220, 58, 239,0xFFE0); //2
    video_box(62, 220, 89, 239,0xFFE0); //3

    
    

    video_box(217, 220, 244, 239,0xFFE0); //8
    video_box(248, 220, 275, 239,0xFFE0); //9
    video_box(279, 220, 306, 239,0xFFE0); //10

    //ANTEANTEPENULTIMA LINHA
    video_box(4, 220, 27, 239,0xFC00); //1
    video_box(31, 220, 58, 239,0xFC00); //2


    
    


    video_box(248, 220, 275, 239,0xFC00); //9
    video_box(279, 220, 306, 239,0xFC00); //10


    //PRIMEIRA LINHA
    video_box(4, 220, 27, 239,0xF800); //1


    
    



    video_box(279, 220, 306, 239,0xF800); //10

}