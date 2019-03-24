#include "../include/simulator.h"
#include <queue>
#include <deque>
#include <utility>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string.h>
#include <string>
#include <cassert>

/* https://stackoverflow.com/questions/4668760/converting-an-int-to-stdstring */
std::string to_string2(int x) {
    int length = snprintf(NULL, 0, "%d", x);
    assert(length >= 0);
    char *buf = new char[length + 1];
    snprintf(buf, length + 1, "%d", x);
    std::string str(buf);
    delete[] buf;
    return str;
}

#define EXPIRE 25.0f

int checksum(struct pkt packet) {

    int sum = 0;
    sum += (packet.seqnum + packet.acknum);

    for (int i = 0; i < 20; ++i) {
        sum += (int) packet.payload[i];
    }
    return sum;

}

bool verify_checksum(struct pkt packet) {
    int sum = packet.seqnum + packet.acknum;
    for (int i = 0; i < 20; ++i) {
        sum += (int) packet.payload[i];
    }
    printf("sum: %i, checksum: %i\n", sum, packet.checksum);
    return sum == packet.checksum;
}

int counter = 0;
int send_base = 0;
int window = -1;
int next = 0;
int expectedb = 0;
int highestacked = -1;
std::queue<struct pkt> buffer;
std::deque <std::pair <struct pkt, float> > unacked;
std::deque<struct pkt> buffer_b;
bool acked[1000] = {false};
struct pkt current = pkt();

void printbufferb(std::deque<struct pkt>q){
printf("buffer_b: ");
    while (!q.empty()){
        printf("seqnum: %i | ", q.front().seqnum);
        q.pop_front();
    }
    fflush(0);
}
void printqueue(std::deque <std::pair <struct pkt, float> > q){
    while (!q.empty()){
        printf("seqnum: %i, expiration time: %0.5f | ", q.front().first.seqnum, q.front().second);
        q.pop_front();
    }
    fflush(0);
}

