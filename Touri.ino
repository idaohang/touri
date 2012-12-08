// Copyright 2012, Freiburg,
// Author: Markus Frey.

// SD Pins
#define SD_CS 4

// VS1053 Pins
#define MP3_XCS 6
#define MP3_XDCS 7
#define MP3_DREQ 2
#define MP3_RESET 8

#define address 0x1E

#include "./SDCard.h"
#include "./Mp3.h"
#include "./FilePlayer.h"
#include <Wire.h>
#include <TinyGPS.h>

SDCard card(SD_CS);
Mp3 mp3chip(MP3_XCS, MP3_XDCS, MP3_DREQ, MP3_RESET);
FilePlayer player(&card, &mp3chip);
TinyGPS gps;

bool gpsIsActive = false;
long lat, lng;

void setup() {
  Serial.begin(38400);
  // Wire.begin();

  // Wire.beginTransmission(address);
  // Wire.write(0x02);
  // Wire.write(0x00);
  // Wire.endTransmission();

  if (!card.init()) while (1);
  if (!mp3chip.init()) while (1);
  player.init();
  player.play("ok.mp3");
}

void printCompass() {
  int x,y,z;
  Wire.beginTransmission(address);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(address, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8;
    x |= Wire.read();
    z = Wire.read() << 8;
    z |= Wire.read();
    y = Wire.read() << 8;
    y |= Wire.read();
  }

  float heading = atan2(y, x);
  if (heading < 0) heading += 2*PI;
  heading = heading * 180/M_PI;

  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.print(z);

  Serial.print(" Heading: ");
  Serial.println(heading);
}

void loop() {
  player.tick();
  while (Serial.available()) {
    gps.encode(Serial.read());
  }
  if ((gps.satellites() != TinyGPS::GPS_INVALID_SATELLITES) != gpsIsActive) {
    gpsIsActive ^= true;
    Serial.println((unsigned short) gps.satellites());
    if (gpsIsActive) player.play("bell.mp3");
    else player.play("error.mp3");
  }
  if (gpsIsActive) {
    gps.get_position(&lat, &lng);
    Serial.print("GPS lat: ");
    Serial.print(lat);
    Serial.print(" lng: ");
    Serial.println(lng);
  }
}
