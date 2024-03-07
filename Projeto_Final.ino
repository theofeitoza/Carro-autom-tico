// Inclusão de bibliotecas
#include <Ultrasonic.h>
#include <IRremote.h>

// Definição de pinos
#define PINO_TRIGGER 4
#define PINO_ECHO 5
#define RECV_PIN 9

// Variáveis globais
char t;
int parada = 0;
int x = 0;
float cmMsec;

// Criação de objetos
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
IRrecv irrecv(RECV_PIN);
decode_results results;

// Funções de movimento do robô
void andar_frente(int tempo) {
  float cmMsec;
  Serial.println(cmMsec);
  for (tempo; tempo>0; tempo--){
    long microsec = ultrasonic.timing();
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    if (cmMsec < 10) {
      Serial.println("Objeto obstruindo passagem");
      desligarMotores();
      delay(5000);
    }
    else{
      digitalWrite(13, HIGH);
      digitalWrite(11, HIGH);
      delay(100);
    }
  }
  desligarMotores();
}

void andar_re(int tempo) {
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  delay(tempo);
  desligarMotores();
}

void virar_esquerda(int tempo) {
  float cmMsec;
  for (tempo; tempo>0; tempo--){
    long microsec = ultrasonic.timing();
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    if (cmMsec < 10) {
      Serial.println("Objeto obstruindo passagem");
      desligarMotores();
      delay(5000);
    }
    else{
      digitalWrite(13, HIGH);
      delay(100);
    }
  }
  desligarMotores();
}

void virar_direita(int tempo) {
  float cmMsec;
  for (tempo; tempo>0; tempo--){
    long microsec = ultrasonic.timing();
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    if (cmMsec < 10) {
      Serial.println("Objeto obstruindo passagem");
      desligarMotores();
      delay(5000);
    }
    else{
      digitalWrite(11, HIGH);
      delay(100);
    }
  }
  desligarMotores();
}

void desligarMotores() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
}

// Funções de rotas
void Rota1() {
  for (int rota = 0; rota < 1; rota++) {
    andar_frente(50);
    virar_esquerda(40);
    andar_frente(50);
  }
}

void Rota2() {
  for (int rota = 0; rota < 1; rota++) {
    andar_frente(30);
    virar_direita(40);
    andar_frente(30);
  }
}

void Rota3() {
  for (int rota = 0; rota < 1; rota++) {
    andar_frente(40);
    virar_esquerda(40);
    andar_re(20);
    virar_direita(40);
    andar_frente(40);
  }
}

// Configuração inicial
void setup() {
  pinMode(13, OUTPUT);    // Motores esquerdos para frente
  pinMode(12, OUTPUT);    // Motores esquerdos para trás
  pinMode(11, OUTPUT);    // Motores direitos para frente
  pinMode(10, OUTPUT);    // Motores direitos para trás
  pinMode(9, OUTPUT);     // Led
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Lendo dados do sensor..."); 
}

// Loop principal
void loop() {
  while (true) {
    if (irrecv.decode(&results)) {
      if (results.value == 16753245) {
        Serial.println("Rota 1 se iniciando...");
        Rota1();
        desligarMotores();
      } else if (results.value == 16736925) {
        Serial.println("Rota 2 se iniciando...");
        Rota2();
        desligarMotores();
      } else if (results.value == 16769565) {
        Serial.println("Rota 3 se iniciando...");
        Rota3();
        desligarMotores();
      } else {
        desligarMotores();
      }
      Serial.println(results.value); // Imprime o valor em hexadecimal
      irrecv.resume();
    }
  }
  delay(100);
}