/* ******************************************************************
 *  ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
 *
 *     This code should be used for PA2, unidirectional data transfer
 *        protocols (from A to B). Network properties:
 *           - one way network delay averages five time units (longer if there
 *                are other messages in the channel for GBN), but can be larger
 *                   - packets can be corrupted (either the header or the data portion)
 *                        or lost, according to user-defined probabilities
 *                           - packets will be delivered in the order in which they were sent
 *                                (although some can be lost).
 *                                **********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) {
    std::cout << "------------A-output-begin-------------" << std::endl;
    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    if (!unacked.empty())std::cout << "A_output called, unacked:" << to_string2(unacked.front().first.seqnum) << std::endl;
    struct pkt packet = pkt();
    packet.seqnum = counter++;
    strncpy(packet.payload, message.data, 20);
    packet.checksum = checksum(packet);
    current = packet;
    if (next < (send_base + window) and buffer.empty()) {
        std::cout << "sending packet: " << packet.seqnum << std::endl;
        tolayer3(0, packet);
        unacked.push_back(std::make_pair(packet, get_sim_time() + EXPIRE));
        if (send_base == next) {
            std::cout << "starting timer" << std::endl;
            starttimer(0, EXPIRE);
        }
        ++next;
    } else {
        std::cout << "buffering packet: " << packet.seqnum << std::endl;
        buffer.push(packet);
    }

    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    std::cout << "------------A-output-end-------------" << std::endl;

}

bool checkpreviousacked(int acknum){
    for (int i = std::max(acknum - 1, 0); i < acknum; ++i){
        if (!acked[i]) return false;
    }
    return true;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
    std::cout << "------------A-input-begin-------------" << std::endl;
    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    if (!unacked.empty())std::cout << "A_input called, unacked:" << to_string2(unacked.front().first.seqnum) << std::endl;
    printf("receiving ack for : %i\n", packet.acknum);
    if (!verify_checksum(packet))goto corrupted; 
    if (acked[packet.acknum]) return;
    if (verify_checksum(packet) && packet.acknum >= send_base) {

        if (packet.acknum > highestacked) highestacked = packet.acknum;
        printf("highestacked: %i\n", highestacked);
        std::deque<std::pair<struct pkt, float> >::iterator it = unacked.begin();
        std::deque<std::pair<struct pkt, float> >::iterator del = unacked.end();
        std::deque<std::pair<struct pkt, float> >::iterator minimum = unacked.end();
        int min = 10000;
        while (it != unacked.end()){
            if ((*it).first.seqnum == packet.acknum) del = it;
            if ((*it).first.seqnum < min) {
                min = (*it).first.seqnum;
                minimum = it;
            }
            printf("seqnum: %i, min: %i\t", (*it).first.seqnum, min);
            it++;
        }
        std::cout << std::endl;
        acked[packet.acknum] = true;
        if ((*minimum).first.seqnum > send_base || (*minimum).first.seqnum == (*del).first.seqnum){
            while (acked[send_base] and send_base < highestacked + 1)send_base++;
            printf("moving base to : %i\n", send_base);
        }

        unacked.erase(del);
        printf("popped unacked\n");
        acked[packet.acknum] = true;
        if (send_base == next) {
            printf("next: %i\n", next);
            printf("stopping timer\n");
            stoptimer(0);

            while (buffer.size() > 0 && next < send_base + window) {
                printf("next: %i\n", next);
                tolayer3(0, buffer.front());
                if (send_base == next) {
                    starttimer(0, EXPIRE);
                    printf("starting timer\n");
                }
                unacked.push_back(std::make_pair(buffer.front(), get_sim_time() + EXPIRE));
                printf("pushing %i to unacked from the buffer\n", buffer.front().seqnum);
                printf("popping buffer\n");
                buffer.pop();
                ++next;

            }
        } else {
            stoptimer(0);
            starttimer(0, unacked.front().second - get_sim_time());
            printf("stopped and restarted timer\n");
        }

    } 
    corrupted:
    printf("received corrupted packet\n");
    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    std::cout << "------------A-input-end-------------" << std::endl;
}

/* called when A's timer goes off */
void A_timerinterrupt() {
    std::cout << "------------A-timerinterrupt-begin-------------" << std::endl;
    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    if (!unacked.empty())
        std::cout << "A_timerinterrupt called, unacked:" << to_string2(unacked.front().first.seqnum) << std::endl;
    int base = send_base;
    std::cout << "resending packet: " << to_string2(unacked.front().first.seqnum) << std::endl;
    tolayer3(0, unacked.front().first);
    unacked.push_back(std::make_pair(unacked.front().first, get_sim_time() + EXPIRE));
    unacked.pop_front();
    ++base;

    starttimer(0,unacked.front().second  - get_sim_time());

    std::cout << "Status of unacked queue: [";
    printqueue(unacked);
    std::cout << "]" << std::endl;
    std::cout << "------------A-timerinterrupt-end-------------" << std::endl;
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
    window = getwinsize();
}
bool received[1000] = {false};
/* Note that with simplex transfer from a-to-B, there is no B_output() */
bool comppacket(struct pkt a, struct pkt b){
    return a.seqnum < b.seqnum;
}
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet) {
    std::cout << "------------B-input-------------" << std::endl;
        printbufferb(buffer_b);std::cout << std::endl;
    if (!verify_checksum(packet))return;
    if ((packet.seqnum == expectedb)) {
        std::cout << "received expected packet, delivering to layer 5: " << to_string2(expectedb) << std::endl;
        tolayer5(1, packet.payload);
        expectedb++;
        std::sort(buffer_b.begin(), buffer_b.end(), comppacket);
        while (!buffer_b.empty() && buffer_b.front().seqnum == expectedb){
            expectedb++;
            tolayer5(1, buffer_b.front().payload);
            buffer_b.pop_front();
        }

    }
    if (packet.seqnum > expectedb and !received[packet.seqnum]){
        buffer_b.push_back(packet);
    }
        printf("Received packet, sending ack: %i\n", packet.seqnum);
        struct pkt ack = pkt();
        ack.seqnum = 0;
        ack.acknum = packet.seqnum;
        ack.checksum = checksum(ack);
        tolayer3(1, ack);
    received[packet.seqnum] = true;
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() {

}

