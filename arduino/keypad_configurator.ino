
#include <Keyboard.h>
#include <EEPROM.h>

const int hallPin1 = A0;
const int hallPin2 = A1;

struct KeyConfig {
  char action[16];
  uint16_t actuationPoint;
  bool rapidTrigger;
};

KeyConfig key1, key2;

bool wasPressed1 = false;
bool wasPressed2 = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  loadFromEEPROM();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("CFG;")) {
      parseAndSaveConfig(input);
    }
  }

  handleKey(hallPin1, key1, wasPressed1);
  handleKey(hallPin2, key2, wasPressed2);
}

void handleKey(int pin, KeyConfig &key, bool &wasPressed) {
  int val = analogRead(pin);
  if (key.rapidTrigger) {
    static bool ready = true;
    if (val > key.actuationPoint && ready) {
      sendAction(key.action);
      ready = false;
    } else if (val < key.actuationPoint - 50) {
      ready = true;
    }
  } else {
    if (val > key.actuationPoint && !wasPressed) {
      sendAction(key.action);
      wasPressed = true;
    } else if (val < key.actuationPoint - 50) {
      wasPressed = false;
    }
  }
}

void sendAction(const char* action) {
  String act = String(action);
  if (act == "Ctrl+C") {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    delay(100);
    Keyboard.releaseAll();
  } else if (act == "Ctrl+V") {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    delay(100);
    Keyboard.releaseAll();
  }
}

void saveToEEPROM() {
  EEPROM.put(0, key1);
  EEPROM.put(sizeof(KeyConfig), key2);
}

void loadFromEEPROM() {
  EEPROM.get(0, key1);
  EEPROM.get(sizeof(KeyConfig), key2);
}

void parseAndSaveConfig(String cfg) {
  key1 = parseKeyConfig(cfg, "T1=");
  key2 = parseKeyConfig(cfg, "T2=");
  saveToEEPROM();
}

KeyConfig parseKeyConfig(String cfg, String prefix) {
  KeyConfig kc;
  int start = cfg.indexOf(prefix);
  if (start == -1) return kc;
  int end = cfg.indexOf(";", start + 1);
  if (end == -1) end = cfg.length();

  String section = cfg.substring(start + prefix.length(), end);
  String act = extractField(section, "", ",");
  String actPt = extractField(section, "ACT=", ",");
  String rt = extractField(section, "RT=", "");

  act.toCharArray(kc.action, sizeof(kc.action));
  kc.actuationPoint = actPt.toInt();
  kc.rapidTrigger = rt.toInt();

  return kc;
}

String extractField(String data, String key, String delimiter) {
  int idx = data.indexOf(key);
  if (idx == -1) return "";
  idx += key.length();
  int endIdx = delimiter == "" ? data.length() : data.indexOf(delimiter, idx);
  return data.substring(idx, endIdx);
}
