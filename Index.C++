#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>

// ==== CONFIGURAÇÕES DO SEU WI-FI ====
const char* ssid = "CLARO_2GE46BD4";
const char* password = "7AE46BD4";

// ==== CONFIGURAÇÕES DO HIVE MQ CLOUD ====
const char* mqtt_server = "30c68303315d4a4491e1cf6de7d23795.s1.eu.hivemq.cloud";
const int mqtt_port = 8883; //
const char* mqtt_username = "ProjetoAgua123";
const char* mqtt_password = "Senha1234";
const char* mqtt_topic = "joaovictor/projetoagua";

// ==== OBJETOS ====
WiFiClientSecure espClient;
PubSubClient client(espClient);

// ==== PINOS ====
const int trigPin = 32;
const int echoPin = 33;
const int ledVerde = 26;
const int ledVermelho = 25;

// ==== VARIÁVEIS ====
unsigned long ultimoTempo = 0;
const unsigned long intervalo = 10000;
bool lembreteAtivo = false;

void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi conectado!");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao HiveMQ Cloud...");
    
    String clientId = "ESP32_Hidratacao_";
    clientId += String(random(0xffff), HEX);
    
    // CONEXÃO COM AUTENTICAÇÃO
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Conectado ao HiveMQ Cloud!");
      Serial.print("Usuário: ");
      Serial.println(mqtt_username);
      
      // Publicar mensagem de conexão
      client.publish(mqtt_topic, "Sistema conectado ao HiveMQ Cloud!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s...");
      delay(5000);
    }
  }
}

long medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 30000);
  if (duracao == 0) return -1;

  long distancia = duracao * 0.034 / 2;
  if (distancia < 2 || distancia > 400) return -1;

  return distancia;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledVermelho, LOW);

  // Mantém isso — conecta ao WiFi
  espClient.setInsecure();
  conectarWiFi();

  //relógio com NTP
  Serial.println("Sincronizando hora com NTP...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n Hora sincronizada com sucesso!");

  //
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  long distancia = medirDistancia();
  Serial.print("Distância: ");
  Serial.println(distancia);

  if (distancia > 0 && distancia < 15) {
    Serial.println("Objeto detectado!");
    digitalWrite(ledVermelho, LOW);
    client.publish(mqtt_topic, "Objeto detectado - LED vermelho apagado");
    ultimoTempo = millis();
    lembreteAtivo = false;
  } else if (millis() - ultimoTempo > intervalo && !lembreteAtivo) {
    digitalWrite(ledVermelho, HIGH);
    Serial.println("Lembrete: LED vermelho aceso!");
    client.publish(mqtt_topic, "Lembrete: Hora de beber água!");
    lembreteAtivo = true;
  }

  // 10s
  static unsigned long statusTempo = 0;
  if (millis() - statusTempo > 10000) {
    client.publish(mqtt_topic, "ESP32 online e funcionando!");
    statusTempo = millis();
  }

  delay(500);
}
