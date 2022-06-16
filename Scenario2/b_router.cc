#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#include "message_m.h"
#include "router.h"


class Brouter : public Router
{
protected:
    void routing(TicTocMsg13* pkt) override;
};

Define_Module(Brouter);

// DECIDE A CHI INOLTRARE IL PACCHETTO RICEVUTO
void Brouter::routing(TicTocMsg13* pkt)
{
    if(pkt->arrivedOn("gate_net$i")) {
        sendOrWait(pkt, "gate_servers$o", 0);
    } else {
        int i = (pkt->getDestination()-8)/10;
        sendOrWait(pkt, "gate_net$o", i);
    }

}
