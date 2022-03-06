//ALICI

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>


typedef struct struct_message 
{
  int pot; // Potansiyometreden Okunan Değer
  
} struct_message;

struct_message haberlesmeverileri;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) // Veri Alım Kontrolü
{
  memcpy(&haberlesmeverileri, incomingData, sizeof(haberlesmeverileri));
  Serial.print("Okunan Potansiyometre Degeri: ");
  Serial.println(haberlesmeverileri.pot);
  Serial.print("Alınan Veri Boyutu (byte): ");
  Serial.println(len);
}

Servo servo1; // Sistemdeki servo tanıtıldı
 
void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // WiFi Station Modu Aktif
  
  servo1.attach(5); // Servoya bağlı olan pin tanıtıldı [D1 pini]

  if (esp_now_init() != 0) // ESP-NOW Aktif
  {
    Serial.println("Hata: ESP-NOW Başlatılırken Bir Sorun Oluştu!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE); // Kart alıcı moduna ayarlandı
  esp_now_register_recv_cb(OnDataRecv); // Veri Alma İşlemi
}

void loop()
{
  servo1.write(haberlesmeverileri.pot);
  delay(20);
}
