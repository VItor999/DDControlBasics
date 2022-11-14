#include <TimedAction.h>     // BIBLIOTECA PARA UTILIZAR AS INTERRUPÇÕES MAIS FACILMENTE

//----------------------DEFINIÇÕES DE PINOS----------------------------------//
#define ACC 8                // PINO DO ACIONADOR
#define SEN A7               // PINO DO SENSOR DA PLANTA
#define REF A15              // PINO DO SELETOR DE REFERÊNCIA
//#define SACC A0              // PINO DO SENSOR QUE MEDE A TENSÃO QUE O ARDUINO ENTREGA DE FATO
#define TATT 500             // TEMPO DE AMOSTRAGEM/ ATUALIZAÇÃO DE  VALORES EM mS
#define TCOM 500             // TEMPO DE COMUNIÇÃO DOS DADOS EM mS
// QUANTO MAIOR TCOM MAIS SUAVE O GRÁFICO GERADO EXTERNAMENTE
// MAS MAIS DADOS REPETIDOS ELE CONTÉM UMA VEZ QUE O QUE IMPORTA
// É O TEMPO DE AMOSTRAGEM/ATUALIZAÇÃO
#define VSAT 10              // TENSão MAXIMA POSSIVEL 

//---------------------------CABEÇALHOS--------------------------------------//

void comunicar();
void atualizar();

//----------------------VARIÁVEIS GLOBAIS------------------------------------//

TimedAction timedActionATT = TimedAction(TATT, atualizar); // REALIZA AMOSTRA E ATUALIZA O CONTROLE

float r = 0;
float y = 0;
float erro = 0;
float erroAnt = 0;

float ctrl = 0;
float ctrlP = 0;
float ctrlI = 0;
float ctrlD = 0;
float ctrlIant = 0;
float A = 0;
//Antigo polo continuo em 0.07 //MA 2757500 polo 0.07 // MA2757500 polo 0.04//
float Kp = 2.3477832; //2.34668505;////
float Ki = 0.03262217;//1.36576395;//0.03200404; //0.0192965;//
float Kd = 10.39820587;//8.87871051;//6.89487292;//
float Kw =  0.056*0.4;//0.06;// 0.0529;//

int PWM = 0 ;

//-----------------------------SETUP-----------------------------------------//
void setup() {
  // Definição dos Pinos
  pinMode(REF, INPUT);
  pinMode(SEN, INPUT);
  //pinMode(SACC, INPUT);
  pinMode(ACC, OUTPUT);
  Serial.begin(9600);
}

//-----------------------------MAIN LOOP-------------------------------------//
void loop() {
  // O TEMPO DE ATUALIZAÇÃO É A AMOSTRAGEM QUE É DE 0.5 S
  timedActionATT.check(); // VERIFICA SE DEVE ATUALIZAR OS VALORES DE CONTROLE
}

//-----------------------FUNÇÕES CHAMADAS NAS INTERRUPÇÕES-------------------//

void comunicar() {
  // MANDA OS DADOS QUE DEVEM SER PLOTADOS
  //FORMATO REFERENCIA # SAIDA # CONTROLE # ERRO
  float aux;  // Variável auxiliar para mostrar a saturação
  float ep = (erro / r) * 100; // erro percentual
  if (ctrl > VSAT) {
    aux = VSAT;
  }
  else if (ctrl < 0) {
    aux = 0;
  }
  else {
    aux = ctrl;
  }
  Serial.print(r);
  Serial.print("#");
  Serial.print(y);
  Serial.print("#");
  Serial.print(aux); // controle atual
  Serial.print("#");
  Serial.print(ep);
  Serial.print("#");
  Serial.print(ctrlP);
  Serial.print("#");
  Serial.print(ctrlI);
  Serial.print("#");
  Serial.print(ctrlD);
  Serial.print("#");
  Serial.println(ctrl);
}

void atualizar() {
  //leituras
  y   = (analogRead(SEN) / 1023.0) * 10;         // Valor do processo (2*O valor lido) (0 -10 V)
  r = (analogRead(REF) / 1023.0) * 10;         // Valor da referencia 0-10 V
  //A = (analogRead(SACC) / 1023.0) * 10; ;
  // Atualizações
  erroAnt = erro;
  erro = r - y ;
  ctrlIant = ctrlI;
  ctrlP = Kp * erro;
  ctrlI = (ctrlIant + Ki * erro + Kw * (A - ctrlP - ctrlD)) / (1 + Kw);
  ctrlD = Kd * (erro - erroAnt);
  ctrl = ctrlP + ctrlI + ctrlD;

  //Conversão para PWM
  PWM = (ctrl / (VSAT)) * 255;
  if (PWM > 255) {
    PWM = 255;
    A = 10;
  }
  else if (PWM < 0) {
    A = 0;
    PWM = 0;
  }
  else {
    A = ctrl;
  }
  //controle separado;
  analogWrite(ACC, PWM); // 0- 4.8 V -> arduino não consegue mandar 5 V certinho
  comunicar();
}
