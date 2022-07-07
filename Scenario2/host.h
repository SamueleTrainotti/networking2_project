#ifndef HOST_H_
#define HOST_H_


#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <map>

#include "message_m.h"

using namespace omnetpp;

enum types {PING = 10, TRAN = 20, MATCH = 30, START = 40, TIMEOUT = 50, UPDATE = 60};


class Host : public cSimpleModule
{
    protected:
    long numSent = 0;
    long numReceived = 0;
    long err = 0;
    long totData = 0;
    long totDropped = 0;
    std::map<long, simtime_t> sendTimes;
    cMessage *ping, *tran, *timeout;
    cHistogram data, dropped, latency;
    cOutVector dataVector, droppedVector, latencyVector;
    cQueue fifo;

    virtual NetworkMsg *generateMessage();
    virtual void refreshDisplay() const override;
    virtual void initialize() override;
    void sendOrWait(cMessage * msg, const char * gatename, int gateIndex=-1);
    simtime_t tranTime(const char * gatename, int gateIndex=-1);
    bool handle(cMessage *msg);
};


#endif /* HOST_H_ */
