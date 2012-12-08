// Copyright 2012, Freiburg,
// Author: Markus Frey.

#include "./Mp3.h"
#include <SPI.h>

// _____________________________________________________________________________
Mp3::Mp3(byte XCS, byte XDCS, byte DREQ, byte RESET) : XCS(XCS), XDCS(XDCS),
    DREQ(DREQ), RESET(RESET), Debugable("MP3") {
}

// _____________________________________________________________________________
bool Mp3::init() {
  // Set the pins
  pinMode(DREQ, INPUT);
  pinMode(XCS, OUTPUT);
  pinMode(XDCS, OUTPUT);
  pinMode(RESET, OUTPUT);

  digitalWrite(XCS, HIGH);  // Deselect Control
  digitalWrite(XDCS, HIGH);  // Deselect Data
  digitalWrite(RESET, LOW);  // Put VS1053 into hardware reset

  debugStart("Starting VS1053");
  // Set SPI to 1Mhz to start off
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.transfer(0xFF);  // Dummy byte
  delay(10);
  digitalWrite(RESET, HIGH);  // Bring up VS1053
  setVolume(40);
  debugDone();

  // Output some info
  debugOutput("SCI_MODE (should be 0x4800) = 0x");
  debugOutputLn(readRegister(SCI_MODE),  HEX);

  int MP3Status = readRegister(SCI_STATUS);
  debugOutput("SCI_STATUS (should be 0x48) = 0x");
  debugOutputLn(MP3Status, HEX);
  debugOutput("VS Version (VS1053 is 4) = ");
  debugOutputLn((MP3Status >> 4) & 0x000F, DEC);  // Mask the four vesrion bits

  debugStart("Increasing internal clock and SPI speed");
  // Increase the internal clock multiplier and the SPI rate
  writeRegister(SCI_CLOCKF, 0x60, 0x00);  // Set multiplier to 3.0x

  // Max SCI reads are CLKI/7. Input clock is 12.288MHz.
  // Internal clock multiplier is was just raised to 3x.
  // Therefore, max SPI speed is 5MHz. 4MHz will be safe.
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  debugDone();

  // Ouput clock speed
  debugOutput("SCI_CLOCKF = 0x");
  debugOutputLn(readRegister(SCI_CLOCKF), HEX);

  return true;
}

// _____________________________________________________________________________
bool Mp3::feedData(byte* data, byte size) {
  // DREQ is high when chips receive buffer is ready/empty
  if (digitalRead(DREQ)) {
    digitalWrite(XDCS, LOW);  // Tell the chip we want to transfer data

    // Send all bytes of data from the given buffer to the chip
    for(byte i = 0 ; i < size; i++) SPI.transfer(data[i]);

    digitalWrite(XDCS, HIGH);
    return true;
  }
  return false;  // Chip is not ready yet
}

// _____________________________________________________________________________
unsigned int Mp3::readRegister(byte addressByte) {
  while (!digitalRead(DREQ)) ;  // Wait for chip to be ready
  digitalWrite(XCS, LOW);  // Select control (we want to give instructions)

  SPI.transfer(0x03);  // Tell the chip we want to read
  SPI.transfer(addressByte);  // Read from this address

  // Read data. What we transfer to the chip doesnt matter.
  char responseH = SPI.transfer(0xFF);
  while (!digitalRead(DREQ));  // Wait for the chip to be ready again
  char responseL = SPI.transfer(0xFF);
  while (!digitalRead(DREQ));

  digitalWrite(XCS, HIGH);  // Deselect Control

  int resultvalue = (responseH << 8) | responseL;
  return resultvalue;
}

// _____________________________________________________________________________
void Mp3::writeRegister(byte addressByte, byte highByte, byte lowByte) {
  while (!digitalRead(DREQ));  // Wait for chip to be ready
  digitalWrite(XCS, LOW);  // Select control (We want to give instructions)

  SPI.transfer(0x02);  // Tell the chip we want to write to register
  SPI.transfer(addressByte);  // Where we want to write to
  SPI.transfer(highByte);  // What we want to write
  SPI.transfer(lowByte);

  while (!digitalRead(DREQ));  // When DREQ goes high write is complete

  digitalWrite(XCS, HIGH);  // Deselect Control
}

// _____________________________________________________________________________
void Mp3::setVolume(byte volume) {
  writeRegister(SCI_VOL, volume, volume);  // Left channel, right channel
}
