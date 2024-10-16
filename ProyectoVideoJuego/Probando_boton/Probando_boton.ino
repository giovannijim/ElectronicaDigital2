#define BUTTON_PIN 15 // Pin al que conectaste el botón

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura el pin como entrada con pull-up interno
  Serial.begin(115200);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // Lee el estado del botón

  if (buttonState == LOW) { // Si el botón está presionado
    Serial.println("Botón presionado");
  } else {
    Serial.println("Botón no presionado");
  }
  delay(100); // Pequeña pausa para evitar ruido
}