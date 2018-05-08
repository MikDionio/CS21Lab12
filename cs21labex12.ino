#define left A0
#define enter A1
#define right A2
#define plus A3
#define minus A4

#define PIN_D0_0 1
#define PIN_D0_1 2
#define PIN_D0_2 3
#define PIN_D0_DOT 4
#define PIN_D1_0 5
#define PIN_D1_1 6
#define PIN_D1_2 7
#define PIN_D1_DOT 8
#define PIN_D2_0 9
#define PIN_D2_1 10
#define PIN_D2_2 11
#define PIN_D2_DOT 12

#define S0 13
#define S1 A5

int phase;
int factor; //identifies which digit use is currently editing, multiples of 10
int digit;
int answer;
int guess;

int digit2;//hundredths
int digit1;//tenths
int digit0;//ones
int currentdigit;//digit indicator

//Final Answers
int ansdigit2;
int ansdigit1;
int ansdigit0;

int prevleftState;
int prevrightState;
int preventerState;
int prevplusState;
int prevminusState;

void setup() {
  int i = 0;

  for(i=0;i<=13;i++){
    pinMode(i, OUTPUT);
  }
  
  pinMode(A5, OUTPUT);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  phase = 0;//0 - Setup, 1 = WP1, 2 = GP, 3 = WP2, 4 = DP
  factor = 100; //start with leftmost digit
  answer = 0;
  guess = 0;
  digit = 0;

  prevleftState = LOW;
  prevrightState = LOW;
  preventerState = LOW;
  prevplusState = LOW;
  prevminusState = LOW;

  Serial.begin(9600);
}

void loop() {
  switch(phase){
    case 0:{//Setup
      digitalWrite(S0, HIGH);
      digitalWrite(S1, LOW);
      outputBCD();
      editDigits();
      if(digitalRead(enter) == HIGH){//enter current guess
        ansdigit2 = digit2;
        ansdigit1 = digit1;
        ansdigit0 = digit0;
        phase = 1;
      }
      //Serial.println(factor);
      outputBCD();
      //delay(1000);
      break;
    }case 1:{//Wait 1
      //Turn Off 7-segment LEDs
      delay(1000);
      phase = 2;
      break;
    }case 2:{//Guess
      editDigits();
      outputBCD();
    }case 3:{//Wait 2
      
    }case 4:{//Done
      
    }
  }
}

void outputBCD2(int num) {//output BCD
  int i;
  int d2;
  
  for (i = 9; i < 12; i++) {
    digitalWrite(i, d2 % 2);
    d2 /= 2;
  }
}

void outputBCD1(int num) {//output BCD
  int i;
  int d1;
  for (i = 5; i < 8; i++) {
    digitalWrite(i, d1 % 2);
    d1 /= 2;
  }
}

void outputBCD0(int num) {//output BCD
  int i;
  int d0;
  
  for (i = 1; i < 4; i++) {
    digitalWrite(i, d0 % 2);
    d0 /= 2;
  }
}

void outputBCD(){
  outputBCD2(digit2);
  outputBCD1(digit1);
  outputBCD0(digit0);
}

void bullsAndCows(){
  int bulls;
  int cows;
  //Bulls
  if(ansdigit0 == digit0){
    bulls += 1;
  }

  if(ansdigit1 == digit1){
    bulls += 1;
  }

  if(ansdigit2 == digit2){
    bulls += 1;
  }

  //Cows
  if(digit0 == ansdigit1 || digit0 == ansdigit2){
    cows += 1;
  }

  if(digit1 == ansdigit0 || digit1 == ansdigit2){
    cows += 1;
  }

  if(digit2 == ansdigit1 || digit2 == ansdigit0){
    cows += 1;
  }

  Serial.println(bulls);
  Serial.println('B');
  Serial.println(cows);
  Serial.println('C');
}



void editDigits(){
  if(digitalRead(right) == HIGH){//edit digit to the right
        if(prevrightState == LOW){
          if(currentdigit == 2){
            currentdigit = 0;
          }else{
            currentdigit--;
          }
        }
      }else if(digitalRead(left) == HIGH){//edit digit to the left
        if(prevleftState == LOW){
          currentdigit = (currentdigit++)%3;
        }
      }else if(digitalRead(plus) == HIGH){//increment current digit
        switch(currentdigit){
          case 0:{
            digit0 = (digit0++)%8;
            break;
          }case 1:{
            digit1 = (digit1++)%8;
            break;
          }case 2:{
            digit2 = (digit2++)%8;
            break;
          }
        }
      }else if (digitalRead(minus) == HIGH){//decrement current digit
        switch(currentdigit){
          case 0:{
            if(digit0 <= 0){
              digit0 = 7;
            }else{
              digit0--;
            }
            break;
          }case 1:{
            if(digit1 <= 0){
              digit1 = 7;
            }else{
              digit1--;
            }
            break;
          }case 2:{
            if(digit2 <= 0){
              digit2 = 7;
            }else{
              digit2--;
            }
            break;
          }
        }
      }
      prevrightState = digitalRead(right);
      prevleftState = digitalRead(left);
      preventerState = digitalRead(enter);
      prevplusState = digitalRead(plus);
      prevminusState = digitalRead(minus);
}

