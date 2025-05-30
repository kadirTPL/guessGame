#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Keypad.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC
RTC_DS3231 rtc;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 10, 11, 12};
byte colPins[COLS] = {A0, A1, A2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
const int BUZZER_PIN = 8;
int playerNum;
int roundNum;
int tryNum;
int curRound;
int curPlayer;
int getInput(){
  char key=keypad.getKey();
  String input="";
  int num=0;
  bool runAgain=false;
  while(num<=0){
      while(key!='#'){
        if(key && key!='*'){
          input+=key;
         lcd.setCursor(0,1);
         lcd.print("               ");
         lcd.setCursor(0,1);
         lcd.print(input);
        }
        delay(100);
        key=keypad.getKey();
        }
      num=input.toInt();
      if(num<=0){
        lcd.print("Number>0");
        input="";
        delay(1000);
        runAgain=true;
        break;
      }
 }
 if(runAgain){
  num=getInput(true);
 }
  return num;
}
int getInput(bool guess){
  char key=keypad.getKey();
  String input="";
  int num=0;
  bool runAgain=false;
  while(num<=0){
      while(key!='#'){
        if(key && key!='*'){
          input+=key;
        lcd.setCursor(0,1); lcd.print(input); // optional live display
        }
       delay(100);
        key=keypad.getKey();
        }
      num=input.toInt();
      if(num<1 || num>100){
        lcd.clear();
        lcd.print("Range 1-100");
        input="";
        delay(1000);
        runAgain=true;
        break;
      }
 }
 if(runAgain){
  num=getInput(true);
 }
  return num;
}
class guessingGame{
  private:
  int* playerScores=nullptr;
  unsigned long roundStartTime; // 20 seconds
  bool gameActive;
  int roundAmount;
  int roundDuration;
  int tryAmount;
  public:
  guessingGame(){
    playerScores = nullptr;
    roundAmount=0;
    tryAmount=0;
  }
  guessingGame(int playerNum,int roundNum,int tryNum){
    roundAmount=roundNum;
    tryAmount=tryNum;
    if (playerScores) delete[] playerScores;
    playerScores=new int[playerNum];
    for(int i=0;i<playerNum;i++){
      this->playerScores[i]=0;
    }
    roundDuration=20;
  }
  ~guessingGame(){
    delete[] playerScores;
  }
  int* getScores() {return playerScores;}
  String displayWinners(int playerNum){
    int max=0;
    int* winners=new int[playerNum];
    int winnerIndex=0;
    String msg="";
    for(int i=0;i<playerNum;i++){
      if(this->playerScores[i]>max) max=this->playerScores[i];
    }
    for(int i=0;i<playerNum;i++){
      if(this->playerScores[i]==max){ 
        winners[winnerIndex]=(i+1);
        winnerIndex++;
    }}
    for(int i=0;i<winnerIndex;i++){
      msg=msg+"P"+winners[i]+" ";
    }
    String fileMsg=msg+String(max);
    Serial.println(fileMsg);
    delay(1000);
    delete[] winners;
    return fileMsg;
  }
  void startRound(int playerIndex,int roundIndex){
    int secretNum=random(1,101);
    lcd.setCursor(0, 0);
    lcd.print(secretNum);
    delay(1000);
    roundDuration=20;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("P:"+ String(playerIndex+1)+" A:1"+" R:"+String(roundIndex+1));
    lcd.setCursor(12,0);
    lcd.print("T:20");
    roundStartTime=millis();
    int guess=getInput(true);
    int tryNum=1;
    String msg="";

    while(!this->checkGuess(secretNum,guess,playerIndex,msg) && roundDuration>0 ){
      noTone(BUZZER_PIN);
      tryNum++;
      if(tryNum>this->tryAmount){
        break;
      }
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("P:"+String(playerIndex+1)+" A:"+ String(tryNum) +" R:"+String(roundIndex+1));
      displayTime();
      guess=getInput(true);
      displayTime();
    }
    if(tryNum>this->tryAmount){
      msg="No attempt left.";    
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(msg);
    }
    if(roundDuration<=0){
      msg="Time ended.";  
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(msg);
    }
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Secret num:" + String(secretNum));
    delay(1500);
  }
    private:
    void displayTime(){
      if((millis()-roundStartTime)>=1000){
        roundDuration--;
        roundStartTime=millis();}
        lcd.setCursor(12,0);
        lcd.print("    ");
        lcd.setCursor(12,0);
        lcd.print("T:"+String(roundDuration));
    }
    bool checkGuess(int secretNum,int guess,int playerIndex,String msg){
      displayTime();
      bool guessCheck=true;
      if(guess==secretNum){
        this->playerScores[playerIndex]=this->playerScores[playerIndex]+20;
        Serial.println(this->playerScores[playerIndex]);
        msg="Correct!";
        tone(BUZZER_PIN, 1000, 300);
        delay(300);  
      }
      else if (guess-secretNum<=5 && guess-secretNum>=-5 ){
        this->playerScores[playerIndex]=this->playerScores[playerIndex]+10;
        Serial.println(this->playerScores[playerIndex]);
        msg="Close!";
        tone(BUZZER_PIN, 700, 200);
        delay(300);  
      }
      else{
        msg=guess>secretNum?"Wrong. Lower":"Wrong. Higher";
        tone(BUZZER_PIN, 400, 150);
        delay(300);
        displayTime();
        guessCheck=false;
      }
      lcd.setCursor(0,1);
      lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(msg);
      delay(1500);
      lcd.setCursor(0,1);
      lcd.print("               ");
      return guessCheck;
    }
};
guessingGame* Game;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC Error!");
    while (1);
  }
  if (rtc.lostPower()) {
  rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guessing Game");
  lcd.setCursor(0,1);
  lcd.print("# for continuing");
  char key = keypad.getKey(); 
  delay(50);
  while (key != '#') {
  key = keypad.getKey();
   delay(50);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player amount:");
  playerNum=getInput();
  curPlayer=0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Round amount:");
  roundNum=getInput();
  curRound=0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Try amount:");
  tryNum=getInput();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press # to start");
  randomSeed(analogRead(A3));
  key = keypad.getKey(); 
  delay(50);
  Game = new guessingGame(playerNum,roundNum,tryNum);
  while (key != '#') {
  key = keypad.getKey(); 
  delay(50);
  }
}


void loop() {
  lcd.clear();
  if(curRound<roundNum){
    Game->startRound(curPlayer,curRound);
    curPlayer++;
    if(curPlayer==playerNum){
      curPlayer=0;
      curRound++;
    }
  }
  if(curRound==roundNum){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("End. Winners are");
    String winnerMsg = Game->displayWinners(playerNum);
    lcd.setCursor(0, 1);
    lcd.print(winnerMsg);
    delete Game;
    Game = nullptr;
    while(true);
  }
} 