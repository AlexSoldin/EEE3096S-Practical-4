/*
 * Prac4.cpp
 *
 * Originall written by Stefan Schröder and Dillion Heald
 *
 * Adapted for EEE3096S 2019 by Keegan Crankshaw
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

#include "Prac4.h"

using namespace std;

bool playing = true; // should be set false when paused
bool stopped = false; // If set to true, program should close
unsigned char buffer[2][BUFFER_SIZE][2];
int bufferLocation = 0;
bool bufferReading = 0; //using this to switch between column 0 and 1 - the first column
bool threadReady = false; //using this to finish writing the first column at the start of the song, before the column is played


// Configure your interrupts here.
// Don't forget to use debouncing.
void play_pause_isr(void){
    printf("Pause button pressed\n");
    playing = !playing;
    if(!playing){
        delay(1);
        wiringPiSPIDataRW(SPI_CHAN, (unsigned char*)PAUSE_STATE, sizeof(PAUSE_STATE));
    }
}

void stop_isr(void){
    stopped = true;
    printf("Stop button pressed\n");
    cleanup(0);
}

/*
 * Setup Function. Called once
 */
 int setup = -1;
 int setup_gpio(void){
     if(setup==-1)
         return 0;

     printf("Setting Up GPIO pins\n");

     wiringPiSetup();

    for (unsigned int i = 0 ; i < (sizeof(BTNS) / sizeof(BTNS[0])) ; i++) {     // Set up Buttons
 		   pinMode(BTNS[i], INPUT);
 		   pullUpDnControl(BTNS[i], PUD_UP);                                     // PDR for Op-Amp to push
 	   }

     wiringPiISR(PAUSE_BUTTON, INT_EDGE_FALLING, play_pause_isr);
 	   wiringPiISR(STOP_BUTTON, INT_EDGE_FALLING, stop_isr);
                                                                                 // Set up the SPI interface
     if(wiringPiSPISetup(SPI_CHAN, SPICLOCKSPEED) != -1){                            // Defaults to Mode 0
         printf("SPI Connected.\n");
     }
     else{
         printf("SPI Connection Failed.\n");
         return -1;
     }

     setup = 1;
     printf("Finished GPIO Setup\n");
     return 0;
 }

/*
 * Thread that handles writing to SPI
 *
 * You must pause writing to SPI if not playing is true (the player is paused)
 * When calling the function to write to SPI, take note of the last argument.
 * You don't need to use the returned value from the wiring pi SPI function
 * You need to use the bufferLocation variable to check when you need to switch buffers
 */
 PI_THREAD (play_audio){
     while(!threadReady)
         delay(1);

     digitalWrite(LDAC, LOW);
     while(!stopped){
         while(!playing)
             delay(1);

         wiringPiSPIDataRW(SPI_CHAN, buffer[bufferReading][bufferLocation], 2);

         bufferLocation++;
         piLock(0);
         if(bufferLocation >= BUFFER_SIZE) {
             bufferLocation = 0;
             bufferReading = (bufferReading+1) % 2;                              // Switches column once it finishes current column
             //wiringPiSPISetup(SPI_CHAN, SPI_SPEED);                               // Reset the speed
         }
         piUnlock(0);
     }
     printf("Exiting Thread.\n");
     return(0);
 }

int main(){
   signal(SIGINT, cleanup);

   digitalWrite(LDAC, HIGH);
   // Call the setup GPIO function
	 if(setup_gpio()==-1){
         return 0;
     }

    piThreadCreate(play_audio);

    // Open the file
    char ch;
    FILE *filePointer;
    printf("%s\n", FILENAME);
    filePointer = fopen(FILENAME, "r"); // read mode

    if (filePointer == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    int counter = 0;
    int bufferWriting = 0;

    // Have a loop to read from the file
	 while((ch = fgetc(filePointer)) != EOF){
        while(threadReady && bufferWriting==bufferReading && counter==0){
            //waits in here after it has written to a side, and the thread is still reading from the other side
            delay(1);
        }

        buffer[bufferWriting][counter][0] = ch>>6;
        buffer[bufferWriting][counter][0] |=playConfig;

        buffer[bufferWriting][counter][1] = ch<<2;

        counter++;
        piLock(0);
        if(counter >= BUFFER_SIZE+1){
            if(!threadReady){
                threadReady = true;
            }

            counter = 0;
            bufferWriting = (bufferWriting+1)%2;
        }
        piUnlock(0);
    }

    // Close the file
    fclose(filePointer);
    printf("Complete reading");
    cleanup(0);
}

/*
 * Clean up function
 */
void cleanup(int args){
	printf("Cleaned Up Successful.\n");
	exit(0);
}
