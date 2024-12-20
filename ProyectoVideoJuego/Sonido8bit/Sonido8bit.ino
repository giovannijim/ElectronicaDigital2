#include <Arduino.h>
#include "pitches.h"

#define pin_Buzzer 4

#define SOUND_PWM_CHANNEL 2
#define SOUND_RESOLUTION 8                     // 8 bit resolution
#define SOUND_ON (1 << (SOUND_RESOLUTION - 1)) // 50% duty cycle
#define SOUND_OFF 0                            // 0% duty cycle

void beep( int frequency, int duration);
void Starwars();
void GameOfThrones();
void HappyBirthday();
void HarryPotter();
void Pirate();
void mario();
void McGyver();
void StarWars2();
void BonBruteTruand();
void IndianaJones();
void twentyCentFox();
void looney();
void Entertainement();
void BarbieGirl();
void Greensleaves();
void Bond();


void setup() {
  Serial.begin(115200); 
  delay(100);
  //beep(NOTE_A4, 500);
  HarryPotter();  
}

void loop() {
  
}

////////////////////////// Les chansons ////////////////////////
//****************************************************************************************
// Beep
//****************************************************************************************
// Function to generate beep sounds
void beep(int frequency, int duration) {
  // Manually attach pin with the chosen channel
  if (ledcAttachChannel(pin_Buzzer, frequency, SOUND_RESOLUTION, SOUND_PWM_CHANNEL)) {
    // Start sound with 50% duty cycle
    ledcWrite(SOUND_PWM_CHANNEL, 128);  // Use channel 0, 50% duty cycle for sound
    delay(duration);
    
    // Stop sound
    ledcWrite(SOUND_PWM_CHANNEL, 0);    // Turn off sound (0% duty cycle)
  } else {
    Serial.println("Error: Unable to attach LEDC channel!");
  }
}
//******* Starwars coté obscure **************** 
 
void Starwars() {   
  //firstSection
  beep(NOTE_A4, 500);
  beep(NOTE_A4, 500);    
  beep(NOTE_A4, 500);
  beep(NOTE_F4, 350);
  beep(NOTE_C5, 150);  
  beep(NOTE_A4, 500);
  beep(NOTE_F4, 350);
  beep(NOTE_C5, 150);
  beep(NOTE_A4, 650); 
  delay(500); 
  beep(NOTE_E5, 500);
  beep(NOTE_E5, 500);
  beep(NOTE_E5, 500);  
  beep(NOTE_F5, 350);
  beep(NOTE_C5, 150);
  beep(NOTE_GS4, 500);
  beep(NOTE_F4, 350);
  beep(NOTE_C5, 150);
  beep(NOTE_A4, 650); 
  delay(500); 
  //secondSection
  beep(NOTE_A5, 500);
  beep(NOTE_A4, 300);
  beep(NOTE_A4, 150);
  beep(NOTE_A5, 500);
  beep(NOTE_GS5, 325);
  beep(NOTE_G5, 175);
  beep(NOTE_FS5, 125);
  beep(NOTE_F5, 125);    
  beep(NOTE_FS5, 250); 
  delay(325); 
  beep(NOTE_AS5, 250);
  beep(NOTE_DS5, 500);
  beep(NOTE_D5, 325);  
  beep(NOTE_CS5, 175);  
  beep(NOTE_C5, 125);  
  beep(NOTE_AS4, 125);  
  beep(NOTE_C5, 250);   
  delay(350);
  //Variant 1
  beep(NOTE_F4, 250);  
  beep(NOTE_GS4, 500);  
  beep(NOTE_F4, 350);  
  beep(NOTE_A4, 125);
  beep(NOTE_C5, 500);
  beep(NOTE_A4, 375);  
  beep(NOTE_C5, 125);
  beep(NOTE_E5, 650); 
  delay(500);  
  //secondSection
  beep(NOTE_A5, 500);
  beep(NOTE_A4, 300);
  beep(NOTE_A4, 150);
  beep(NOTE_A5, 500);
  beep(NOTE_GS5, 325);
  beep(NOTE_G5, 175);
  beep(NOTE_FS5, 125);
  beep(NOTE_F5, 125);    
  beep(NOTE_FS5, 250); 
  delay(325); 
  beep(NOTE_AS5, 250);
  beep(NOTE_DS5, 500);
  beep(NOTE_D5, 325);  
  beep(NOTE_CS5, 175);  
  beep(NOTE_C5, 125);  
  beep(NOTE_AS4, 125);  
  beep(NOTE_C5, 250);   
  delay(350);
  //Variant 2
  beep(NOTE_F4, 250);  
  beep(NOTE_GS4, 500);  
  beep(NOTE_F4, 375);  
  beep(NOTE_C5, 125);
  beep(NOTE_A4, 500);  
  beep(NOTE_F4, 375);  
  beep(NOTE_C5, 125);
  beep(NOTE_A4, 650); 
  delay(650);
}
  
