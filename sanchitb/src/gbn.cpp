#include "../include/simulator.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string.h>
#include <string>
#include <cassert>

/* https://stackoverflow.com/questions/4668760/converting-an-int-to-stdstring */
std::string to_string2( int x ) {
  int length = snprintf( NULL, 0, "%d", x );
  assert( length >= 0 );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  std::string str( buf );
  delete[] buf;
  return str;
}


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
	printf("sum: %i, checksum: %i\n", sum, packet.checksum);
    return sum == packet.checksum;
}
int counter = 0;
int send_base = 0;
int window = -1;
int next = 0;
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
void A_output(struct msg message) {
    std::cout << "------------A-output-begin-------------" << std::endl;
    if (!unacked.empty())std::cout << "A_output called, unacked:" << to_string2(unacked.front().seqnum) << std::endl;
    struct pkt packet = pkt();
    packet.seqnum = counter++;
    strncpy(packet.payload, message.data, 20);
    packet.checksum = checksum(packet);
    current = packet;
    if (next < (send_base + window) and buffer.empty()){
        std::cout << "sending packet: " << packet.seqnum << std::endl;
        tolayer3(0, packet);
        unacked.push(packet);
        if (send_base == next){
            std::cout << "starting timer" << std::endl;
            starttimer(0, EXPIRE);
        }
        ++next;
    }
    else{
        std::cout << "buffering packet: " << packet.seqnum << std::endl;
        buffer.push(packet);
    }

    std::cout << "------------A-output-end-------------" << std::endl;

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
    std::cout << "------------A-input-begin-------------" << std::endl;
    if (!unacked.empty())std::cout << "A_input called, unacked:" << to_string2(unacked.front().seqnum) << std::endl;
    printf("receiving ack for : %i\n", packet.acknum);
	if (verify_checksum(packet) && packet.acknum >= send_base){
        send_base = packet.acknum + 1;
        printf("moving base to : %i\n", send_base);
	while(!unacked.empty() && unacked.front().seqnum < send_base) unacked.pop();
	printf("popped unacked\n");
        if (send_base == next) {
	printf("next: %i\n", next);
            stoptimer(0);
		printf("stopped timer\n");

            while (buffer.size() > 0 && next < send_base + window){
		printf("next: %i\n", next);
                tolayer3(0, buffer.front());
                if (send_base == next) {starttimer(0, EXPIRE);printf("starting timer\n");}
                unacked.push(buffer.front());
		printf("pushing %i to unacked from the buffer\n", buffer.front().seqnum);
	printf("popping buffer\n");	
                buffer.pop();
                ++next;

            }
        }
        else {
            /*TODO: make sure this is correct*/
            stoptimer(0);
            starttimer(0, EXPIRE);
		printf("stopped and restarted timer\n");
        }

    }
else printf("received corrupted packet\n");
    std::cout << "------------A-input-end-------------" << std::endl;
}

/* called when A's timer goes off */
void A_timerinterrupt() {
    std::cout << "------------A-timerinterrupt-begin-------------" << std::endl;
    if (!unacked.empty())std::cout << "A_timerinterrupt called, unacked:" << to_string2(unacked.front().seqnum) << std::endl;
    starttimer(0, EXPIRE);
    int base = send_base;

    while (base < next){
        std::cout << "resending packet: " << to_string2(unacked.front().seqnum)<< std::endl;
        tolayer3(0, unacked.front());
        unacked.push(unacked.front());
        unacked.pop();
        ++base;
    }
    std::cout << "------------A-timerinterrupt-end-------------" << std::endl;
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
    window = getwinsize();
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet) {
    std::cout << "------------B-input-------------" << std::endl;
    if (!verify_checksum(packet))return;
	if ((packet.seqnum == expectedb)){
        std::cout << "received expected packet, delivering to layer 5: " << to_string2(expectedb) << std::endl;
        tolayer5(1, packet.payload);
        expectedb++;
    }
if (packet.seqnum <= expectedb){
    printf("Received packet, sending ack: %i\n", packet.seqnum);
    struct pkt ack = pkt();
    ack.seqnum = 0;
    ack.acknum = packet.seqnum;
    ack.checksum = checksum(ack);
    tolayer3(1, ack);
}}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() {

}
