#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#include "message_m.h"
#include "router.h"


class Brouter : public Router
{
protected:
    void routing(NetworkMsg* pkt) override;
};

Define_Module(Brouter);

// DECIDE A CHI INOLTRARE IL PACCHETTO RICEVUTO
void Brouter::routing(NetworkMsg* pkt)
{
    if(pkt->arrivedOn("gate_net$i")) {
        sendOrWait(pkt, "gate_servers$o", pkt->getDestination()-2);
    } else {
        int i = (pkt->getDestination()-12)/5;
        switch (i) {
            case 0:
            case 1:
                sendOrWait(pkt, "gate_net$o", 0);
                break;
            case 2:
            case 3:
                sendOrWait(pkt, "gate_net$o", 1);
                break;
            case 4:
                sendOrWait(pkt, "gate_net$o", 2);
                break;
            case 5:
                sendOrWait(pkt, "gate_net$o", 3);
                break;
        }
    }

}
