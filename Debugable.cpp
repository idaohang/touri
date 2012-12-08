// Copyright 2012, Freiburg,
// Author: Markus Frey.

#include "./Debugable.h"

// _____________________________________________________________________________
Debugable::Debugable(const char* name) : name(name) {
  openLine = false;
}

// _____________________________________________________________________________
void Debugable::debugPre() {
  if (!openLine) {
    Serial.print("[");
    Serial.print(this->name);
    Serial.print("]: ");
    openLine = true;
  }
}

// _____________________________________________________________________________
void Debugable::debugStart(const char* name) {
  debugPre();
  Serial.print(name);
  Serial.print(" ... ");
}

// _____________________________________________________________________________
void Debugable::debugFailed() {
  Serial.println("[FAILED]");
  openLine = false;
}

// _____________________________________________________________________________
void Debugable::debugDone() {
  Serial.println("[DONE]");
  openLine = false;
}

// _____________________________________________________________________________
void Debugable::debugStartTimer() {
  timerStart = millis();
}

// _____________________________________________________________________________
void Debugable::debugStopTimer(const char* name) {
  debugOutput(name);
  debugOutput(" took ");
  debugOutput(millis() - timerStart);
  debugOutputLn("ms");
}
