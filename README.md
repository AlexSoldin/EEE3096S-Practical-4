# EEE3096S-Practical-4

This practical deals with improving the audio capabilities and quality, on a Raspberry Pi 3B+, by using SPI, Threading and a DAC (digital to analogue converter). In order to fully grasp the scope of this assignment, it is necessary to understand the basics behind digital audio.

The MCP4911 DAC, is a 10-bit DAC thereby allowing the voltage outputs to be split into 1024 distinct levels. This practical uses 8-bit 16kHz audio. This is quite low quality due to the quality of the DAC.

The Pi uses a Linux based operating system and is therefore unable to perform these time constraint operations at the same level of eﬃciency as alternative solutions. In order to ensure that samples are ready to play as soon as requested, a similar technique to circular buﬀering will be used. A single array, split into two parts, will be used. While one half of this array loads, the other half will play. A ﬁnal buﬀer size of 1000 will be suﬃcient to complete this practical.

The circuit schematic is attached showing the hardware setup.
