#ifndef NODES_H
#define NODES_H

#include "sim.h"

struct distance_table {
    int costs[4][4];
};

void rtinit0();
void rtupdate0(struct rtpkt *rcvdpkt);
void linkhandler0(int linkid, int newcost);

void rtinit1();
void rtupdate1(struct rtpkt *rcvdpkt);
void linkhandler1(int linkid, int newcost);

void rtinit2();
void rtupdate2(struct rtpkt *rcvdpkt);

void rtinit3();
void rtupdate3(struct rtpkt *rcvdpkt);

void printdt0();
void printdt1();
void printdt2();
void printdt3();

void sharedt(struct distance_table *dt, int src, int *neighbours, int neighcount);
void printdt(struct distance_table *dtptr, int src);
void printdts(char *printtitle);
int updatedt(struct distance_table *dtptr, int src, int *neighbours, int neighcount, int *conncosts);

#endif
