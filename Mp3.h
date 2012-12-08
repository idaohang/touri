// Copyright 2012, Freiburg,
// Author: Markus Frey.

#ifndef MP3_H_
#define MP3_H_

//VS10xx SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

#include "./Arduino.h"
#include "./Debugable.h"
#include <SdFat.h>

// Class to play MP3 with the VS1053, SPI needs to be initialized externally
class Mp3 : private Debugable {
 public:
  Mp3(byte XCS, byte XDCS, byte DREQ, byte RESET);
  bool init();
  bool feedData(byte* data, byte size);
  void setVolume(byte volume);

 private:
  byte XCS;
  byte XDCS;
  byte DREQ;
  byte RESET;

  unsigned int readRegister(byte addressByte);
  void writeRegister(byte addressByte, byte highByte, byte lowByte);
};

#endif  // MP3_H_
