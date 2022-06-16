#include "server.h"

using namespace omnetpp;

Define_Module(Server);

// DECIDE A CHI INVIARE IL PACCHETTO
void Server::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        int tot = clients.size();

        if (msg->getKind() == timers::PING) {
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
                    /*
                    // Soluzione per rete congedtionata: non inviare i ping
                    totDropped ++;
                    dropped.collect(totDropped);
                    droppedVector.record(totDropped);*/
                }
            }
            // schedule next ping
            scheduleAt(simTime()+exponential(1.0), msg);
        } else {    // timers::TRAN == 20
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
    } else {
        if(Host::handle(msg) == 0){
            TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);
            int i = ttmsg->getSource();
            long n = ttmsg->getNumSeq();
            clients.insert(i);

            ttmsg = Host::generateMessage();

            int b = rand()%(1500-1000+1)+1000;  // rispnde al client con dati di posizione
            ttmsg->setDestination(i);
            ttmsg->setByteLength(b); // 1 KB

            ttmsg->setNumSeq(n);

            data.collect(b);
            dataVector.record(b);
            totData += b;

            simtime_t t = tranTime("gate$o", 0);

            if (t <= simTime()) {
                send(ttmsg->dup(), "gate$o", 0);
            } else {
                /*totDropped ++;
                dropped.collect(totDropped);
                droppedVector.record(totDropped);*/
                fifo.insert(ttmsg->dup());
                if(tran->isScheduled() == 0) {
                    scheduleAt(t, tran);
                }
            }

            numSent++;

            delete ttmsg;
            //dropAndDelete(ttmsg);
        } // ALTRIMENTI È UN ERRORE QUINDI "SCARTALO"

        delete msg;
        //dropAndDelete(msg);
    }
}


void Server::initialize() {
    Host::initialize();
    scheduleAt(simTime()+exponential(2.0), ping);
    WATCH_SET(clients);
}
