#include <stdio.h>
#include <string.h>
#include "nodes.h"

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts1[4] = { 1,  0,  1, 999 };
struct distance_table dt1;
static int neighbours[] = { 0, 2 };
static int NEIGH_COUNT = (sizeof(neighbours) / sizeof((neighbours)[0]));

/* students to write the following two routines, and maybe some others */

void rtinit1() {
    int init_costs[4][4] = {
        { 999, 1,   999, 999 },
        { 999, 0,   999, 999 },
        { 999, 1,   999, 999 },
        { 999, 999, 999, 999 }
    };
    memcpy(dt1.costs, init_costs, sizeof dt1.costs);

    sharedt(&dt1, 1, neighbours, NEIGH_COUNT);
}


void rtupdate1(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    int *costs = rcvdpkt->mincost;

    for (int i = 0; i < 4; i++) {
        dt1.costs[i][src] = costs[i];
    }

    int changed = updatedt(&dt1, 1, neighbours, NEIGH_COUNT, connectcosts1);

    if (changed) {
        sharedt(&dt1, 1, neighbours, NEIGH_COUNT);
    }
}


void printdt1() {
    printdt(&dt1, 1);
}


void linkhandler1(int linkid, int newcost)
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
    connectcosts1[linkid] = newcost;

    int changed = updatedt(&dt1, 1, neighbours, NEIGH_COUNT, connectcosts1);
    if (changed) {
        sharedt(&dt1, 1, neighbours, NEIGH_COUNT);
    }
}

