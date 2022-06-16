#include "router.h"


Define_Module(Router);

void Router::initialize()
{
    numForwarded = 0;
    tran = new cMessage("tran", types::TRAN);
    WATCH(numForwarded);
}

// MOSTRA PACCHETTI INVIATI, RICEVUTI E PERSI
void Router::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "forwarded: %ld, dropped: %ld", numForwarded, totDropped);
    getDisplayString().setTagArg("t", 0, buf);
}

void Router::handleMessage(cMessage* msg) {

    if (msg->isSelfMessage()) {
        routing(check_and_cast<TicTocMsg13 *>(fifo.pop()));

        if(tran->isScheduled() == 0 && fifo.getLength() != 0) {
            scheduleAt(simTime()+0.001, tran);   // che schifo!!!
        }
    } else {
        TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);
        if (ttmsg->hasBitError() == 0) {

            ttmsg->setHopCount(ttmsg->getHopCount()+1);

            routing(ttmsg);
        } else {
            totDropped ++;
            dropped.collect(totDropped);
            droppedVector.record(totDropped);
            bubble("ERROR!!");
        }

        delete ttmsg;
    }
}

void Router::routing(TicTocMsg13* pkt) {}

void Router::sendOrWait(TicTocMsg13* pkt, const char * gatename, int gateIndex) {
    simtime_t t = tranTime(gatename, gateIndex);
    if (t <= simTime()) {
        send(pkt->dup(), gatename, gateIndex);
        numForwarded++;
    } else {
        fifo.insert(pkt->dup());
        if(tran->isScheduled() == 0) {
            scheduleAt(t, tran);
        }
    }
}

// RITORNA DURATA TRASMISSIONE IN CORSO SU QUEL CANALE
simtime_t Router::tranTime(const char * gatename, int gateIndex) {
    cGate *out = gate(gatename, gateIndex);
    const cChannel* ch = out->getTransmissionChannel();
    return ch->getTransmissionFinishTime();
}
