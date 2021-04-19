/**********************************************************************
  Filename    : Bluetooth Music By PCM5102
  Description : Use ESP32 to play the audio received via Bluetooth.
                Use your phone's Bluetooth connection to ESP32, 
                then open the music APP and play the music, 
                and you'll hear ESP32 play the phone's music
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include "BluetoothSerial.h"
#include "driver/i2s.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"

////

#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Keypad.h>
//set up to connect to an existing network (e.g. mobile hotspot from laptop that will run the python code)
const char* ssid = "APlaceToDequeue";
const char* password = "elsebeer++";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets
char song[255];
///


#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins
////
#define CONFIG_I2S_LRCK_PIN 25
#define CONFIG_I2S_BCK_PIN  26
#define CONFIG_I2S_DATA_PIN 22

BluetoothSerial SerialBT;
//

LiquidCrystal_I2C lcd(0x27,16,2); 
void setup() {

  Wire.begin(SDA, SCL);           // attach the IIC pin
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  lcd.setCursor(0,0);             // Move the cursor to row 0, column 0
  lcd.print("");     // The print content is displayed on the LCD
/////
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Init seccess!");

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
 
  i2s_config_t i2s_config;
  i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
  i2s_config.sample_rate = 44100;
  i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
  i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
  i2s_config.intr_alloc_flags = 0;
  i2s_config.dma_buf_count = 6;
  i2s_config.dma_buf_len = 60;
  i2s_config.tx_desc_auto_clear = true;
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  
  i2s_pin_config_t pin_config;
  pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
  pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
  pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
  pin_config.data_in_num = -1;
  i2s_set_pin(I2S_NUM_0, &pin_config);

  bt_app_task_start_up();
  
 /* initialize A2DP sink */
  esp_a2d_register_callback(&bt_app_a2d_cb);
  esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
  esp_a2d_sink_init();
  /* initialize AVRCP controller */
  esp_avrc_ct_init();
  esp_avrc_ct_register_callback(bt_app_rc_ct_cb);
  /* set discoverable and connectable mode, wait to be connected */
  esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
  Serial.println("ok");
      int status = WL_IDLE_STATUS;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Hello, World!");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  // we recv one packet from the remote so we can know its IP and port

}

void loop(){
   bool readPacket = false;
  while (!readPacket) {
    int packetSize = Udp.parsePacket();
    if (packetSize)
     {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      for (int i = 0; i < 255 ; i++){
        song[i] = incomingPacket[i];
      }
      Serial.print(song);
      lcd.setCursor(0,1);             // Move the cursor to row 1, column 0
      lcd.print("Song:");          // The count is displayed every second
      lcd.print(song);
      delay(1000);

      //readPacket = true;
    } 
  }
  // once we know where we got the inital packet from, send data back to that IP address and port
  //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  
//Udp.print("HEELO?????");
  //Udp.endPacket();
  //delay(1000);
  ;  
 
}
