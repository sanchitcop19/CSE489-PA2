#include "../include/simulator.h"
#include <queue>

#define EXPIRE 25.0f

int checksum(struct pkt packet){

    int sum = 0;
    sum += (packet.seqnum + packet.acknum);

    for (int i = 0; i < 20; ++i){
        sum += (int)packet.payload[i];
    }
    return sum;

}

bool verify_checksum(struct pkt packet){
    int sum = packet.seqnum + packet.acknum;
    for (int i = 0; i < 20; ++i){
        sum += (int)packet.payload[i];
    }
    return sum == packet.checksum;
}

int send_base = 0;
int window = -1;
int next_seqnum = 0;
int expectedb = 0;
std::queue<struct pkt> buffer;
std::queue<struct pkt> unacked;
struct pkt current = pkt();

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
    struct pkt packet = pkt();
    packet.seqnum = next_seqnum;
    packet.acknum = 0;
    strncpy(packet.payload, message.data, 20);
    packet.checksum = checksum(packet);
    if (next_seqnum < (send_base + window)){
        tolayer3(0, packet);
        unacked.push(packet);
        if (send_base == next_seqnum){
            starttimer(0, EXPIRE);
        }
    }
    else{
        buffer.push(packet);
    }

    ++next_seqnum;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if (verify_checksum(packet)){
        send_base = packet.acknum + 1;
        if (send_base == next_seqnum) stoptimer(0);
        else starttimer(0, EXPIRE);
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    starttimer(0, EXPIRE);
    int base = send_base;
    while (base < next_seqnum){
        tolayer3(0, unacked.front());
        unacked.pop();
        ++base;
    }

}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{

    window = getwinsize();

}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if (verify_checksum(packet) and (packet.seqnum == expectedb)){
        tolayer5(1, packet.payload);
        struct pkt ack = pkt();
        ack.seqnum = 0;
        ack.acknum = expectedb;
        ack.checksum = checksum(ack);
        tolayer3(1, ack);
        expectedb++;
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
