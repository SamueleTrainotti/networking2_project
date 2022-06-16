#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace omnetpp;

#include "message_m.h"
#include "host.h"

struct match {
    bool full = false;
    std::vector<int> partyA[5];
    std::vector<int> partyB[5];

    friend std::ostream& operator<<(std::ostream& os, const match m);
} tmp;

bool isIncomplete(match m) {
    return !m.full;
}

std::ostream& operator<<(std::ostream& os, const match m);

class Server : public Host
{
protected:
    std::vector<cMessage *> timeouts;
    std::set<int> clients;
    std::vector<match>   matches;
    std::vector<cMessage *> updates;
    void initialize() override;
    void handleMessage(cMessage *msg) override;
};


#endif /* SERVER_H_ */
