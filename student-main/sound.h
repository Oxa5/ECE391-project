#ifndef _SOUND_H_
#define _SOUND_H_

#include "lib.h"
#define PIT_FRQ 1193180
#define PIT_CH2 0x42
#define PIT_MODEREG 0x43
#define PIT_SOUND 0xB6
#define DO	262
#define RE	277
#define MI	294
#define FA	311
#define SO	330
#define LA	349
#define XI	370
#define DO2	392
#define RE2	415
#define MI2	440

#define PIT_INPUT 0x61





// typedef struct sound_packet{
//     uint16_t type; // 0: play music at a certain frequency 1: mute no sound
//     uint16_t frequency;  //if type is 0, this is frequency; if type is 1, this is useless
// }sound_packet_t;

void sound_init();
void play_sound ( uint32_t inputfreq );
void nosound ();


#endif
