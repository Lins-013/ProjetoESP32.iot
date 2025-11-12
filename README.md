 Hidratação Inteligente Utilizando IoT
 Descrição do Projeto

O projeto Hidratação Inteligente tem como objetivo monitorar o nível de água em um recipiente utilizando um sensor ultrassônico HC-SR04 e o microcontrolador ESP32.
Com base na distância medida, o sistema aciona LEDs indicadores para alertar o usuário sobre a necessidade de reabastecimento.
Os dados coletados são enviados via Wi-Fi para o HiveMQ Cloud, possibilitando o acompanhamento remoto do status do reservatório em tempo real.

 Funcionamento e Uso

1. O sensor ultrassônico mede a distância entre o sensor e o nível de água.

2. O ESP32 interpreta os dados e aciona os LEDs:

- LED vermelho: nível baixo de água.

- LED verde: nível adequado.

3. As medições são enviadas via protocolo MQTT para o servidor HiveMQ Cloud.

4. O usuário pode visualizar os dados remotamente utilizando um cliente MQTT (como MQTT Explorer ou painel online do HiveMQ).

Software e Documentação de Código

O código principal foi desenvolvido em C++ na IDE Arduino, utilizando as seguintes bibliotecas:

- WiFi.h — Conexão Wi-Fi.

- WiFiClientSecure.h — Comunicação segura (TLS).

- PubSubClient.h — Implementação do protocolo MQTT.

- time.h — Sincronização com servidor NTP.

Arquivos incluídos:

- index.cpp — Código principal do projeto.

- diagram.png — Diagrama de montagem no protoboard.

- README.md — Este arquivo explicativo.

Documentação do código:
O código contém comentários explicando as etapas de inicialização do Wi-Fi, leitura do sensor, lógica dos LEDs e envio de mensagens MQTT.

Hardware Utilizado
Componente	Função	Pinos / Observações
- ESP32	Microcontrolador com Wi-Fi integrado	Alimentado por USB (5V)
- HC-SR04	Sensor de distância ultrassônico	TRIG = GPIO 32 / ECHO = GPIO 33
- LED Verde	Indica nível adequado	GPIO 26
- LED Vermelho	Indica lembrete ativo	GPIO 25
- Jumpers / Protoboard	Conexões elétricas	—
- Resistores 330Ω	Limita corrente dos LEDs	Em série com cada LED

 Interfaces, Protocolos e Módulos de Comunicação
 Interfaces de Comunicação

Digital: Comunicação entre ESP32 e o sensor (TRIG/ECHO).

Digital: Controle dos LEDs pelos pinos GPIO.

Wi-Fi: Conexão do ESP32 à internet.

 Protocolos de Comunicação

TCP/IP — Base para a comunicação com o HiveMQ Cloud.

MQTT (Message Queuing Telemetry Transport) — Envia e recebe mensagens leves entre o ESP32 e o servidor.

NTP — Sincronização de horário.

 Módulos e Bibliotecas

Wi-Fi interno do ESP32 — Comunicação via rede.

PubSubClient.h — Publicação e assinatura no broker HiveMQ.

WiFiClientSecure.h — Conexão MQTT com TLS.

Comunicação via Internet e MQTT

O sistema utiliza o protocolo MQTT com o HiveMQ Cloud para comunicação segura.

Broker: 30c68303315d4a4491e1cf6de7d23795.s1.eu.hivemq.cloud

Porta: 8883

Usuário: ProjetoAgua123

Tópico: joaovictor/projetoagua

Mensagens publicadas: status do sistema e alertas de hidratação.

Autores

- João Victor Santos Lins

- Leandro Mateus de Freitas Santos


