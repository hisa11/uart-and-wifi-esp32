#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>

const char *ssid = "GL-SFT1200-19f";
const char *password = "goodlife";

#include <HardwareSerial.h>

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  SerialPort.begin(15200, SERIAL_8N1, 16, 17);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("つないでるぜ");
  }

  Serial.println("つながったぜ");
}

void loop() {
  if (Serial.available() > 0) {
    char buffer[255];
    int bytesRead = Serial.readBytesUntil('\n', buffer, 255); // 改行までのデータを読み込む
    buffer[bytesRead] = '\0'; // NULL終端を追加

    udp.beginPacket(IPAddress(192, 168, 8, 154), 1234); // 受信側ESP32のIPアドレスとポート
    udp.print(buffer);
    udp.endPacket();
    Serial.println("送信しました: " + String(buffer)); // 送信したデータをシリアルモニターに出力
  }
  if (SerialPort.available() > 0) {
}
