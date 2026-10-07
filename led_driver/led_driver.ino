#if ( defined(CORE_TEENSY) )
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN       10

  #if defined(__IMXRT1062__)
    // For Teensy 4.1/4.0
    #if defined(ARDUINO_TEENSY41)
      #define BOARD_TYPE      "TEENSY 4.1"
      // Use true for NativeEthernet Library, false if using other Ethernet libraries
      #define USE_NATIVE_ETHERNET       true
      #define WEBSOCKETS_NETWORK_TYPE   NETWORK_NATIVEETHERNET
    #elif defined(ARDUINO_TEENSY40)
      #define BOARD_TYPE      "TEENSY 4.0"
    #else
      #define BOARD_TYPE      "TEENSY 4.x"
    #endif
  #elif defined(__MK66FX1M0__)
    #define BOARD_TYPE "Teensy 3.6"
  #elif defined(__MK64FX512__)
    #define BOARD_TYPE "Teensy 3.5"
  #elif defined(__MKL26Z64__)
    #define BOARD_TYPE "Teensy LC"
  #elif defined(__MK20DX256__)
    #define BOARD_TYPE "Teensy 3.2" // and Teensy 3.1 (obsolete)
  #elif defined(__MK20DX128__)
    #define BOARD_TYPE "Teensy 3.0"
  #elif defined(__AVR_AT90USB1286__)
    #error Teensy 2.0++ not supported yet
  #elif defined(__AVR_ATmega32U4__)
    #error Teensy 2.0 not supported yet
  #else
    // For Other Boards
    #define BOARD_TYPE      "Unknown Teensy Board"
  #endif
#else
  #error This code is intended to run only on the Teensy boards ! Please check your Tools->Board setting.
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#define USE_UIP_ETHERNET        false

// Only one if the following to be true
#define USE_ETHERNET_GENERIC    false
#define USE_ETHERNET_ESP8266    false
#define USE_ETHERNET_ENC        false

#if ( USE_ETHERNET_GENERIC )
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_W5100
#elif (USE_ETHERNET_ENC)
  #define WEBSOCKETS_NETWORK_TYPE   NETWORK_ETHERNET_ENC
#endif

#if ( USE_ETHERNET_GENERIC || USE_ETHERNET_ESP8266 || USE_ETHERNET_ENC || USE_NATIVE_ETHERNET )
  #ifdef USE_CUSTOM_ETHERNET
    #undef USE_CUSTOM_ETHERNET
  #endif
  #define USE_CUSTOM_ETHERNET   false
#endif

#if USE_NATIVE_ETHERNET
  #include "NativeEthernet.h"
  #warning Using NativeEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "Custom Ethernet using Teensy 4.1 NativeEthernet Library"
#elif USE_ETHERNET_GENERIC
  #include "Ethernet_Generic.h"
  #warning Using Ethernet_Generic lib

  #define ETHERNET_LARGE_BUFFERS

  #define _ETG_LOGLEVEL_        1

  #define SHIELD_TYPE           "W5x00 using Ethernet_Generic Library"
#elif USE_ETHERNET_ESP8266
  #include "Ethernet_ESP8266.h"
  #warning Using Ethernet_ESP8266 lib
  #define SHIELD_TYPE           "W5x00 using Ethernet_ESP8266 Library"
#elif USE_ETHERNET_ENC
  #include "EthernetENC.h"
  #warning Using EthernetENC lib
  #define SHIELD_TYPE           "ENC28J60 using EthernetENC Library"
#elif USE_CUSTOM_ETHERNET
  //#include "Ethernet_XYZ.h"
  #include "Ethernet.h"
  //#warning Using Custom Ethernet library. You must include a library and initialize.
  #define SHIELD_TYPE           "Custom Ethernet using Ethernet_XYZ Library"
#else
  #ifdef USE_ETHERNET_GENERIC
    #undef USE_ETHERNET_GENERIC
  #endif
  #define USE_ETHERNET_GENERIC   true
  #include "Ethernet_Generic.h"
  #warning Using Ethernet lib
  #define SHIELD_TYPE           "W5x00 using default Ethernet_Generic Library"
