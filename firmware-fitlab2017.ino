// Librerías utilizadas en el programa.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Credenciales del broker MQTT.
const char* ssid         = "****";
const char* password     = "****";
const char* mqttServer   = "m10.cloudmqtt.com";
const int   mqttPort     = 14072;
const char* mqttUser     = "mcntvkbj";
const char* mqttPassword = "boAB3D4cMuYu";

WiFiClient espClient;
PubSubClient client(espClient);

// Inicialización del microcontrolador.
void setup() {
  // Inicialización del puerto serial.
  Serial.begin(115200);
  
  // Definimos la red Wi-Fi a la que nos conectaremos.
  WiFi.begin(ssid, password);
  
  // Intentamos conectarnos a la red Wi-Fi definida anteriormente.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("- Conectando a Wi-Fi..");
  }
  
  // Intentamos conectarnos a la red Wi-Fi definida anteriormente.
  Serial.println("- ¡Conexión Wi-Fi exitosa!");
  client.setServer(mqttServer, mqttPort);
  
  // Definimos la función que se ejecutará cuando el servidor nos envíe información de vuelta. 
  client.setCallback(callback);
  
  // Intentamos conectarnos al broker MQTT definido anteriormente.
  while (!client.connected()) {
    Serial.println("- Conectando a broker MQTT...");
    if (client.connect("FIT01Client", mqttUser, mqttPassword )) {
      Serial.println("- ¡Conexión con servidor MQTT exitosa!");  
    } else {
      Serial.print("- ¡ERROR DE CONEXIÓN CON SERVIDOR MQTT!");
      Serial.print(client.state());
      delay(2000);

    }
  }
  
  // Si logramos conectarnos al broker MQTT,
  // nos suscribimos al tema que "escucharemos".
  client.subscribe("FIT01");
  
  // Publicamos la información de los sensores (JSON) al broker MQTT.
  client.publish("data", "{\"id\":\"FIT01\",\"T\":14,\"H\":35,\"L\":23,\"O\":89}");
}

// Función que nos mostrará la respuesta del broker MQTT al NodeMCU.
void callback(char* topic, byte* payload, unsigned int length) {

  // Mostramos el tema que nos está enviando una respuesta.
  Serial.print("- Mensaje recibido del tema: ");
  Serial.println(topic);
  
  // Mostramos el mensaje que el tema nos está enviando.
  Serial.print("- Mensaje:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();
  Serial.println("-----------------------");

}

// Ciclo principal del programa.
void loop() {
  client.loop();
}
