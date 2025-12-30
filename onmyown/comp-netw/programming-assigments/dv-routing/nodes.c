#include <stdio.h>
#include "nodes.h"

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


void printdts(char *printtitle) {
    printf("\n------------------------------------------------------\n");
    printf("%s\n\n", printtitle);
    printdt0();
    printdt1();
    printdt2();
    printdt3();
    printf("------------------------------------------------------\n\n");
}


int updatedt(struct distance_table *dt, int src, int *neighbours, int neighcount, int *conncosts) {
    int tmp[4] = { dt->costs[0][src], dt->costs[1][src], dt->costs[2][src], dt->costs[3][src] };
    // Reset lowest costs
    for (int i = 0; i < 4; i++) {
        dt->costs[i][src] = conncosts[i];
    }
    int changed = 0;
    // Calculate new lowest costs
    for (int d = 0; d < 4; d++) {
        for (int a = 0; a < neighcount; a++) {
            int n = neighbours[a];
            if (dt->costs[n][src] + dt->costs[d][n] < dt->costs[d][src]) {
                dt->costs[d][src] = dt->costs[n][src] + dt->costs[d][n];
            }
        }
    }
    // Check for changes
    for (int i = 0; i < 4; i++) {
        if (dt->costs[i][src] != tmp[i]) {
            changed = 1;
            break;
        }
    }

    return changed;
}
