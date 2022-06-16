#include "host.h"


using namespace omnetpp;

// mostra pacchetti inviati/ricevuti/persi
void Host::refreshDisplay() const
{
    char buf[50];
    sprintf(buf, "rcvd: %ld sent: %ld err: %ld", numReceived, numSent, err);
    getDisplayString().setTagArg("t", 0, buf);
}

void Host::initialize()
{
    ping = new cMessage("ping", timers::PING);
    tran = new cMessage("tran", timers::TRAN);
    WATCH(numSent);
    WATCH(numReceived);
    WATCH(err);
    WATCH(totData);
    WATCH(totDropped);
    WATCH_MAP(sendTimes);
}

// INCREMENTA PACCHETTI RICEVUTI
// CONTROLLA SE IL MESSAGGIO HA UN FLAG DI ERRORE
// CONTROLLA SE IL MESSAGGIO È ARRIVATO ALL'HOST DI DESTINAZIONE

bool Host::handle(cMessage *msg)
{
    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);
    numReceived++;

    if (ttmsg->hasBitError()) {
        err++;
        bubble("ERROR!!");
        return true; // contiene errore
    } else if (ttmsg->getDestination() == getId()) {
        long n = ttmsg->getNumSeq();

        std::map<long, simtime_t>::iterator itr = sendTimes.find(n);

        if(itr != sendTimes.end()) {
            simtime_t l = simTime()-itr->second;

            EV << "L : " << l << "\n";

            latency.collect(l);
            latencyVector.record(l);
        }
        bubble("ARRIVED!");
    }

    return false; // non ha errori
}

// CREA PACCHETTO E IMPOSTA DESTINAZIONE E "COLORE"
TicTocMsg13 *Host::generateMessage() {
    char msgname[20];

    sprintf(msgname, "%d", getId());

    // Create message object and set source and destination field.
    TicTocMsg13 *msg = new TicTocMsg13(msgname);

    int src = getId();

    msg->setSource(src);
    msg->setDestination(-1);

    if((src > 11 && src < 22)) { // host area 1
        msg->setKind(0);
    } else if (src > 21 && src < 32) { // host area 2
        msg->setKind(1);
    } else if (src > 31 && src < 37) { // host area 3
        msg->setKind(2);
    } else if (src > 36) { // host area 4
        msg->setKind(3);
    }

    if(src == 2 || src == 3) { // server 0 e 1
        msg->setKind(4);
    } else if (src == 4) { // server 2
        msg->setKind(5);
    } else { // server 3 e 4
        msg->setKind(6);
    }

    return msg;
}

// RITORNA DURATA TRASMISSIONE IN CORSO SU QUEL CANALE
simtime_t Host::tranTime(const char * gatename, int gateIndex) {
    cGate *out = gate(gatename, gateIndex);
    const cChannel* ch = out->getTransmissionChannel();
    return ch->getTransmissionFinishTime();
}
