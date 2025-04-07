#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define LDR_INPUT A0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress* sensorAddresses;

struct DeviceInfo {
  String lampAddress;
  int lampIndex = -1;
  String mainAddress;
  int mainIndex = -1;
};

const int count = 1;
int deviceCount = 0;
float tempC;
DeviceInfo deviceInfo[count];

String addressToString(DeviceAddress deviceAddress) {
  String addressString = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) addressString += "0";
    addressString += String(deviceAddress[i], HEX);
  }
  return addressString;
}

bool calculatingCondition(float lamp, float main) {
  return ((lamp - main) > 5.0);
}

void assignDevice() {
  deviceCount = sensors.getDeviceCount();
  if (deviceCount < 2 * count) {
    Serial.println("Some sensors are not operating.");
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
}

void setup() {
  sensors.begin();
  Serial.begin(9600);
  while (!Serial);
  deviceInfo[0].lampAddress = "28b41981e3b03c20";
  deviceInfo[0].mainAddress = "28a32581e3113c71";
  assignDevice();
  pinMode(LDR_INPUT, INPUT);
  delay(1200);
  Serial.println("MainTemp,LampTemp,Intensity,DefectFlag"); 
}

void loop() {
  if (deviceCount >= 2 * count) {
    sensors.requestTemperatures();
    for (int i = 0; i < count; i++) {
      int lampIndex = deviceInfo[i].lampIndex;
      int mainIndex = deviceInfo[i].mainIndex;
      float tempL = sensors.getTempC(sensorAddresses[lampIndex]);
      float tempM = sensors.getTempC(sensorAddresses[mainIndex]);
      int intensity = analogRead(LDR_INPUT);
      float lightIntensity=100.0 - (intensity/1023.0)*100.0;
      bool flag = calculatingCondition(tempL, tempM);
      Serial.print(tempM);
      Serial.print(",");
      Serial.print(tempL);
      Serial.print(",");
      Serial.print(lightIntensity);
      Serial.print(",");
      Serial.println(flag);
    }
  }
  delay(3000);
}
