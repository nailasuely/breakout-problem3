
#include <stdio.h>
#include <intelfpgaup/accel.h>
#include <intelfpgaup/KEY.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SlateBlue3 0x6959CD


void fechar_hardwares();
int iniciar_hardwares();
void printTelaInicial();
int telaInicial();
void limparTela();
void printTelaParabens();
void gerar_bordas();
void moverRaquete();
void inicializarBlocos();
void gerar_bola_quadrada(int x, int y);
void palavra_score(int scoreInt, int shift_X, int shift_Y);
void sairPause();
void informacao_pause(int boolean);
void moverBola(int colisao);
void mover_bola(int *x, int *y, int *dx, int *dy);
void desenharBola(int x, int y, int raio);
void printTelaPerdeu();
void detectarColisao();
void detectarColisaoRaquete();
char *  itoa ( int value, char * str );
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

#define LARGURA_BLOCO 32
#define ALTURA_BLOCO 10
#define NUM_BLOCOS_X 10
#define NUM_BLOCOS_Y 6
#define ESPACAMENTO 2
#define MARGEM_ESQUERDA 17
#define MARGEM_SUPERIOR 35

typedef struct {
    int x;
    int y;
    int largura;
    int altura;
    int destruido; // 0 se não estiver destruído, 1 se estiver destruído
    short cor;
} Bloco;

Bloco blocos[NUM_BLOCOS_X * NUM_BLOCOS_Y];


int xInicialBola = 155;
int yInicialBola = 135;
int posicaoRaqueteX = 155;
int velocidadePadraoBolaX = 1;
int velocidadePadraoBolaY = 1;
int screen_x = 320;
int screen_y = 240;
int quantColunasTexto = 0; //ta pegando esses valores em iniciar hardware
int quantLinhasTexto = 0;
int scoreInt = 0;


int main()
{
    char *pause = "paused";
    int p, a;
	int btn_data = 0 ;

	if(!iniciar_hardwares()){
		printf("Não foi possivel iniciar os dispositivos de hardware");
		return -1;
	    }
    
	limparTela();
	if(!telaInicial()){
		//vai para a função de sair do jogo, e la faz a operação que precisa
		return 0;

	}
	limparTela();
    inicializarBlocos();


	while (blocosAtivos() ||btn_data != 0b0010){
		limparTela();
		KEY_read(&btn_data);
		
		
        
		gerar_bordas();
		palavra_score(calcularScore(), 0, 0);
		moverRaquete();
        detectarColisao();
        detectarColisaoRaquete();

		/*printf("posicao raquete X: %d \n", posicaoRaqueteX );
		printf("posicao X bola: %d \n", xInicialBola);
		printf("posicao Y bola: %d \n", yInicialBola);*/

		if(posicaoRaqueteX == xInicialBola && yInicialBola == 219){
			moverBola(1);
			
		} else{
			moverBola(0);
		}
        renderizarBlocos();
        //ROTINA DE QUANDO PERDE

        
        /*
        int x1; int y1; int x2; int y2;
        for (p = 0; p < NUM_BLOCOS_X; p++) {
            for (a = 0; a < NUM_BLOCOS_Y; a++) {
                if (!blocos[p * NUM_BLOCOS_Y + a].destruido) {
                    int x1 = blocos[p * NUM_BLOCOS_Y + a].x;
                    int y1 = blocos[p * NUM_BLOCOS_Y + a].y;
                    int x2 = x1 + blocos[p * NUM_BLOCOS_Y + a].largura;
                    int y2 = y1 + blocos[p * NUM_BLOCOS_Y + a].altura;
                    
                    // Para olhar se os blocos estão dentro dos limites da tela
                    if (x1 >= 0 && x2 <= screen_x && y1 >= 0 && y2 <= screen_y) {
                        video_box(x1, y1, x2, y2, blocos[p * NUM_BLOCOS_Y + a].cor);
                    }
                }
            }
        }
        */
        if(btn_data == 0b0100){
            //video_erase();
            informacao_pause(1);
		    sairPause();
		}
		informacao_pause(0);

 
		video_show (); 


        if(blocosAtivos()){
            sairTelaParabens();
        }
		if(yInicialBola > 230){
            int scoreHist = calcularScore();
            sairTelaPerder(scoreHist);
            //return 0;
            
            //sairTelaPerder();
           // video_show();
           // return 0;   
        }


	}
	limparTela();

    //ROTINA DE QUANDO GANHA
    if(blocosAtivos()){
        limparTela();
        printTelaParabens();
        video_show ();
    }
    video_show();
    fechar_hardwares();
	return 0;
}

void inicializarBlocos() {
    int indiceBloco = 0;
    int larguraDisponivel = (screen_x - (NUM_BLOCOS_X - 1) * ESPACAMENTO) + 5;

    printf("largura dispoivel : %d \n", larguraDisponivel);

    int larguraBloco = (larguraDisponivel / NUM_BLOCOS_X);
    int linha, coluna;

    for (linha = 0; linha < NUM_BLOCOS_Y; linha++) {
        for (coluna = 0; coluna < NUM_BLOCOS_X; coluna++) {
            blocos[indiceBloco].x = coluna * (larguraBloco + ESPACAMENTO) + MARGEM_ESQUERDA;
            blocos[indiceBloco].y = linha * (ALTURA_BLOCO + ESPACAMENTO) + MARGEM_SUPERIOR;
            blocos[indiceBloco].largura = larguraBloco;
            blocos[indiceBloco].altura = ALTURA_BLOCO;
            blocos[indiceBloco].destruido = 0;
            printf("Bloco %i - %i - x: %d \n", linha, coluna, blocos[linha * NUM_BLOCOS_Y + coluna].x);
            printf("Bloco %i - %i - y: %d \n", linha, coluna, blocos[linha * NUM_BLOCOS_Y + coluna].y);
            printf("Bloco %i - %i - largura: %d \n", linha, coluna, blocos[linha * NUM_BLOCOS_Y + coluna].largura);
            printf("Bloco %i - %i - altura: %d \n", linha, coluna, blocos[linha * NUM_BLOCOS_Y + coluna].altura);
           
            int cor1 = 0xF81F; 
            int cor2 = 0xF809; 
            int cor3 = 0xD0C0; 

            float percentual = (float)coluna / NUM_BLOCOS_X;
            int corIntermediaria;
            if (percentual < 0.5) {
                corIntermediaria = (int)((0.5 - percentual) * 2 * cor1 + (percentual * 2) * cor2);
            } else {
                corIntermediaria = (int)((1.0 - percentual) * 2 * cor2 + (percentual * 2 - 1.0) * cor3);
            }
            
            blocos[indiceBloco].cor = corIntermediaria;

            indiceBloco++;

            if (indiceBloco >= NUM_BLOCOS_X * NUM_BLOCOS_Y) {
                break;
            }
        }

        if (indiceBloco >= NUM_BLOCOS_X * NUM_BLOCOS_Y) {
            break;
        }
    }
}


