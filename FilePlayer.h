// Copyright 2012, Freiburg,
// Author: Markus Frey.

#ifndef FILEPLAYER_H_
#define FILEPLAYER_H_

#include <Arduino.h>
#include "./Debugable.h"
#include "./SDCard.h"
#include "./Mp3.h"

// Mp3 Player class that reads files off of an SDCard and plays them through
// a Mp3 Chip
class FilePlayer : private Debugable {
 public:
  FilePlayer(SDCard* card, Mp3* player);
  bool init();
  bool play(char* path);
  bool tick();
 private:
  SDCard* card;
  Mp3* player;
  SdBaseFile* rootDir;

  SdFile currentTrack;
  byte currentBuffer[32];
};

#endif  // FILEPLAYER_H_
