#include <TimedAction.h>     // BIBLIOTECA PARA UTILIZAR AS INTERRUPÇÕES MAIS FACILMENTE

//----------------------DEFINIÇÕES DE PINOS----------------------------------//
#define ACC 8                // PINO DO ACIONADOR
#define SEN A7               // PINO DO SENSOR DA PLANTA
#define REF A15               // PINO DO SELETOR DE REFERÊNCIA
#define SACC A0             // PINO DO SENSOR QUE MEDE A TENSÃO QUE O ARDUINO ENTREGA DE FATO
#define TATT 500             // TEMPO DE AMOSTRAGEM/ ATUALIZAÇÃO DE  VALORES EM mS
#define TCOM 500             // TEMPO DE COMUNIÇÃO DOS DADOS EM mS
// QUANTO MAIOR TCOM MAIS SUAVE O GRÁFICO GERADO EXTERNAMENTE
// MAS MAIS DADOS REPETIDOS ELE CONTÉM UMA VEZ QUE O QUE IMPORTA
// É O TEMPO DE AMOSTRAGEM/ATUALIZAÇÃO


//---------------------------CABEÇALHOS--------------------------------------//
void comunicar();
void atualizar();

//----------------------VARIÁVEIS GLOBAIS------------------------------------//
// Leituras (0- 5V)
float sensor;                 // Valor do processo (2*O valor lido)
int PWM = 64;                    // VALOR DO ACIONAMENTO NO ARDUINO (0-255) 0 V - ~4.8V
float referencia = 2.5;           // VALOR DA REFERÊNCIA

TimedAction timedActionCOM = TimedAction(500, comunicar);  // EFETUA A COMUNIAÇÃO COM O PC
TimedAction timedActionATT = TimedAction(250000, atualizar); // REALIZA AMOSTRA E ATUALIZA O CONTROLE

//-----------------------------SETUP-----------------------------------------//
void setup() {
  // Definição dos Pinos
  pinMode(REF, INPUT);
  pinMode(SEN, INPUT);
  pinMode(SACC, INPUT);
  pinMode(ACC, OUTPUT);
  Serial.begin(9600);
  analogWrite(ACC, PWM);
}

//-----------------------------MAIN LOOP-------------------------------------//
void loop() {
  // OS TEMPOS DE ATUALIZAÃO E COMUNICAÇÃO SÃO DISTINTOS
  // O TEMPO DE ATUALIZAÇÃO É A AMOSTRAGEM QUE É DE 0.5 S
  // O TEMPO DE COMUNICACAO É DE 100mS
  // ESSA DIFERENÇA DE TEMPO DEIXA O GRÁFICO EM TEMPO REAL MAIS SUAVE
  // NO ENSAIO ENSTAMOS UTILIZANDO A VELOCIDADE DE COMUNICAÇÃO = VELOCIDADE DE
  // ATUALIZAÇÃO PARA QUE O GRÁFICO GERADO SEJA COMPOSTO SOMENTE COM OS PONTOS
  // AMOSTRADOS SE A REPETIÇÃO QUE UMA COMUNICAÇÃO RÁPIDA USA PARA SUAVIZAR
  // AS  CURVAS
  timedActionCOM.check(); // VERIFICA SE DEVE EXECTUTAR COMUNICAÇÃO
  timedActionATT.check(); // VERIFICA SE DEVE ATUALIZAR OS VALORES DE CONTROLE
}


//-----------------------FUNÇÕES CHAMADAS NAS INTERRUPÇÕES-------------------//
void comunicar() {
  // MANDA OS DADOS QUE DEVEM SER PLOTADOS
  //FORMARTO REFERENCIA # SENSOR # CONTROLE # ERRO
  sensor      = (analogRead(SEN) / 1023.0) * 10; // Valor do processo (2*O valor lido)
  Serial.print(referencia);
  Serial.print("#");
  Serial.println(sensor);
  
}

void atualizar() {

  if (PWM == 191) {
    PWM = 64;
    referencia = 2.5;
  }
  else if (PWM == 64) {
    PWM = 191;
    referencia = 7.5;
  }

  analogWrite(ACC, PWM);
}
