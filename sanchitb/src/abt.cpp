#include <iostream>
#include "../include/simulator.h"
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>

using namespace std;
#define EXPIRE 25.0f

struct pkt current_packet = pkt();
std::queue<struct msg> buffer;
bool acked = true;
int expectedb = 0;


int checksum(struct pkt packet){

    int sum = 0;
    sum += (packet.seqnum + packet.acknum);

    for (int i = 0; i < 20; ++i){
        sum += (int)packet.payload[i];
    }
    return sum;

}

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{

    if (!acked) {
        buffer.push(message);
        return;
    }
    struct pkt packet = pkt();
    packet.seqnum = !current_packet.seqnum;
    packet.acknum = 0;
    memset(packet.payload, '\0', 20);
    strncpy(packet.payload, message.data, 20);
    packet.checksum = checksum(packet);
    current_packet = packet;
    starttimer(0, EXPIRE);
    tolayer3(0, packet);
    acked = false;


}

bool verify_checksum(struct pkt packet){
    int sum = packet.seqnum + packet.acknum;
    for (int i = 0; i < 20; ++i){
        sum += (int)packet.payload[i];
    }
    return sum == packet.checksum;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if (!verify_checksum(packet)) return;
    if (!(current_packet.seqnum == packet.acknum)) return;
    stoptimer(0);
    acked = true;
    if (!buffer.empty()) {

        struct pkt packet2 = pkt();
        packet2.seqnum = !current_packet.seqnum;
        packet2.acknum = 0;
        memset(packet2.payload, '\0', 20);
        strncpy(packet2.payload, buffer.front().data, 20);
        packet2.checksum = checksum(packet2);
        current_packet = packet2;
        starttimer(0, EXPIRE);
        tolayer3(0, packet2);
        acked = false;
        buffer.pop();
    }

}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    tolayer3(0, current_packet);
    starttimer(0, EXPIRE);
}


/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    current_packet.seqnum = 1;
    current_packet.acknum = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if (!verify_checksum(packet)){return;}
    struct pkt ack = pkt();
    ack.acknum = packet.seqnum;
    ack.seqnum = 0;
    ack.checksum = ack.acknum+ack.seqnum;
    if (expectedb == packet.seqnum) {
        tolayer5(1, (char*)packet.payload);
        expectedb = !expectedb;
    }
    tolayer3(1, ack);

}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
