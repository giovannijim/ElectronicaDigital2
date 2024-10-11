void setup() {
  // put your setup code here, to run once:
  //Serial2.begin(115200);
  Serial.begin(115200);
}

void loop() {
  //Serial2.print('a');
  Serial.print('a');
  delay(1000);
  /*
  if(Serial2.available()){
    char data = Serial2.read();    // Lee el dato recibido
    Serial.print("Dato recibido: ");
    Serial.println(data);          // Muestra el dato en la terminal
  }*/
}
