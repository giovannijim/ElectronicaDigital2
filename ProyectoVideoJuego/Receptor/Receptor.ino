#include <esp_now.h>
#include <WiFi.h>

#define BUTTON_PIN 32 // Pin al que conectaste el botón
#define BUTTON_PIN_1 33 // Pin al que conectaste el botón
// Buffer para almacenar el mensaje recibido
char mensajeRecibido[10];

// Función de callback para recibir datos
void recibirCallback(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // Copiar el dato recibido al buffer mensajeRecibido
  memcpy(mensajeRecibido, incomingData, len);
  mensajeRecibido[len] = '\0'; // Asegurar que se termine con un carácter nulo para usarlo como cadena
  
  // Imprimir el valor recibido
  //Serial.print("Valor recibido: ");
  Serial.print(mensajeRecibido);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura el pin como entrada con pull-up interno
  pinMode(BUTTON_PIN_1, INPUT_PULLUP); // Configura el pin como entrada con pull-up interno
  // Inicialización de ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error al inicializar ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(recibirCallback);
}

void loop() {
    // Leer valores del joystick
  int xVal = analogRead(34);
  int yVal = analogRead(35);
  int buttonState = digitalRead(BUTTON_PIN); // Lee el estado del botón
  int buttonState1 = digitalRead(BUTTON_PIN_1); // Lee el estado del botón

  // Comprobar si hay movimiento significativo en X+
  if (xVal >= 4000) {
    Serial.print('3');
  }
   // Comprobar si hay movimiento significativo en X-
  if (xVal <= 100) {
    Serial.print('1');
  }
  // Comprobar si hay movimiento significativo en Y+
  if (yVal >= 4000) {
    Serial.print('4');
  }
  // Comprobar si hay movimiento significativo en Y+
  if (yVal <= 100) {
    Serial.print('2');
  }
  if (buttonState == LOW) { // Si el botón está presionado
    Serial.print('5');
  }
  if (buttonState1 == LOW) { // Si el botón está presionado
    Serial.print('6');
  }
  delay(100); // Pequeño retraso para evitar envíos constantes
}

