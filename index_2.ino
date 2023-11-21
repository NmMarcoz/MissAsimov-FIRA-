#include "RoboCore_Vespa.h"
#include "Ultrasonic.h"
#include "Math.h";

VespaMotors motors;
Ultrasonic ultraF(18,5);
Ultrasonic ultraD(19,23);
Ultrasonic ultraE(22,21);


const int PAUSE_TIME = 2000;

int distanceD;
int distanceE;
int distanceF;

//-----PID AREA-------//
const int KP = 10;
int sub;
const int ERRO = 100;
int erroD;
int erroE;
String lastCurve;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Começando o código");
}

void loop() {
  // put your main code here, to run repeatedly
  console();
  if(distanceF < 17){
    Serial.println("Se afastando do muro!");
    frente(50);
    delay(400);
    motors.stop();
    distanceD = ultraD.Ranging(CM);
    distanceE = ultraE.Ranging(CM);
    Serial.println("Analisando a melhor decisão...");
    if(distanceD > distanceE){
      Serial.println("A melhor decisão é para direita!");
      giroDireita(50);
    }else{
      Serial.println("A melhor decisão é para Esquerda");
      giroEsquerda(50);
    }
    Serial.println("Manobra finalizada!");
  }else{
     pid(); 
  }

  
  
}


void frente(int velo){
  Serial.println("Indo para frente");
  motors.forward(velo);
}

void giroDireita(int velo){
  distanceD = ultraD.Ranging(CM);
  Serial.println("Girando girandooo (pra direita");
  while(distanceD > 5){
    distanceD = ultraD.Ranging(CM); 
    moveTank(70, -70);
  }
  Serial.println("Giro finalizado");
  lastCurve = "DIREITA";
  delay(400);
}


void giroEsquerda(int velo){
  distanceE = ultraD.Ranging(CM);
  Serial.println("Girando girandooo (pra esquerda");
  while(distanceE > 5){
    distanceE = ultraD.Ranging(CM);
    moveTank(-70, 70);
  }
  Serial.println("Giro finalizado");
  lastCurve = "ESQUERDA";
  delay(400);
}

void moveTank(int velocidade_esquerda, int velocidade_direita){
  motors.setSpeedLeft(velocidade_esquerda);
  motors.setSpeedRight(velocidade_direita);
}

void pid(){
  //Tenho que fazer ainda! 
  if(distanceD < 15 && distanceE < 15){
    proporcional();
  }else{
      Serial.println("Distância muito grande para o PID!");
      Serial.println("Ativando modo de uma parede rs");
      if(lastCurve == "DIREITA"){ //Falta a lógica pra seguir apenas uma parede aqui rs
        Serial.println("\n\n\n\n\n\n\n");
        Serial.println("Tenho que seguir apenas a parede da esquerda!");
      }else if(lastCurve == "ESQUERDA"){
        Serial.println("\n\n\n\n\n\n\n");
        Serial.println("Tenho que seguir apenas a parede da direita!");
      }
    }
}

void proporcional(){
  erroD = (100 - distanceD);
  erroE = (100 - distanceE);
  sub = (int)  (fabs(100 - ((erroE - erroD))*KP));

  if(distanceE > distanceD){
    Serial.println("Proporcional para esquerda");
    Serial.println("Motor direito à: " + String(erroD));
    Serial.println("Motor esquerdo à: " + String(sub));
    motors.turn(sub, erroD);
  }else{
    Serial.println("Proporcional para direita");
    Serial.println("Motor esquerdo à: " + String(erroE));
    Serial.println("Motor direito à: " + String(sub));
    motors.turn(erroE, sub);
  }
}

//Ainda em andamento isso aqui!
void steering(int steer){
  int quoci;       

  if (steer < 0){
    quoci = 70 + steer;
  }else{
    quoci = 70 - steer;   
  }
  Serial.println("Steer: " + String(steer));
  Serial.println("Quoci: " + String(quoci));
  motors.turn(steer, quoci);
 
}

void console(){
  distanceF = ultraF.Ranging(CM);
  distanceD = ultraD.Ranging(CM);
  distanceE = ultraE.Ranging(CM);
  //------------------------------------//--------------------------------------//-------------------------------------------//-------------------------------------------//
 
  Serial.print("Distância da ESQUERDA: " + (String)distanceE + "cm ");
  Serial.print("======== Distância da FRENTE: "+ (String)distanceF+ "cm ========");
  Serial.println(" Distância da DIREITA: " + (String)distanceD + "cm");
  delay(800);
}
