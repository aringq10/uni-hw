#include <stdio.h>
#include <string.h>
#include "nodes.h"
#include "sim.h"

extern int TRACE;
extern int YES;
extern int NO;

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
    int changed = 0;

    for (int i = 0; i < 4; i++) {
        dt0.costs[i][src] = costs[i];
        if (dt0.costs[src][0] + costs[i] < dt0.costs[i][0]) {
            dt0.costs[i][0] = dt0.costs[src][0] + costs[i];
            changed = 1;
        }
    }

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
}


void sharedt(struct distance_table *dt, int src, int *neighbours, int neighcount) {
    for (int i = 0; i < neighcount; i++) {
        struct rtpkt msg = {
            src, neighbours[i],
            { dt->costs[0][src], dt->costs[1][src], dt->costs[2][src], dt->costs[3][src] }
        };
        tolayer2(msg);
    }
}


void printdt(struct distance_table *dtptr, int src) {
    printf("                via     \n");
    printf("   D%d |    0     1     2     3\n", src);
    printf("  ----|-----------------------\n");
    printf("     0|  %3d   %3d   %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][1],
           dtptr->costs[0][2],dtptr->costs[0][3]);
    printf("     1|  %3d   %3d   %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][1],
           dtptr->costs[1][2],dtptr->costs[1][3]);
    printf("dest 2|  %3d   %3d   %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][1],
           dtptr->costs[2][2],dtptr->costs[2][3]);
    printf("     3|  %3d   %3d   %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][1],
           dtptr->costs[3][2],dtptr->costs[3][3]);
}