//******* GameOfThrones ****************
void GameOfThrones() {
  for(int i=0; i<3; i++) {
    beep(NOTE_G4, 500);     
    beep(NOTE_C4, 500);    
    beep(NOTE_DS4, 250);    
    beep(NOTE_F4, 250);    
  }
  
  for(int i=0; i<3; i++) {
    beep(NOTE_G4, 500);    
    beep(NOTE_C4, 500);    
    beep(NOTE_E4, 250);    
    beep(NOTE_F4, 250);    
  }
  
  beep(NOTE_G4, 500);
  beep(NOTE_C4, 500);        
  beep(NOTE_DS4, 250);
  beep(NOTE_F4, 250);
  beep(NOTE_D4, 500);   
       
  for(int i=0; i<2; i++) {
      beep(NOTE_G3, 500);
      beep(NOTE_AS3, 250);
      beep(NOTE_C4, 250);
      beep(NOTE_D4, 500);    
  }
  
  beep(NOTE_G3, 500);
  beep(NOTE_AS3, 250);
  beep(NOTE_C4, 250);
  beep(NOTE_D4, 1000);
  beep(NOTE_F4, 1000);
  beep(NOTE_AS3, 1000);
  beep(NOTE_DS4, 250);
  beep(NOTE_D4, 250);
  beep(NOTE_F4, 1000);
  beep(NOTE_AS3, 1000);
  beep(NOTE_DS4, 250);
  beep(NOTE_D4, 250);
  beep(NOTE_C4, 500);
  
  for(int i=0; i<2; i++) {
    beep(NOTE_GS3, 250);
    beep(NOTE_AS3, 250);
    beep(NOTE_C4, 500);
    beep(NOTE_F3, 500);    
  }
  
  beep(NOTE_G4, 1000);
  beep(NOTE_C4, 1000);
  beep(NOTE_DS4, 250);
  beep(NOTE_F4, 250);
  beep(NOTE_G4, 1000);
  beep(NOTE_C4, 1000);
  beep(NOTE_DS4, 250);
  beep(NOTE_F4, 250);
  beep(NOTE_D4, 500);    
        
  for(int i=0; i<3; i++) {
    beep(NOTE_G3, 500);
    beep(NOTE_AS3, 250);
    beep(NOTE_C4, 250);
    beep(NOTE_D4, 500);
  }
}
  
  
//******* Happy Birthday ****************
void HappyBirthday() {
  beep(NOTE_G3, 200);
  beep(NOTE_G3, 200);
  beep(NOTE_A3, 500);
  beep(NOTE_G3, 500);
  beep(NOTE_C4, 500);
  beep(NOTE_B3, 1000);
  beep(NOTE_G3, 200);
  beep(NOTE_G3, 200);
  beep(NOTE_A3, 500);
  beep(NOTE_G3, 500);
  beep(NOTE_D4, 500);
  beep(NOTE_C4, 1000);
  beep(NOTE_G3, 200);
  beep(NOTE_G3, 200);
  beep(NOTE_G4, 500);
  beep(NOTE_E4, 500);
  beep(NOTE_C4, 500);
  beep(NOTE_B3, 500);
  beep(NOTE_A3, 750);
  beep(NOTE_F4, 200);
  beep(NOTE_F4, 200);
  beep(NOTE_E4, 500);
  beep(NOTE_C4, 500);
  beep(NOTE_D4, 500);
  beep(NOTE_C4, 1000); 
}
  
  
//******* Harry Potter ****************
void HarryPotter() {
  beep(NOTE_B4, 333);  
  beep(NOTE_E5, 500);   
  beep(NOTE_G5, 166);    
  beep(NOTE_FS5, 333);    
  beep(NOTE_E5, 666);
  beep(NOTE_B5, 333);   
  beep(NOTE_A5, 1000);    
  beep(NOTE_FS5, 1000);    
  beep(NOTE_E5, 500);   
  beep(NOTE_G5, 166);    
  beep(NOTE_FS5, 333);    
  beep(NOTE_DS5, 666);    
  beep(NOTE_F5, 333);    
  beep(NOTE_B4, 1666);    
  beep(NOTE_B4, 333);    
  beep(NOTE_E5, 500);    
  beep(NOTE_G5, 166);    
  beep(NOTE_FS5, 333);    
  beep(NOTE_E5, 666);    
  beep(NOTE_B5, 333);    
  beep(NOTE_D6, 666);    
  beep(NOTE_CS6, 333);    
  beep(NOTE_C6, 666);    
  beep(NOTE_GS5, 333);    
  beep(NOTE_C6, 500);    
  beep(NOTE_B5, 166);    
  beep(NOTE_AS5, 333); 
  beep(NOTE_AS4, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_E5, 1666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_B5, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_B5, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_C6, 666);    
  beep(NOTE_B5, 333);    
  beep(NOTE_AS5, 666);    
  beep(NOTE_FS5, 333);    
  beep(NOTE_G5, 500);    
  beep(NOTE_B5, 166);    
  beep(NOTE_AS5, 333);    
  beep(NOTE_AS4, 666);    
  beep(NOTE_B4, 333);    
  beep(NOTE_B5, 1666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_B5, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_B5, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_D6, 666);    
  beep(NOTE_CS6, 333);    
  beep(NOTE_C6, 666);    
  beep(NOTE_GS5, 333);    
  beep(NOTE_C6, 500);    
  beep(NOTE_B5, 166);    
  beep(NOTE_AS5, 333);    
  beep(NOTE_AS4, 666);    
  beep(NOTE_G5, 333);    
  beep(NOTE_E5, 1666);    
}
  
  
//******* Pirate des caraïbes ****************
void Pirate() {  
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50); 
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 125); 
  beep(NOTE_C5, 250); 
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_C5, 125);
  beep(NOTE_D5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 375);
  delay(50);  
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 125);
  beep(NOTE_C5, 250);
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_C5,125); 
  beep(NOTE_D5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4, 125); 
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 375);
  delay(50);    
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_C5, 125);
  beep(NOTE_D5, 250);
  beep(NOTE_D5, 125);
  delay(50);
  beep(NOTE_D5, 125); 
  beep(NOTE_E5, 125);
  beep(NOTE_F5,250);
  beep(NOTE_F5, 125);
  delay(50);
  beep(NOTE_E5, 125); 
  beep(NOTE_D5, 125);
  beep(NOTE_E5, 125);
  beep(NOTE_A4, 250);
  delay(50);  
  beep(NOTE_A4,125); 
  beep(NOTE_B4, 125);
  beep(NOTE_C5, 250);
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_D5, 250);
  beep(NOTE_E5, 125);
  beep(NOTE_A4, 250);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_C5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_C5, 125);
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 375);
  delay(200);
  beep(NOTE_A4, 250); 
  beep(NOTE_A4, 125);     
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50); 
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 125); 
  beep(NOTE_C5, 250); 
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_C5, 125);
  beep(NOTE_D5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 375);
  delay(50);  
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 125);
  beep(NOTE_C5, 250);
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_C5,125); 
  beep(NOTE_D5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4, 125); 
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 375);
  delay(50);    
  beep(NOTE_E4, 125);
  beep(NOTE_G4, 125);
  beep(NOTE_A4, 250);
  beep(NOTE_A4, 125);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_C5, 125);
  beep(NOTE_D5, 250);
  beep(NOTE_D5, 125);
  delay(50);
  beep(NOTE_D5, 125); 
  beep(NOTE_E5, 125);
  beep(NOTE_F5, 250);
  beep(NOTE_F5, 125);
  delay(50);
  beep(NOTE_E5, 125); 
  beep(NOTE_D5, 125);
  beep(NOTE_E5, 125);
  beep(NOTE_A4, 250);
  delay(50);  
  beep(NOTE_A4,125); 
  beep(NOTE_B4, 125);
  beep(NOTE_C5, 250);
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_D5, 250);
  beep(NOTE_E5, 125);
  beep(NOTE_A4, 250);
  delay(50);
  beep(NOTE_A4, 125);
  beep(NOTE_C5, 125);
  beep(NOTE_B4, 250);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_C5, 125);
  beep(NOTE_A4, 125);
  beep(NOTE_B4, 375);
  delay(200);
  beep(NOTE_E5, 250);
  delay(400);
  beep(NOTE_F5, 250);
  delay(400); 
  beep(NOTE_E5, 125);
  beep(NOTE_E5, 125);
  delay(50);
  beep(NOTE_G5, 125);
  delay(50);
  beep(NOTE_E5, 125);  
  beep(NOTE_D5, 125);
  delay(400);    
  beep(NOTE_D5, 250);
  delay(400);
  beep(NOTE_C5, 250);
  delay(400);  
  beep(NOTE_B4, 125);  
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4,  500);    
  beep(NOTE_E5, 250);  
  delay(400);
  beep(NOTE_F5, 250);
  delay(400);    
  beep(NOTE_E5, 125);
  beep(NOTE_E5, 125);
  delay(50);
  beep(NOTE_G5, 125);
  delay(50);
  beep(NOTE_E5, 125);
  beep(NOTE_D5, 125);
  delay(400);  
  beep(NOTE_D5, 250);
  delay(400);
  beep(NOTE_C5, 250);
  delay(400);  
  beep(NOTE_B4, 125);
  beep(NOTE_C5, 125);
  delay(50);
  beep(NOTE_B4, 125);
  delay(50);
  beep(NOTE_A4, 500);
}
  
  
//******* Mario ****************
void mario() {
  beep(NOTE_E7, 120);
  beep(NOTE_E7, 120);
  delay(120);  
  beep(NOTE_E7, 120);
  delay(120);  
  beep(NOTE_C7, 120);
  beep(NOTE_E7, 120);
  delay(120);  
  beep(NOTE_G7, 120);
  delay(240);    
  beep(NOTE_G6, 120);
  delay(360); 
  beep(NOTE_C7, 120);
  delay(240); 
  beep(NOTE_G6, 120);
  delay(240);  
  beep(NOTE_E6, 120);
  delay(240);    
  beep(NOTE_A6, 120);
  delay(120);  
  beep(NOTE_B6, 120);
  delay(120);  
  beep(NOTE_AS6, 120);
  beep(NOTE_A6, 120);
  delay(120); 
  beep(NOTE_G6, 90);
  beep(NOTE_E7, 90);
  beep(NOTE_G7, 90);
  beep(NOTE_A7, 120);
  delay(120);  
  beep(NOTE_F7, 120);
  beep(NOTE_G7, 120);
  delay(120);  
  beep(NOTE_E7, 120);
  delay(120);  
  beep(NOTE_C7, 120);
  beep(NOTE_D7, 120);
  beep(NOTE_B6, 120);
  delay(240);  
  beep(NOTE_C7, 120);
  delay(240);  
  beep(NOTE_G6, 120);
  delay(240);    
  beep(NOTE_E6, 120);
  delay(240);     
  beep(NOTE_A6, 120);
  delay(120);  
  beep(NOTE_B6, 120);
  delay(120);  
  beep(NOTE_AS6, 120);
  beep(NOTE_A6, 120);
  delay(120); 
  beep(NOTE_G6, 90);
  beep(NOTE_E7, 90);
  beep(NOTE_G7, 90);
  beep(NOTE_A7, 120);
  delay(120);  
  beep(NOTE_F7, 120);
  beep(NOTE_G7, 120);
  delay(120);  
  beep(NOTE_E7, 120);
  delay(120);  
  beep(NOTE_C7, 120);
  beep(NOTE_D7, 120);
  beep(NOTE_B6, 120);
  delay(240);  
  beep(NOTE_C4, 120);
  beep(NOTE_C5, 120);
  beep(NOTE_A3, 120);
  beep(NOTE_A4, 120);
  beep(NOTE_AS3, 120);
  beep(NOTE_AS4, 120);
  delay(90);    
  beep(NOTE_C4, 120);
  beep(NOTE_C5, 120);
  beep(NOTE_A3, 120);
  beep(NOTE_A4, 120);
  beep(NOTE_AS3, 120);
  beep(NOTE_AS4, 120);
  delay(90);    
  beep(NOTE_F3, 120);
  beep(NOTE_F4, 120);
  beep(NOTE_D3, 120);
  beep(NOTE_D4, 120);
  beep(NOTE_DS3, 120);
  beep(NOTE_DS4, 120);
  delay(90);   
  beep(NOTE_F3, 120);
  beep(NOTE_F4, 120);
  beep(NOTE_D3, 120);
  beep(NOTE_D4, 120);
  beep(NOTE_DS3, 120);
  beep(NOTE_DS4, 120);
  delay(120);    
  beep(NOTE_DS4, 180);
  beep(NOTE_CS4, 180);
  beep(NOTE_D4, 180);
  beep(NOTE_CS4, 60);
  beep(NOTE_DS4, 60);
  beep(NOTE_DS4, 60);
  beep(NOTE_GS3, 60);
  beep(NOTE_G3, 60);
  beep(NOTE_CS4, 60);
  beep(NOTE_C4, 180);
  beep(NOTE_FS4, 180);
  beep(NOTE_F4, 180);
  beep(NOTE_E3, 180);
  beep(NOTE_AS4, 180);
  beep(NOTE_A4, 180);
  beep(NOTE_GS4, 100);
  beep(NOTE_DS4, 100);
  beep(NOTE_B3, 100);
  beep(NOTE_AS3, 100);
  beep(NOTE_A3, 100);
  beep(NOTE_GS3, 100);
  delay(90);  
}
  