void detectarColisao() {
    int i; int t;

    // Calcula a velocidade 
    int velocidadeX = velocidadePadraoBolaX;
    int velocidadeY = velocidadePadraoBolaY;

    // próxima posição da bola
    int proximoX = xInicialBola + velocidadeX;
    int proximoY = yInicialBola + velocidadeY;

    
    for (t = 0; t <= 1; t++) {
        // Calcula a posição intermediária
        int passoX = xInicialBola + (velocidadeX * t);
        int passoY = yInicialBola + (velocidadeY * t);

        // Verifica colisão com cada bloco
        for (i = 0; i < NUM_BLOCOS_X * NUM_BLOCOS_Y; i++) {
            if (!blocos[i].destruido) {
                int centroBlocoX = blocos[i].x + blocos[i].largura / 2;
                int centroBlocoY = blocos[i].y + blocos[i].altura / 2;

                int distanciaX = abs(passoX + 3 - centroBlocoX);
                int distanciaY = abs(passoY + 3 - centroBlocoY);

                int somaRaios = 3 + blocos[i].largura / 2;

                if (distanciaX <= somaRaios && distanciaY <= somaRaios) {

                    velocidadePadraoBolaY = -velocidadePadraoBolaY;
                    blocos[i].destruido = 1;

                    // Sair do loop 
                    return;
                }
            }
        }
    }
}

void gerar_bola_quadrada(int x, int y){
	video_box(x, y, x+6, y+6, 0xFFFFFF);
}



/*
void detectarColisaoRaquete() {

    int raqueteEsquerda = 0;
    int raqueteDireita = 0;
    int raqueteTopo = 0;
    int raqueteBase = 0;
    int bolaEsquerda = 0;
    int bolaDireita = 0;
    int bolaTopo = 0;
    int bolaBase = 0;
    int deslocamento = 0;
    int velocidadeX = 0;

 
    raqueteEsquerda = posicaoRaqueteX;
    raqueteDireita = posicaoRaqueteX + 40; // Largura da raquete
    raqueteTopo = 240 - 10; // Posição Y do topo da raquete
    raqueteBase = 240; // Posição Y da base da raquete

    // Coordenadas da bola
    bolaEsquerda = xInicialBola;
    bolaDireita = xInicialBola + 10; // Tamanho da bola
    bolaTopo = yInicialBola;
    bolaBase = yInicialBola + 10; // Tamanho da bola

    // Verificar se existe sobreposição entre a bola e a raquete
    if (bolaDireita >= raqueteEsquerda && bolaEsquerda <= raqueteDireita &&
        bolaBase >= raqueteTopo && bolaTopo <= raqueteBase) {
        // Inverter a direção da bola no eixo Y
        velocidadePadraoBolaY = -velocidadePadraoBolaY;

        // Ajustar a direção da bola 
        int centroRaquete = posicaoRaqueteX + 20; // Centro da raquete
        deslocamento = centroRaquete - (xInicialBola + 3); // Distância do centro da raquete até o centro da bola
        velocidadeX = deslocamento / 5; // Ajustar a velocidade proporcionalmente ao deslocamento

        // Limitar a velocidade máxima
        if (velocidadeX > 3) velocidadeX = 3;
        if (velocidadeX < -3) velocidadeX = -3;
    }
}
*/

void detectarColisaoRaquete() {

    int raqueteEsquerda = 0;
    int raqueteDireita = 0;
    int raqueteTopo = 0;
    int raqueteBase = 0;
    int bolaEsquerda = 0;
    int bolaDireita = 0;
    int bolaTopo = 0;
    int bolaBase = 0;
    int deslocamento = 0;
    int velocidadeX = 0;

 
    raqueteEsquerda = posicaoRaqueteX;
    raqueteDireita = posicaoRaqueteX + 40; 
    raqueteTopo = 240 - 5; 
    raqueteBase = 240; 

    
    bolaEsquerda = xInicialBola;
    bolaDireita = xInicialBola + 10; // Tamanho da bola
    bolaTopo = yInicialBola;
    bolaBase = yInicialBola + 10; 

    // olhar se existe sobreposição 
    if (bolaDireita >= raqueteEsquerda && bolaEsquerda <= raqueteDireita &&
        bolaBase >= raqueteTopo && bolaTopo <= raqueteBase) {
        // Inverte a direção 
        velocidadePadraoBolaY = -velocidadePadraoBolaY;

        // Ajustar a direção da bola 
        int centroRaquete = posicaoRaqueteX + 20; // Centro da raquete
        deslocamento = centroRaquete - (xInicialBola + 3); // Distância do centro da raquete até o centro da bola
        velocidadeX = deslocamento / 5; // Ajustar a velocidade proporcionalmente ao deslocamento


        
        // Limitar a velocidade máxima da bola
        if (velocidadeX > 3) velocidadeX = 3;
        if (velocidadeX < -3) velocidadeX = -3;
        
       /*
       // Limitar a velocidade máxima da bola
        if (velocidadeX > 13) velocidadeX = 12;
        if (velocidadeX < -13) velocidadeX = -12;
        */
    }
}




void moverBola(int colisao){
	
	xInicialBola += velocidadePadraoBolaX;
	yInicialBola += velocidadePadraoBolaY;

	if(colisao){
		velocidadePadraoBolaX = -velocidadePadraoBolaX;
		velocidadePadraoBolaY = - velocidadePadraoBolaY;
	} else{
		if(xInicialBola < 12 || xInicialBola > 306){
			velocidadePadraoBolaX = -velocidadePadraoBolaX;
		}

	    if(yInicialBola < 35 || yInicialBola > 300){
		velocidadePadraoBolaY = - velocidadePadraoBolaY;
	    }
	}
    //gerar_bola_quadrada(xInicialBola, yInicialBola);
	desenhar_bola(xInicialBola, yInicialBola);
	
}



