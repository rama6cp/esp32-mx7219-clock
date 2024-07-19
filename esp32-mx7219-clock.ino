#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Network credentials
const char* ssid     = "*******";
const char* password = "******";

// NTP Client
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 12 * 3600; // Jakarta is UTC+7
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000);

// Define the number of devices we have in the chain and the hardware interface
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 23
#define CS_PIN 5
#define CLK_PIN 18

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  Serial.begin(115200);
  
  // Initialize the display
  myDisplay.begin();
  myDisplay.setIntensity(0); // Set brightness level (0 is min, 15 is max)
  myDisplay.displayClear();
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  
  // Start the NTP client
  timeClient.begin();
}

void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  // Format time as HH:MM
  char timeString[6];
  sprintf(timeString, "%02d:%02d", (ptm->tm_hour + 7) % 24, ptm->tm_min); // Jakarta timezone adjustment
  
  // Display the time on the LED matrix
  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(timeString);
  
  delay(1000);
}