//******* McGyver ****************
void McGyver() {
  beep(NOTE_B4, 200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 400);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 200);
  delay(200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 400);
  delay(200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 400);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 200);
  delay(200);
  beep(NOTE_A5, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B4, 200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 400);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 200);
  delay(200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 400);
  delay(200);
  beep(NOTE_E5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 400);
  beep(NOTE_E5, 200);
  beep(NOTE_B4, 200);
  delay(200);
  beep(NOTE_A5, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 400);
  delay(200);
  beep(NOTE_B5, 800);
  delay(200);
  beep(NOTE_B5, 400);
  delay(200);
  beep(NOTE_A5, 400);
  beep(NOTE_D6, 600);
  beep(NOTE_B5, 400);
  delay(200);
  beep(NOTE_B5, 800);
  delay(200);
  beep(NOTE_B5, 200);
  delay(200);
  beep(NOTE_A5, 800);
  delay(400);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 800);
  delay(100);
  beep(NOTE_FS5, 200);
  beep(NOTE_A5, 200);
  delay(200);
  beep(NOTE_G5, 800);
  delay(200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  delay(200);
  beep(NOTE_B5, 400);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_G5, 200);
  delay(200);
  beep(NOTE_E6, 400);
  beep(NOTE_A5, 800);
  delay(100);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  delay(200);
  beep(NOTE_B5, 800);
  delay(200);
  beep(NOTE_FS5, 200);
  beep(NOTE_A5, 200);
  delay(200);
  beep(NOTE_G5, 800);
  delay(200);
  beep(NOTE_C6, 200);
  beep(NOTE_C6, 200);
  delay(200);
  beep(NOTE_B5, 400);
  beep(NOTE_A5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_G5, 200);
  delay(200);
  beep(NOTE_E6, 400);
  beep(NOTE_A5, 800);
  beep(NOTE_B5, 800);
  delay(50);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_C6, 400);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_D6, 400);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_E6, 400);
  beep(NOTE_D6, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_FS6, 400);
  beep(NOTE_B5, 400);
  beep(NOTE_G6, 400);
  delay(200);
  beep(NOTE_FS6, 400);
  beep(NOTE_F6, 400);
  beep(NOTE_B5, 400);
  beep(NOTE_G6, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_FS6, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_E5, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_D5, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_B5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_AS5, 400);
  beep(NOTE_A5, 400);
  beep(NOTE_G6, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_DS6, 200);
  beep(NOTE_DS5, 200);
  beep(NOTE_AS5, 200);
  beep(NOTE_A5, 200);
  beep(NOTE_G5, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_D5, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_DS5, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_AS4, 200);
  beep(NOTE_A4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  beep(NOTE_G4, 200);
  delay(200);
}
  
  
//******* IndianaJones ****************
void IndianaJones() {
  beep(NOTE_E5, 240);
  delay(120);
  beep(NOTE_F5, 120);
  beep(NOTE_G5, 120);
  delay(120);
  beep(NOTE_C6, 960);
  delay(180);
  beep(NOTE_D5, 240);
  delay(120);
  beep(NOTE_E5, 120);
  beep(NOTE_F5, 960);
  delay(360);
  beep(NOTE_G5, 240);
  delay(120);
  beep(NOTE_A5, 120);
  beep(NOTE_B5, 120);
  delay(120);
  beep(NOTE_F6, 960);
  delay(240);
  beep(NOTE_A5, 240);
  delay(120);
  beep(NOTE_B5, 120);
  beep(NOTE_C6, 480);
  beep(NOTE_D6, 480);
  beep(NOTE_E6, 480);
  beep(NOTE_E5, 240);
  delay(120);
  beep(NOTE_F5, 120);
  beep(NOTE_G5, 120);
  delay(120);
  beep(NOTE_C6, 960);
  delay(240);
  beep(NOTE_D6, 240);
  delay(120);
  beep(NOTE_E6, 120);
  beep(NOTE_F6, 1440);
  beep(NOTE_G5, 240);
  delay(120);
  beep(NOTE_G5, 120);
  beep(NOTE_E6, 360);
  delay(120);
  beep(NOTE_D6, 240);
  delay(120);
  beep(NOTE_G5, 120);
  beep(NOTE_E6, 360);
  delay(120);
  beep(NOTE_D6, 240);
  delay(120);
  beep(NOTE_G5, 120);
  beep(NOTE_F6, 360);
  delay(120);
  beep(NOTE_E6, 240);
  delay(120);
  beep(NOTE_D6, 120);
  beep(NOTE_C6, 480);
}
  
  
//******* StarWars coté force ****************
void StarWars2() {
  beep(NOTE_F5, 166);
  beep(NOTE_F5, 166);
  beep(NOTE_F5, 166);
  beep(NOTE_AS5, 999);
  beep(NOTE_F6, 999);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_C6, 166);
  beep(NOTE_AS6, 999);
  beep(NOTE_F6, 499);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_C6, 166);
  beep(NOTE_AS6, 999);
  beep(NOTE_F6, 499);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_DS6, 166);
  beep(NOTE_C6, 666);
  delay(333);
  beep(NOTE_F5, 166);
  beep(NOTE_F5, 166);
  beep(NOTE_F5, 166);
  beep(NOTE_AS5, 999);
  beep(NOTE_F6, 999);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_C6, 166);
  beep(NOTE_AS6, 999);
  beep(NOTE_F6, 499);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_C6, 166);
  beep(NOTE_AS6, 999);
  beep(NOTE_F6, 499);
  beep(NOTE_DS6, 166);
  beep(NOTE_D6, 166);
  beep(NOTE_DS6, 166);
  beep(NOTE_C6, 666);  
}
  
  
//******* BonBruteTruand ****************
void BonBruteTruand() {
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 802);
  beep(NOTE_FS5, 400);
  beep(NOTE_GS5, 400);
  beep(NOTE_DS5, 1071);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 802);
  beep(NOTE_FS5, 400);
  beep(NOTE_GS5, 400);
  beep(NOTE_CS6, 1071);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 802);
  beep(NOTE_FS5, 400);
  beep(NOTE_F5, 199);
  beep(NOTE_DS5, 199);
  beep(NOTE_CS5, 1071);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 133);
  beep(NOTE_DS6, 133);
  beep(NOTE_AS5, 802);
  beep(NOTE_GS5, 400);
  beep(NOTE_DS5, 1071);  
}
  
  
//******* Entertainement ****************
void Entertainement() {
  beep(NOTE_D5, 214);
  beep(NOTE_DS5, 214);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 428);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 428);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 1284);
  beep(NOTE_C6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_DS6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_E6, 428);
  beep(NOTE_B5, 214);
  beep(NOTE_D6, 428);
  beep(NOTE_C6, 856);
  delay(428);
  beep(NOTE_D5, 214);
  beep(NOTE_DS5, 214);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 428);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 428);
  beep(NOTE_E5, 214);
  beep(NOTE_C6, 1284);
  delay(214);
  beep(NOTE_A5, 214);
  beep(NOTE_G5, 214);
  beep(NOTE_FS5, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_E6, 428);
  beep(NOTE_D6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_D6, 856);  
}
  
  
//******* 20thCenFox ****************
void twentyCentFox() {
  beep(NOTE_B5, 107);
  delay(214);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 856);
  delay(107);
  beep(NOTE_C6, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_C6, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_C6, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(214);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(53);
  beep(NOTE_GS5, 107);
  delay(53);
  beep(NOTE_A5, 107);
  delay(53);
  beep(NOTE_B5, 107);
  delay(214);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 107);
  beep(NOTE_B5, 856);
  delay(428);
  beep(NOTE_E5, 214);
  beep(NOTE_GS5, 214);
  beep(NOTE_B5, 214);
  beep(NOTE_CS6, 1712);
  beep(NOTE_FS5, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_CS6, 214);
  beep(NOTE_E6, 1712);
  beep(NOTE_A5, 214);
  beep(NOTE_CS6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_E6, 1712);
  beep(NOTE_B5, 214);
  beep(NOTE_GS5, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_B5, 856);  
}
  
  
//******* looney ****************
void looney() {
  beep(NOTE_C6, 428);
  beep(NOTE_F6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_A5, 642);
  beep(NOTE_C6, 214);
  beep(NOTE_F6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_DS6, 214);
  beep(NOTE_E6, 642);
  beep(NOTE_E6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_E6, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_D6, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_C6, 214);
  beep(NOTE_G5, 214);
  beep(NOTE_AS5, 214);
  beep(NOTE_A5, 214);
  beep(NOTE_F5, 214);  
}
  
  
//******* BarbieGirl ****************
void BarbieGirl() {
  beep(NOTE_GS5, 240);
  beep(NOTE_E5, 240);
  beep(NOTE_GS5, 240);
  beep(NOTE_CS6, 240);
  beep(NOTE_A5, 480);
  delay(480);
  beep(NOTE_FS5, 240);
  beep(NOTE_DS5, 240);
  beep(NOTE_FS5, 240);
  beep(NOTE_B5, 240);
  beep(NOTE_GS5, 480);
  beep(NOTE_FS5, 240);
  beep(NOTE_E5, 240);
  delay(480);
  beep(NOTE_E5, 240);
  beep(NOTE_CS5, 240);
  beep(NOTE_FS5, 480);
  beep(NOTE_CS5, 480);
  delay(480);
  beep(NOTE_FS5, 240);
  beep(NOTE_E5, 240);
  beep(NOTE_GS5, 480);
  beep(NOTE_FS5, 480);
}
  
  
//******* Greensleaves ****************
void Greensleaves() {
  beep(NOTE_G5, 428);
  beep(NOTE_AS5, 856);
  beep(NOTE_C6, 428);
  beep(NOTE_D6, 642);
  beep(NOTE_DS6, 214);
  beep(NOTE_D6, 428);
  beep(NOTE_C6, 856);
  beep(NOTE_A5, 428);
  beep(NOTE_F5, 642);
  beep(NOTE_G5, 214);
  beep(NOTE_A5, 428);
  beep(NOTE_AS5, 856);
  beep(NOTE_G5, 428);
  beep(NOTE_G5, 642);
  beep(NOTE_F5, 214);
  beep(NOTE_G5, 428);
  beep(NOTE_A5, 856);
  beep(NOTE_F5, 428);
  beep(NOTE_D5, 856);
  beep(NOTE_G5, 428);
  beep(NOTE_AS5, 856);
  beep(NOTE_C6, 428);
  beep(NOTE_D6, 642);
  beep(NOTE_E6, 214);
  beep(NOTE_D6, 428);
  beep(NOTE_C6, 856);
  beep(NOTE_A5, 428);
  beep(NOTE_F5, 642);
  beep(NOTE_G5, 214);
  beep(NOTE_A5, 428);
  beep(NOTE_AS5, 642);
  beep(NOTE_A5, 214);
  beep(NOTE_G5, 428);
  beep(NOTE_FS5, 642);
  beep(NOTE_E5, 214);
  beep(NOTE_FS5, 428);
  beep(NOTE_G5, 856);
}
  
  
//******* James Bond *******************  
void Bond() {
  beep(NOTE_CS6, 187);
  beep(NOTE_DS6, 93);
  beep(NOTE_DS6, 93);
  beep(NOTE_DS6, 187);
  beep(NOTE_DS6, 375);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_E6, 93);
  beep(NOTE_E6, 93);
  beep(NOTE_E6, 187);
  beep(NOTE_E6, 375);
  beep(NOTE_DS6, 187);
  beep(NOTE_DS6, 187);
  beep(NOTE_DS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_DS6, 93);
  beep(NOTE_DS6, 93);
  beep(NOTE_DS6, 187);
  beep(NOTE_DS6, 375);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_E6, 93);
  beep(NOTE_E6, 93);
  beep(NOTE_E6, 187);
  beep(NOTE_E6, 375);
  beep(NOTE_DS6, 187);
  beep(NOTE_D6, 187);
  beep(NOTE_CS6, 187);
  beep(NOTE_CS7, 187);
  beep(NOTE_C7, 1125);
  beep(NOTE_GS6, 187);
  beep(NOTE_FS6, 187);
  beep(NOTE_GS6, 1125);
}

