#include "server.h"

using namespace omnetpp;

Define_Module(Server);


/*
 * SISTEMARE LA RETE
 * 10 pc OGNI ROUTER
 * */

// DECIDE A CHI INVIARE IL PACCHETTO
void Server::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        int tot = clients.size();

        if (msg->getKind() == types::PING) {
            if (tot != 0) {     // ping if it knows at least one clients
                TicTocMsg13 *update = generateMessage();

                // se il server "pinga", non ha senso calcolare la latenza perché non
                // c'è una risposta
                update->setNumSeq(-1);

                int i = rand()%(tot);
                std::set<int>::iterator itr = clients.begin();
                std::advance(itr, i);
                update->setDestination(*itr);

                int b = 1000;      // aggiornamento periodico mappa
                update->setByteLength(b);
                simtime_t t = tranTime("gate$o", 0);
                if (t <= simTime()) {
                    send(update->dup(), "gate$o", 0);
                    totData += b;
                    data.collect(b);
                    dataVector.record(b);
                    numSent++;
                } else {
                    fifo.insert(update->dup());
                    if(tran->isScheduled() == 0) {
                        scheduleAt(t, tran);
                    }
                }

                delete update;
            }
            // schedule next ping
            //scheduleAt(simTime()+1.0, updates[mID]);
        }
        if (msg->getKind() == types::TRAN) {    // types::TRAN == 20
            simtime_t t = tranTime("gate$o", 0);

            if (t <= simTime()) {
                cMessage* oldMsg = check_and_cast<cMessage *>(fifo.pop());
                send(oldMsg->dup(), "gate$o", 0);
                t = tranTime("gate$o", 0);
                delete oldMsg;
            }

            if(tran->isScheduled() == 0 && fifo.getLength() != 0) {
                scheduleAt(t, tran);
            }
        }

        if (msg->getKind() == types::TIMEOUT) {
            int mID = msg->par("matchID");
            for(auto it = matches[mID].partyA->begin(); it != matches[mID].partyA->end(); it++) {
                clients.erase(*it);
            }
            for(auto it = matches[mID].partyB->begin(); it != matches[mID].partyB->end(); it++) {
                clients.erase(*it);
            }
            matches[mID].partyA->clear();
            matches[mID].partyB->clear();
            matches[mID].full = false;
        }

        if (msg->getKind() == types::UPDATE) {
            int mID = msg->par("matchID");
            TicTocMsg13 *update = generateMessage();
            int b = rand()%(1500-1000+1)+1000;  // aggiornamento periodico
            update->setByteLength(b);
            update->setNumSeq(-1);
            for(auto it = matches[mID].partyA->begin(); it != matches[mID].partyA->end(); it++) {
                update->setDestination(*it);

                simtime_t t = tranTime("gate$o", 0);
                if (t <= simTime()) {
                    send(update->dup(), "gate$o", 0);
                } else {
                    fifo.insert(update->dup());
                    if(tran->isScheduled() == 0) {
                        scheduleAt(t, tran);
                    }
                }
                numSent++;
            }
            for(auto it = matches[mID].partyB->begin(); it != matches[mID].partyB->end(); it++) {
                update->setDestination(*it);

                simtime_t t = tranTime("gate$o", 0);
                if (t <= simTime()) {
                    send(update->dup(), "gate$o", 0);
                } else {
                    fifo.insert(update->dup());
                    if(tran->isScheduled() == 0) {
                        scheduleAt(t, tran);
                    }
                }
                numSent++;
            }

            //delete update;
            scheduleAt(simTime()+0.05, updates[mID]);
        }
    } else {
        if(Host::handle(msg) == 0){
            TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);
            int i = ttmsg->getSource();
            long n = ttmsg->getNumSeq();
            std::pair<std::set<int>::iterator, bool> ret = clients.insert(i);

            if(ttmsg->getKind() == types::MATCH) {
                if(ret.second == true) {
                    int mID = ttmsg->par("matchID");
                    // il aspetta 5 secondi per effettuare la connessione dei client
                    // altrimenti annulla il matchmaking
                    if (timeouts[mID]->isScheduled() == 0) {
                        cMsgPar *p = new cMsgPar("matchID");
                        p->setLongValue(mID);
                        timeouts[mID]->addPar(p);
                        scheduleAt(simTime()+5, timeouts[mID]);
                    }

                    int c = matches[mID].partyA->size();
                    if (c < 5) {
                        matches[mID].partyA->push_back(i);
                    } else {
                        matches[mID].partyB->push_back(i);
                        if (matches[mID].partyB->size() == 5) {
                            matches[mID].full = true;
                            cancelEvent(timeouts[mID]);
                            // timer avvio partita
                            cMsgPar *p = new cMsgPar("matchID");
                            p->setLongValue(mID);
                            updates[mID]->addPar(p);
                            scheduleAt(simTime()+0.5, updates[mID]);
                            //std::vector<int> data = {32, }
                            for (int i = 0; i < 13001; i++) { // 20 MB
                                TicTocMsg13 *start = generateMessage();
                                start->setNumSeq(-1);
                                if (i == 0) {
                                    start->setKind(types::START);
                                    start->setByteLength(32);       // messaggio di inizio caricamento partita
                                }
                                else
                                    start->setByteLength(1500);     // caricamento partita
                                for(auto it = matches[mID].partyA->begin(); it != matches[mID].partyA->end(); it++) {
                                    start->setDestination(*it);

                                    simtime_t t = tranTime("gate$o", 0);
                                    if (t <= simTime()) {
                                        send(start->dup(), "gate$o", 0);
                                    } else {
                                        fifo.insert(start->dup());
                                        if(tran->isScheduled() == 0) {
                                            scheduleAt(t, tran);
                                        }
                                    }
                                    numSent++;
                                }
                                for(auto it = matches[mID].partyB->begin(); it != matches[mID].partyB->end(); it++) {
                                    start->setDestination(*it);

                                    simtime_t t = tranTime("gate$o", 0);
                                    if (t <= simTime()) {
                                        send(start->dup(), "gate$o", 0);
                                    } else {
                                        fifo.insert(start->dup());
                                        if(tran->isScheduled() == 0) {
                                            scheduleAt(t, tran);
                                        }
                                    }
                                    numSent++;
                                }
                                delete start;
                            }

                        }
                    }
                }
            } else {
                ttmsg = Host::generateMessage();

                ttmsg->setDestination(i);
                ttmsg->setByteLength(32); // ACK, 32 byte

                ttmsg->setNumSeq(n);

                data.collect(32);
                dataVector.record(32);
                totData += 32;

                simtime_t t = tranTime("gate$o", 0);

                if (t <= simTime()) {
                    send(ttmsg->dup(), "gate$o", 0);
                } else {
                    fifo.insert(ttmsg->dup());
                    if(tran->isScheduled() == 0) {
                        scheduleAt(t, tran);
                    }
                }

                numSent++;

                delete ttmsg;
            }
        } // ALTRIMENTI È UN ERRORE QUINDI "SCARTALO"

        delete msg;
        //dropAndDelete(msg);
    }
}


void Server::initialize() {
    Host::initialize();

    //timeouts.resize(4);
    //updates.resize(4);
    matches.resize(4);
    for(int i = 0; i < 4; ++i) {
        timeouts.push_back(new cMessage("timeout", types::TIMEOUT));
    }
    for(int i = 0; i < 4; ++i) {
        updates.push_back(new cMessage("update", types::UPDATE));
    }
    scheduleAt(simTime()+exponential(2.0), ping);
    WATCH_VECTOR(matches);
    WATCH_VECTOR(timeouts);
    WATCH_VECTOR(updates);
}

std::ostream& operator<<(std::ostream& os, const match m) {
    os << "is full: " << m.full << std::endl;
    os << "partyA: [ ";
    for (auto it = m.partyA->begin(); it != m.partyA->end(); it++) {
        os << *it << " ";
    }
    os << "]\n";
    os << "partyB: [ ";
    for (auto it = m.partyB->begin(); it != m.partyB->end(); it++) {
        os << *it << " ";
    }
    os << "]\n";

    return os;
}
