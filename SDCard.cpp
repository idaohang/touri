// Copyright 2012, Freiburg,
// Author: Markus Frey.

#include "./SDCard.h"
#include <SdFat.h>

// _____________________________________________________________________________
SDCard::SDCard(byte CSPin) : CSPin(CSPin), Debugable("SDCard") {
}

// _____________________________________________________________________________
bool SDCard::init() {

  debugStart("Initialize card");
  if (!card.init(SPI_SIXTEENTH_SPEED, CSPin)) {
    debugFailed();
    return false;
  }
  debugDone();

  debugOutput("Card size: ");
  debugOutput(card.cardSize()*0.000512);
  debugOutputLn("MB");

  debugStart("Open volume");
  if (!volume.init(&card)) {
    debugFailed();
    if (card.errorCode()) debugOutputLn("Can't read the card.");
    else debugOutputLn("Can't find a valid FAT16/FAT32 partition.");
    return false;
  }
  debugDone();

  debugOutput("Volume type: FAT");
  debugOutputLn(volume.fatType());

  debugStart("Open root");
  if (!rootDir.openRoot(&volume)) {
    debugFailed();
    return false;
  }
  debugDone();

  // Output files in root directory
  debugOutputLn("Files in root directory:");
  SdBaseFile f;
  char fname[13];
  while (f.openNext(&rootDir, O_READ)) {
    if (!f.getFilename(fname)) {
      debugOutputLn("Couldn't get filename!");
      continue;
    }
    debugOutputLn(fname);
    f.close();
  }

  debugOutput("Card size: ");
  debugOutputLn(sizeof(card));
  debugOutput("Volume size: ");
  debugOutputLn(sizeof(volume));
  debugOutput("RootDir size: ");
  debugOutputLn(sizeof(rootDir));

  return true;
}

// _____________________________________________________________________________
SdBaseFile* SDCard::getRoot() {
  return &rootDir;
}