void moverRaquete() {
    
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
	
    //melhorar essa questão do y1 para melhorar os valores
    video_box (posicaoRaqueteX, y1-10, posicaoRaqueteX + 40, y1-7, 0xFFFF);
    posicaoRaqueteX += ptr_x/10;

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


void sairPause(){
    int btn_data=0;
    while(btn_data != 0b0100){
    	informacao_pause(1);
        KEY_read(&btn_data);
    }
    
}

void sairTelaPerder(int histScore){
    int btn_data=0;
    int contador=0;
    char * info = "press button 1 to return";
    xInicialBola = 155;
    yInicialBola = 135;
    posicaoRaqueteX = 155;
    limparTela();
    printTelaPerdeu();
    palavra_score(histScore, 86,166);
    video_show();
    inicializarBlocos();
    while(btn_data != 0b1000){
        KEY_read(&btn_data);
        if(contador <1000){
            contador ++;
        } else{
            printf("\nquant de linhas de texto: %d", quantLinhasTexto);
            printf("\nquant de colunas que o texto tem: %d", strlen(info));
            printf("\ntexto reproduzido: %s", info);
            video_text( 28,30,info);
        }
    }
    
}

void sairTelaParabens(){
    limparTela();
    printTelaParabens();
    video_show ();
    inicializarBlocos();
    xInicialBola = 155;
    yInicialBola = 135;
    posicaoRaqueteX = 155;
    int btn_data=0;
    int contador =0;
    char * info = "press button 1 to return";
    while(btn_data != 0b1000){
        KEY_read(&btn_data);
        if(contador <1000){
            contador ++;
        } else{
            printf("\nquant de colunas de texto: %d", quantColunasTexto);
            printf("\nquant de colunas que o texto tem: %d", strlen(info));
            printf("\ntexto reproduzido: %s", info);
            video_text( 28,30,info);
        }
    }
}



void informacao_pause(int boolean){
	char *pause = "pause";
	char *play = "play";
    
	if(boolean){
		video_text(quantColunasTexto - 6, 3, pause);
	} else{
		//video_text(quantColunasTexto - 6, 3, play);
	}
}

/*void desenhar_bola(int x, int y) {
    int px, py;
    for (py = -3; py <= 3; py++) {
        for (px = -3; px <= 3; px++) {
            if (px * px + py * py <= 3 * 3) {
                video_pixel(x + px, y + py, 0xFFFF); 
            }
        }
    }
}*/

/*void desenhar_bola(int centroX, int centroY) {
    int raio;
    raio = 3;
    int y;
    int x;
    for (y = -raio; y <= raio; y++) {
        for (x = -raio; x <= raio; x++) {

            if (x*x + y*y <= raio*raio) {
                int pixelX;
                int pixelY;
                pixelX = centroX + x;
                pixelY = centroY + y;

                 video_pixel(pixelX, pixelY, 0xFFFF);
            }
        }
    }
}*
*/



void desenhar_bola(int centroX, int centroY) {
    int raio = 5; // Defina o raio da bola
    int x, y;

    for (y = -raio; y <= raio; y++) {
        for (x = -raio; x <= raio; x++) {
           
            if (x * x + y * y <= raio * raio) {
                int pixelX = centroX + x;
                int pixelY = centroY + y;

                
                int distSquared = x * x + y * y;

              
                float aproxSqrt = 1.0 - (float)distSquared / (raio * raio);

                
                if (aproxSqrt < 0) {
                    aproxSqrt = 0;
                } else if (aproxSqrt > 1) {
                    aproxSqrt = 1;
                }

               
                int cor = (int)(aproxSqrt * 0xFFFF); // Branco diferente

                // Desenhar o pixel com a cor 
                if (pixelX >= 0 && pixelX < screen_x && pixelY >= 0 && pixelY < screen_y) {
                    video_pixel(pixelX, pixelY, cor);
                }
            }
        }
    }
}



/*void desenhar_bola(int centroX, int centroY) {
    int raio = 5; // Defina o raio da bola
    int x, y;

    for (y = -raio; y <= raio; y++) {
        for (x = -raio; x <= raio; x++) {
      
            if (x * x + y * y <= raio * raio) {
                int pixelX = centroX + x;
                int pixelY = centroY + y;

              
                if (pixelX >= 0 && pixelX < screen_x && pixelY >= 0 && pixelY < screen_y) {
                    video_pixel(pixelX, pixelY, 0xFFFF); 
                }
            }
        }
    }
}*/

/*
fechar a porta dos hardwares que foram utilizados
*/
void fechar_hardwares(){
	accel_close();
	video_close();
    KEY_close();
}

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
    quantColunasTexto = char_x;
    quantLinhasTexto = char_y;
}






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
   corL1 = 0xF800;
   corL2 = 0xFC00;
   corL3 = 0xFFE0;
   corL4 = 0x97e0;
   corL5 = 0x07E0;
   
    
    //x1, y1, x2, y2
    //começa x
    //começa y
    //termina x
    //termina y

    //NOME BREAKOUT DA TELA QUE AMANDA FEZ A FOTO
    //PRIMEIRA LINHA 15 BLOCOS #ff004a
    video_box(15, 5, 23, 11, corL1); //1
    video_box(25, 5, 33, 11, corL1); //2
    //video_box(35, 5, 43, 11, corL1); //3
    //video_box(45, 5, 53, 11, corL1); //4
    video_box(55, 5, 63, 11, corL1); //5
    video_box(65, 5, 73, 11, corL1); //6
    //video_box(75, 5, 83, 11, corL1); //7
    //video_box(85, 5, 93, 11, corL1); //8
    //video_box(95, 5, 103, 11, corL1); //9
    video_box(105, 5, 113, 11, corL1); //10
    video_box(115, 5, 123, 11, corL1); //11
    //video_box(125, 5, 133 11, corL1); //12
    //video_box(135, 5, 143, 11, corL1); //13
    video_box(145, 5, 153, 11, corL1); //14
    //video_box(155, 5, 163, 11, corL1); //15
    //video_box(165, 5, 173, 11, corL1); //16
    video_box(175, 5, 183, 11, corL1); //17
    //video_box(185, 5, 193, 11, corL1); //18
    video_box(195, 5, 203, 11, corL1); //19
    //video_box(205, 5, 213, 11, corL1); //20
    //video_box(215, 5, 223, 11, corL1); //21
    video_box(225, 5, 233, 11, corL1); //22
    //video_box(235, 5, 243, 11, corL1); //23
    //video_box(245, 5, 253, 11, corL1); //24
    video_box(255, 5, 263, 11, corL1); //25
    //video_box(265, 5, 273, 11, corL1); //26
    video_box(275, 5, 283, 11, corL1); //27
    video_box(285, 5, 293, 11, corL1); //28
    video_box(295, 5, 303, 11, corL1); //29
    video_box(305, 5, 313, 11, corL1); //30



    //SEGUNDA LINHA 14 BLOCOS LARANJAS
    video_box(15, 13, 23, 19, corL2); //1
    //video_box(25, 13, 33, 19, 0xF809); //2
    video_box(35, 13, 43, 19,corL2); //3
    //video_box(45, 13, 53, 19, 0xF809); //4
    video_box(55, 13, 63, 19, corL2); //5
    //video_box(65, 13, 73, 19, 0xF809); //6
    video_box(75, 13, 83, 19, corL2); //7
    //video_box(85, 13, 93, 19, 0xF809); //8
    video_box(95, 13, 103, 19, corL2); //9
    //video_box(105, 13, 113, 19, 0xF809); //10
    //video_box(115, 13, 123, 19, 0xF809); //11
    //video_box(125, 13, 133 19, 0xF809); //12
    video_box(135, 13, 143, 19, corL2); //13
    //video_box(145, 13, 153, 19, 0xF809); //14
    video_box(155, 13, 163, 19, corL2); //15
    //video_box(165, 13, 173, 19, 0xF809); //16
    video_box(175, 13, 183, 19, corL2); //17
    video_box(180, 13, 188, 19, corL2); //18 - deslocar para esquerda - V
    //video_box(195, 13, 203, 19, 0xF809); //19
    //video_box(205, 13, 213, 19, 0xF809); //20
    video_box(215, 13, 223, 19, corL2); //21
    //video_box(225, 13, 233, 19, 0xF809); //22
    video_box(235, 13, 243, 19, corL2); //23
    //video_box(245, 13, 253, 19, 0xF809); //24
    video_box(255, 13, 263, 19, corL2); //25
    //video_box(265, 13, 273, 19, 0xF809); //26
    video_box(275, 13, 283, 19, corL2); //27
    //video_box(285, 13, 293, 19, 0xF809); //28
    video_box(295, 13, 303, 19,corL2); //29
    //video_box(305, 13, 313, 19, 0xF809); //30
    

    //TERCEIRA LINHA 15 BLOCOS AMARELOS
    video_box(15, 21, 23, 27, corL3); //1
    video_box(25, 21, 33, 27, corL3); //2
    //video_box(35, 21, 43, 27, 0xF809); //3
    //video_box(45, 21, 53, 27, 0xF809); //4
    video_box(55, 21, 63, 27, corL3); //5
    video_box(65, 21, 73, 27, corL3); //6
    //video_box(75, 21, 83, 27, 0xF809); //7
    //video_box(85, 21, 93, 27, 0xF809); //8
    video_box(95, 21, 103, 27, corL3); //9
    video_box(105, 21, 113, 27, corL3); //10
    //video_box(115, 21, 123, 27, 0xF809); //11
    //video_box(125, 21, 133 27, 0xF809); //12
    video_box(135, 21, 143, 27, corL3); //13
    video_box(145, 21, 153, 27,corL3); //14
    video_box(155, 21, 163, 27,corL3); //15
    //video_box(165, 21, 173, 27, 0xF809); //16
    video_box(175, 21, 183, 27, corL3); //17
    video_box(185, 21, 193, 27, corL3); //18
    //video_box(195, 21, 203, 27, 0xF809); //19
    //video_box(205, 21, 213, 27, 0xF809); //20
    video_box(215, 21, 223, 27, corL3); //21
    //video_box(225, 21, 233, 27, 0xF809); //22
    video_box(235, 21, 243, 27, corL3); //23
    //video_box(245, 21, 253, 27, 0xF809); //24
    video_box(255, 21, 263, 27, corL3); //25
    //video_box(265, 21, 273, 27, 0xF809); //26
    video_box(275, 21,283, 27, corL3); //27
    //video_box(285, 21, 293, 27, 0xF809); //28
    video_box(295, 21, 303, 27, corL3); //29
    //video_box(305, 21, 313, 27, 0xF809); //30

    //QUARTA LINHA 14 BLOCOS VERMELHOS
    video_box(15, 29, 23, 35, corL4); //1
    //video_box(25, 29, 33, 35, corL4); //2
    video_box(35, 29, 43, 35, corL4); //3
    //video_box(45, 29, 53, 35, corL4); //4
    video_box(55, 29, 63, 35, corL4); //5
    //video_box(65, 29, 73, 35, corL4); //6
    video_box(75, 29, 83, 35, corL4); //7
    //video_box(85, 29, 93, 35, corL4); //8
    video_box(95, 29, 103, 35, corL4); //9
    //video_box(105, 29, 113, 35, corL4); //10
    //video_box(115, 29, 123, 35, corL4); //11
    //video_box(125, 29, 133, 35, corL4); //12
    video_box(135, 29, 143, 35, corL4); //13
    //video_box(145, 29, 153, 35, corL4); //14
    video_box(155, 29, 163, 35, corL4); //15
    //video_box(165, 29, 173, 35, corL4); //16
    video_box(175, 29, 183, 35, corL4); //17
    video_box(180, 29, 188, 35, corL4); //18 - deslocar a esquerda - V
    //video_box(195, 29, 203, 35, corL4); //19
    //video_box(205, 29, 213, 35, corL4); //20
    video_box(215, 29, 223, 35, corL4); //21
    //video_box(225, 29, 233, 35, corL4); //22
    video_box(235, 29, 243, 35, corL4); //23
    //video_box(245, 29, 253, 35, corL4); //24
    video_box(255, 29, 263, 35, corL4); //25
    //video_box(265, 29, 273, 35, corL4); //26
    video_box(275, 29, 283, 35, corL4); //27
    //video_box(285, 29, 293, 35, corL4); //28
    video_box(295, 29, 303, 35, corL4); //29
    //video_box(305, 29, 313, 35, corL4); //30
    
    //QUINTA LINHA 15 BLOCOS VERMELHOS
    video_box(15, 37, 23, 43, corL5); //1
    video_box(25, 37, 33, 43, corL5); //2
    //video_box(35, 37, 43, 43, corL5); //3
    //video_box(45, 37, 53, 43, corL5); //4
    video_box(55, 37, 63, 43, corL5); //5
    //video_box(65, 37, 73, 43, corL5); //6
    video_box(75, 37, 83, 43, corL5); //7
    //video_box(85, 37, 93, 43, corL5); //8
    //video_box(95, 37, 103, 43, corL5); //9
    video_box(105, 37, 113, 43, corL5); //10
    video_box(115, 37, 123, 43, corL5); //11
    //video_box(125, 37, 133, 43, corL5); //12
    video_box(135, 37, 143, 43, corL5); //13
    //video_box(145, 37, 153, 43, corL5); //14
    video_box(155, 37, 163, 43, corL5); //15
    //video_box(165, 37, 173, 43, corL5); //16
    video_box(175, 37, 183, 43, corL5); //17
    //video_box(185, 37, 193, 43, corL5); //18
    video_box(195, 37, 203, 43, corL5); //19
    //video_box(205, 37, 213, 43, corL5); //20
    //video_box(215, 37, 223, 43, corL5); //21
    video_box(225, 37, 233, 43, corL5); //22
    //video_box(235, 37, 243, 43, corL5); //23
    //video_box(245, 37, 253, 43, corL5); //24
    //video_box(255, 37, 263, 43, corL5); //25
    video_box(265, 37, 273, 43, corL5); //26
    //video_box(275, 37, 283, 43, corL5); //27
    //video_box(285, 37, 293, 43, corL5); //28
    video_box(295, 37, 303, 43, corL5); //29
    //video_box(305, 37, 313, 43, corL5); //30


    //PALAVRAS PLAY E QUIT
    //x1, y1, x2, y2
	//começa x
	//começa y
	//termina x
	//termina y
    int x_center =25;
    //LETRA P
    video_box(x_center + 93, 130, x_center + 95, 144, 0xFFFF); //traço de cima do P
    video_box(x_center + 96, 130, x_center + 104, 133, 0xFFFF);
    video_box(x_center + 105, 130,x_center +  107, 139, 0xFFFF);
    video_box(x_center + 96, 137, x_center + 104, 139, 0xFFFF);
    

    //LETRA L
    video_box(x_center + 110, 130,x_center +  112, 144, 0xFFFF);
    video_box(x_center + 112, 142,x_center +  124, 144, 0xFFFF);

    //LETRA A
    video_box(x_center + 127, 130,x_center +  129, 144, 0xFFFF); //traço de cima do P
    video_box(x_center + 130, 130,x_center +  138, 133, 0xFFFF);
    video_box(x_center + 139, 130,x_center +  141, 144, 0xFFFF);
    video_box(x_center + 130, 137,x_center +  138, 139, 0xFFFF);

    //LETRA Y
    video_box(x_center + 144, 130,x_center +  146, 132, 0xFFFF); //traço de cima do P
    video_box(x_center + 146, 133,x_center +  148, 135, 0xFFFF);
    video_box(x_center + 148, 136,x_center +  154, 138, 0xFFFF);
    video_box(x_center + 154, 133,x_center +  156, 135, 0xFFFF);
    video_box(x_center + 156, 130,x_center +  158, 132, 0xFFFF);
    video_box(x_center + 150, 139,x_center +  152, 144, 0xFFFF);

    //LETRA Q
    video_box(x_center + 101,152,x_center + 103,166, 0xFFFF); //traço deitado pequeno do
    video_box(x_center + 104, 152,x_center +  112, 154, 0xFFFF);
    video_box(x_center + 113, 152,x_center +  115, 166, 0xFFFF);
    video_box(x_center + 104, 164,x_center +  112, 166, 0xFFFF);
    video_box(x_center + 107, 167,x_center +  109, 168, 0xFFFF);
    video_box(x_center + 110, 169,x_center +  113, 170, 0xFFFF);


    //LETRA U
    video_box(x_center + 118, 152,x_center +  120, 166, 0xFFFF);
    video_box(x_center + 121, 164,x_center +  129, 166, 0xFFFF);
    video_box(x_center + 130, 152,x_center +  132, 166, 0xFFFF);

    //LETRA I
    video_box(x_center + 135, 152,x_center +  137, 166, 0xFFFF);

    //LETRA T
    video_box(x_center + 140, 152,x_center +  150, 154, 0xFFFF);
    video_box(x_center + 144, 155,x_center +  146, 166, 0xFFFF);


    printBlocosInferiores();

}

