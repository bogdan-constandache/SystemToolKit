#ifndef SMARTDEFINES_H
#define SMARTDEFINES_H

//#define _WIN32_WINNT 0x500
#define DRIVE_HEAD_REG	0xA0
#define OUT_BUFFER_SIZE IDENTIFY_BUFFER_SIZE+16

#define INDEX_ATTRIB_INDEX									0
#define INDEX_ATTRIB_UNKNOWN1								1
#define INDEX_ATTRIB_UNKNOWN2								2
#define INDEX_ATTRIB_VALUE									3
#define INDEX_ATTRIB_WORST									4
#define INDEX_ATTRIB_RAW									5


// identify field defines
#define START_SERIAL    10
#define LENGTH_SERIAL   10 // words
#define BUFFER_TYPE     20
#define BUFFER_SIZE     21
#define START_FIRMWARE  23
#define LENGTH_FIRMWARE 4
#define START_MODEL 27
#define LENGTH_MODEL    20
#define CAPABILITIES_0  49
#define CAPABILITIES_1  50
#define FORM_FACTOR     168
#define NMRR            217 // nominal media rotation rate
#define START_MANUFACTURER 196
#define LENGTH_MANUFACTURER 10

#endif
