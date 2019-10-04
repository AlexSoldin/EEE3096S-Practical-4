/*
 * Prac4.cpp
 *
 * Written for EEE3096S 2019 by Keegan Crankshaw
 *
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PRAC4_H
#define PRAC4_H

//Includes
#include <wiringPi.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>
#include <signal.h>

//Define buttons
const int PAUSE_BUTTON = 23; //wiringPi 23  AND BCM 13
const int STOP_BUTTON = 21;//wiringPi 21 AND BCM 5
const int BTNS[] = {23, 21};

//SPI: wiringPi 10 (BCM 8), wiringPi 14 (BCM 11), wiringPi 12 (BCM 10) in use.

//SPI Settings
#define SPI_CHAN 0
#define SPI_MODE 0
#define CHANNELS 2
#define WIDTH 16

//Filename
#define FILENAME "src/sound_16k_8bit.raw"

#define SAMPLE_RATE 10000
const int BUFFER_SIZE = 100000;
const int SCALING = 8/5;

const int SPICLOCKSPEED = SAMPLE_RATE*WIDTH*CHANNELS*SCALING;

#define BUFFER 0x40
#define GAIN 0x20
#define SHUTDOWN 0x10
#define PAUSE_STATE SHUTDOWN

//Function definitions
void play_audio(void);
void stop_audio(void);
int setup_gpio(void);
int main(void);
void cleanup(int args);


#endif
