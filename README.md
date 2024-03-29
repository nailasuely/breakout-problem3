
<h1 align="center">
  <br>
 <img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/logo%20(2).png" alt="Breakout" width="250"></a>
  <br>
  Breakout
  <br>
</h1>


<h4 align="center">Projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais.</h4>

<p align="center">
<div align="center">

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/nailasuely/breakout-problem3/blob/main/LICENSE)

<img width="800px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/gif1.gif">


> Esse é um projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais, no qual ocorre o desenvolvimento de um jogo inspirado no clássico breakout. Este utiliza o acelerômetro do DE1-SoC para controlar o jogador e botões para funções de jogo e a visualização é feita através da interface VGA em monitor CRT.

## Download do repositório


```
gh repo clone nailasuely/breakout-problem3
```

</div>

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)

## Sumário
- [Visão Geral do Projeto](#visao-geral-do-projeto)
- [Tecnologias e Ferramentas Utilizadas](#Tecnologias-e-Ferramentas-Utilizadas)
- [Arquitetura do Kit de Desenvolvimento DE1-SoC](#Arquitetura-do-Kit-de-Desenvolvimento-DE1-SoC)
  -  [Visão Geral sobre comunicação de vídeo](#Visão-Geral-sobre-comunicação-de-vídeo)
  - [Funções de vídeo utilizadas](#Funções-de-vídeo-utilizadas)
  - [Acelerômetro ADXL345](#Acelerômetro-ADXL345)
  - [Funções para controle do Acelerômentro](#Funções-para-controle-do-Acelerômentro)
- [Descrição do Desenvolvimento em C](#Descrição-do-Desenvolvimento-em-C)
- [Regras do Jogo](#Regras-do-Jogo)
- [Executando o Projeto](#executando-o-projeto)
- [Testes](#testes)
- [Considerações Finais](#Considerações-Finais) 
- [Tutor](#tutor)
- [Equipe](#equipe)
- [Referências](#referências)

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)


## Visão Geral do Projeto
O atual documento descreve o desenvolvimento de um jogo inspirado no clássico "Breakout", popularizado na década de 1970 e produzido por Nolan Bushnell e Steve Bristow na empresa de videogames Atari. Este jogo foi desenvolvido como parte de um projeto prático, utilizando a placa de desenvolvimento DE1-SoC, que integra uma FPGA e recursos como acelerômetro e interface VGA para visualização em um monitor CRT.

O objetivo do jogo é ajudar a equipe a programar em C para arquiteturas ARM e entender as instruções para usá-las no kit de desenvolvimento. Assim, o jogador controla uma barra na parte inferior da tela para rebater uma bola em direção a blocos coloridos organizados no topo da tela. O maior desafio é eliminar todos os blocos sem deixar a bola cair abaixo da barra inferior.

Para implementar o jogo, foram utilizadas técnicas de programação em linguagem C, aproveitando os recursos disponíveis na placa DE1-SoC e suas bibliotecas associadas. O desenvolvimento envolveu a manipulação de entrada do acelerômetro para controlar o movimento da barra, e também a geração de gráficos na tela usando o módulo de vídeo da placa.

Neste README, serão detalhados os aspectos técnicos do jogo, desde a inicialização do ambiente de desenvolvimento até a implementação das funcionalidades principais, bem como os desafios enfrentados durante o processo de criação e as soluções escolhidas pela equipe. Além disso, também serão discutidas as possíveis melhorias para futuras versões do projeto.

A respeito do codigo desenvolvido, é possível acessar sua documentação seja por meio do pdf, [latex__1_.pdf](https://github.com/nailasuely/breakout-problem3/blob/main/src/doc/latex__1_.pdf), ou por meio de acesso utilizando o browser, em que se torna mais dinamico, contudo é necessario estar com o repositorio salvo na maquina, ou baixar apenas a [pasta](https://github.com/nailasuely/breakout-problem3/tree/main/src/doc/html) referente a documentação do codigo e abrir o arquivo [`index.html`](https://github.com/nailasuely/breakout-problem3/blob/main/src/doc/html/index.html).

<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/estrutura%20(2).png">

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)

## Tecnologias e Ferramentas Utilizadas
- Linguagem de Programação: C
- Softwares Utilizados:
  - Compilador GCC
  - Git para controle de versão
- Terminal Linux para execução de comandos
- Kit de Desenvolvimento DE1-SoC
- Monitor CRT
- Visual Studio Code

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)

## Arquitetura do Kit de Desenvolvimento DE1-SoC
<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/kit.jpg">

O [Kit de Desenvolvimento DE1-SoC](https://www.terasic.com.tw/cgi-bin/page/archive_download.pl?Language=China&No=836&FID=ae336c1d5103cac046279ed1568a8bc3) é uma plataforma de design de hardware construída em torno do FPGA System-on-Chip (SoC) da Altera. Este SoC integra os mais recentes núcleos embarcados Cortex-A9 dual-core, combinados com um sistema de processador baseado em ARM (HPS). Essa combinação oferece uma variedade de recursos, incluindo processador, periféricos e interfaces de memória, todos conectados à matriz FPGA através de uma espinha dorsal de alta largura de banda.

Para gerar a imagem na tela, o jogo utiliza o módulo de vídeo da placa DE1-SoC, que é capaz de gerar gráficos em resolução VGA. A placa também possui 6 PLLs fracionários que podem ser usados para gerar diferentes frequências de clock para os diferentes componentes do sistema. Para controle externo, o jogo utiliza o acelerômetro da placa DE1-SoC, que permite ao jogador controlar a barra movendo a placa. A placa também possui 2 controladores de memória rígidos que podem ser usados para conectar dispositivos de armazenamento externos, como um cartão SD. No caso do projeto é utilizado um cartão SD carregado com um sistema operacional.

O DE1-SoC é equipado com memória DDR3 de alta velocidade, capacidades de vídeo e áudio, rede Ethernet e muito mais. O sistema oferece uma variedade de componentes e interfaces. Abaixo está uma visão detalhada dos componentes-chave presentes no DE1-SoC:

<div align="center">
  
| Componente               | Descrição                                                                                                                                                                             |
|--------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Dispositivo FPGA         | Dispositivo Cyclone V SoC 5CSEMA5F31, Núcleo duplo ARM Cortex-A9 (HPS), 85K elementos lógicos programáveis, 4.450 Kbits de memória embutida, 6 PLLs fracionais, 2 controladores de memória |
| Configuração e Depuração | Dispositivo de configuração serial quádrupla - EPCQ256 no FPGA, USB-Blaster II embarcado (conector USB tipo B normal)                                                                   |
| Dispositivo de Memória   | 64MB (32Mx16) SDRAM no FPGA, 1GB (2x256Mx16) DDR3 SDRAM no HPS, soquete para cartão Micro SD no HPS                                                                                   |
| Comunicação              | Duas portas USB 2.0 (interface ULPI com conector USB tipo A), UART para USB (conector USB Mini-B), Ethernet 10/100/1000, mouse/teclado PS/2, emissor/receptor IR, multiplexador I2C |
| Conectores               | Dois cabeçalhos de expansão de 40 pinos, um cabeçalho de entrada ADC de 10 pinos, um conector LTC (uma interface SPI Mestre, uma I2C e uma interface GPIO)                           |
| Display                  | Conversor digital-analógico (DAC) VGA de 24 bits                                                                                                                                      |
| Áudio                    | CODEC de 24 bits, Entrada/Saída de linha e entrada de microfone                                                                                                                       |
| Entrada de Vídeo         | Decodificador de TV (NTSC/PAL/SECAM) e conector de entrada de TV                                                                                                                      |
| Conversor Analógico-Digital (ADC) | Taxa de transferência rápida: 1 MSPS, Número de canais: 8, Resolução: 12 bits, Faixa de entrada analógica: 0 ~ 2,5 V ou 0 ~ 5V, conforme selecionado pelo bit de FAIXA no registro de controle |
| Chaves, Botões e Indicadores | 5 teclas de usuário (FPGA x4, HPS x1), 10 interruptores de usuário (FPGA x10), 11 LEDs de usuário (FPGA x10, HPS x 1), 2 botões de reset HPS (HPS_RESET_n e HPS_WARM_RST_n), seis displays de 7 segmentos |
| Sensores                 | G-Sensor no HPS                                                                                                                                                                       |
| Alimentação              | Entrada de 12V DC                                                                                                                                                                     |


<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/diagram1.png">


</div>
<div>

### Visão Geral sobre comunicação de vídeo

  
O controlador do buffer de pixels desempenha um papel crucial na exibição de dados de pixels na tela VGA, oferecendo uma interface entre o sistema e a exibição gráfica. Ele compreende um conjunto de registradores, incluindo os vitais Buffer e Backbuffer que são representados na figura abaixo, que armazenam os endereços iniciais dos buffers de pixels. Inicialmente, o registrador Buffer é configurado para o endereço padrão 0xC8000000, representando a memória on-chip do FPGA, o que também é refletido no Backbuffer, indicando um único buffer de pixels.


<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/pixelbuffer.jpg">


A intervenção do software permite a criação de um segundo buffer de pixels ao modificar o endereço do registrador Backbuffer. Essa adição proporciona uma área adicional para operações de desenho, permitindo a manipulação de imagens por meio da escrita nos endereços de pixels.

Uma funcionalidade crítica do controlador é a troca de buffer de pixels, iniciada escrevendo-se o valor 1 no registrador Buffer. Isso desencadeia uma troca entre os conteúdos dos registradores Buffer e Backbuffer. No entanto, a troca é diferida até a conclusão de um ciclo de desenho da tela VGA, alinhando-se com o tempo de sincronização vertical que ocorre a cada 1/60 segundos.

Para monitorar o término da troca, o software pode verificar o valor do bit S no registrador de Status (endereço 0xFF20302C). Quando o registrador Buffer é definido como 1, o bit S é elevado, indicando uma troca pendente. Após a conclusão da troca, o bit S é redefinido para 0.

Além disso, o registrador de Status oferece informações adicionais, como os bits m e n, que especificam o número de bits de endereço y e x da VGA, respectivamente. Os bits BS indicam o tamanho do pixel, com um valor de 15 para um tamanho de pixel de dois bytes.

A interface de programação inclui um registrador de resolução, que armazena as resoluções X e Y dos buffers de pixels. No contexto específico, a resolução padrão é de 320 x 240 pixels, sendo uma característica importante para o desenvolvimento de gráficos e aplicativos de exibição de imagens.

É importante observar que a resolução do monitor VGA é dobrada, o que significa que cada pixel é replicado tanto horizontal quanto verticalmente para atender à resolução do monitor.

### Funções de vídeo utilizadas

A biblioteca video_h, fornecida pela [FPGACADEMY.ORG](https://fpgacademy.org/), desempenha um papel crucial na renderização dos elementos visuais do jogo no monitor CRT. A inclusão do cabeçalho #include <intelfpgaup/video.h> no código-fonte do jogo, permite o acesso a uma série de funções que simplificam a manipulação de gráficos. No código fonte do projeto foram atualizadas:
 
1. `video_open()`: serve para abrir o dispositivo de vídeo VGA e  configura os parâmetros necessários para a comunicação com o monitor.

2. `video_clear()`: permite limpar a tela de todos os elementos visuais existentes. Isso é útil para garantir que cada quadro do jogo comece com uma tela limpa, pronta para a renderização dos elementos do próximo cenário do jogo.

3. `video_pixel(x, y, color)`: permite desenhar pixels individuais na tela. Isso é útil para criar detalhes precisos ou manipular elementos gráficos. 

4. `video_box(x1, y1, x2, y2, color)`: podemos desenhe caixas preenchidas na tela. Essa função é frequentemente utilizada para representar elementos como paredes, objetos sólidos ou áreas delimitadas.

5. `video_text(x, y, string)`: nos permite adicionar texto à tela em uma posição específica. Isso é fundamental para exibir informações importantes, como pontuações, instruções ou mensagens de status.

6. `video_close()`: para encerrar a comunicação com o dispositivo de vídeo VGA e libera os recursos utilizados. 



### Acelerômetro ADXL345

O Acelerômetro Digital ADXL345 fornece uma visão abrangente deste sistema de medição de aceleração de 3 eixos. Sua estrutura de micromaquinagem de superfície de polissilício permite faixas de medição selecionáveis de ±2 g, ±4 g, ±8 g ou ±16 g. Essa capacidade de detectar tanto aceleração dinâmica quanto estática o torna útil como sensor de inclinação.

A construção do sensor apresenta molas de polissilício que suspendem a estrutura sobre uma pastilha de silício, resistindo às forças resultantes da aceleração aplicada. A deflexão da estrutura é medida por meio de capacitores diferenciais, cujo desbalanceamento devido à aceleração resulta em uma saída proporcional. A demodulação sensível à fase é usada para determinar a magnitude e a polaridade da aceleração.

O ADXL345 oferece baixo consumo de energia, resolução selecionável, e funções especiais como detecção de toque único/duplo, monitoramento de atividade/inatividade e detecção de queda livre. Além disso, possui uma ampla faixa de tensão de alimentação e interfaces digitais flexíveis.

[O arquivo PDF do ADXL345](https://github.com/nailasuely/breakout-problem3/blob/main/assets/files/ADXL345.pdf) fornece um diagrama de blocos funcional detalhado, delineando os componentes do sensor, eletrônicos de sensor, filtro digital, gerenciamento de energia, controle, lógica de interrupção e componentes de E/S serial.

### Funções para controle do Acelerômentro

1. `accel_open()`: Inicia o dispositivo do acelerômetro 3D e realiza configurações iniciais necessárias para a comunicação e operação adequada do sensor.

2. `accel_read(ready, tap, dtap, x, y, z, mg_per_lsb)`: Realiza a leitura dos dados do acelerômetro 3D. Os parâmetros de saída incluem:
   - `ready`: sinaliza se novos dados estão prontos (1 se houver novos dados, caso contrário, 0).
   - `tap`: sinaliza se ocorreu um evento de toque (1 se houver um toque, caso contrário, 0).
   - `dtap`: sinaliza se ocorreu um evento de duplo toque (1 se houver um duplo toque, caso contrário, 0).
   - `x`: aceleração nos eixos x.
   - `y`: aceleração nos eixos y.
   - `z`: aceleração nos eixos z.
   - `mg_per_lsb`: fator de escala dos dados de aceleração.
   
3. `accel_init()`: Inicializa o dispositivo do acelerômetro 3D, preparando-o para a leitura de dados.

4. `accel_calibrate()`: Realiza a calibragem do dispositivo do acelerômetro 3D, garantindo a precisão dos dados de aceleração fornecidos.

5. `accel_device()`: Solicita a impressão do identificador do dispositivo do acelerômetro, útil para diagnósticos e identificação do sensor.

6. `accel_format(full, range)`: Define o formato dos dados de aceleração, incluindo o alcance completo e a faixa de valores suportados.

7. `accel_rate(rate)`: Define a taxa de dados de aceleração, especificando a frequência em Hz na qual os dados são fornecidos.

8. `accel_close()`: Encerra a comunicação com o dispositivo do acelerômetro 3D e libera os recursos associados.



</div>

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)
## Descrição do Desenvolvimento em C

### Mover raquete
<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/moveRaquete.svg">

A função `moverRaquete()` é responsável por controlar o movimento da raquete no jogo, permitindo que o jogador interaja com o Kit DE1-SoC, como demonstrado no gif a seguir:

<div align="center">
  
<img width="500px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/gif_Exemplo.gif">

</div>

O processo de movimentação da raquete inicia-se com a leitura dos dados do acelerômetro. Uma vez lidos os dados do acelerômetro, o programa utiliza as informações para determinar a direção do movimento da raquete. No caso da função `moverRaquete()`, o movimento horizontal da raquete é controlado pelo valor da variável `ptr_x`, que representa a inclinação do Kit para a esquerda ou direita.

Com base nesse valor, a função ajusta a posição horizontal da raquete (`posicaoRaqueteX`) de acordo com a inclinação detectada. O movimento é realizado somando o valor lido do acelerômetro dividido por 10 à posição atual da raquete.

É importante mencionar que o valor de `y1`, utilizado na função `video_box()` para desenhar a raquete na tela, pode ser ajustado para melhorar os valores visuais do jogo, como a posição vertical inicial da raquete.

Além disso, há uma verificação para garantir que a raquete permaneça dentro dos limites da tela do jogo. A raquete tem uma largura de 40 pixels. Assim, a posição horizontal da raquete é limitada para não ultrapassar os limites da tela:

- Se `posicaoRaqueteX` for maior que 272, ela é ajustada para 272 para evitar que a raquete ultrapasse o limite direito da tela.
- Se `posicaoRaqueteX` for menor que 8, ela é ajustada para 8 para evitar que a raquete ultrapasse o limite esquerdo da tela.

Essas verificações são feitas para que a raquete permaneça visível na tela do jogo e que o jogador possa controlá-la dentro dos limites.

 ### Detectar colisão raquete
 
 <img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/colisaoRaquete.svg">
 
A função “detectarColisaoRaquete()" desempenha o papel de determinar se houve uma colisão entre a bola do jogo e a raquete. Quando o jogador inclina o Kit para a esquerda ou direita, o acelerômetro detecta esses movimentos e os converte em sinais elétricos que são interpretados pelo sistema digital. Esses sinais são então utilizados para deslocar a raquete horizontalmente na tela do jogo.

Na iniciação, ocorre a definição das variáveis para representar os limites da raquete e da bola. As coordenadas da raquete são determinadas pela sua posição atual, enquanto as coordenadas da bola são calculadas com base em sua posição inicial. Em seguida, ocorre a verificação da possível colisão entre a bola e a raquete. Esta verificação é feita comparando as coordenadas da bola com as coordenadas da raquete. Se a bola atinge os limites horizontais e verticais da raquete, isso indica uma colisão entre os dois elementos.

Para ilustrar o funcionamento detalhado da função “detectarColisaoRaquete()”, o fluxograma ilustra as etapas pelas quais o programa passa durante a execução da função, desde a verificação das coordenadas até a atualização da velocidade da bola.

### Inicializar Blocos

Essa função é utilizada para dispor os blocos na tela de maneira adequada. Durante a inicialização, a função percorre uma matriz de blocos com dimensões definidas pela quantidade de blocos na vertical e na horizontal. Cada bloco é caracterizado por sua posição X e Y, largura e altura, cor, e se foi destruído ou não.  O posicionamento dos blocos é calculado de acordo com a largura e altura de cada bloco, o espaçamento entre eles e as margens da tela para garantir a uniformidade deles na tela. 

Cada bloco recebe uma cor única. A variação de cores é realizada com base na posição do bloco na matriz, para proporcionar um efeito visual de degradê. As cores são definidas gradativamente através da variação entre três cores base (cor1, cor2, cor3), que são mescladas de acordo com a posição do bloco em relação ao total de blocos em uma linha.


### Renderizar blocos
 <img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/renderBlocos.svg">
Ao iniciar o processo de renderização  dos blocos, a função percorre cada posição na matriz de blocos do jogo. O percurso é feito seguindo um padrão de linha por linha, começando do canto superior esquerdo do cenário e indo de cima para baixo e da esquerda para a direita.

Para cada bloco que a função encontra, ela verifica se ele ainda não está destruído. Se o bloco estiver inteiro, a função prossegue para desenhar ele na tela. Antes de desenhar cada bloco, a função verifica se as coordenadas do bloco estão dentro dos limites da tela do jogo para assegurar que os blocos sejam desenhados apenas nas áreas visíveis ao jogador, evitando que partes dos blocos fiquem fora da visualização.

Ao desenhar um bloco na tela, a função utiliza as informações sobre as cores e posições do bloco em questão. Isso significa que cada bloco colorido será apresentado no local apropriado na tela. 

Durante a jogo, conforme a bola colide com os blocos e eles são destruídos, a função renderizarBlocos() é novamente chamada para atualizar a tela e não desenhar novamente os blocos destruídos. É um processo dinâmico para manter o cenário do jogo sendo atualizado, lembrando da taxa de atualização da tela. 




### Detectar colisão com os blocos
 <img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/colisaoBloco.svg">

A função detectarColisao() é responsável por verificar se ocorreu uma colisão entre a bola do jogo e os blocos que compõem o cenário para determinar como a bola deve reagir ao tocar em um bloco.

O processo de detecção de colisão ocorre em cada iteração do loop, onde são verificados todos os blocos presentes no cenário do jogo. Para cada bloco não destruído, são calculadas as coordenadas de suas bordas esquerda, direita, superior e inferior. Em seguida, é verificado se as coordenadas da próxima posição da bola estão dentro dos limites do bloco.

Se a bola atinge ou ultrapassa as bordas de um bloco, isso indica que aconteceu uma colisão. Assim, o bloco é marcado como destruído e a direção da bola é invertida de acordo com a borda atingida. Por exemplo, se a bola colide com a borda superior do bloco, sua direção vertical (velocidadePadraoBolaY) é invertida para simular a inversão da bola para baixo. O mesmo ocorre para as outras direções. 

### Main
 <img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/mainC.svg">


O ponto de partida do jogo Breakout é a `main`, onde todas as operações principais são coordenadas e executadas. Nesta função, o jogo é inicializado, os elementos visuais são renderizados na tela e a interação com o jogador é tratada.

Antes de qualquer coisa, é muito importante garantir que todos os dispositivos de hardware estão devidamente configurados. Para isso, é chamada a função `iniciar_hardwares()` que é responsável por verificar se os dispositivos de aceleração 3D (3D-acelerômetro accel), vídeo VGA (vídeo), e teclado (KEY) foram abertos corretamente.

Se algum dos dispositivos não puder ser iniciado corretamente, uma mensagem de erro é exibida informando que não foi possível iniciar os dispositivos de hardware, e o programa é encerrado.

Após a confirmação da verificação bem sucedida, o jogo avança para a tela inicial, onde o jogador pode decidir iniciar o jogo, pausar, ou sair. Caso o jogador escolha iniciar, a tela é limpa e os blocos do jogo são inicializados utilizando a função `inicializarBlocos()`. Assim, o jogo entra em um loop principal que continua enquanto houver blocos ativos no ou o jogador não pressiona o botão específico para sair.

Durante cada iteração deste loop, são realizadas as seguintes operações:

1. **Leitura das Teclas**
   
A função `KEY_read(&btn_data)` é responsável por ler o estado das teclas pressionadas.

3. **Atualização da Tela**
   
As funções de renderização da tela são chamadas para desenhar os elementos visuais:
- As bordas do cenário são geradas com a função `gerar_bordas()`.
- A pontuação do jogador é calculada e exibida na tela com as funções `calcularScore()` e `palavra_score()`.
- O movimento da raquete é controlado pela função `moverRaquete()`.
- É verificada a ocorrência de colisão entre a bola e os blocos com `detectarColisao()`.
- A colisão entre a bola e a raquete é verificada com `detectarColisaoRaquete()`.
- O movimento da bola é atualizado com a função `moverBola(0)`.
- Os blocos são renderizados na tela com `renderizarBlocos()`.

3. **Tratamento de Pausa**
   
- Se o jogador pressionar o botão específico para pausar (0b0100), o jogo é pausado até que o jogador decida continuar, com `sairPause()` que basicamente espera até que o jogador clique no botão de continuar.

4. **Atualização Visual**
- A tela é atualizada para mostrar as alterações feitas durante a iteração do loop com `video_show()`.

5. **Vitória e Derrota**
   
- Se todos os blocos forem destruídos (`blocosAtivos()` retorna falso), uma tela de parabéns é exibida.
- Se a bola ultrapassar a posição vertical de `230` pixels, o jogador perde e uma tela de derrota é exibida.

6. **Encerramento do Jogo**
   
Ao sair do loop principal, o jogo executa rotinas finais, dependendo do resultado da partida:
- Se o jogador ganhar, uma tela de parabéns é exibida novamente.
- Os hardwares são fechados com a função `fechar_hardwares()`, garantindo a correta liberação dos recursos utilizados.
  
![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)
## Regras do Jogo

Objetivo: Destruir todos os blocos sem deixar a bola cair no chão.

Controles: O jogador controla a raquete movimentando a placa, fazendo assim a raquete ir para a direita ou para a esquerda.

Raquete: O local onde a bola acerta a raquete que determina o ângulo que a bola volta, na metade esquerda volta no ângulo espelhado e na metade direita volta no mesmo ângulo.

Jogabilidade: O jogo começa com a bola sendo lançada em direção a raquete para poder realizar, caso acerte, a primeira inversão de sentido de movimento para ir atingir os blocos, e cada bloco vale 10 pontos, ganha o jogo quem conseguir destruir todos os blocos.

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)

## Como executar

Para utilizar o jogo, siga as instruções abaixo:

1. Certifique-se de que a placa DE1-SoC esteja corretamente conectada ao monitor CRT através da interface VGA, assim como ao cabo RJ45 de Internet.

2. Clone o repositório do jogo na placa DE1-SoC seguindo as etapas abaixo:
   - Conecte-se à rede SSH da placa usando um dos seguintes comandos, em que muda o endereço a depender da placa em que esteja-se utilizando:
     ```
     ssh aluno@10.0.0.120
     ssh aluno@10.0.0.121
     ssh aluno@10.0.0.122
     ```
   - Insira a senha quando solicitado: `@luno*123`
   - Navegue até a pasta do projeto utilizando os comandos do Linux.

3. Compile o projeto utilizando o comando `make all`.

4. Inicie o jogo pressionando o primeiro botão, “key-3”, na placa DE1-SoC.

5. Utilize do acelerômetro embutido na placa DE1-SoC para mover a barra para a esquerda e para a direita.

6. Pressione o botão 3, “key-1” para sair durante o jogo.

7. Para retornar a jogar novamente, após ganhar ou perder, tem de pressionar o primeiro botão, “key-3”

8. Para pausar ou sair do pause do jogo, tem de pressionar o segundo botão, da esquerda para a direita, “key-2”


![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)

## Testes
Nesse projeto foi utilizado uma estratégia modular, ou seja os testes foram realizados de forma unitária em funcionalidades que eram de suma importância para o andamento do projeto. Foram utilizados códigos separados que tinham o objetivo de apenas confirmar e entender sobre o funcionamento do sistema de maneira coerente. Com o melhor entendimento e desenvolvimento os testes passaram a ser de funcionalidades mais completas. 

O primeiro módulo separado tinha como objetivo aprender o funcionamento do acelerômetro embutido na FPGA. O código em si depende das bibliotecas fornecidas pela Intel FPGA para acessar e controlar o acelerômetro. No caso do código, são utilizadas funções como accel_open(), accel_init(), accel_format(), accel_calibrate(), accel_read(), e accel_close() para configurar e ler dados do acelerometro. Com as informações colhidas, é escrito na tela o valor de x enquanto é feita a movimentação da placa, para verificar sua oscilação, e quais os valores eram retornados para então saber como usá-los no programa completo. Essa visualização pode ser vista na tela do terminal apresentada abaixo.

<div align="center">
  
<img width="800px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/captura_do_teste_do_acelerometro.png">

</div>

Para o desenvolvimento do jogo, também fez-se necessário testar as funcionalidades que a porta VGA proporciona. Logo foi criado um módulo para verificar as funções pré-existentes da biblioteca “video”, usando como auxiliar um código exemplo disponibilizado no site da FPGA Academy. A partir deste foi possível compreender melhor o funcionamento para criar um módulo próprio, em que foram testadas as funções de uso do video, como o video_box(), video_line(), video_read(), video_clear(), e video_erase(), e então geradas os primeiros testes de tela, que vieram a compor a tela do projeto final. Nas imagens é possível ver, na primeira um prototipo de borda e de raquete, ja na segunda imagem a borda ja mais desenvolvida, e prototipo de tamanho de bloco, e como ele apareceria na tela durante o jogo.

<div align="center">
  
<div style="display: flex; justify-content: space-between;">
  <img width="400px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/Teste_monitor.png" alt="Imagem 1">
  <img width="400px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/imagem_no_monitor.png" alt="Imagem 2">
</div>

</div>

Com o entendimento sobre o funcionamento de como reproduzir video, além de como colher os valores do acelerômetro, foi o momento de mesclar ambos os hardwares em um único código, para testar gerar um elemento que se move a depender do valor lido pelo acelerometro. Este que após aperfeiçoamento tornou-se a função de movimentar a raquete presente no código final. O teste inicial foi necessario para entender o comportamento que o elemento teria, além de perceber a alta sensibilidade do sensor, e a necessidade de reduzir em 10 vezes para melhor jogabilidade. O gif abaixo permite visualizar a tela com a raquete se movendo, de acordo com o movimento da placa.

<div align="center">
  
<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/Movimenta%C3%A7%C3%A3o_raquete.gif">

</div>

A fim de obter um jogo com melhor jogabilidade e funcionamento, o uso dos botões presentes na placa DE1-SoC, foram de grande utilidade. Esse hardware foi último a ser testado, e seguindo a lógica dos testes anteriores, foi criado um módulo para teste unitário do mesmo, e após integração deste no arquivo de trabalho principal. Ao integra-lo no arquivo principal, foi testado o funcionamento para verificar se a função "sair" ocorria conforme esperado, assim como com o pause do jogo, e com isso ja conseguia testar também a logica do codigo, se estava coerente. Esse ultimo pode ser visualizado na imagem, em que no canto superior direito apresenta o texto indicando que o jogo está pausado.

<div align="center">
  
<img width="600px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/pause.png">

</div>

Diante do funcionamento dos hardwares no sistema entendidos, a implementação do jogo pode prosseguir com o desenvolvimento da lógica de funcionamento, além da construção das telas que são visualizadas durante o jogo. Sendo assim, para visualizar a imagem desenhada na tela de forma prática, manipulações no código fizeram-se necessárias. Estas aconteciam com o retirar de condicionais para funcionamento do jogo, para fazer com que chegar até a tela desejada torna-se algo mais rápido e simples. Essas manipulações foram de grande utilidade para testar a tela de parabéns, visto que não necessitava quebrar todos os blocos para visualizar, visto que dos 60 blocos, era posto para apresentar apenas 1 bloco para destruir, e ao realizar o teste se o fluxo do jogo levaria até ela, usou-se dessa estratégia, afim de facilitar a chegada até ela e verificação de quebra de todos os blocos. Esse teste é visivel com o visualizar do primeiro gif abaixo. Outro teste relacionado ao vídeo foi a exibição do valor da pontuação se estava na posição correta, ja que o texto aparece na tela durante o jogo, assim como na tela de “game over” em outra posição. Ainda sobre o teste de apresentação de telas, houve também o momento de ver a tela de "game over" aparente quando o jogador perde, ao deixar a bola ultrapassar a borda inferior, e o resultado dessa tela pode ser visto na imagem da sequencia abaixo, assim como no gif.

<div align="center">
  
<a href="https://youtu.be/g64wYvrjTxM">
  <img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/tela_de_parabens_1.gif">
</a>

<img width="600px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/Tela_game_over.png">

<br>
<a href="https://youtu.be/yls5UNrVokE">
  <img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/perdendo_o_jogo_1.gif">
</a>

</div>

A presença desses testes de tela não exclui o fato de estar testando a maquina do jogo durante, já que para poder chegar em cada uma das telas, ha de se passar por um determinado fluxo de codigo, logo possiveis erros para se encontrar nesse momento, podiam ser detectados e corrigidos. O funcionamento do jogo em si, após o desenvolvimento das telas estar completo, tomou um tempo nos testes, dada a necessidade de correcao de erros que surgiam a cada implementação, visando um melhor funcionamento. Inicialmente com o teste de colisão da bola com as bordas da janela, e partindo para a colisao com a raquete, em que problemas como a bola não efetuar o ricocheteamento correto, foram pertinentes até conseguir solucionar. Outro problema de colisão ocorreu com os blocos para serem destruídos, em que o sentido reverso da bola não era ocorria conforme previsto. Os erros nesse momento ao testar a colisão, necessitavam de testes sucessivos, e por vezes longos, para tentar reproduzir a situacao em que ocoreu algum erro e verificar se a correção foi efetivada.

Com esses testes dos hardwares, das telas e da logica do jogo, foi possivel chegar a um produto final satisfatorio, mas que ainda precisava de um teste simulando o uso real, passando pelas diversas situações integradas em um unico arquivo final, sem a interferencia de simplificações para testar casos isolados, para isso está documentado em [video](https://youtu.be/t1MIjK76e3k), contendo o fluxo do jogo.


![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)


## Considerações Finais

Por fim, o projeto do jogo Breakout foi concluído com sucesso, alcançando os objetivos estabelecidos para proporcionar uma experiência interativa aos jogadores. Especialmente em relação à interação eficaz com os elementos de hardware do Kit DE1-SoC.

A interação com os dispositivos de hardware, em especial o 3D-acelerômetro e o vídeo VGA, ocorreu de maneira eficiente, garantindo movimentos suaves e uma experiência de jogo responsiva. O monitor CRT com resolução de 640x480 e taxa de atualização de 60Hz contribuiu para a exibição nítida, proporcionando a experiência visual do jogo. 

Contudo, também são necessárias melhorias futuras no projeto. Uma possível implementação seria a adição de uma funcionalidade de salvamento de progresso na memória, permitindo aos jogadores retomarem o jogo de onde pararam em sessões posteriores. Isso proporciona uma experiência mais conveniente, sem a necessidade de reiniciar o jogo a cada nova partida sem perder o progresso alcançado.

Outrossim, ao lidar com uso de memória para persistencia de dados, poderia adicionar a implementação o salvar da maior pontuação realizada por qualquer jogador que tenha utilizado o jogo naquela pasta. Com isso, refletiria em uma pontuação recorde aparente, e que haveria a motivação para o jogador atual tentar alcançar e passar do valor de pontuação.

Em resumo, o projeto do jogo Breakout não apenas demonstrou a capacidade de integrar software e hardware de forma eficaz, mas também ofereceu uma experiência de jogo imersiva e interativa aos usuários. A interação com o acelerômetro, o monitor CRT e a renderização gráfica foram elementos fundamentais para o sucesso do projeto, que pode servir como um exemplo prático de aplicação de conceitos de sistemas digitais em um contexto de entretenimento.


![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png) 

## Equipe

[//]: contributor-faces

<a href="https://github.com/nailasuely"><img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/naila.png" title="naila" width="100"></a>
<a href="https://github.com/rhianpablo11"><img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/rhian.png" title="rhian" width="100" ></a>
<a href="https://github.com/AmandaLimaB"><img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/amanda.png" title="amanda" width="100" ></a>
<a href="https://github.com/joaogabrielaraujo"><img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/joaoGabriel.png" title="joao" width="100" ></a>

[//]: contributor-faces


## Tutor

<div style="display:flex;">
    <a href="https://github.com/x-anf" style="display: inline-block; border: none;">
        <img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/professor.png" style="margin-left: 5px; width: 100px; height: auto;">
    </a>
</div>


## Referências 
> - [1] Terasic. (2024). https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=205&No=836&PartNo=1#contents
> - [2] FPGAcademy. (2024). https://fpgacademy.org/
