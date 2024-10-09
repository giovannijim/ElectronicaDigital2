
// Se crean variables
int xVal;
int yVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Se leen los puertos analogos
  xVal = analogRead(34);
  yVal = analogRead(35);
  
  // Se hace la comparacion de valores, recordar que los puertos ADC son de 12 Bits
  if (xVal >= 4000){ Serial.print("\n 1"); }
  if (xVal <= 100){ Serial.print("\n 2"); }
  if (yVal >= 4000){ Serial.print("\n 3"); }
  if (yVal <= 100){ Serial.print("\n 4"); }
  delay(100);
}
