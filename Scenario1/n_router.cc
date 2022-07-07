#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#include "message_m.h"
#include "router.h"


class Nrouter : public Router
{
    protected:
    void routing(NetworkMsg* pkt) override;
};

Define_Module(Nrouter);


// DECIDE A CHI INOLTRARE IL PACCHETTO RICEVUTO
void Nrouter::routing(NetworkMsg* pkt)
{
    totData += pkt->getByteLength();
    data.collect(totData);
    dataVector.record(totData);
    if(pkt->arrivedOn("gate_hosts$i")) {
        sendOrWait(pkt, "gate_backbone$o", 0);
    } else {
        int i = pkt->getDestination()-12;
        if (i < 20) {
            sendOrWait(pkt, "gate_hosts$o", i%10);
        } else {
            sendOrWait(pkt, "gate_hosts$o", i%5);
        }
    }
}
