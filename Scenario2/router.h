#ifndef ROUTER_H_
#define ROUTER_H_


#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#include "message_m.h"

enum types {PING = 10, TRAN = 20, MATCH = 30, START = 40};

class Router : public cSimpleModule
{
  protected:
    long numForwarded;
    long totData = 0;
    long totDropped = 0;
    cHistogram data, dropped;
    cOutVector dataVector, droppedVector;
    cMessage *tran;
    cQueue fifo;

    virtual void refreshDisplay() const override;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void routing(TicTocMsg13* pkt);
    void sendOrWait(TicTocMsg13* pkt, const char * gatename, int gateIndex=-1);
    simtime_t tranTime(const char * gatename, int gateIndex=-1);
};


#endif /* ROUTER_H_ */
