#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

struct DeviceInfo {
  String lampAddress;
  int lampIndex = -1;
  String mainAddress;
  int mainIndex = -1;
  bool flag = true;
};

const int count = 1;
DeviceInfo deviceInfo[count];
const char *ssid = "";
const char *password = "";
String endPoint = "https://device-fault-detection-jxo2.onrender.com";

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

void wifiConnect() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void smtpCall(String address) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    String serverAddress = endPoint + "/sendalert/?" + "address=" + address;
    http.begin(client, serverAddress);
    http.addHeader("Content-Type", "application/json");
    String payload = "{}";  
    int httpCode = http.POST(payload);

    if (httpCode > 0) {
      Serial.print("HTTP response code: ");
      Serial.println(httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        Serial.println("Server response:");
        Serial.println(response);
      } else {
        Serial.println("HTTP request failed");
      }
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

void setup(void) {
  sensors.begin();
  Serial.begin(9600);
  while (!Serial);
  wifiConnect();
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
      bool flag = calculatingCondition(tempL, tempM);
      if ((flag == true) && (deviceInfo[i].flag == true)) {
        Serial.print("Devices are working fine in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      } else if ((flag == true) && (deviceInfo[i].flag == false)) {
        deviceInfo[i].flag = true;
        Serial.print("Devices are now working fine in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      } else if ((flag == false) && (deviceInfo[i].flag == true)) {
        smtpCall(String(i));
        deviceInfo[i].flag = false;
        Serial.print("Devices not working in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      } else {
        Serial.print("Devices not working in address : ");
        Serial.println(deviceInfo[i].lampAddress);
      }
    }
  }

  delay(3000);
}
