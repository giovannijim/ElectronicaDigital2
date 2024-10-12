#include <esp_now.h>
#include <WiFi.h>

// Dirección MAC del receptor
uint8_t receptorMAC[] = {0x24, 0x0A, 0xC4, 0x5F, 0x8D, 0x3C};

// Estructura de datos
typedef struct {
  int xVal1;
  int xVal2;
  int yVal1;
  int yVal2;
} Datos;

// Definir un buffer para enviar el mensaje como cadena
char mensaje[10];

Datos datosEnvio;

// Función de callback
void enviarCallback(const uint8_t *mac, esp_now_send_status_t status) {
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envío exitoso" : "Envío fallido");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  // Inicialización de ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error al inicializar ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(enviarCallback);

  // Añadir el receptor como peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receptorMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    //Serial.println("Error al añadir el peer");
    return;
  }
}

void loop() {
  // Leer valores del joystick
  int xVal = analogRead(34);
  int yVal = analogRead(35);

  // Comprobar si hay movimiento significativo en X+
  if (xVal >= 4000) {
    strcpy(mensaje, "r");
    Serial.print('r');
    //datosEnvio.xVal1 = 1;
    // Enviar datos al receptor
    esp_now_send(receptorMAC, (uint8_t *) &mensaje, sizeof(mensaje));
  }
   // Comprobar si hay movimiento significativo en X-
  if (xVal <= 100) {
    //datosEnvio.xVal2 = 2;
    strcpy(mensaje, "l");
    Serial.print('l');
    // Enviar datos al receptor
    esp_now_send(receptorMAC, (uint8_t *) &mensaje, sizeof(mensaje));
  }
  // Comprobar si hay movimiento significativo en Y+
  if (yVal >= 4000) {
    //datosEnvio.yVal1 = 3;
    //strcpy(mensaje, "3");
    // Enviar datos al receptor
    //esp_now_send(receptorMAC, (uint8_t *) &mensaje, sizeof(mensaje));
  }
  // Comprobar si hay movimiento significativo en Y+
  if (yVal <= 100) {
    //datosEnvio.yVal2 = 4;
    //strcpy(mensaje, "4");
    // Enviar datos al receptor
    //esp_now_send(receptorMAC, (uint8_t *) &mensaje, sizeof(mensaje));
  }

  delay(100); // Pequeño retraso para evitar envíos constantes
}
