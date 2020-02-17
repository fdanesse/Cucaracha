#include <Servo.h>

int MAX_D = 110;   // tope atras motor[1] 
int MIN_D = 50;    // tope delante motor[1]

int UP_D = 65;     // tope arriba motor[0]
int DOWN_D = 120;  // tope abajo motor[0]

int APOYO_D = 110; //motores verticales derechos
int CENTRO = 90;

int MIN_I = 90;
int MAX_I = 0;
int UP_I = 90;
int DOWN_I = 90;
int APOYO_I = 90;

int incr[] = {1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1};
int velocidad = 8;
int direccion = 1;

// 0,2,4,6,8,10 = vertical - 0,1,2,3,4,5,6 = derecha
Servo *motor[12];
String command;

  
void setup(){
  for (int x=4; x<13; x++){pinMode(x, OUTPUT);}
  pinMode(A0, OUTPUT); pinMode(A1, OUTPUT); pinMode(A2, OUTPUT);
  digitalWrite(13, LOW);
  for (int x=0; x<12; x++){motor[x] = new Servo();}
  motor[0]->attach(4); motor[1]->attach(5); motor[2]->attach(6); motor[3]->attach(7);
  motor[4]->attach(8); motor[5]->attach(9); motor[6]->attach(10); motor[7]->attach(11);
  motor[8]->attach(12); motor[9]->attach(A0); motor[10]->attach(A1); motor[11]->attach(A2);
  Serial.begin(9600); Serial.println(F("INIT"));
  MIN_I = map(MIN_D, 0, 180, 180, 0);
  MAX_I = map(MAX_D, 0, 180, 180, 0);
  UP_I = map(UP_D, 0, 180, 180, 0);
  DOWN_I = map(DOWN_D, 0, 180, 180, 0); 
  APOYO_I = map(APOYO_D, 0, 180, 180, 0);
  //Serial.println(MIN_I); Serial.println(MAX_I);
  //Serial.println(UP_I); Serial.println(DOWN_I);Serial.println(APOYO_I);
  posInicial();
  command = "avanzar";
}
  
void changePosition(Servo *servo, int val){
  servo->write(constrain(servo->read()+val, 0, 180));
}

void fixPosition(Servo *servo, int val){
  servo->write(constrain(val, 0, 180));
}

void posInicial(){
  // Para montar los motores adecuadamente.
  for (int x=0; x<12; x++){
    while(motor[x]->read()<CENTRO){changePosition(motor[x],1);delay(5);}
    while(motor[x]->read()>CENTRO){changePosition(motor[x],-1);delay(5);}
    }
  command = "stop";
}

void loop(){
  if (Serial.available()) {
    command = Serial.readStringUntil(char('\0'));
    command.replace("\n", "");
    Serial.println(command + "_");
    }
  if (command == "avanzar") {direccion = 1; set_Pos_Caminar(); command = "caminando";}
  else if (command == "retroceder") {direccion = 2; set_Pos_Caminar(); command = "caminando";}
  
  else if (command == "inicial") {posInicial();}
  else if (command == "delante") {todo_adelante();command = "stop";}
  else if (command == "atras") {todo_atras();command = "stop";}
  else if (command == "arriba") {todo_arriba();command = "stop";}
  else if (command == "abajo") {todo_abajo();command = "stop";}
  else if (command == "apoyo") {todo_apoyado();command = "stop";}
  else if (command == "poscaminar") {direccion = 1; set_Pos_Caminar();command = "stop";}
  else if (command == "caminando") {caminar();}
  else {command = "stop";}
}

// Ir a posiciones límites
void todo_adelante(){
  while(motor[1]->read() > MIN_D || motor[3]->read() > MIN_D || motor[5]->read() > MIN_D || motor[7]->read() < MIN_I|| motor[9]->read() < MIN_I || motor[11]->read() < MIN_I){
    for (int x=1; x<6; x+=2){if (motor[x]->read() > MIN_D){changePosition(motor[x], -1);}}
    for (int x=7; x<12; x+=2){if (motor[x]->read() < MIN_I){changePosition(motor[x], 1);}}
    delay(velocidad);}}

void todo_atras(){
  while(motor[1]->read() < MAX_D || motor[3]->read() < MAX_D || motor[5]->read() < MAX_D || motor[7]->read() > MAX_I|| motor[9]->read() > MAX_I|| motor[11]->read() > MAX_I) {
    for (int x=1; x<6; x+=2){if (motor[x]->read() < MAX_D){changePosition(motor[x], 1);}}
    for (int x=7; x<12; x+=2){if (motor[x]->read() > MAX_I){changePosition(motor[x], -1);}}
    delay(velocidad);}}
  
void todo_arriba(){
  while(motor[0]->read() > UP_D || motor[2]->read() > UP_D || motor[4]->read() > UP_D || motor[6]->read() < UP_I|| motor[8]->read() < UP_I || motor[10]->read() < UP_I) {
    for (int x=0; x<5; x+=2){if (motor[x]->read() > UP_D){changePosition(motor[x], -1);}}
    for (int x=6; x<11; x+=2){if (motor[x]->read() < UP_I){changePosition(motor[x], 1);}}
    delay(velocidad);}}
    
void todo_abajo(){
  while(motor[0]->read() < DOWN_D || motor[2]->read() < DOWN_D || motor[4]->read() < DOWN_D || motor[6]->read() > DOWN_I || motor[8]->read() > DOWN_I || motor[10]->read() > DOWN_I){
    for (int x=0; x<5; x+=2){if (motor[x]->read() < DOWN_D){changePosition(motor[x], 1);}}
    for (int x=6; x<11; x+=2){if (motor[x]->read() > DOWN_I){changePosition(motor[x], -1);}}
    delay(velocidad);}}

