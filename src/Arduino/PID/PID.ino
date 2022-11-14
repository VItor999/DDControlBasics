#include <TimedAction.h>     // BIBLIOTECA PARA UTILIZAR AS INTERRUPÇÕES MAIS FACILMENTE

//----------------------DEFINIÇÕES DE PINOS----------------------------------//
#define ACC 8                // PINO DO ACIONADOR
#define SEN A7               // PINO DO SENSOR DA PLANTA
#define REF A15              // PINO DO SELETOR DE REFERÊNCIA
#define SACC A0              // PINO DO SENSOR QUE MEDE A TENSÃO QUE O ARDUINO ENTREGA DE FATO
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

// Leituras (0- 5V)
//float rho[3] = { 13.2357,-23.8480,10.6457};//{12.7830,  -23.1500,   10.4000}; //Parâmetros do PID
float saida;                 // Valor do processo (2*O valor lido)
double referencia;             // VALOR DA REFERÊNCIA
//double erro[3] = {0, 0, 0};    // Sinal de erro k,k-1,k-2;
double U[2] = {0, 0};          // Sinal de controle (k,k-1)
int PWM; 
//parametros isolados
double Kp =   2.3477832;
double Ki =  0.03262217;
double Kd = 10.39820587;
double Kw =  0.0560;
double P = 0;
double I[2] = {0,0};
double D = 0;
double e[2]= {0,0};
double A =0;
TimedAction timedActionATT = TimedAction(TATT, atualizar); // REALIZA AMOSTRA E ATUALIZA O CONTROLE

//-----------------------------SETUP-----------------------------------------//
void setup() {
  // Definição dos Pinos
  pinMode(REF, INPUT);
  pinMode(SEN, INPUT);
  pinMode(SACC, INPUT);
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
  //FORMARTO REFERENCIA # SAIDA # CONTROLE # ERRO
  double aux;  // Variável auxiliar para mostrar a saturação
  //double erroP = (erro[0]/referencia)*100; // erro percentual
  double eP = (e[0]/referencia)*100; // erro percentual
  if (U[0] > 10) {
    aux = 10;
  }
  else if (U[0] < 0) {
    aux = 0;
  }
  else {
    aux = U[0];
  }
  Serial.print(referencia);
  Serial.print("#");
  Serial.print(saida);
  Serial.print("#");
  Serial.print(aux); // controle atual
  Serial.print("#");
  Serial.println(eP);
  //Serial.println(erroP); //erro atual
}

void atualizar() {
  //leituras
  saida      = (analogRead(SEN) / 1023.0) * 10;         // Valor do processo (2*O valor lido) (0 -10 V)
  referencia = (analogRead(REF) / 1023.0) * 10;         // Valor da referencia 0-10 V
  A =       (analogRead(SACC) / 1023.0) * 10; ;
  //controle tudo junto
//  erro[2] = erro[1];                                    // erro[k-2]
//  erro[1] = erro[0];                                    // erro[k-1]
//  erro[0] = referencia - saida;                         // erro[k]
//  U[1] = U[0]; // controle Anterior
//  U[0] = erro[0] * rho[0] + erro[1] * rho[1] + erro[2] * rho[2] + U[1]; // valor ideal
//Conversão para PWM  
//  PWM = (U[0] / (VSAT)) * 255;
//  if (PWM > 255) PWM = 255;
//  else if (PWM < 0) PWM = 0;

  e[1]= e[0];
  e[0] = referencia - saida;
  I[1]= I[0];
  P = Kp*e[0];
  D = Kd*(e[0]-e[1]);
  I[0] = Ki*e[0]+I[1];
  U[0] = P+I[0]+D;
  //Conversão para PWM  
  PWM = (U[0] / (VSAT)) * 255;
  if (PWM > 255) {
    PWM = 255;
  }
  else if (PWM < 0) {
    PWM = 0;
  }
  else {
    A = U[0];
   }
 
  //controle separado;
  analogWrite(ACC, PWM); // 0- 4.8 V -> arduino não consegue mandar 5 V certinho
  comunicar();
}