#endif

// Default pin 10 to SS/CS
#define USE_THIS_SS_PIN         10

#include <WebSocketsClient_Generic.h>
#include <SPI.h>
#include <SD.h>

#include "sdcard.h"

// ---------------- IO ----------------
const int SD_CS_PIN = BUILTIN_SDCARD;   // Teensy 4.1 built-in microSD
const int MAX_EVENTS = 100;
const char* LED0_EVENT_TIMESTEPS_FILENAME = "/led0_event_timesteps.txt";
const char* LED0_EVENT_SETTINGS_FILENAME = "/led0_event_settings.txt";
uint32_t led0_event_timesteps[MAX_EVENTS];
uint32_t led0_event_settings[MAX_EVENTS];
uint32_t led0_event_idx = 0;
uint32_t led0_event_count = 0;
const char* LED1_EVENT_TIMESTEPS_FILENAME = "/led1_event_timesteps.txt";
const char* LED1_EVENT_SETTINGS_FILENAME = "/led1_event_settings.txt";
uint32_t led1_event_timesteps[MAX_EVENTS];
uint32_t led1_event_settings[MAX_EVENTS];
uint32_t led1_event_idx = 0;
uint32_t led1_event_count = 0;
const char* LED2_EVENT_TIMESTEPS_FILENAME = "/led2_event_timesteps.txt";
const char* LED2_EVENT_SETTINGS_FILENAME = "/led2_event_settings.txt";
uint32_t led2_event_timesteps[MAX_EVENTS];
uint32_t led2_event_settings[MAX_EVENTS];
uint32_t led2_event_idx = 0;
uint32_t led2_event_count = 0;
const char* SHUTTER_EVENT_TIMESTEPS_FILENAME = "/shutter_event_timesteps.txt";
const char* SHUTTER_EVENT_SETTINGS_FILENAME = "/shutter_event_settings.txt";
uint32_t shutter_event_timesteps[MAX_EVENTS];
uint32_t shutter_event_settings[MAX_EVENTS];
uint32_t shutter_event_idx = 0;
uint32_t shutter_event_count = 0;
const char* event_filenames[] = {
  LED0_EVENT_TIMESTEPS_FILENAME,
  LED0_EVENT_SETTINGS_FILENAME,
  LED1_EVENT_TIMESTEPS_FILENAME,
  LED1_EVENT_SETTINGS_FILENAME,
  LED2_EVENT_TIMESTEPS_FILENAME,
  LED2_EVENT_SETTINGS_FILENAME,
  SHUTTER_EVENT_TIMESTEPS_FILENAME,
  SHUTTER_EVENT_SETTINGS_FILENAME
};
uint32_t* event_arrays[] = {
  led0_event_timesteps,
  led0_event_settings,
  led1_event_timesteps,
  led1_event_settings,
  led2_event_timesteps,
  led2_event_settings,
  shutter_event_timesteps,
  shutter_event_settings
};
uint32_t* event_counts[] = {
  &led0_event_count,
  &led0_event_count,
  &led1_event_count,
  &led1_event_count,
  &led2_event_count,
  &led2_event_count,
  &shutter_event_count,
  &shutter_event_count
};

const int led0_OUT = 2;
const int led1_OUT = 3;
const int led2_OUT = 4;
const int shutter_OUT = 5;

const int extra_ground0_OUT = 23;

bool  led0_on = false, previous_message_led0_on = false;
bool  led1_on = false, previous_message_led1_on = false;
bool  led2_on = false, previous_message_led2_on = false;
bool  shutter_open = true, previous_message_shutter_open = true; // HIGH means shutter is open, LOW means shutter is closed

bool session_started = false;
bool trial_ongoing = false;
unsigned long trial_start_time = 0;

// ---------------- Network ----------------
byte mac[] = {0x00,0xAA,0xBB,0xCC,0xDE,0x02};

