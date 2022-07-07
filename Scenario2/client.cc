#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "host.h"

using namespace omnetpp;


class Client : public Host
{
protected:
    int matchID;
    long numSeq = 0;
    std::list<int> n_burst;
    NetworkMsg *generateMessage() override;
    void initialize() override;
    void handleMessage(cMessage *msg) override;
};

Define_Module(Client);

// IMPOSTA IL TIMER INZIALE PER OGNI CLIENT PER PINGARE
void Client::initialize()
{
    Host::initialize();
    WATCH_LIST(n_burst);
    matchID = (getId()-8)/10;
    NetworkMsg *m = Host::generateMessage();
    m->setDestination(2);
    m->setKind(types::MATCH);
    cMsgPar *p = new cMsgPar("matchID");
    p->setLongValue(matchID);
    m->addPar(p);

    for (int i = 8; i < 48; i++) {
        if (getId() == i){
            sendDelayed(m->dup(), simTime()+exponential(1.0)+0.5, "gate$o", 0);
            scheduleAt(simTime()+5, timeout);
            numSent++;
        }

    }

    delete m;
}

// SE TIMER ALLORA È SCADUTO E PINGA UN NUOVO HOST
// ALTRIMENTI CONTROLLA SE È ERRORE E NON FARE NULLA
void Client::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if(msg->getKind() == types::TIMEOUT) {
            NetworkMsg *m = Host::generateMessage();
            m->setDestination(2);
            m->setKind(types::MATCH);
            cMsgPar *p = new cMsgPar("matchID");
            p->setLongValue(matchID);
            m->addPar(p);
            send(m->dup(), "gate$o", 0);
            numSent++;
        } else {
            int burst = rand()%10+1;
            n_burst.push_back(burst);
            //int burst = 1;
            for(int i = 0; i < burst; i++) {
                NetworkMsg *pong = generateMessage();

                simtime_t t = tranTime("gate$o", 0);
                if (t <= simTime()) {
                    pong->setNumSeq(numSeq);

                    sendTimes.insert(std::pair<long, simtime_t>(numSeq++, simTime()));
                    send(pong->dup(), "gate$o", 0);
                    numSent++;
                } else { // ALTRIMENTI NON FARE NULLA
                    totDropped ++;
                    dropped.collect(totDropped);
                    droppedVector.record(totDropped);
                }
                delete pong;

                // schedule next call
                if(ping->isScheduled() == 0) {
                    scheduleAt(simTime()+uniform(0.1, 0.2), ping);
                }
            }
        }
    } else {
        if (Host::handle(msg) == false) {
            if (msg->getKind() == types::START) {
                cancelEvent(timeout);
                scheduleAt(simTime()+uniform(0.5, 1.0), ping);
            }
        }

        delete msg;
    }


    // ALTRIMENTI È UN ERRORE QUINDI "SCARTALO"
}

// IMPOSTA DESTINAZIONE SEMI-CASUALE IN BASE ALLA SOTTORETE
NetworkMsg *Client::generateMessage()
{
    NetworkMsg *msg = Host::generateMessage();

    //Solo 1 server, id=2
    msg->setDestination(2);

    int b = 1024;
    totData += b;

    msg->setByteLength(b);
    return msg;
}
