
<h1 align="center">
  <br>
 <img src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/logo.png" alt="Breakout" width="250"></a>
  <br>
  Breakout
  <br>
</h1>


<h4 align="center">Projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais.</h4>

<p align="center">
<div align="center">

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/nailasuely/breakout-problem3/blob/main/LICENSE)

<img width="800px" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/gif2.gif">


> Esse é um projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais, no qual ocorre o desenvolvimento de um jogo inspirado no clássico breakout. Utiliza o acelerômetro do DE1-SoC para controlar o jogador e botões para funções de jogo e a visualização é feita através da interface VGA em monitor CRT.

## Download do repositório


```
gh repo clone nailasuely/breakout-problem3
```

</div>

<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/Prancheta%201%20(4).png">

## Sumário
- [Apresentação](#apresentação)
- [Documentação utilizada](#documentação-utilizada)
- [Hardware utilizado](#hardware-utilizado)
- [Implementação](#implementação)
  - [C](#gpio)
- [Executando o Projeto](#executando-o-projeto)
- [Testes](#testes)
- [Conclusão](#conclusão) 
- [Tutor](#tutor)
- [Equipe](#equipe)
- [Referências](#referências)

![-----------------------------------------------------](https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/prancheta.png)


## Visão Geral do Projeto
O atual documento descreve o desenvolvimento de um jogo inspirado no clássico "Breakout", popularizado na década de 1970 e produzido por Nolan Bushnell e Steve Bristow na empresa de videogames Atari. Este jogo foi desenvolvido como parte de um projeto prático, utilizando a placa de desenvolvimento DE1-SoC, que integra uma FPGA e recursos como acelerômetro e interface VGA para visualização em um monitor CRT.

O objetivo do jogo é ajudar a equipe a programar em C para arquiteturas ARM e entender as instruções para usá-las no kit de desenvolvimento. Assim, o jogador controla uma barra na parte inferior da tela para rebater uma bola em direção a blocos coloridos organizados no topo da tela. O maior desafio é eliminar todos os blocos sem deixar a bola cair abaixo da barra inferior.

Para implementar o jogo, foram utilizadas técnicas de programação em linguagem C, aproveitando os recursos disponíveis na placa DE1-SoC e suas bibliotecas associadas. O desenvolvimento envolveu a manipulação de entrada do acelerômetro para controlar o movimento da barra, e também a geração de gráficos na tela usando o módulo de vídeo da placa.

Neste README, serão detalhados os aspectos técnicos do jogo, desde a inicialização do ambiente de desenvolvimento até a implementação das funcionalidades principais, bem como os desafios enfrentados durante o processo de criação e as soluções escolhidas pela equipe. Além disso, também serão discutidas as possíveis melhorias para futuras versões do projeto.

<img width="" src="https://github.com/nailasuely/breakout-problem3/blob/main/assets/img/estrutura.png">

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


### Arquitetura do Kit de Desenvolvimento DE1-SoC

O [Kit de Desenvolvimento DE1-SoC](https://www.terasic.com.tw/cgi-bin/page/archive_download.pl?Language=China&No=836&FID=ae336c1d5103cac046279ed1568a8bc3) é uma plataforma de design de hardware construída em torno do FPGA System-on-Chip (SoC) da Altera. Este SoC integra os mais recentes núcleos embarcados Cortex-A9 dual-core, combinados com um sistema de processador baseado em ARM (HPS). Essa combinação oferece uma variedade de recursos, incluindo processador, periféricos e interfaces de memória, todos conectados à matriz FPGA através de uma espinha dorsal de alta largura de banda.

Para gerar a imagem na tela, o jogo utiliza o módulo de vídeo da placa DE1-SoC, que é capaz de gerar gráficos em resolução VGA. A placa também possui 6 PLLs fracionários que podem ser usados para gerar diferentes frequências de clock para os diferentes componentes do sistema. nPara controle externo, o jogo utiliza o acelerômetro da placa DE1-SoC, que permite ao jogador controlar a barra movendo a placa. A placa também possui 2 controladores de memória rígidos que podem ser usados para conectar dispositivos de armazenamento externos, como um cartão SD. No caso do projeto é utilizado um cartão SD carregado com um sistema operacional.

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

### Módulo de vídeo 

escrever aqui

### Módulo do Acelerômetro 

escrever aqui

### Módulo para botões 

escrever aqui 


## Como executar

Para utilizar o jogo, siga as instruções abaixo:

1. Certifique-se de que a placa DE1-SoC esteja corretamente conectada ao monitor CRT através da interface VGA.

2. Clone o repositório do jogo na placa DE1-SoC seguindo as etapas abaixo:
   - Conecte-se à rede SSH da placa usando um dos seguintes comandos:
     ```
     ssh aluno@10.0.0.120
     ssh aluno@10.0.0.121
     ssh aluno@10.0.0.121
     ```
   - Insira a senha quando solicitado: `@aluno*123`
   - Navegue até a pasta do projeto utilizando os comandos do Linux.

3. Compile o projeto utilizando o comando `make all`.

4. Inicie o jogo pressionando o botão correspondente na placa DE1-SoC.

5. Utilize o acelerômetro embutido na placa DE1-SoC para mover a barra para a esquerda e para a direita.

6. Durante o jogo, pressione o botão para iniciar o jogo ou pausar/sair.



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

