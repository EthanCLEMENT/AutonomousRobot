#include <NewPing.h>

int threshold = 5;
int threshold_gauche = 3;
int distance_maximum = 4000;
int vitesse_de_base = 100;
int temps_tournant = 10;
int temps_d_avancement = 150;

int trigger_gauche = A1;
int echo_gauche = A0;
int trigger_angle_gauche = A3;
int echo_angle_gauche = A2;
int trigger_devant = A5;
int echo_devant = A4;
int trigger_angle_droite = A7;
int echo_angle_droite = A6;
int trigger_droite = A9;
int echo_droite = A8;

const int lFowd = 6;
const int lBack = 4;
const int rBack = 7;
const int rFowd = 5; 

bool startReceived = false;

double lDist, rDist, fDist, lcDist, rcDist;

double lms = vitesse_de_base, rms = vitesse_de_base;

NewPing lSensor(trigger_gauche, echo_gauche, distance_maximum);
NewPing lcSensor(trigger_angle_gauche, echo_angle_gauche, distance_maximum);
NewPing fSensor(trigger_devant, echo_devant, distance_maximum);
NewPing rcSensor(trigger_angle_droite, echo_angle_droite, distance_maximum);
NewPing rSensor(trigger_droite, echo_droite, distance_maximum);

void setup() {
  Serial.begin(9600);
  pinMode(rBack, OUTPUT);
  pinMode(rFowd, OUTPUT);
  pinMode(lBack, OUTPUT);
  pinMode(lFowd, OUTPUT);
  Serial.println("ready");
}

void loop() {
  if (!startReceived) {
    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      message.trim();
      if (message.startsWith("star")) {
        startReceived = true;
      }
    }
  } else {
    readSensors();
    Print();
    decide();
  }
}

void readSensors() {
  stopAll();
  int iterations = 1;

  int lDura = lSensor.ping_median(iterations, 400);
  delay(5);
  int fDura = fSensor.ping_median(iterations, 400);
  delay(5);
  int rDura = rSensor.ping_median(iterations, 400);
  delay(5);
  int lcDura = lcSensor.ping_median(iterations, 400);
  delay(5);
  int rcDura = rcSensor.ping_median(iterations, 400);
  delay(5);

  lDist = lSensor.convert_cm(lDura);
  lcDist = lcSensor.convert_cm(lcDura);
  fDist = fSensor.convert_cm(fDura);
  rcDist = rcSensor.convert_cm(rcDura);
  rDist = rSensor.convert_cm(rDura);
}

void rTurn() {
  Serial.println("right");
  digitalWrite(lFowd, LOW);
  digitalWrite(lBack, lms);
  digitalWrite(rFowd, rms);
  digitalWrite(rBack, LOW);
  delay(temps_tournant);
}
void lTurn() {
  Serial.println("left");
  digitalWrite(rFowd, LOW);
  digitalWrite(rBack, rms);
  digitalWrite(lFowd, lms);
  digitalWrite(lBack, LOW);
  delay(temps_tournant);
}

void goBack() {
  Serial.println("back");
  analogWrite(lFowd, LOW);
  digitalWrite(lBack, lms);
  analogWrite(rFowd, LOW);
  digitalWrite(rBack, rms);
  delay(temps_d_avancement);
}

void goFowd() {
  Serial.println("forward");
  analogWrite(lFowd, lms);
  digitalWrite(lBack, LOW);
  analogWrite(rFowd, rms);
  digitalWrite(rBack, LOW);
  delay(temps_d_avancement);
}

void stopAll() {
  digitalWrite(rBack, LOW);
  digitalWrite(rFowd, LOW);
  digitalWrite(lBack, LOW);
  digitalWrite(lFowd, LOW);
  delay(10);
}
void decide() {
  if (lDist == 0 || lcDist == 0) {
    readSensors();
    if (lDist == 0 || lcDist == 0) {
      goFowd();
      stopAll();
      readSensors();
      if (lDist == 0 || lcDist == 0) {
        goFowd();
        stopAll();
        goFowd();
        stopAll();
        lTurn();
        stopAll();
        lTurn();
        stopAll();
        goFowd();
        stopAll();
        lTurn();
        stopAll();
        lTurn();
        stopAll();
        goFowd();
        stopAll();
        lTurn();
        stopAll();
        lTurn();
        stopAll();
        goFowd();
        stopAll();
        lTurn();
        stopAll();
        lTurn();
        stopAll()""
        goFowd();
        stopAll();
        readSensors();
        if (fDist == 0) {
          while (1)
            ;
        }
      }
    }
  } else if (fDist == 0) {
    readSensors();
    if (fDist == 0) {
      goFowd();
      stopAll();
      readSensors();
      if (fDist == 0) {
        goFowd();
        stopAll();
        readSensors();
        if (fDist == 0) {
          while (1) //reste bloquer, pour que le code reste bloquer ici
            ;
        }
      }
    }
  } else if (fDist < threshold + 4) {
    rTurn();
  } else if (lDist > 35 && lcDist > 40 && rcDist > 35) {
    goFowd();
    stopAll();
    lTurn();
    stopAll();
    lTurn();
    stopAll();
    lTurn();
  } else if (lDist < threshold || lcDist < threshold || rcDist < threshold) {
    rTurn();
  } else if (rDist < threshold_gauche) {
    if (rDist == 0) {
      goFowd();
    } else {
      lTurn();
    }
  } else {
    goFowd();
  } 
}

void Print() {
  Serial.print("Leftdistance = ");
  Serial.println(lDist);
  Serial.print("Leftcdistance = ");
  Serial.println(lcDist);
  Serial.print("Frontdistance = ");
  Serial.println(fDist);
  Serial.print("Rightcdistance = ");
  Serial.println(rcDist);
  Serial.print("Rightdistance = ");
  Serial.println(rDist);
  Serial.print("\n");
}