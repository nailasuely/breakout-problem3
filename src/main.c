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


void gerar_bola_quadrada(int x, int y){
	video_box(x, y, x+10, y+10, 0xFFFFFF);
	video_box(100, 100, 130, 110, 0xFFFFFF);
	video_box(200, 200, 230, 207, 0xFFFFFF);
	video_box(232, 200, 264, 207, 0xFFFFFF);
	video_box(166, 200, 198, 207, 0xFFFFFF);
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
	video_read (&larguraTela, &alturaTela, &char_x, &char_y);
	
	video_box(1, 1,larguraTela-1, alturaTela-1, 0xFF00);
	video_box(6, 6,larguraTela-6, alturaTela-6, 0x000000);
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
