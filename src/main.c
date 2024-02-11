#include <stdio.h>
#include <intelfpgaup/accel.h>
#include <intelfpgaup/video.h>

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
    int a=1; int b=2;


    ccel_open();
    video_open();
    accel_init();
    video_read (&larguraTela, &alturaTela, &char_x, &char_y); // get screen & text size
    accel_format(a, b);
    accel_calibrate();
    x1=screen_x - 310;
	y1 = screen_y;

    video_clear();
    gerar_bordas();
    inicializarBlocos();
    renderBlocos();
    video_show();

    while (1) {
        moverRaquete();


    }
	accel_close();
    video_close();

    return 0;
}


void moverRaquete() {
    
    int ptr_x;
    // Não sei exatamente se pode colocar null, caso não possa é só 
    // adicionar as outras variaveis indicadas.
    accel_read(NULL, NULL, NULL, &ptr_X, NULL, NULL, NULL);

    posicaoRaqueteX += ptr_X 
    video_box (x1, y1-10, x1 +40, y1-7, SlateBlue3);

    if (posicaoRaqueteX < 313) {
        posicaoRaqueteX = 273;
    } else if (x1<7) {
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
    video_line(64,13,84,23,0xFFFF);

	//E
	video_box(86,3,106,7,0xFFFF); //lateral superior do E
	video_box(86,11,106,15,0xFFFF); //lateral meio do E
	video_box(86,19,106,23,0xFFFF); //lateral inferior do E
	video_box(86,3,90,23,0xFFFF); //lateral direita do E
}

/*
Serve para poder apresentar na tela o texto de pause
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
}

/*
realizar abertura e configuração dos hardwares a serem utilizados
*/
void iniciar_hardwares(){
	int larguraTela, alturaTela, char_x, char_y;
	int resolucao = 1;
	int range = 16;
	accel_open();
    video_open();
    accel_init();
    video_read (&larguraTela, &alturaTela, &char_x, &char_y); // get screen & text size
    accel_format(resolucao, range);
    accel_calibrate();
}
