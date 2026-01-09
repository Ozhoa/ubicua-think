#include <DHT.h>
#include <WiFiNINA.h>   // Librería correcta para Nano 33 IoT
#include <ThingSpeak.h>

// --- Configuración del Sensor DHT22 ---
#define DHTPIN 15       // Pin de datos conectado al pin 15 del Nano
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Configuración del WiFi ---
// REEMPLAZA ESTOS DATOS CON LOS DE TU CASA/MOVIL
const char* ssid = "Doraemon"; 
const char* password = "aaaaaaaa"; 

// --- Configuración de ThingSpeak ---
unsigned long myChannelNumber = 3224241;         // Tu ID de canal
const char* myWriteAPIKey = "L4HX5XKQU14QQING";  // Tu API Key

WiFiClient client; // El cliente para la conexión a internet

void setup() {
  Serial.begin(9600);
  while (!Serial); // Espera a que el monitor serial se abra
  
  dht.begin();
  ThingSpeak.begin(client); // Inicia ThingSpeak con el cliente WiFi
  
  // Conectar a la red WiFi
  Serial.print("Conectando a: Doraemon");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n¡Conectado exitosamente!");
}

void loop() {
  // Leer los datos del sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verificar si hay errores en la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer el sensor DHT22!");
    return;
  }

  // Mostrar datos en el Monitor Serial (Como en tu Figura 9)
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" °C Humedad: ");
  Serial.print(h);
  Serial.println(" %");

  // Asignar los valores a los campos de la gráfica
  ThingSpeak.setField(1, t); // Field 1 para Temperatura
  ThingSpeak.setField(2, h); // Field 2 para Humedad
  
  // Enviar los datos a ThingSpeak
  int respuesta = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(respuesta == 200){
    Serial.println("Datos enviados correctamente a ThingSpeak.");
  } else {
    Serial.println("Error al enviar a ThingSpeak. Código: " + String(respuesta));
  }

  // ThingSpeak requiere al menos 15 segundos entre actualizaciones
  delay(20000); 
}