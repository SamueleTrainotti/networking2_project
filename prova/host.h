#ifndef HOST_H_
#define HOST_H_


#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <map>

#include "message_m.h"

using namespace omnetpp;

// 2 different timers
enum timers {PING = 10, TRAN = 20};


class Host : public cSimpleModule
{
    protected:
    long numSent = 0;
    long numReceived = 0;
    long err = 0;
    long totData = 0;
    long totDropped = 0;
    std::map<long, simtime_t> sendTimes;
    cMessage *ping, *tran;
    cHistogram data, dropped, latency;
    cOutVector dataVector, droppedVector, latencyVector;
    cQueue fifo;

    virtual TicTocMsg13 *generateMessage();
    virtual void refreshDisplay() const override;
    virtual void initialize() override;
    void sendOrWait(cMessage * msg, const char * gatename, int gateIndex=-1);
    simtime_t tranTime(const char * gatename, int gateIndex=-1);
    bool handle(cMessage *msg);
};


#endif /* HOST_H_ */
