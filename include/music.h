#ifndef MUSICA_H
#define MUSICA_H

#include <stdlib.h>
#include <stdio.h>


void tocar_musica() {

        system("cvlc --play-and-exit --intf dummy --file-caching=2000 --no-audio-time-s>");

}





#endif

