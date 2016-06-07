/*************************************************** 
  DESCRIÇÃO:
  Acionamento de saídas a relê através de comandos BlueTooth
  
  POR:
  PandoraLab
  www.pandoralab.com.br
  
  COLABORADORES:
  Mauricio Jabur
  maumaker -at- pandoralab -dot- com -dot- br
  
  HARDWARE:
  Arduino Nano - https://pandoralab.com.br/loja/arduino-nano-atmega328p-au/
  Módulo Relê 2 canais - https://pandoralab.com.br/loja/modulo-rele-2-canais/
  Módulo bluetooth HC-05 - https://pandoralab.com.br/loja/modulo-bluetooth-hc-05/

  TOOLCHAIN:PandoraLab
  Desenvolvido na IDE Arduino 1.6.7
  
  BIBLIOTECAS UTILIZADAS:
  SoftwareSerial - V 1.0.0 
  https://www.arduino.cc/en/Reference/SoftwareSerial
  
  LICENÇA:
  Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
  http://creativecommons.org/licenses/by-sa/4.0/
  
 ****************************************************/

   // Serial para o modulo bluetooth
 #include <SoftwareSerial.h>
SoftwareSerial bluetooth(3, 2); // RX, TX

// Pinos onde está conectado o módulo de relês
const int rele_1 = 4;
const int rele_2 = 5;

int selecionado = rele_1;

// Constantes para ara facilitar a leitura do código
const int LIGADO = LOW;
const int DESLIGADO = HIGH;

void setup() {
  // inicalização das saídas
  digitalWrite(rele_1, DESLIGADO);
  pinMode(rele_1, OUTPUT);

  digitalWrite(rele_2, DESLIGADO);
  pinMode(rele_2, OUTPUT);

  // Porta serial - conversa com o PC
  Serial.begin(9600);
  Serial.println("Iniciado");

  // porta serial - conversa com o modulo BlueTooth
  bluetooth.begin(9600);
}

void loop() {
  // processa bluetooth
  if (bluetooth.available()) {
    parser (bluetooth.read()) ;
  }

  // processa Serial
  if (Serial.available()) {
    parser (Serial.read()) ;
  }
}

// Função que analisa os caracters enviados
// e executa os comandos apropriados
void parser (char incoming) {
  Serial.print(incoming);

  switch (incoming) {
    case 'A':
    case 'a':
      selecionado = rele_1;
      break;
    case 'B':
    case 'b':
      selecionado = rele_2;
      break;
    case '+':
      print_rele();
      Serial.println("LIGA");
      digitalWrite(selecionado, LIGADO);
      break;
    case '-':
      print_rele();
      Serial.println("desliga");
      digitalWrite(selecionado, DESLIGADO);
      break;
    case '/':
      print_rele();
      Serial.print("inverte -> ");
      if (invert(selecionado) == 0) {
        Serial.println(" LIGA");
      }
      else {
        Serial.println(" desliga");
      }
       break;
  }

}

// função para inverter o estado de uma saída no Arduino
int invert(int pin) {
  int result = 1 - digitalRead(pin);
  digitalWrite(pin, result);
  return result;
}

// função auxiliar para informar a saída selecionada pela serial do computador
inline void print_rele() {
  if (selecionado == rele_1) {
    Serial.print(" Rele 1: ");
  }
  else if (selecionado == rele_2) {
    Serial.print(" Rele 2: ");
  }
  else {
    Serial.print(" Indefinido: ");
  }
}

