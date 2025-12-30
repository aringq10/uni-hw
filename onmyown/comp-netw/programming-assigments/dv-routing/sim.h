#ifndef SIM_H
#define SIM_H

#define FROM_LAYER2 2
#define LINK_CHANGE 10
#define LINKCHANGES 1 


struct rtpkt {
    int sourceid;       /* id of sending router sending this pkt */
    int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
    int mincost[4];     /* min cost to node 0 ... 3 */
};


struct event {
    float evtime;           /* event time */
    int evtype;             /* event type code */
    int eventity;           /* entity where event occurs */
    struct rtpkt *rtpktptr; /* ptr to packet (if any) assoc w/ this event */
    struct event *prev;
    struct event *next;
};


void init();
float jimsrand();
void insertevent(struct event *p);
void printevlist();
void tolayer2(struct rtpkt packet);


#endif
