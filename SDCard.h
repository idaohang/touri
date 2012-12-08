// Copyright 2012, Freiburg,
// Author: Markus Frey.

#ifndef SDCARD_H_
#define SDCARD_H_

#include <Arduino.h>
#include <SdFat.h>
#include "./Debugable.h"

// Wrapper class for SdFat
class SDCard : private Debugable {
 public:
  SDCard(byte CSPin);
  bool init();
  SdBaseFile* getRoot();
 private:
  Sd2Card card;
  SdVolume volume;
  SdBaseFile rootDir;
  byte CSPin;
};

#endif  // SDCARD_H_
