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
    ping = new cMessage("ping", types::PING);
    tran = new cMessage("tran", types::TRAN);
    timeout = new cMessage("timeout", types::TIMEOUT);
    dataVector.setName("dati "+getId());
    droppedVector.setName("errori "+getId());
    latencyVector.setName("latenza "+getId());
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
    NetworkMsg *ttmsg = check_and_cast<NetworkMsg *>(msg);
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
NetworkMsg *Host::generateMessage() {
    char msgname[20];

    sprintf(msgname, "%d", getId());

    // Create message object and set source and destination field.
    NetworkMsg *msg = new NetworkMsg(msgname);

    int src = getId();

    msg->setSource(src);
    msg->setDestination(-1);

    msg->setKind((src-12)%5);

    return msg;
}

// RITORNA DURATA TRASMISSIONE IN CORSO SU QUEL CANALE
simtime_t Host::tranTime(const char * gatename, int gateIndex) {
    cGate *out = gate(gatename, gateIndex);
    const cChannel* ch = out->getTransmissionChannel();
    return ch->getTransmissionFinishTime();
}
