#ifndef IA_H
#define IA_H

#include "game.h"

struct IA {
	Player* p;
	Player* other;
};
typedef struct IA IA;

int ia_play(Game *g, IA *ia);

#endif