// Set your WS server here
const char* WS_HOST = "10.16.101.209"; // fortyninety server
const uint16_t WS_PORT = 3141;
const char* WS_PATH = "/";

WebSocketsClient web_socket;
bool ws_connected = false;
bool ws_previously_connected = false;

void sendLEDStatusIfChanged() {
  if ((led0_on != previous_message_led0_on) ||
      (led1_on != previous_message_led1_on) ||
      (led2_on != previous_message_led2_on) ||
      (shutter_open != previous_message_shutter_open)) {
    char msg[5];
    msg[0] = 'i';
    msg[1] = shutter_open ? '3' : '2';
    msg[2] = led0_on ? '3' : '2';
    msg[3] = led1_on ? '3' : '2';
    msg[4] = led2_on ? '3' : '2';
    web_socket.sendTXT((uint8_t*)msg, 5);
    previous_message_led0_on = led0_on;
    previous_message_led1_on = led1_on;
    previous_message_led2_on = led2_on;
    previous_message_shutter_open = shutter_open;
  }
}

void applyCommand(const char* s) {
  if (!s || !*s) { Serial.println(F("Not recognized...")); return; }
  if (s[0] == 'a') { // START SESSION
    session_started = true;  trial_ongoing = false;
    digitalWrite(led0_OUT, LOW); led0_on = false;
    digitalWrite(led1_OUT, LOW); led1_on = false;
    digitalWrite(led2_OUT, LOW); led2_on = false;
    delay(50);
    digitalWrite(shutter_OUT, HIGH); shutter_open = true;
    previous_message_led0_on = led0_on;
    previous_message_led1_on = led1_on;
    previous_message_led2_on = led2_on;
    previous_message_shutter_open = shutter_open;

  } else if (s[0] == 'k') { // STOP SESSION
    session_started = false; trial_ongoing = false;
    digitalWrite(led0_OUT, LOW); led0_on = false;
    digitalWrite(led1_OUT, LOW); led1_on = false;
    digitalWrite(led2_OUT, LOW); led2_on = false;
    delay(50);
    digitalWrite(shutter_OUT, HIGH); shutter_open = true;
    previous_message_led0_on = led0_on;
    previous_message_led1_on = led1_on;
    previous_message_led2_on = led2_on;
    previous_message_shutter_open = shutter_open;

  } else if (s[0] == 'q' && session_started) { // START TRIAL (only if session started)
    led0_event_idx = 0;
    led1_event_idx = 0;
    led2_event_idx = 0;
    shutter_event_idx = 0;
    trial_start_time = millis();
    trial_ongoing = true;
  } else {
    // Serial.println("Other command...");
  }
}

