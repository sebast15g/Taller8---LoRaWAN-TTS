#include "LoRaWan_APP.h"
#include "HT_SSD1306Wire.h"
#include <Wire.h>

// ======================================================
// =============== DISPLAY (externo) ====================
// ======================================================
extern SSD1306Wire display;

// ======================================================
// =================== OTAA KEYS =========================
// ======================================================
// Nodo 2

uint8_t devEui[8] = {
  0x61, 0x65, 0x19, 0xCD,
  0x46, 0x77, 0x36, 0xAD
};

uint8_t appEui[8] = {
  0xFD, 0x44, 0xC1, 0x9A,
  0x4B, 0x44, 0x12, 0xE2
};

uint8_t appKey[16] = {
  0x3B, 0x4A, 0xF8, 0xF9,
  0xF4, 0x90, 0x92, 0xDE,
  0xDD, 0x4C, 0xEB, 0x59,
  0xCD, 0x2B, 0xF1, 0x68
};

// ======================================================
// ====== REQUERIDOS POR LA LIB (INCLUSO OTAA) ===========
// ======================================================
uint32_t devAddr = 0;
uint8_t nwkSKey[16] = {0};
uint8_t appSKey[16] = {0};

// ======================================================
// ================= LoRaWAN PARAMS ======================
// ======================================================

// US915 → sub-banda 0 (canales 0–7)
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
uint8_t appPort = 3;              // distinto al Nodo 1
uint8_t confirmedNbTrials = 8;

// ======================================================
// ================= OLED HELPERS ========================
// ======================================================

void showStatus(String line1, String line2) {
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Nodo 2 - LoRaWAN");
  display.drawLine(0, 12, 127, 12);
  display.drawString(0, 20, line1);
  display.drawString(0, 34, line2);
  display.display();
}

// ======================================================
// ================= PAYLOAD =============================
// ======================================================

static void prepareTxFrame(uint8_t port) {
  // Humedad simulada (30–90 %)
  float hum = 30.0 + random(0, 600) / 10.0;
  uint16_t hum_int = hum * 10;

  appData[0] = (hum_int >> 8) & 0xFF;
  appData[1] = hum_int & 0xFF;
  appDataSize = 2;

  Serial.print("Nodo 2 TX -> Humedad: ");
  Serial.print(hum);
  Serial.println(" %");

  showStatus("TX Humedad", String(hum) + " %");
}

// ======================================================
// ================= SETUP ===============================
// ======================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Encender OLED
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(50);

  display.init();
  display.clear();
  display.display();

  // Inicialización MCU / Radio
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

  Serial.println("Nodo 2 iniciado - OTAA");
  showStatus("INIT", "Esperando JOIN");
}

// ======================================================
// ================= LOOP ================================
// ======================================================

void loop() {
  switch (deviceState) {

    case DEVICE_STATE_INIT:
      LoRaWAN.init(loraWanClass, loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      deviceState = DEVICE_STATE_JOIN;
      break;

    case DEVICE_STATE_JOIN:
      Serial.println("Nodo 2 -> JOIN OTAA...");
      showStatus("JOIN", "Enviando request");
      LoRaWAN.join();
      break;

    case DEVICE_STATE_SEND:
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;

    case DEVICE_STATE_CYCLE:
      txDutyCycleTime = appTxDutyCycle;
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;

    case DEVICE_STATE_SLEEP:
      LoRaWAN.sleep(loraWanClass);
      break;

    default:
      deviceState = DEVICE_STATE_INIT;
      break;
  }
}