void printTelaParabens(){
    //x1, y1, x2, y2
    //começa x
    //começa y
    //termina x
    //termina y
    
    
    /*
    TELA PARABENS ANTIGA

    //LETRA P
    video_box(10, 20, 26, 24,0xFFFF); //traço de cima do P
	video_box(10,20,14,40,0xFFFF); //traco da perna do P esquerda
	video_box(26,25,30,32,0xFFFF); //traço em pé menor direito do P
	video_box(15,32,24,36,0xFFFF); //traço debaixo horizontal do P 

    //LETRA A
    video_box(46, 20, 57, 24,0xFFFF); //traço de cima do A
	video_box(42,24,45,40,0xFFFF); //traço em pé esquerdo do P 
	video_box(46, 30, 57, 34,0xFFFF); //traço em pé esquerdo do P
	video_box(53,24,56,40,0xFFFF); //traço deitado pequeno do P

    

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
    */

   /*
   TELA PARABENS ARAUJO VERSION
    //Letra P
    video_box(62,20,77,23);//Parte de cima do P
    video_box(62,20,65,39);//Parte da esquerda do P
    video_box(66,32,77,35);//Parte do Meio do P
    video_box(78,24,81,32);//Parte da direita do P

    //Letra A
    video_box(92,20,103,23);//Parte de cima do A 
    video_box(88,24,91,39);//Parte da esquerda do A
    video_box(91,30,103,33);//Parte do Meio do A
    video_box(104,24,107,39);//Parte da Direita do A

    // Letra R
    video_box(114,20,129,23);//Parte de cima do R
    video_box(114,20,117,39);//Parte de esquerda do R
    video_box(114,32,117,35);//Parte do Meio do R
    video_box(130,24,133,32);//Parte da direita em cima do R
    video_box(130,36,133,39);//Parte da direita em baixo do R

    // Letra A
    video_box(143,20,154,23);//Parte de cima do A 
    video_box(139,24,142,39);//Parte da esquerda do A
    video_box(139,30,158,33);//Parte do Meio do A
    video_box(155,24,158,39);//Parte da Direita do A

    // Letra B
    video_box(164,20,179,23);//Parte de cima do B
    video_box(164,20,167,39);//Parte da esqueda do B
    video_box(164,28,179,31);//Parte do meio do B
    video_box(180,24,183,27);//Parte direita do B
    video_box(180,32,183,35);//Parte debaixo do B direita

    //Letra E
    video_box(189,20,208,23);//Parte de cima do E
    video_box(189,20,192,39);//Parte Esquerda do E
    video_box(192,28,204,31);//Parte do Meio do E
    video_box(189,36,208,39);// Parte debaixo do E

    //Letra N
    video_box(214,20,217,39);//Parte Esquerda do N
    video_box(218,24,221,27);//Primeiro Retangulo do N
    video_box(222,28,225,31);//Segundo Retangulo do N
    video_box(226,32,229,35);//Terceiro Retangulo do N
    video_box(230,20,233,39);//Parte direita do N

    //Letra s
    video_box(243,20,258,23);//Parte de cima do S
    video_box(239,24,242,27);//Retagulo esquerdo do S
    video_box(243,28,254,31);//Parte do Meio do S
    video_box(255,32,258,35);//Retagulo direito do S
    video_box(239,36,254,39);//Parte debaixo do S
   */

    
    //PALAVRA PARABENS RHIAN VERSION
    int x_desloc =0;
    //LETRA P
    video_box(x_desloc +33, 20, x_desloc+ 36, 49,0xFFFF);
    video_box(x_desloc +37, 20, x_desloc+ 58, 23,0xFFFF);
    video_box(x_desloc +59, 20, x_desloc+ 62, 37,0xFFFF);
    video_box(x_desloc +37, 34, x_desloc+ 58, 37,0xFFFF);

    //LETRA A
    video_box(x_desloc +65, 20, x_desloc+ 68, 49,0xFFFF);
    video_box(x_desloc +69, 20, x_desloc+ 90, 23,0xFFFF);
    video_box(x_desloc +91, 20, x_desloc+ 94, 49,0xFFFF);
    video_box(x_desloc +69, 37, x_desloc+ 90, 40,0xFFFF);

    //LETRA R
    video_box(x_desloc +97, 20, x_desloc+ 100, 49, 0xFFFF);
    video_box(x_desloc +101, 20,x_desloc+  122, 23, 0xFFFF);
    video_box(x_desloc +123, 20,x_desloc+  126, 37, 0xFFFF);
    video_box(x_desloc +101, 34,x_desloc+  122, 37, 0xFFFF);
    video_box(x_desloc +101, 38,x_desloc+  105, 40, 0xFFFF);
    video_box(x_desloc +107, 41,x_desloc+  111, 43, 0xFFFF);
    video_box(x_desloc +112, 43,x_desloc+  116, 45, 0xFFFF);
    video_box(x_desloc +117, 45,x_desloc+  122, 47, 0xFFFF);
    video_box(x_desloc +123, 47,x_desloc+  126, 49, 0xFFFF);

    //LETRA A
    video_box(x_desloc +129, 20,x_desloc+  132, 49,0xFFFF);
    video_box(x_desloc +133, 20,x_desloc+  154, 23,0xFFFF);
    video_box(x_desloc +155, 20,x_desloc+  158, 49,0xFFFF);
    video_box(x_desloc +133, 37,x_desloc+  154, 40,0xFFFF);

    //LETRA B
    video_box(x_desloc +161, 20,x_desloc+  164, 49,0xFFFF);
    video_box(x_desloc +165 , 20,x_desloc+ 186, 23,0xFFFF);
    video_box(x_desloc +165, 32,x_desloc+  186, 36,0xFFFF);
    video_box(x_desloc +165, 46,x_desloc+  186, 49,0xFFFF);
    video_box(x_desloc +187, 24,x_desloc+  190, 31,0xFFFF);
    video_box(x_desloc +187, 37,x_desloc+  190, 45,0xFFFF);

    //LETRA E
    video_box(x_desloc +193, 20,x_desloc+  196, 49,0xFFFF);
    video_box(x_desloc +197, 20,x_desloc+  222, 23,0xFFFF);
    video_box(x_desloc +197, 32,x_desloc+  218, 36,0xFFFF);
    video_box(x_desloc +197, 46,x_desloc+  222, 49,0xFFFF);

    //LETRA N
    video_box(x_desloc +225, 20,x_desloc+  228, 49, 0xFFFF);
    video_box(x_desloc +229, 20,x_desloc+  230, 24, 0xFFFF);
    video_box(x_desloc +231, 22,x_desloc+  232, 28, 0xFFFF);
    video_box(x_desloc +233, 25,x_desloc+  234, 28, 0xFFFF);
    video_box(x_desloc +235, 29,x_desloc+  238, 32, 0xFFFF);
    video_box(x_desloc +239, 33,x_desloc+  242, 36, 0xFFFF);
    video_box(x_desloc +243, 37,x_desloc+  246, 40, 0xFFFF);
    video_box(x_desloc +247, 41,x_desloc+  248, 45, 0xFFFF);
    video_box(x_desloc +249, 43,x_desloc+  250, 247, 0xFFFF);
    video_box(x_desloc +251, 20,x_desloc+  254, 49, 0xFFFF);

    //LETRA S
    video_box(x_desloc +257, 20,x_desloc+  286, 23,0xFFFF);
    video_box(x_desloc +257, 24,x_desloc+  260, 31,0xFFFF);
    video_box(x_desloc +257, 32,x_desloc+  286, 36,0xFFFF);
    video_box(x_desloc +283, 37,x_desloc+  286, 45,0xFFFF);
    video_box(x_desloc +257, 46,x_desloc+  286, 49,0xFFFF);
    

    printBlocosInferiores();
}





