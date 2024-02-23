#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <HardwareSerial.h>

const char *ssid = "GL-SFT1200-19f";
const char *password = "goodlife";

WebServer server(80);
WiFiUDP udp;
HardwareSerial SerialPort(2);  // use UART2

void setup() {
  Serial.begin(115200);
  SerialPort.begin(15200, SERIAL_8N1, 16, 17);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("つないでるぜ");
  }

  Serial.println("つながったぜ");
  udp.begin(1234);  // Port to listen on
  Serial.println(WiFi.localIP());

  server.begin();
  
  pinMode(33, OUTPUT);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    udp.read(incomingPacket, packetSize);
    incomingPacket[packetSize] = '\0';
    Serial.println(incomingPacket);
    SerialPort.print(incomingPacket);
    SerialPort.print("\n");

    // 文字列に 'a' が含まれ、かつ後ろの数値が300以上の場合はLEDを点灯
    if (incomingPacket[0] == 'a') {
      int value = atoi(incomingPacket + 1); // 文字列から数値を取得
      if (value >= 300) {
        digitalWrite(33, HIGH);
      }
    }
    // 文字列に 'b' が含まれ、かつ後ろの数値が300未満の場合はLEDを消灯
    else if (incomingPacket[0] == 'b') {
      int value = atoi(incomingPacket + 1); // 文字列から数値を取得
      if (value < 300) {
        digitalWrite(33, LOW);
      }
    }
  }

  server.handleClient();
}
