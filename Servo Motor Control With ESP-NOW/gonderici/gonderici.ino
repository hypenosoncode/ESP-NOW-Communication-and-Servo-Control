//VERİCİ

#include <ESP8266WiFi.h>
#include <espnow.h>

int potpin = A0;
int potpindeger;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Alıcı MAC Adresi

typedef struct struct_message // Gönderilecek Veri
{
  int pot; // Potansiyometreden Okunan Değer

} struct_message;

struct_message haberlesmeverileri;

unsigned long ilkzaman = 0;  
unsigned long gonderme_gecikmesi = 500;  // Veriler ne kadar sürede bir gönderilecek?

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) // Veri Gönderim Kontrolü
{
  Serial.print("Veri Gonderim Durumu: ");
  
  if (sendStatus == 0)
  {
    Serial.println("Veri Gönderme İşlemi Başarılı");
  }
  else{
    Serial.println("Veri Gönderilemedi");
  }
}

void setup() 
{

  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // WiFi Station Modu Aktif

  if (esp_now_init() != 0) // ESP-NOW Aktif
  {
    Serial.println("Hata: ESP-NOW Başlatılırken Bir Sorun Oluştu!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // Kart verici moduna ayarlandı
  esp_now_register_send_cb(OnDataSent); // Veri Gönderme İşlemi
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0); // MAC Adresinin alıcı ile eşleşme işlemi
}
 
void loop() 
{
  if ((millis() - ilkzaman) > gonderme_gecikmesi) 
  {
  potpindeger = analogRead(potpin); // Potansiyometreden Alınan Veri Okunur
  potpindeger = map(potpindeger, 0, 1023, 0, 180); // Potansiyometreden alınan veri 0-180 derece olarak kesilir
  
  if (isnan(potpindeger)) // Potansiyometre veri kontrolü
  {
    Serial.println("Potansiyemete degeri okunamadı!");
  }
  Serial.println(potpindeger); // Potansiyometreden Okunan değer ekrana bastırılır.

  haberlesmeverileri.pot = potpindeger; // Potansiyometreden okunan değer gönderilecek olan struct verisine aktarılır
  esp_now_send(broadcastAddress, (uint8_t *) &haberlesmeverileri, sizeof(haberlesmeverileri)); // Okunan değer gönderilmeye başlanır

  ilkzaman = millis(); // Sayaç güncelleme işlemi
  }
}
