#include "LoRaWan_APP.h"
#include "HT_SSD1306Wire.h"
#include <Wire.h>

// ======================================================
// ================= DISPLAY (extern) ===================
// ======================================================
extern SSD1306Wire display;

// ======================================================
// =================== OTAA KEYS =========================
// ======================================================
// Nodo 1

uint8_t devEui[8] = {
  0x2B, 0x60, 0x79, 0xCE,
  0x32, 0x5A, 0x6D, 0x85
};

uint8_t appEui[8] = {
  0xAB, 0xCF, 0x5B, 0x76,
  0xDA, 0x19, 0x94, 0xFF
};

uint8_t appKey[16] = {
  0xE3, 0xCE, 0x19, 0x56,
  0x65, 0xBA, 0x43, 0xFB,
  0x01, 0xD8, 0x3B, 0x1A,
  0xD3, 0x35, 0x17, 0xA0
};

// ======================================================
// ===== Required by Heltec stack (even OTAA) ============
// ======================================================
uint32_t devAddr = 0;
uint8_t nwkSKey[16] = {0};
uint8_t appSKey[16] = {0};

// ======================================================
// ================= LoRaWAN PARAMS ======================
// ======================================================

// US915 → sub-band 0 (canales 0–7)
uint16_t userChannelsMask[6] = {
  0x00FF, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000
};

LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_US915;
DeviceClass_t loraWanClass = CLASS_A;

uint32_t appTxDutyCycle = 1000;   // 1 segundo
bool overTheAirActivation = true;
bool loraWanAdr = false;
bool isTxConfirmed = false;
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 8;

// ======================================================
// ================= OLED HELPERS ========================
// ======================================================

void showStatus(String title, String msg) {
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Nodo 1 - LoRaWAN");
  display.drawLine(0, 12, 127, 12);
  display.drawString(0, 20, title);
  display.drawString(0, 36, msg);
  display.display();
}

// ======================================================
// ================= PAYLOAD =============================
// ======================================================

static void prepareTxFrame(uint8_t port) {
  float temp = 20.0 + random(-50, 50) / 10.0;
  int16_t temp_int = temp * 10;

  appData[0] = (temp_int >> 8) & 0xFF;
  appData[1] = temp_int & 0xFF;
  appDataSize = 2;

  Serial.print("[TX] Nodo 1 -> Temp = ");
  Serial.print(temp);
  Serial.println(" C");

  showStatus("TX UPLINK", String(temp) + " C");
}

// ======================================================
// ================= SETUP ===============================
// ======================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(50);

  display.init();
  display.clear();
  display.display();

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

  Serial.println("================================");
  Serial.println(" Nodo 1 - Heltec LoRaWAN OTAA ");
  Serial.println("================================");

  showStatus("INIT", "Esperando JOIN");
}

// ======================================================
// ================= LOOP ================================
// ======================================================

void loop() {

  switch (deviceState) {

    case DEVICE_STATE_INIT:
      Serial.println("[STATE] INIT");
      LoRaWAN.init(loraWanClass, loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      deviceState = DEVICE_STATE_JOIN;
      break;

    case DEVICE_STATE_JOIN:
      Serial.println("[STATE] JOINING (OTAA)");
      showStatus("JOINING", "Enviando request");
      LoRaWAN.join();
      break;

    case DEVICE_STATE_SEND:
      Serial.println("[STATE] SEND");
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;

    case DEVICE_STATE_CYCLE:
      Serial.println("[STATE] CYCLE");
      txDutyCycleTime = appTxDutyCycle;
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;

    case DEVICE_STATE_SLEEP:
      Serial.println("[STATE] SLEEP");
      showStatus("SLEEP", "Esperando ciclo");
      LoRaWAN.sleep(loraWanClass);
      break;

    default:
      deviceState = DEVICE_STATE_INIT;
      break;
  }
}
