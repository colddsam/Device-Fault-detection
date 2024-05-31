#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

struct DeviceInfo {
  String lampAddress;
  int lampIndex = -1;
  String mainAddress;
  int mainIndex = -1;
};

const int count = 1;
DeviceInfo deviceInfo[count];

int deviceCount = 0;
float tempC;

DeviceAddress* sensorAddresses;

String addressToString(DeviceAddress deviceAddress) {
  String addressString = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) addressString += "0";
    addressString += String(deviceAddress[i], HEX);
  }
  return addressString;
}

bool calculatingCondition(float lamp, float main) {
  return ((lamp - main) > 5.0) ? true : false;
}

void assignDevice() {
  Serial.print("Locating devices...");
  deviceCount = sensors.getDeviceCount();

  if (deviceCount < 2 * count) {
    Serial.println(" Some sensors are not operating.");
  } else {
    sensorAddresses = new DeviceAddress[deviceCount];
    for (int i = 0; i < deviceCount; i++) {
      sensors.getAddress(sensorAddresses[i], i);
    }
    for (int i = 0; i < count; i++) {
      for (int j = 0; j < deviceCount; j++) {
        String temp = addressToString(sensorAddresses[j]);
        if (deviceInfo[i].lampAddress == temp) {
          deviceInfo[i].lampIndex = j;
          break;
        }
      }
      for (int j = 0; j < deviceCount; j++) {
        String temp = addressToString(sensorAddresses[j]);

        if (deviceInfo[i].mainAddress == temp) {
          deviceInfo[i].mainIndex = j;
          break;
        }
      }
    }
  }
  Serial.println(" Done.");
}

void setup(void) {
  sensors.begin();
  Serial.begin(9600);

  deviceInfo[0].lampAddress = "28b41981e3b03c20";
  deviceInfo[0].mainAddress = "28a32581e3113c71";

  assignDevice();
  delay(1200);
}

void loop(void) {
  if (deviceCount >= 2 * count) {
    sensors.requestTemperatures();
    for (int i = 0; i < count; i++) {
      int lampIndex = deviceInfo[i].lampIndex;
      int mainIndex = deviceInfo[i].mainIndex;

      float tempL = sensors.getTempC(sensorAddresses[lampIndex]);
      float tempM = sensors.getTempC(sensorAddresses[mainIndex]);
      Serial.print("Lamp temperature of ");
      Serial.print(i + 1);
      Serial.print(" Lamp : ");
      Serial.print(tempL);
      Serial.println("°C");

      Serial.print("Main temperature of ");
      Serial.print(i + 1);
      Serial.print(" Main : ");
      Serial.print(tempM);
      Serial.println("°C");

      if (calculatingCondition(tempL, tempM)) {
        Serial.print("Devices are working fine in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      } else {
        Serial.print("Devices are not working in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      }
    }
  }

  delay(3000);
}
