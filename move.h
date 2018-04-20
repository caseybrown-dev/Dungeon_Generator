#ifndef MOVE_H
# define MOVE_H

# include "dungeon.h"

spot_t dijkstra(dungeon_t *d, pair_t from, pair_t to);

#endif