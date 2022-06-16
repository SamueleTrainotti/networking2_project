#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "host.h"

using namespace omnetpp;


class Client : public Host
{
protected:
    TicTocMsg13 *generateMessage() override;
    void initialize() override;
    void handleMessage(cMessage *msg) override;
};

Define_Module(Client);

// IMPOSTA IL TIMER INZIALE PER OGNI CLIENT PER PINGARE
void Client::initialize()
{
    Host::initialize();

    for (int i = 12; i < 40; i++) {
        if (getId() == i)
            scheduleAt(simTime()+exponential(1.0)+0.5, ping);
    }

}

// SE TIMER ALLORA È SCADUTO E PINGA UN NUOVO HOST
// ALTRIMENTI CONTROLLA SE È ERRORE E NON FARE NULLA
void Client::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        TicTocMsg13 *pong = generateMessage();

        simtime_t t = tranTime("gate$o", 0);
        if (t <= simTime()) {
            pong->setNumSeq(numSent);

            sendTimes.insert(std::pair<long, simtime_t>(numSent++, simTime()));
            cModule *target = getModuleByPath("Tictoc13.gNodeB");
            sendDirect(pong->dup(), target, "radioIn");
            //send(pong->dup(), "gate$o", 0);
        } else { // ALTRIMENTI NON FARE NULLA
            totDropped ++;
            dropped.collect(totDropped);
            droppedVector.record(totDropped);
        }
        delete pong;

        // schedule next call
        scheduleAt(simTime()+exponential(0.05), ping);
    } else {
        Host::handle(msg);
    }


    // ALTRIMENTI È UN ERRORE QUINDI "SCARTALO"
}

// IMPOSTA DESTINAZIONE SEMI-CASUALE IN BASE ALLA SOTTORETE
TicTocMsg13 *Client::generateMessage()
{
    TicTocMsg13 *msg = Host::generateMessage();
    int src = getId();

    if((src > 11 && src < 22) || (src > 21 && src < 32)) { // host area 1 & 2
        msg->setDestination(rand()%2 ? 2 : 3);
    } else if (src > 31 && src < 37) { // host area 3
        msg->setDestination(4);
    } else if (src > 36) { // host area 4
        msg->setDestination(rand()%2 ? 5 : 6);
    }

    int b = rand()%(1500-1000+1)+1000;
    //data.collect(b);
    //dataVector.record(b);
    totData += b;

    msg->setByteLength(b); // circa 'b' KB
    return msg;
}
