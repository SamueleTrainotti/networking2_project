#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <set>

using namespace omnetpp;

#include "message_m.h"
#include "host.h"


class Server : public Host
{
protected:
    std::set<int> clients;
    void initialize() override;
    void handleMessage(cMessage *msg) override;
};


#endif /* SERVER_H_ */