void desenharBola(int x, int y, int raio) {
    int px, py;
    for (py = -raio; py <= raio; py++) {
        for (px = -raio; px <= raio; px++) {
            
            if (px * px + py * py <= raio * raio) {
                video_pixel(x + px, y + py, 0xFFFFFF); 
            }
        }
    }
}


int telaInicial(){
    int aux_while=1;
    int btn_data, retorno;
    while(aux_while){
        KEY_read(&btn_data);
        limparTela();
        
        //construir tela de inicio para apresentar antes de ir ao jogo
        printTelaInicial();
        video_show (); // swap Front/Back to display the line



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

void limparTela(){
    video_erase ( ); // erase any text on the screen
	video_clear ( ); // clear current VGA Back buffer
}

void gerar_bordas(){
    int larguraTela, alturaTela, char_x, char_y;
	video_read (&larguraTela, &alturaTela, &char_x, &char_y);
	
	video_box(1, 25,larguraTela-1, alturaTela-1, 0x8430);
	video_box(6, 31,larguraTela-6, alturaTela-6, 0x0000);
}





void palavra_score(int scoreInt, int shift_X, int shift_Y){
	//x1, y1, x2, y2
	//começa x
	//começa y
	//termina x
	//termina y

	//S
	video_box(8 + shift_X,3 + shift_Y,22 + shift_X,5 + shift_Y,0xFFFF); //perna alta do S
	video_box(8 + shift_X,6 + shift_Y,10 + shift_X,8 + shift_Y,0xFFFF); //perna comprima do S
	video_box(8 + shift_X,9 + shift_Y,22 + shift_X,11 + shift_Y,0xFFFF); //perna meio do S
	video_box(20 + shift_X,12 + shift_Y,22 + shift_X,14 + shift_Y,0xFFFF); //perna comprima do S
	video_box(8 + shift_X,15 + shift_Y,22 + shift_X,17 + shift_Y,0xFFFF); //perna alta do S

	//C
	video_box(25 + shift_X,3 + shift_Y,39 + shift_X,5 + shift_Y,0xFFFF); //lateral superior do C
	video_box(25 + shift_X,6 + shift_Y,27 + shift_X,14 + shift_Y,0xFFFF); //perna comprida do C
	video_box(25 + shift_X,15 + shift_Y,39 + shift_X,17 + shift_Y,0xFFFF); //lateral inferior do C

	//O
	video_box(42 + shift_X,3 + shift_Y,56 + shift_X,5 + shift_Y,0xFFFF); //lateral superior do O
	video_box(42 + shift_X,6 + shift_Y,44 + shift_X,14 + shift_Y,0xFFFF); //lateral direita do O
	video_box(54 + shift_X,6 + shift_Y,56 + shift_X,14 + shift_Y,0xFFFF); //lateral esquerda do O
	video_box(42 + shift_X,15 + shift_Y,56 + shift_X,17 + shift_Y,0xFFFF); //lateral inferior do O

	//R
	video_box(59 + shift_X,3 + shift_Y,73 + shift_X,5 + shift_Y,0xFFFF); //parte de cima do R
    video_box(59 + shift_X,6 + shift_Y,61 + shift_X,17 + shift_Y,0xFFFF); //lateral direita do R
    video_box(71 + shift_X,6 + shift_Y,73 + shift_X,9 + shift_Y,0xFFFF); //parte de cima do R
    video_box(62 + shift_X,10 + shift_Y,73 + shift_X,12 + shift_Y,0xFFFF); //parte de cima do R
    video_box(62 + shift_X, 13 + shift_Y, 67 + shift_X, 15+shift_Y, 0xFFFF);
    video_box(68 + shift_X, 15 + shift_Y, 73 + shift_X, 17+shift_Y, 0xFFFF);

	//E
	video_box(76 + shift_X,3 + shift_Y,90 + shift_X,5 + shift_Y,0xFFFF); //lateral superior do E
	video_box(76 + shift_X,6 + shift_Y,78 + shift_X,14 + shift_Y,0xFFFF); //lateral meio do E
	video_box(76 + shift_X,15 + shift_Y,90 + shift_X,17 + shift_Y,0xFFFF); //lateral inferior do E
	video_box(79 + shift_X,9 + shift_Y,87 + shift_X,11 + shift_Y,0xFFFF); //lateral direita do E

    // dois pontos
    video_box(93 + shift_X,4 + shift_Y,95 + shift_X, 7 + shift_Y,0xFFFF); //ponto superior
    video_box(93 + shift_X,13 + shift_Y,95 + shift_X,16 + shift_Y,0xFFFF); //ponto inferior
    
    printValorScore(scoreInt, shift_X, shift_Y);
    
    /*
    //PARTE QUE FAZ A IMPRESSAO DO VALOR DO SCORE
    char scoreChar[5]={0} ;
    itoa(scoreInt,scoreChar);
    //printf("\nmano, olha o score %s\n", scoreChar);
    video_text(70, 3, scoreChar);   
    */
}



//Função para converter inteiro em string
char *  itoa ( int value, char * str ){
    char temp;
    int i;
    i=0;
    while (value > 0) {
        int digito;
        digito = value % 10;
 
        str[i] = digito + '0';
        value /= 10;
        i++;
 
    }
   i = 0;
   int j;
   j = strlen(str);
    j -=1;
   while (i < j) {
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++;
      j--;
   }
    return str;
    /*
    REFERENCIA:
    https://pt.stackoverflow.com/questions/260415/convertendo-int-em-string
    */

}


void printTelaPerdeu(){
    int corG;
    int corA;
    int corM;
    int corE;
    int corO;
    int corV;
    int corE2;
    int corR;

    corG = 0xf021;
    corA = 0x0b98;
    corM = 0x968a;
    corE = 0xffc0;
    corO = 0x968a;
    corV = 0x0b98;
    corE2 = 0xf021;
    corR = 0xffc0;

    //PRIMEIRA LINHA 15 BLOCOS VERMELHOS
    //video_box(15, 5, 23, 11, corG); //1
    video_box(25, 5, 33, 11, corG); //2
    video_box(35, 5, 43, 11, corG); //3
    video_box(45, 5, 53, 11, corG); //4
    video_box(55, 5, 63, 11, corG); //5


    video_box(65, 5, 73, 11, corA); //6
    video_box(75, 5, 83, 11, corA); //7
    video_box(85, 5, 93, 11, corA); //8


    video_box(95, 5, 103, 11, corM); //9
    //video_box(105, 5, 113, 11, corM); //10
    //video_box(115, 5, 123, 11, corM); //11
    //video_box(125, 5, 133 11, corM); //12
    video_box(135, 5, 143, 11, corM); //13


    video_box(145, 5, 153, 11, corE); //14
    video_box(155, 5, 163, 11, corE); //15
    video_box(165, 5, 173, 11, corE); //16


    //video_box(175, 5, 183, 11, 0xF800); //17

    video_box(185, 5, 193, 11, corO); //18
    video_box(195, 5, 203, 11, corO); //19
    video_box(205, 5, 213, 11, corO); //20


    video_box(215, 5, 223, 11, corV); //21
    //video_box(225, 5, 233, 11, corV); //22
    video_box(235, 5, 243, 11, corV); //23


    video_box(245, 5, 253, 11, corE2); //24
    video_box(255, 5, 263, 11, corE2); //25
    video_box(265, 5, 273, 11, corE2); //26


    video_box(275, 5, 283, 11, corR); //27
    video_box(285, 5, 293, 11, corR); //28
    video_box(295, 5, 303, 11, corR); //29
    //video_box(305, 5, 313, 11, corR); //30



    //SEGUNDA LINHA 14 BLOCOS LARANJAS
    //video_box(15, 13, 23, 19, corG); //1
    video_box(25, 13, 33, 19, corG); //2
    //video_box(35, 13, 43, 19, corG); //3
    //video_box(45, 13, 53, 19, corG); //4
    //video_box(55, 13, 63, 19, corG); //5


    video_box(65, 13, 73, 19, corA); //6
    //video_box(75, 13, 83, 19, corA); //7
    video_box(85, 13, 93, 19, corA); //8


    video_box(95, 13, 103, 19, corM); //9
    video_box(105, 13, 113, 19, corM); //10
    //video_box(115, 13, 123, 19, corM); //11
    video_box(125, 13, 133, 19, corM); //12
    video_box(135, 13, 143, 19, corM); //13


    video_box(145, 13, 153, 19, corE); //14
    //video_box(155, 13, 163, 19, corE); //15
    //video_box(165, 13, 173, 19, corE); //16


    //video_box(175, 13, 183, 19, corO); //17
    video_box(185, 13, 193, 19, corO); //18 - deslocar para esquerda - V
    //video_box(195, 13, 203, 19, corO); //19
    video_box(205, 13, 213, 19, corO); //20


    video_box(215, 13, 223, 19, corV); //21
    //video_box(225, 13, 233, 19, corV); //22
    video_box(235, 13, 243, 19, corV); //23


    video_box(245, 13, 253, 19, corE2); //24
    //video_box(255, 13, 263, 19, corE2); //25
    //video_box(265, 13, 273, 19, corE2); //26


    video_box(275, 13, 283, 19, corR); //27
    //video_box(285, 13, 293, 19, corR); //28
    video_box(295, 13, 303, 19, corR); //29
    //video_box(305, 13, 313, 19, corR); //30
    


    //TERCEIRA LINHA 15 BLOCOS AMARELOS
    //video_box(15, 21, 23, 27, corG); //1
    video_box(25, 21, 33, 27, corG); //2
    //video_box(35, 21, 43, 27, corG); //3
    video_box(45, 21, 53, 27, corG); //4
    video_box(55, 21, 63, 27, corG); //5


    video_box(65, 21, 73, 27, corA); //6
    //video_box(75, 21, 83, 27, corA); //7
    video_box(85, 21, 93, 27, corA); //8


    video_box(95, 21, 103, 27, corM); //9
    //video_box(105, 21, 113, 27, corM); //10
    video_box(115, 21, 123, 27, corM); //11
    //video_box(125, 21, 133 27, corM); //12
    video_box(135, 21, 143, 27, corM); //13


    video_box(145, 21, 153, 27, corE); //14
    video_box(155, 21, 163, 27, corE); //15
    //video_box(165, 21, 173, 27, corE); //16


    //video_box(175, 21, 183, 27, corO); //17
    video_box(185, 21, 193, 27, corO); //18
    //video_box(195, 21, 203, 27, corO); //19
    video_box(205, 21, 213, 27, corO); //20


    video_box(215, 21, 223, 27, corV); //21
    //video_box(225, 21, 233, 27, corV); //22
    video_box(235, 21, 243, 27, corV); //23


    video_box(245, 21, 253, 27, corE2); //24
    video_box(255, 21, 263, 27, corE2); //25
    //video_box(265, 21, 273, 27, corE2); //26


    video_box(275, 21, 283, 27, corR); //27
    video_box(285, 21, 293, 27, corR); //28
    video_box(295, 21, 303, 27, corR); //29
    //video_box(305, 21, 313, 27, corR); //30



    //QUARTA LINHA 14 BLOCOS VERMELHOS
    //video_box(15,29, 23, 35, corG); //1
    video_box(25,29, 33, 35, corG); //2
    //video_box(35,29, 43, 35, corG); //3
    //video_box(45,29, 53, 35, corG); //4
    video_box(55,29, 63, 35, corG); //5


    video_box(65,29, 73, 35, corA); //6
    video_box(75,29, 83, 35, corA); //7
    video_box(85,29, 93, 35, corA); //8


    video_box(95,29, 103, 35, corM); //9
    //video_box(105,29, 113, 35, corM); //10
    //video_box(115,29, 123, 35, corM); //11
    //video_box(125,29, 133 35, corM); //12
    video_box(135,29, 143, 35, corM); //13


    video_box(145,29, 153, 35, corE); //14
    //video_box(155,29, 163, 35, corE); //15
    //video_box(165,29, 173, 35, corE); //16


    //video_box(175,29, 183, 35, corO); //17
    video_box(185,29, 193, 35, corO); //18 - deslocar a esquerda - V
    //video_box(195,29, 203, 35, corO); //19
    video_box(205,29, 213, 35, corO); //20


    video_box(215,29, 223, 35, corV); //21
    //video_box(225,29, 233, 35, corV); //22
    video_box(235,29, 243, 35, corV); //23


    video_box(245,29, 253, 35, corE2); //24
    //video_box(255,29, 263, 35, corE2); //25
    //video_box(265,29, 273, 35, corE2); //26


    video_box(275,29, 283, 35, corR); //27
    video_box(285,29, 293, 35, corR); //28
    //video_box(295,29, 303, 35, corR); //29
    //video_box(305,29, 313, 35, corR); //30
    
    //QUINTA LINHA 15 BLOCOS VERMELHOS
    //video_box(15, 37, 23, 43 corG); //1
    video_box(25, 37, 33, 43, corG); //2
    video_box(35, 37, 43, 43, corG); //3
    video_box(45, 37, 53, 43, corG); //4
    video_box(55, 37, 63, 43, corG); //5


    video_box(65, 37, 73, 43, corA); //6
    //video_box(75, 37, 83, 43, corA); //7
    video_box(85, 37, 93, 43, corA); //8


    video_box(95, 37, 103,43, corM); //9
    //video_box(105, 37, 113,43, corM); //10
    //video_box(115, 37, 123,43, corM); //11
    //video_box(125, 37, 133 43 corM); //12
    video_box(135, 37, 143,43, corM); //13


    video_box(145, 37, 153,43, corE); //14
    video_box(155, 37, 163,43, corE); //15
    video_box(165, 37, 173,43, corE); //16


    //video_box(175, 37, 183,43, corO); //17
    video_box(185, 37, 193,43, corO); //18
    video_box(195, 37, 203,43, corO); //19
    video_box(205, 37, 213,43, corO); //20


    //video_box(215, 37, 223,43, corV); //21
    video_box(225, 37, 233,43, corV); //22
    //video_box(235, 37, 243,43, corV); //23


    video_box(245, 37, 253,43, corE2); //24
    video_box(255, 37, 263,43, corE2); //25
    video_box(265, 37, 273,43, corE2); //26

    
    video_box(275, 37, 283,43, corR); //27
    //video_box(285, 37, 293,43, corR); //28
    video_box(295, 37, 303,43, corR); //29
    //video_box(305, 37, 313,43, corR); //30
    printBlocosInferiores();
}

int calcularScore(){
    int contador=0;
    int i=0;
    for(i=0; i< NUM_BLOCOS_X * NUM_BLOCOS_Y; i++){
        if(blocos[i].destruido){
            contador += 10;
        }
    }
    return contador;
}

void printBlocosInferiores(){
    //ULTIMA LINHA
    video_box(4, 226, 27, 239,0x07E0);
    video_box(31, 226, 58, 239,0x07E0);
    video_box(62, 226, 89, 239,0x07E0);
    video_box(93, 226, 120, 239,0x07E0);
    video_box(124, 226, 151, 239,0x07E0);
    video_box(155, 226, 182, 239,0x07E0);
    video_box(186, 226, 213, 239,0x07E0);
    video_box(217, 226, 244, 239,0x07E0);
    video_box(248, 226, 275, 239,0x07E0);
    video_box(279, 226, 306, 239,0x07E0);
    
    //PENULTIMA LINHA
    video_box(4, 211, 27, 224,0x97e0); //1
    video_box(31, 211, 58, 224,0x97e0); //2
    video_box(62, 211, 89, 224,0x97e0); //3
    video_box(93, 211, 120, 224,0x97e0); //4
    

    video_box(186, 211, 213, 224,0x97e0); //7
    video_box(217, 211, 244, 224,0x97e0); //8
    video_box(248, 211, 275, 224,0x97e0); //9
    video_box(279, 211, 306, 224,0x97e0); //10

    //ANTEPENULTIMA LINHA
    video_box(4, 196, 27, 209,0xFFE0); //1
    video_box(31, 196, 58, 209,0xFFE0); //2
    video_box(62, 196, 89, 209,0xFFE0); //3

    
    

    video_box(217, 196, 244, 209,0xFFE0); //8
    video_box(248, 196, 275, 209,0xFFE0); //9
    video_box(279, 196, 306, 209,0xFFE0); //10

    //ANTEANTEPENULTIMA LINHA
    video_box(4, 181, 27, 194,0xFC00); //1
    video_box(31, 181, 58, 194,0xFC00); //2


    
    


    video_box(248, 181, 275, 194,0xFC00); //9
    video_box(279, 181, 306, 194,0xFC00); //10


    //PRIMEIRA LINHA
    video_box(4,166, 27, 179,0xF800); //1


    
    



    video_box(279, 166, 306, 179,0xF800); //10

}

void numeroOne(int x, int y, short cor){
    video_box( 1+x, 7+y, 3+x, 9+y, cor);
    video_box( 4+x, 4+y, 6+x, 6+y, cor);
    video_box( 7+x, 1+y, 9+x, 15+y, cor);
}

void numeroTwo(int x, int y, short cor){
    video_box( 1+x, 1+y, 11+x, 3+y, cor);
    video_box( 9+x, 4+y, 11+x, 6+y, cor);
    video_box( 4+x, 7+y, 8+x, 9+y, cor);
    video_box( 1+x, 10+y, 3+x, 12+y, cor);
    video_box( 1+x, 13+y, 11+x, 15+y, cor);
}

void numeroThree(int x, int y, short cor){
    video_box( 1+x, 1+y, 11+x, 3+y, cor);
    video_box( 9+x, 1+y, 11+x, 15+y, cor);
    video_box( 3+x, 7+y, 11+x, 9+y, cor);
    video_box( 1+x, 13+y, 11+x, 15+y, cor);
}

void numeroFour(int x, int y, short cor){
    video_box( 9+x, 1+y, 11+x, 15+y, cor);
    video_box( 7+x, 2+y, 8+x, 3+y, cor);
    video_box( 4+x, 4+y, 6+x, 6+y, cor);
    video_box( 1+x, 7+y, 3+x, 12+y, cor);
    video_box( 4+x, 10+y, 8+x, 12+y, cor);

}

void numeroFive(int x, int y, short cor){
    video_box( 1+x, 1+y, 11+x, 3+y, cor);
    video_box( 1+x, 4+y, 3+x, 6+y, cor);
    video_box( 1+x, 7+y, 11+x, 9+y, cor);
    video_box( 9+x, 10+y, 11+x, 12+y, cor);
    video_box( 1+x, 13+y, 11+x, 15+y, cor);
}

void numeroSix(int x, int y, short cor){
    video_box( 1+x, 1+y, 11+x, 3+y, cor);
    video_box( 1+x, 4+y, 3+x, 15+y, cor);
    video_box( 4+x, 7+y, 11+x, 9+y, cor);
    video_box( 9+x, 10+y, 11+x, 12+y, cor);
    video_box( 4+x, 13+y, 11+x, 15+y, cor);

}

void numeroSeven(int x, int y, short cor){
    video_box( 1+x, 1+y, 11+x, 3+y, cor);
    video_box( 8+x, 4+y, 10+x, 8+y, cor);
    video_box( 5+x, 8+y, 7+x, 11+y, cor);
    video_box( 2+x, 11+y, 4+x, 15+y, cor);

}

void numeroEight(int x, int y, short cor){
    video_box( 1+x, 1+y, 3+x, 15+y, cor);
    video_box( 4+x, 1+y, 8+x, 3+y, cor);
    video_box( 9+x, 1+y, 11+x, 15+y, cor);
    video_box( 4+x, 8+y, 8+x, 9+y, cor);
    video_box( 4+x, 13+y, 8+x, 15+y, cor);
}

void numeroNine(int x, int y, short cor){
    video_box( 1+x, 1+y, 3+x, 9+y, cor);
    video_box( 4+x, 1+y, 8+x, 3+y, cor);
    video_box( 4+x, 7+y, 8+x, 9+y, cor);
    video_box( 9+x, 1+y, 11+x, 15+y, cor);
    video_box( 1+x, 13+y, 8+x, 15+y, cor);

}

void numeroZero(int x, int y, short cor){
    video_box( 1+x, 1+y, 3+x, 15+y, cor);
    video_box( 9+x, 1+y, 11+x, 15+y, cor);
    video_box( 4+x, 1+y, 8+x, 3+y, cor);
    video_box( 4+x, 13+y, 8+x, 15+y, cor);
    video_box( 4+x, 9+y, 5+x, 10+y, cor);
    video_box( 5+x, 7+y, 6+x, 8+y, cor);
    video_box( 7+x, 5+y, 8+x, 6+y, cor);
}

void printValorScore(int Score, int desloc_X, int desloc_Y){

    int digito1 = 0;
    int digito2 = 0;
    int digito3 = 0;
    short cor = 0xFFFF;

    digito1 = Score/100;
    digito2 = (Score%100)/10;
    digito3 = (Score%100)%10;

    if(Score >100 && Score <200){
        cor = 0xfde0;

    } else if(Score >=200 && Score <300){
        cor = 0xfc80;


    } else if(Score >=300 && Score <400){
        cor = 0xfae0;

    } else if(Score >=400 && Score <500){
        cor = 0xf800;

    } else{ 
        cor = 0xfec0;
    }   


    switch (digito1){
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

    switch (digito2){
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

    switch (digito3){
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


void renderizarBlocos(){
    int x1; int y1; int x2; int y2;
    int p, a;
    for (p = 0; p < NUM_BLOCOS_X; p++) {
        for (a = 0; a < NUM_BLOCOS_Y; a++) {
            if (!blocos[p * NUM_BLOCOS_Y + a].destruido) {
                int x1 = blocos[p * NUM_BLOCOS_Y + a].x;
                int y1 = blocos[p * NUM_BLOCOS_Y + a].y;
                int x2 = x1 + blocos[p * NUM_BLOCOS_Y + a].largura;
                int y2 = y1 + blocos[p * NUM_BLOCOS_Y + a].altura;
                
                // Para olhar se os blocos estão dentro dos limites da tela
                if (x1 >= 0 && x2 <= screen_x && y1 >= 0 && y2 <= screen_y) {
                    video_box(x1, y1, x2, y2, blocos[p * NUM_BLOCOS_Y + a].cor);
                }
            }
        }
    }
}

int blocosAtivos(){
    int contador=0;
    int i=0;
    for(i=0; i< NUM_BLOCOS_X * NUM_BLOCOS_Y; i++){
        if(blocos[i].destruido){
            contador ++;
        }
    }
    if(contador == (NUM_BLOCOS_X * NUM_BLOCOS_Y)){
        return 1; //sem blocos ativos
    } else{
        return 0; //tem blocos ativos
    }
}

