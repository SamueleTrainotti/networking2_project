// 
//                  Simu5G
//
// Authors: Giovanni Nardini, Giovanni Stea, Antonio Virdis (University of Pisa)
// 
// This file is part of a software released under the license included in file
// "license.pdf". Please read LICENSE and README files before using it.
// The above files and the present reference are part of the software itself, 
// and cannot be removed from it.
// 

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth400G;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.visualizer.common.IntegratedVisualizer;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.Upf;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import simu5g.nodes.backgroundCell.BackgroundCell;
import simu5g.world.radio.LteChannelControl;

// SingleCell_standalone
//
// This network represents the deployment option 3 in 3GPP 38.801
//
network SingleCell
{
    parameters:
        int numServer = default(4);
        int numUePC = default(10);
        int numUeConsole = default(3);
        int numUeSimulator = default(5);
        int numBgCells = default(0);
        @display("i=block/network2;bgb=926,608");
    submodules:
        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
            pMax = default(2000mW);
            carrierFrequency = default(3.7GHz);
            numChannels = default(20);
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
        }
        binder: Binder {
            @display("p=50,175;is=s");
            maxDataRatePerRb = default(10.16Gbps);
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        server[numServer]: StandardHost {
            @display("p=320,50,m,5,100,50;is=n;i=device/server");
            // @display("p=511.632,45.288;is=n;i=device/server");
        }
        router: Router {
            @display("p=511.632,172.584;i=device/smallrouter");
        }
        upf: Upf {
            @display("p=510.408,323.136;is=l");
        }
        gnb1: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb2: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb3: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb4: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb5: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb6: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb7: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        gnb8: gNodeB {
            @display("p=726.35,277.35626;is=l");
        }
        // --------------------------------------------------------------------
//                
//                gnb9: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb10: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb11: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb12: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb13: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb14: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb15: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }
//                gnb16: gNodeB {
//                    @display("p=726.35,277.35626;is=l");
//                }        
//                
        // --------------------------------------------------------------------
        bgCell[numBgCells]: BackgroundCell {
            @display("p=416,277;is=vl");
        }
        ueArea1[numUePC]: NRUe {
            @display("p=250,225;is=s");
        }
        ueArea2[numUePC]: NRUe {
            @display("p=650,225;is=s");
        }
        ueArea3[numUeSimulator]: NRUe {
            @display("p=250,425;is=s");
        }
        ueArea4[numUeConsole]: NRUe {
            @display("p=650,425;is=s");
        }
    connections:
        server[0].pppg++ <--> Eth400G <--> router.pppg++;
        server[1].pppg++ <--> Eth400G <--> router.pppg++;
        server[2].pppg++ <--> Eth400G <--> router.pppg++;
        server[3].pppg++ <--> Eth400G <--> router.pppg++;
        router.pppg++ <--> Eth400G <--> upf.filterGate;
        upf.pppg++ <--> Eth400G <--> gnb1.ppp;
        upf.pppg++ <--> Eth400G <--> gnb2.ppp;
        upf.pppg++ <--> Eth400G <--> gnb3.ppp;
        upf.pppg++ <--> Eth400G <--> gnb4.ppp;
        upf.pppg++ <--> Eth400G <--> gnb5.ppp;
        upf.pppg++ <--> Eth400G <--> gnb6.ppp;
        upf.pppg++ <--> Eth400G <--> gnb7.ppp;
        upf.pppg++ <--> Eth400G <--> gnb8.ppp;


        //----------------------------------------------------
//        
//        upf.pppg++ <--> Eth400G <--> gnb9.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb10.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb11.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb12.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb13.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb14.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb15.ppp;
//        upf.pppg++ <--> Eth400G <--> gnb16.ppp;
//        
        //---------------------------------------------------
}

