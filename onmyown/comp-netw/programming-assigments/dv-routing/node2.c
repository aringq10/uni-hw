#include <stdio.h>
#include <string.h>
#include "nodes.h"

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts2[4] = { 3,  1,  0, 2 };
struct distance_table dt2;
static int neighbours[] = { 0, 1, 3 };
static int NEIGH_COUNT = (sizeof(neighbours) / sizeof((neighbours)[0]));

/* students to write the following two routines, and maybe some others */

void rtinit2() {
    int init_costs[4][4] = {
        { 999, 999, 3, 999 },
        { 999, 999, 1, 999 },
        { 999, 999, 0, 999 },
        { 999, 999, 2, 999 }
    };
    memcpy(dt2.costs, init_costs, sizeof dt2.costs);

    sharedt(&dt2, 2, neighbours, NEIGH_COUNT);
}


void rtupdate2(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    int *costs = rcvdpkt->mincost;

    for (int i = 0; i < 4; i++) {
        dt2.costs[i][src] = costs[i];
    }

    int changed = updatedt(&dt2, 2, neighbours, NEIGH_COUNT, connectcosts2);
    if (changed) {
        sharedt(&dt2, 2, neighbours, NEIGH_COUNT);
    }
}


void printdt2() {
    printdt(&dt2, 2);
}
