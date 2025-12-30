#include <stdio.h>
#include <string.h>
#include "nodes.h"

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts3[4] = { 7,  999,  2, 0 };
struct distance_table dt3;
static int neighbours[] = { 0, 2 };
static int NEIGH_COUNT = (sizeof(neighbours) / sizeof((neighbours)[0]));

/* students to write the following two routines, and maybe some others */

void rtinit3() {
    int init_costs[4][4] = {
        { 999, 999, 999, 7   },
        { 999, 999, 999, 999 },
        { 999, 999, 999, 2   },
        { 999, 999, 999, 0   }
    };
    memcpy(dt3.costs, init_costs, sizeof dt3.costs);

    sharedt(&dt3, 3, neighbours, NEIGH_COUNT);
}


void rtupdate3(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    int *costs = rcvdpkt->mincost;

    for (int i = 0; i < 4; i++) {
        dt3.costs[i][src] = costs[i];
    }

    int changed = updatedt(&dt3, 3, neighbours, NEIGH_COUNT, connectcosts3);
    if (changed) {
        sharedt(&dt3, 3, neighbours, NEIGH_COUNT);
    }
}


void printdt3() {
    printdt(&dt3, 3);
}
