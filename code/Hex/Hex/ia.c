#include "ia.h"
#include <stdlib.h>

void ia_play(Game *g) {
	int coup = rand() % (g->nbCases * g->nbCases);
	while (g->plateau[coup] != 0)
		coup = rand() % (g->nbCases * g->nbCases);
	int x = get_column(g, coup);
	int y = get_line(g, coup);
	SDL_Rect rect = { x * g->xPawn + (y * g->xPawn / 2) + g->sizeLeft, y * g->yPawn + g->sizeTop, g->sizePrintPawn, g->sizePrintPawn };
	play(g, x, y, rect);
}