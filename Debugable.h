// Copyright 2012, Freiburg,
// Author: Markus Frey.

#ifndef DEBUGABLE_H_
#define DEBUGABLE_H_

#include <Arduino.h>

// Class to print debug information to Serial out
// Serial needs to be initialized by the main program
class Debugable {
 public:
  Debugable(const char* name);
  void debugStart(const char* name);
  void debugFailed();
  void debugDone();
  void debugStartTimer();
  void debugStopTimer(const char* name);
  template<class T> void debugOutput(T msg) {
    debugPre();
    Serial.print(msg);
  }
  template<class T> void debugOutputLn(T msg) {
    debugPre();
    Serial.println(msg);
    openLine = false;
  }
  template<class T> void debugOutput(T msg, int type) {
    debugPre();
    Serial.print(msg, type);
  }
  template<class T> void debugOutputLn(T msg, int type) {
    debugPre();
    Serial.println(msg, type);
    openLine = false;
  }
 private:
  void debugPre();
  const char* name;
  bool openLine;
  long timerStart;
};

#endif  // DEBUGABLE_H_
