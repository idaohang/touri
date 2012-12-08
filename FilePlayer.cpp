// Copyright 2012, Freiburg,
// Author: Markus Frey.

#include "./FilePlayer.h"

// _____________________________________________________________________________
FilePlayer::FilePlayer(SDCard* card, Mp3* player) : card(card), player(player),
    Debugable("FilePlayer") {
}

// _____________________________________________________________________________
bool FilePlayer::init() {
  rootDir = card->getRoot();
  return true;
}

// _____________________________________________________________________________
bool FilePlayer::play(char* path) {
  debugOutput("Open file ");
  debugStart(path);
  currentTrack.close();
  if (!currentTrack.open(rootDir, path, O_READ)) {
    debugFailed();
    debugOutputLn("Couldn't open file in read mode");
    return false;
  }
  debugDone();

  if (!currentTrack.read(currentBuffer, sizeof(currentBuffer))) {
    currentTrack.close();
    debugOutputLn("File was opened but couldn't read from it");
    return false;
  }

  return true;
}

// _____________________________________________________________________________
bool FilePlayer::tick() {
  if (!currentTrack.isOpen()) return false;

  // This will completely fill up the chips buffer.
  // One empty to full charging takes about 50ms
  // debugStartTimer();
  while (player->feedData(currentBuffer, sizeof(currentBuffer))) {
    // Transfer of data successful, load next data in buffer
    if (!currentTrack.read(currentBuffer, sizeof(currentBuffer))) {
      // End of Track reached
      debugOutputLn("Track finished");
      currentTrack.close();
      return false;
    }
  }
  // debugStopTimer("Fully loading chip's buffer");

  return true;
}
