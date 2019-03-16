
int motorVCC = 14;
int motorGND = 13;
int potPin = 23;
int sanButPin = 0;
int numSquirts = 0;
int ledCycles = 0;
int currentLedCycle = 0;
int squirtDigits[4] = {0, 0, 0, 0};
int sev_seg_2 = 1;
int sev_seg_3 = 2;
int sev_seg_4 = 3;
int sev_seg_5 = 4;
int sev_seg_6 = 5;
int sev_seg_1 = 6;
int sev_seg_7 = 7;
int onPin = 8;

double vibTime;
double lastPotVal;
double vibBeginTimer;
double ledBeginTimer = 0;
double ledEndTimer = 9;

bool sanButLast = 1;
bool sanBut = 1;
bool vibOn = 0;
bool ledOn = 0;
bool startLedCycle = 0;
bool ledDuty = 0;

long int lastVibTime = 0;

void setup() {                
  pinSetup();   
  initializeVib(); 
  digitalWrite(onPin, HIGH);   
}

// the loop routine runs over and over again forever:
void loop() {
  sanButUpdate();
  readPot();
  vibControl();
  ledDisplayInit();
}

void pinSetup(){
  pinMode(motorVCC, OUTPUT);
  pinMode(motorGND, OUTPUT);
  pinMode(sanButPin, INPUT);
  pinMode(onPin, OUTPUT);
  pinMode(sev_seg_2, OUTPUT);
  pinMode(sev_seg_3, OUTPUT);
  pinMode(sev_seg_4, OUTPUT);
  pinMode(sev_seg_5, OUTPUT);
  pinMode(sev_seg_6, OUTPUT);
  pinMode(sev_seg_1, OUTPUT);
  pinMode(sev_seg_7, OUTPUT);
}

void motorVib(bool num){
  if (num){ //motor on
    digitalWrite(motorVCC, HIGH);  
    digitalWrite(motorGND, LOW);   
  }
  else{ //motor off
    digitalWrite(motorVCC, LOW);  
    digitalWrite(motorGND, LOW);    
  }
}

void readPot(){
  double potVal;
  potVal = analogRead(23);
  sanButUpdate();
  potVal = (5 + (25 * potVal / 1023))*6; // CHANGE TO *60 FOR 5-30 minute range (*6 is 30sec-3min)
  //Serial.print(potVal);
  vibTime = potVal;
}

void sanButUpdate(){
  bool b = digitalRead(sanButPin);
  if (b == 0 && sanButLast == 1){
    Serial.println("Button Push Detected");
    sanBut = 1;
  }
  else{
    sanBut = 0;
  }
  sanButLast = b;
  if (sanBut == 1){
    ledBeginTimer = millis()/1000;
    ledOn = 1;
    vibOn = 0;
    numSquirts = numSquirts + 1;
    ledCycleSetup();
    startLedCycle = 1;
    motorVib(false);
    Serial.println(numSquirts);
  }
}

void initializeVib(){ // initialize between values of 5 to 30 minutes
  double potVal = analogRead(23);
  potVal = (5 + (25 * potVal / 1023))*60;
  vibTime = potVal;
  lastVibTime = millis()/1000;
  Serial.println(lastVibTime);
  Serial.println(vibTime);
  motorVib(false);
}

void vibControl(){
  int timeCheck;
  timeCheck = millis()/1000;
  if ((vibTime + lastVibTime) < (timeCheck)){
    vibBeginTimer = timeCheck;
    lastVibTime = timeCheck;
    vibOn = 1;
  }
  if (vibOn){
    motorVib(true);
    if((timeCheck - vibBeginTimer) > 10){
      motorVib(false);
      vibOn = 0;
    }
  }
}

void ledDisplayInit(){ //Turns on led, changes appropriate flages
  if ((ledBeginTimer + ledCycles*2 > (millis()/1000)) && ledOn){ // Display Number of Squirts within 10sec interval
    squirtDigits[1] = numSquirts%10;
    squirtDigits[2] = (numSquirts/10)%10;
    squirtDigits[3] = (numSquirts/100)%10;
    if (startLedCycle){
      startLedCycle = 0;
      currentLedCycle = ledCycles;
      ledEndTimer = millis() + 2000;
      Serial.println(ledEndTimer);
    }else{ // Show numbers on 7SEG largest to smallest in 3 sec intervals
      ledSegControl();
    }
  }
  else{
    ledOn = 0;
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, HIGH);
    digitalWrite(sev_seg_3, HIGH);
    digitalWrite(sev_seg_4, HIGH);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, HIGH);   
  }
}

void ledCycleSetup(){
  if(numSquirts >= 0 && numSquirts <= 9){
    ledCycles = 1;
  }
  if(numSquirts >= 10 && numSquirts <= 99){
    ledCycles = 2;
  }
  if(numSquirts >= 100 && numSquirts <= 999){
    ledCycles = 3;
  }
}

void ledSegControl(){ //
  if(ledEndTimer < millis()){
    currentLedCycle = currentLedCycle - 1;
    ledEndTimer = ledEndTimer + 2000;
    Serial.println(ledEndTimer);
  } else if (ledEndTimer > millis() + 200){
    numControl(squirtDigits[currentLedCycle]); // Code to display squirtDigits[currentLedCycle] value on 7Seg
  }else{
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, HIGH);
    digitalWrite(sev_seg_3, HIGH);
    digitalWrite(sev_seg_4, HIGH);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, HIGH);  
  }
    
}

void numControl (int num){
  if (num == 0 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, LOW);
    digitalWrite(sev_seg_7, HIGH);     
  }
  if (num == 1 && ledDuty){
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, HIGH);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, HIGH);     
  }
  if (num == 2 && ledDuty){
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, HIGH);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, LOW);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 3 && ledDuty){
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 4 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, HIGH);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 5 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, HIGH);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 6 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, HIGH);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, LOW);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 7 && ledDuty){
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, HIGH);     
  }
  if (num == 8 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, LOW);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (num == 9 && ledDuty){
    digitalWrite(sev_seg_1, LOW);  
    digitalWrite(sev_seg_2, LOW);
    digitalWrite(sev_seg_3, LOW);
    digitalWrite(sev_seg_4, LOW);
    digitalWrite(sev_seg_5, LOW);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, LOW);     
  }
  if (ledDuty == 0){
    digitalWrite(sev_seg_1, HIGH);  
    digitalWrite(sev_seg_2, HIGH);
    digitalWrite(sev_seg_3, HIGH);
    digitalWrite(sev_seg_4, HIGH);
    digitalWrite(sev_seg_5, HIGH);
    digitalWrite(sev_seg_6, HIGH);
    digitalWrite(sev_seg_7, HIGH); 
  }
  if(ledDuty)
    ledDuty = 0;
  else
    ledDuty = 1;
}

