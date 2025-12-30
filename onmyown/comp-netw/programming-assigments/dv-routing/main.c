#include <stdio.h>
#include <stdlib.h>

#include "nodes.h"
#include "sim.h"

#define LINKCHANGES 1 
/* ******************************************************************
Programming assignment 3: implementing distributed, asynchronous,
                          distance vector routing.

THIS IS THE MAIN ROUTINE.  IT SHOULD NOT BE TOUCHED AT ALL BY STUDENTS!

**********************************************************************/


/*
   a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3();
   Defined in sim.h

   struct rtpkt {
       int sourceid;       id of sending router sending this pkt
       int destid;         id of router to which pkt being sent 
                           (must be an immediate neighbor)
       int mincost[4];     min cost to node 0 ... 3
   };
*/

int TRACE = 1;             /* for my debugging */
int YES = 1;
int NO = 0;

void creatertpkt(
    struct rtpkt *initrtpkt,
    int srcid,
    int destid,
    int mincosts[]
) {
    int i;
    initrtpkt->sourceid = srcid;
    initrtpkt->destid = destid;
    for (i=0; i<4; i++)
        initrtpkt->mincost[i] = mincosts[i];
}  


/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 2 and below network environment:
  - emulates the tranmission and delivery (with no loss and no
    corruption) between two physically connected nodes
  - calls the initializations routines rtinit0, etc., once before
    beginning emulation

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

/* Defined in sim.h
    struct event {
        float evtime;           event time
        int evtype;             event type code
        int eventity;           entity where event occurs
        struct rtpkt *rtpktptr; ptr to packet (if any) assoc w/ this event
        struct event *prev;
        struct event *next;
    };
*/
struct event *evlist = NULL;   /* the event list */

/* possible events: */
#define  FROM_LAYER2     2
#define  LINK_CHANGE     10
 
float clocktime = 0.000;

int main() {
    struct event *eventptr;

    init();

    printf("\n------------------------------\n");
    printf("Initial DV tables\n\n");
    printdt0();
    printdt1();
    printdt2();
    printdt3();
    printf("------------------------------\n\n");

    while (1) {

        eventptr = evlist;            /* get next event to simulate */
        if (eventptr==NULL)
            goto terminate;
        evlist = evlist->next;        /* remove this event from event list */
        if (evlist!=NULL)
            evlist->prev=NULL;
        if (TRACE>1) {
            printf("MAIN: rcv event, t=%.3f, at %d",
                   eventptr->evtime,eventptr->eventity);
            if (eventptr->evtype == FROM_LAYER2 ) {
                printf(" src:%2d,",eventptr->rtpktptr->sourceid);
                printf(" dest:%2d,",eventptr->rtpktptr->destid);
                printf(" contents: %3d %3d %3d %3d\n", 
                       eventptr->rtpktptr->mincost[0], eventptr->rtpktptr->mincost[1],
                       eventptr->rtpktptr->mincost[2], eventptr->rtpktptr->mincost[3]);
            }
        }
        clocktime = eventptr->evtime;    /* update time to next event time */
        if (eventptr->evtype == FROM_LAYER2 ) {
            if (eventptr->eventity == 0) 
                rtupdate0(eventptr->rtpktptr);
            else if (eventptr->eventity == 1) 
                rtupdate1(eventptr->rtpktptr);
            else if (eventptr->eventity == 2) 
                rtupdate2(eventptr->rtpktptr);
            else if (eventptr->eventity == 3) 
                rtupdate3(eventptr->rtpktptr);
            else { printf("Panic: unknown event entity\n"); exit(0); }
        }
        else if (eventptr->evtype == LINK_CHANGE ) {
            if (clocktime<10001.0) {
                linkhandler0(1,20);
                linkhandler1(0,20);
            }
            else   {
                linkhandler0(1,1);
                linkhandler1(0,1);
            }
        }
        else
    { printf("Panic: unknown event type\n"); exit(0); }
        if (eventptr->evtype == FROM_LAYER2 ) 
            free(eventptr->rtpktptr);        /* free memory for packet, if any */
        free(eventptr);                    /* free memory for event struct   */
    }


terminate:
    printf("\n------------------------------\n");
    printf("Final DV tables\n\n");
    printdt0();
    printdt1();
    printdt2();
    printdt3();
    printf("------------------------------\n\n");
    printf("\nSimulator terminated at t=%f, no packets in medium\n", clocktime);
}



