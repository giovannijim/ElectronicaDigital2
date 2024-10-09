#include <esp_now.h>
#include <WiFi.h>

// Buffer para almacenar el mensaje recibido
char mensajeRecibido[10];

// Función de callback para recibir datos
void recibirCallback(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // Copiar el dato recibido al buffer mensajeRecibido
  memcpy(mensajeRecibido, incomingData, len);
  mensajeRecibido[len] = '\0'; // Asegurar que se termine con un carácter nulo para usarlo como cadena
  
  // Imprimir el valor recibido
  //Serial.print("Valor recibido: ");
  Serial2.println(mensajeRecibido);
}

void setup() {
  Serial2.begin(115200);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  // Inicialización de ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(recibirCallback);
}

void loop() {
  // No se necesita código adicional en el loop
}