void wsEvent(WStype_t type, uint8_t* payload, size_t length) {
  char buf[32];
  switch (type) {
    case WStype_CONNECTED:
      ws_connected = true;
      Serial.println("WS connected");
      break;

    case WStype_DISCONNECTED:
      ws_connected = false;
      Serial.println("WS disconnected");
      session_started = false;
      trial_ongoing = false;
      digitalWrite(led0_OUT, LOW); led0_on = false;
      digitalWrite(led1_OUT, LOW); led1_on = false;
      digitalWrite(led2_OUT, LOW); led2_on = false;
      delay(50);
      digitalWrite(shutter_OUT, HIGH); shutter_open = true;
      break;

    case WStype_TEXT:
      memcpy(buf, payload, min(length, sizeof(buf) - 1));
      buf[min(length, sizeof(buf) - 1)] = '\0';
      applyCommand(buf);
      break;

    default: break;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led0_OUT, OUTPUT);
  pinMode(led1_OUT, OUTPUT);
  pinMode(led2_OUT, OUTPUT);
  pinMode(shutter_OUT, OUTPUT);
  pinMode(extra_ground0_OUT, OUTPUT);

  Serial.begin(9600);
  delay(2000);

  // SD Card setup
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD initialization failed.");
    return;
  }
  Serial.println("SD initialization OK.");

  // read event arrays from SD
  readMultipleIntArraysFromSD(
    event_filenames,
    event_arrays,
    event_counts,
    MAX_EVENTS,
    8
  );
  // for (int i = 0; i < led0_event_count; i++) {
  //   Serial.print("led0_event_timesteps[");
  //   Serial.print(i);
  //   Serial.print("] = ");
  //   Serial.println(led0_event_timesteps[i]);
  // }

  // Ethernet setup
  Serial.println("Initialize Ethernet with DHCP...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP failed");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) Serial.println("No Ethernet hardware");
    else if (Ethernet.linkStatus() == LinkOFF) Serial.println("Ethernet link off");
    while (1) { delay(1000); }
  }
  Serial.print("Local IP: "); Serial.println(Ethernet.localIP());

  // WebSocket client setup
  web_socket.begin(WS_HOST, WS_PORT, WS_PATH);   // ws://HOST:PORT/PATH
  web_socket.onEvent(wsEvent);
  web_socket.setReconnectInterval(2000);         // auto-retry

  digitalWrite(shutter_OUT, HIGH);
  digitalWrite(extra_ground0_OUT, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  Ethernet.maintain();
  web_socket.loop();     // drives callbacks and reconnects

  // ===== led timing state machine =====
  if (session_started && trial_ongoing) {
    unsigned long elapsed = millis() - trial_start_time;

    // led0
    while (led0_event_idx < led0_event_count && elapsed >= led0_event_timesteps[led0_event_idx]) {
      if (led0_event_settings[led0_event_idx] == 1) { // ON
        digitalWrite(led0_OUT, HIGH); led0_on = true;
      } else { // OFF
        digitalWrite(led0_OUT, LOW); led0_on = false;
      }
      led0_event_idx++;
    }

    // led1
    while (led1_event_idx < led1_event_count && elapsed >= led1_event_timesteps[led1_event_idx]) {
      if (led1_event_settings[led1_event_idx] == 1) { // ON
        digitalWrite(led1_OUT, HIGH); led1_on = true;
      } else { // OFF
        digitalWrite(led1_OUT, LOW); led1_on = false;
      }
      led1_event_idx++;
    }

    // led2
    while (led2_event_idx < led2_event_count && elapsed >= led2_event_timesteps[led2_event_idx]) {
      if (led2_event_settings[led2_event_idx] == 1) { // ON
        digitalWrite(led2_OUT, HIGH); led2_on = true;
      } else { // OFF
        digitalWrite(led2_OUT, LOW); led2_on = false;
      }
      led2_event_idx++;
    }

    // shutter LOW = closed and HIGH = open
    while (shutter_event_idx < shutter_event_count && elapsed >= shutter_event_timesteps[shutter_event_idx]) {
      if (shutter_event_settings[shutter_event_idx] == 1) { // CLOSE
        digitalWrite(shutter_OUT, LOW); shutter_open = false;
      } else { // OPEN
        digitalWrite(shutter_OUT, HIGH); shutter_open = true;
      }
      shutter_event_idx++;
    }

  } else {
    // not in-trial: all off
    if (led0_on) { digitalWrite(led0_OUT, LOW); led0_on = false; }
    if (led1_on) { digitalWrite(led1_OUT, LOW); led1_on = false; }
    if (led2_on) { digitalWrite(led2_OUT, LOW); led2_on = false; }
    if (!shutter_open) {
      delay(50);
      digitalWrite(shutter_OUT, HIGH); shutter_open = true;
    }
  }

  if (!ws_previously_connected && ws_connected) {
    char msg[1];
    msg[0] = 'e';
    web_socket.sendTXT((uint8_t*)msg, 1);
    ws_previously_connected = true;
  } else if (ws_previously_connected && !ws_connected) {
    Serial.println("WS disconnected");
    ws_previously_connected = false;
  }

  // send status on change
  if (ws_connected) sendLEDStatusIfChanged();

  delay(1);
}


