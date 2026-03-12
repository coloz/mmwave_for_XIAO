/*
 * mmWave for XIAO - ESP32 Hardware Serial Example
 *
 * This example demonstrates using the mmWave sensor with ESP32 boards
 * via hardware serial (no extra library needed). ESP32 has multiple
 * hardware UARTs, which are more stable and efficient than software serial.
 *
 * Requirements:
 *   - Set sensor baud rate to 9600 using HLKRadarTool APP before use
 *
 * Wiring (XIAO ESP32C3 / ESP32S3 / ESP32C6):
 *   Sensor TX -> D7 (XIAO RX1, mapped to Serial1)
 *   Sensor RX -> D6 (XIAO TX1, mapped to Serial1)
 *
 * Note: The default pins for Serial1 vary by board. Adjust RX_PIN and TX_PIN
 *       below to match your wiring. On XIAO boards, D7/D6 are commonly used.
 *
 * Tested on: XIAO ESP32C3, XIAO ESP32S3, XIAO ESP32C6
 */

#if !defined(ESP32)
#error "This example is for ESP32 boards only."
#endif

#include <mmwave_for_xiao.h>

// Hardware Serial1 pin definitions - adjust for your board/wiring
#define RX_PIN D7
#define TX_PIN D6

#define ShowSerial Serial

// Initialize radar with hardware Serial1 and debug serial
Seeed_HSP24 xiao_config(Serial1, ShowSerial);

Seeed_HSP24::RadarStatus radarStatus;

void setup() {
  // Initialize hardware Serial1 with custom pins
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  ShowSerial.begin(115200);
  delay(500);

  ShowSerial.println("mmWave for XIAO - ESP32 HardwareSerial Example");
  ShowSerial.println("Programme Starting!");
}

void loop() {
  int retryCount = 0;
  const int MAX_RETRIES = 10;

  // Get radar status
  do {
    radarStatus = xiao_config.getStatus();
    retryCount++;
  } while (radarStatus.distance == -1 && retryCount < MAX_RETRIES);

  // Parse and print radar status
  if (radarStatus.distance != -1) {
    ShowSerial.print("Status: " + String(targetStatusToString(radarStatus.targetStatus)) + "  ----   ");
    ShowSerial.println("Distance: " + String(radarStatus.distance) + "  Mode: " + String(radarStatus.radarMode));

    if (radarStatus.radarMode == 1) {
      ShowSerial.print("Move: ");
      for (int i = 0; i < 9; i++) {
        ShowSerial.print(String(radarStatus.radarMovePower.moveGate[i]) + "  ,");
      }
      ShowSerial.println("");
      ShowSerial.print("Static: ");
      for (int i = 0; i < 9; i++) {
        ShowSerial.print(String(radarStatus.radarStaticPower.staticGate[i]) + "  ,");
      }
      ShowSerial.println("");
      ShowSerial.println("Photosensitive: " + String(radarStatus.photosensitive));
    }
  }
  delay(200);
}

const char* targetStatusToString(Seeed_HSP24::TargetStatus status) {
  switch (status) {
    case Seeed_HSP24::TargetStatus::NoTarget:
      return "NoTarget";
    case Seeed_HSP24::TargetStatus::MovingTarget:
      return "MovingTarget";
    case Seeed_HSP24::TargetStatus::StaticTarget:
      return "StaticTarget";
    case Seeed_HSP24::TargetStatus::BothTargets:
      return "BothTargets";
    default:
      return "Unknown";
  }
}
