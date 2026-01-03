#include <string.h>
#include "nodes.h"
#include "sim.h"

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts0[4] = { 0, 1, 3, 7 };
struct distance_table dt0;
static int neighbours[] = { 1, 2, 3 };
static int NEIGH_COUNT = (sizeof(neighbours) / sizeof((neighbours)[0]));

/* students to write the following two routines, and maybe some others */

void rtinit0() {
    int init_costs[4][4] = {
        { 0, 999, 999, 999 },
        { 1, 999, 999, 999 },
        { 3, 999, 999, 999 },
        { 7, 999, 999, 999 }
    };
    memcpy(dt0.costs, init_costs, sizeof dt0.costs);

    sharedt(&dt0, 0, neighbours, NEIGH_COUNT);
}


void rtupdate0(struct rtpkt *rcvdpkt) {
    int src = rcvdpkt->sourceid;
    int *costs = rcvdpkt->mincost;

    for (int i = 0; i < 4; i++) {
        dt0.costs[i][src] = costs[i];
    }

    int changed = updatedt(&dt0, 0, neighbours, NEIGH_COUNT, connectcosts0);

    if (changed) {
        sharedt(&dt0, 0, neighbours, NEIGH_COUNT);
    }
}


void printdt0() {
    printdt(&dt0, 0);
}


void linkhandler0(int linkid, int newcost)
/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
    connectcosts0[linkid] = newcost;

    int changed = updatedt(&dt0, 0, neighbours, NEIGH_COUNT, connectcosts0);
    if (changed) {
        sharedt(&dt0, 0, neighbours, NEIGH_COUNT);
    }
}

