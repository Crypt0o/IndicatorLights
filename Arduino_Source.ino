#define leftSwitch    7
#define hazardSwitch  6
#define rightSwitch   5
#define leftLED       13
#define rightLED      12


void setup() {
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(rightSwitch, INPUT);
  pinMode(hazardSwitch, INPUT);
  pinMode(leftSwitch, INPUT);

  Serial.begin(9600);

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 625;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();    

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{ // extern int state;
  IL_mainfunction();
 // Serial.println(state);
}

void loop() {
 
}

  enum States {
    OFF,
    LANECHANGE,
    NORMAL,
    HAZARD
  };
  

int state = OFF;
int savedSwitch;

void IL_mainfunction(void) {
  extern int state;
  
  switch(state) {
    case LANECHANGE:
      IL_LANECHANGE();
      break;
      
    case NORMAL:
      IL_NORMAL();
      break;
      
    case HAZARD:
      IL_HAZARD();
      break;

    default:
      IL_OFF();
      break;
    
  }
};

void IL_NORMAL(void) {
    digitalWrite(leftLED, 0), digitalWrite(rightLED, 0);
 
  int LEFT_SWITCH = digitalRead(leftSwitch);
  int RIGHT_SWITCH = digitalRead(rightSwitch);
  int HAZARD_SWITCH = digitalRead(hazardSwitch);

    // PTFD = 0
  extern int state;
  extern int savedSwitch;
  static unsigned insideCOUNTER;
  static unsigned cntRELEASE;
  static unsigned cntHAZARD;

  // **************** HAZARD ***************
  if (!HAZARD_SWITCH) 
    ++cntHAZARD;
  else {
     if (cntHAZARD >= 20)       insideCOUNTER = cntRELEASE = cntHAZARD = 0, state = HAZARD;   // Override to mode if it's pressed for at least 200 ms and released
     else                       cntHAZARD = 0;
  }

  if (LEFT_SWITCH && RIGHT_SWITCH) { // if both of them are released, check for 100 ms if one was recently pressed 

          ++cntRELEASE;
          if (cntRELEASE >= 10) {
             cntRELEASE = insideCOUNTER = 0;
             state = OFF;
            }
  }
  else {

      insideCOUNTER++;
      // For 300 ms, the LED will be on
      if (insideCOUNTER < 30) // PTFD = savedSwitch == 1? 1<<7 : 1;
          savedSwitch == 1? digitalWrite(leftLED, 1) : digitalWrite(rightLED, 1);
      // For 700 ms, the LED will be off
      else if (insideCOUNTER < 100)    // PTFD = 0;
          digitalWrite(leftLED, 0), digitalWrite(rightLED, 0);  
      else insideCOUNTER = 0;
  }
}


void IL_LANECHANGE(void) {

  int LEFT_SWITCH = digitalRead(leftSwitch);
  int RIGHT_SWITCH = digitalRead(rightSwitch);
  int HAZARD_SWITCH = digitalRead(hazardSwitch);

  extern int state;
  extern int savedSwitch;
  static unsigned cntRIGHT;
  static unsigned cntLEFT;
  static unsigned cntHAZARD;
  static unsigned countToThree;
  static unsigned countOnOff;


/* --------------------------------------------------------------------------------
 *  Override to NORMAL state or HAZARD
 */

// **************** HAZARD ***************
  if (!HAZARD_SWITCH) 
    ++cntHAZARD;
  else {   // Override to mode if it's pressed for at least 200 ms and released
     if (cntHAZARD >= 20)       cntHAZARD  = cntRIGHT = cntLEFT = countToThree=countOnOff= 0, state = HAZARD;     
     else                       cntHAZARD = 0;
  }
  
// **************** NORMAL ***************  
  if ((!LEFT_SWITCH && RIGHT_SWITCH) || (LEFT_SWITCH && !RIGHT_SWITCH)) {
    cntHAZARD = 0;
    !LEFT_SWITCH? ++cntLEFT : ++cntRIGHT; // We increase the counter for the right switch
    
   if (cntLEFT  == 50 || cntRIGHT == 50) {
        savedSwitch = cntRIGHT< cntLEFT? 1 : 2; // The higher value corresponds to the pressed switch
        cntLEFT = cntRIGHT = 0;
        state = NORMAL;
      }
    
  }
  else if (LEFT_SWITCH && RIGHT_SWITCH)
      cntRIGHT = cntLEFT = 0;
// -----------------------------------------------------------------------------------

  if (countToThree < 3) {
    ++countOnOff;

    if (countOnOff < 30)  //savedSwitch == 1? PTFD = 1 : PTFD = 1<<7;    // LEFT_SWITCH is saved if savedSwitch is 1, RIGHT_SWITCH if 2
          savedSwitch == 1? digitalWrite(leftLED, 1): digitalWrite(rightLED, 1);
    else if (countOnOff < 100) // PTFD = 0;
          digitalWrite(leftLED, 0), digitalWrite(rightLED, 0);
    else countOnOff = 0, countToThree ++; 
 }
  else countToThree = 0, countOnOff = 0, state = OFF;
    

}

void IL_HAZARD(void)  {
  int LEFT_SWITCH = digitalRead(leftSwitch);
  int RIGHT_SWITCH = digitalRead(rightSwitch);
  int HAZARD_SWITCH = digitalRead(hazardSwitch);

  
  extern int state;
  static unsigned LEDsTimeCounter;
  static unsigned cntHAZARD;

// ********** DEFAULT OVERRIDE *********
  if (!HAZARD_SWITCH) 
    ++cntHAZARD;
  else {
     if (cntHAZARD >= 20)       cntHAZARD = 0, state = OFF;      // Override to default mode if it's pressed for at least 200 ms and released
     else                       cntHAZARD = 0;
  }
  
// *******************************************
  ++LEDsTimeCounter;
  if (LEDsTimeCounter < 50) // PTFD = 1 | 1<<7;
      digitalWrite(leftLED, 1), digitalWrite(rightLED, 1);
  else if (LEDsTimeCounter < 100) // PTFD = 0;
     digitalWrite(leftLED, 0), digitalWrite(rightLED, 0);
   else LEDsTimeCounter = 0;
  
}

void IL_OFF(void) {
   int LEFT_SWITCH = digitalRead(leftSwitch);
   int RIGHT_SWITCH = digitalRead(rightSwitch);
   int HAZARD_SWITCH = digitalRead(hazardSwitch);
  
    extern int state;
    extern int savedSwitch;
    static unsigned cntRIGHT;
    static unsigned cntLEFT;
    static unsigned cntHAZARD;
   
    digitalWrite(leftLED, 0);
    digitalWrite(rightLED, 0);
  
/* --------------------------------------------------------------------------------
 *  Override to NORMAL state or HAZARD
 */

// ***************************HAZARD************************************************
  if (!HAZARD_SWITCH) 
    ++cntHAZARD;
  else {  // If the HAZARD_SWITCH is released, we check if it was pressed for longer than 200 ms
     if (cntHAZARD >= 20)       cntHAZARD = 0, state = HAZARD;      // Override to mode if it's pressed for at least 200 ms and released
     else                       cntHAZARD = 0;
  }
  
// ****************************  NORMAL *******************************************

    if ((!LEFT_SWITCH && RIGHT_SWITCH) || (!RIGHT_SWITCH && LEFT_SWITCH)){  // If right switch is pressed or left one is pressed
      !LEFT_SWITCH? ++cntLEFT : ++cntRIGHT; // We increase the counter for the right switch
      
      if (cntLEFT  == 50 || cntRIGHT == 50) {
        savedSwitch = cntRIGHT< cntLEFT? 1 : 2; // The higher value corresponds to the pressed switch
        cntLEFT = cntRIGHT = 0;
        state = NORMAL;
      }

    }
    //****************************** LANE CHANGE ***********************************//
    else if (LEFT_SWITCH && RIGHT_SWITCH) { 
        if ((cntLEFT > 0 && cntLEFT< 50) || (cntRIGHT > 0 && cntRIGHT< 50)) {
            savedSwitch = cntRIGHT < cntLEFT ? 1 : 2;
            cntLEFT = cntRIGHT = 0;
            state = 1;
        }
        else cntLEFT = cntRIGHT = 0, savedSwitch;
        
    }
}
