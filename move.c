#include <limits.h>
#include <unistd.h>

#include "heap.h"
#include "move.h"
#include "dungeon.h"

typedef struct path {
    heap_node_t *hn;
    uint8_t pos[2];
    uint8_t from[2];
    /* Because paths can meander about the dungeon, they can be *
     * significantly longer than DUNGEON_X.                     */
    uint16_t cost;
} path_t;

static int32_t dist_cmp(const void *key, const void *with) {
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

spot_t dijkstra(dungeon_t *d, pair_t from, pair_t to)
{
    /* Currently assumes that monsters only move on floors.  Will *
     * need to be modified for tunneling and pass-wall monsters.  */
    spot_t ret;
    static path_t path[DUNGEON_Y][DUNGEON_X], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;
    
    if (!initialized) {
        for (y = 0; y < DUNGEON_Y; y++) {
            for (x = 0; x < DUNGEON_X; x++) {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }
    
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            path[y][x].cost = SHRT_MAX;
        }
    }
    
    path[from[dim_y]][from[dim_x]].cost = 0;
    
    heap_init(&h, dist_cmp, NULL);
    
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            if (mapxy(x, y) >= ter_floor) {
                path[y][x].hn = heap_insert(&h, &path[y][x]);
            } else {
                path[y][x].hn = NULL;
            }
        }
    }
    
    while ((p = heap_remove_min(&h))) {
        p->hn = NULL;
        
        if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
            //uint32_t x_value; uint32_t y_value;
            for (x = to[dim_x], y = to[dim_y];
                 (x != from[dim_x]) || (y != from[dim_y]);
                 p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
                //mapxy(x, y) = ter_debug;
                int xx = x; int yy = y;
                ret.x = xx; ret.y = yy;
                //somehow return these values as the next move
            }
            //mappair(from) = ter_debug;
            
            heap_delete(&h);
            return ret;
        }
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost + 1)) {
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost = p->cost + 1;
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                         [p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost > p->cost + 1)) {
            path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost = p->cost + 1;
            path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                         [p->pos[dim_x]    ].hn);
        }
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + 1)) {
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost = p->cost + 1;
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                         [p->pos[dim_x] + 1].hn);
        }
        if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost > p->cost + 1)) {
            path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost = p->cost + 1;
            path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                         [p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost > p->cost + 1)) {
            path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost = p->cost + 1;
            path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                         [p->pos[dim_x] + 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost > p->cost + 1)) {
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost = p->cost + 1;
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                         [p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost > p->cost + 1)) {
            path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost = p->cost + 1;
            path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                         [p->pos[dim_x]    ].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost > p->cost + 1)) {
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost = p->cost + 1;
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                         [p->pos[dim_x] + 1].hn);
        }
    }
    return ret;
}