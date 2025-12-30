#ifndef NODES_H
#define NODES_H

#include "sim.h"

struct distance_table {
    int costs[4][4];
};

void rtinit0();
void rtupdate0(struct rtpkt *rcvdpkt);
// void printdt0(struct distance_table *dtptr);
void linkhandler0(int linkid, int newcost);

void rtinit1();
void rtupdate1(struct rtpkt *rcvdpkt);
// void printdt1(struct distance_table *dtptr);
void linkhandler1(int linkid, int newcost);

void rtinit2();
void rtupdate2(struct rtpkt *rcvdpkt);
// void printdt2(struct distance_table *dtptr);

void rtinit3();
void rtupdate3(struct rtpkt *rcvdpkt);
// void printdt3(struct distance_table *dtptr);

void printdt0();
void printdt1();
void printdt2();
void printdt3();

// Implemented in node0.c
void sharedt(struct distance_table *dt, int src, int *neighbours, int neighcount);
void printdt(struct distance_table *dtptr, int src);

#endif
