/*
 * mmWave for XIAO - ESP32 Software Serial Example
 *
 * This example demonstrates using the mmWave sensor with ESP32 boards
 * via software serial (EspSoftwareSerial library).
 *
 * Requirements:
 *   - Install "EspSoftwareSerial" library from Arduino Library Manager
 *   - Set sensor baud rate to 9600 using HLKRadarTool APP before use
 *
 * Wiring:
 *   Sensor TX -> D2 (XIAO RX)
 *   Sensor RX -> D3 (XIAO TX)
 *
 * Tested on: XIAO ESP32C3, XIAO ESP32S3, XIAO ESP32C6
 */

#if !defined(ESP32)
#error "This example is for ESP32 boards only. Use mmwave_for_xiao_example for other platforms."
#endif

#include <SoftwareSerial.h>  // EspSoftwareSerial library
#include <mmwave_for_xiao.h>

// Software serial: D2 as RX, D3 as TX
SoftwareSerial COMSerial(D2, D3);

#define ShowSerial Serial

// Initialize radar with communication serial and debug serial
Seeed_HSP24 xiao_config(COMSerial, ShowSerial);

Seeed_HSP24::RadarStatus radarStatus;

void setup() {
  COMSerial.begin(9600);
  ShowSerial.begin(115200);
  delay(500);

  ShowSerial.println("mmWave for XIAO - ESP32 SoftwareSerial Example");
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