void todo_apoyado(){
  while(motor[0]->read() < APOYO_D || motor[2]->read() < APOYO_D || motor[4]->read() < APOYO_D || motor[6]->read() > APOYO_I || motor[8]->read() > APOYO_I || motor[10]->read() > APOYO_I){
    for (int x=0; x<5; x+=2){if (motor[x]->read() < APOYO_D){changePosition(motor[x], 1);}}
    for (int x=6; x<11; x+=2){if (motor[x]->read() > APOYO_I){changePosition(motor[x], -1);}}
    delay(velocidad);}}

void set_Pos_Caminar(){
  // HD
  fixPosition(motor[1], MIN_D+1);
  fixPosition(motor[3], MAX_D-1);
  fixPosition(motor[5], MIN_D+1);
  // HI
  fixPosition(motor[7], MAX_I+1);
  fixPosition(motor[9], MIN_I-1);
  fixPosition(motor[11], MAX_I+1);

  if (direccion == 1){ // avanzar
    //VD
    fixPosition(motor[0], APOYO_D);
    fixPosition(motor[2], UP_D);
    fixPosition(motor[4], APOYO_D);
    //VI
    fixPosition(motor[6], UP_I);
    fixPosition(motor[8], APOYO_I);
    fixPosition(motor[10], UP_I);
  }else{ // retroceder
    //VD
    fixPosition(motor[0], UP_D);
    fixPosition(motor[2], APOYO_D);
    fixPosition(motor[4], UP_D);
    //VI
    fixPosition(motor[6], APOYO_I);
    fixPosition(motor[8], UP_I);
    fixPosition(motor[10], APOYO_I);
    }
  }

void caminar(){
  for (int d=1; d<6; d+=2){
    int inc = setH(incr[d], motor[d]->read(), MIN_D, MAX_D, 1);    
    changePosition(motor[d], inc);
    incr[d] = inc;
  }
  for (int d=7; d<12; d+=2){
    int inc = setH(incr[d], motor[d]->read(), MIN_I, MAX_I, 2);
    changePosition(motor[d], inc);
    incr[d] = inc;
  }
  
  for (int d=1; d<6; d+=2){
    String zona = getZonaD(motor[d]->read(), incr[d]);
    int v = setVD(zona, motor[d]->read());
    fixPosition(motor[d-1], v);
  }
  for (int d=7; d<12; d+=2){
    String zona = getZonaI(motor[d]->read(), incr[d]);
    int v = setVI(zona, motor[d]->read());
    fixPosition(motor[d-1], v);
  }
  
  delay(velocidad);
  }

String getZonaI(int h, int inc){
  String zona = "";
  if (h <= CENTRO && inc == 1){zona = "D";}
  if (h > CENTRO && inc == -1){zona = "A";}
  if (h <= CENTRO && inc == -1){zona = "B";}
  if (h > CENTRO && inc == 1){zona = "C";}
  return zona;
  }

String getZonaD(int h, int inc){
  String zona = "";
  if (h <= CENTRO && inc == 1){zona = "D";}
  if (h > CENTRO && inc == 1){zona = "A";}
  if (h > CENTRO && inc == -1){zona = "B";}
  if (h <= CENTRO && inc == -1){zona = "C";}
  return zona;
  }
  
int setH(int inc, int h, int mi, int ma, int lado){
  if (lado == 1){if (!(h > mi && h < ma)){inc *= -1;}
  }else{if (!(h < mi && h > ma)){inc *= -1;}}
  return inc;
}

int setVI(String zona, int h){
  int v = 90;
  switch (direccion){
  case 1:
    if (zona == "D"){v = map(h, MAX_I, CENTRO, CENTRO, UP_I);}
    if (zona == "A"){v = map(h, MIN_I, CENTRO, CENTRO, APOYO_I);}
    if (zona == "B"){v = map(h, CENTRO, MAX_I, APOYO_I, CENTRO);}
    if (zona == "C"){v = map(h, CENTRO, MIN_I, UP_I, CENTRO);}
    break;
   case 2:
    if (zona == "D"){v = map(h, MAX_I, CENTRO, CENTRO, APOYO_I);}
    if (zona == "A"){v = map(h, MIN_I, CENTRO, CENTRO, UP_I);}
    if (zona == "B"){v = map(h, CENTRO, MAX_I, UP_I, CENTRO);}
    if (zona == "C"){v = map(h, CENTRO, MIN_I, APOYO_I, CENTRO);}
    break;     
  }
  return v;
  }
  
int setVD(String zona, int h){
  int v = 90;
  switch (direccion){
  case 1:
    if (zona == "D"){v = map(h, MIN_D, CENTRO, CENTRO, APOYO_D);}
    if (zona == "A"){v = map(h, CENTRO, MAX_D, APOYO_D, CENTRO);}
    if (zona == "B"){v = map(h, MAX_D, CENTRO, CENTRO, UP_D);}
    if (zona == "C"){v = map(h, CENTRO, MIN_D, UP_D, CENTRO);}
    break;
   case 2:
    if (zona == "D"){v = map(h, MIN_D, CENTRO, CENTRO, UP_D);}
    if (zona == "A"){v = map(h, CENTRO, MAX_D, UP_D, CENTRO);}
    if (zona == "B"){v = map(h, MAX_D, CENTRO, CENTRO, APOYO_D);}
    if (zona == "C"){v = map(h, CENTRO, MIN_D, APOYO_D, CENTRO);}
    break;     
  }
  return v;
  }
