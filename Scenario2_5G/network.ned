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

import inet.node.ethernet.Eth400G;
import inet.node.ethernet.Eth200G;
import inet.examples.mobileipv6.fiberline;
import inet.visualizer.common.IntegratedVisualizer;
import inet.visualizer.common.RadioVisualizer;
import ned.DatarateChannel;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import inet.node.ethernet.Eth10M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.Ue;
import simu5g.nodes.eNodeB;
import simu5g.nodes.PgwStandard;
import simu5g.stack.phy.ChannelModel.NRChannelModel;
import simu5g.world.radio.LteChannelControl;


network SingleCell
{
    parameters:
        int numUe = default(10);
        @display("i=block/network2;bgb=991,558;bgi=background/budapest");
    submodules:
        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
            pMax = default(2000mW);
            carrierFrequency = default(3.7GHz);
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
        }
        binder: Binder {
            @display("p=50,175;is=s");
            //maxDataRatePerRb = default(1.16Gbps);
            maxDataRatePerRb = default(1.16Gbps);
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        server: StandardHost {
            @display("p=910.425,74.625;is=n;i=device/server");
        }
        router: Router {
            @display("p=711.425,73.38125;i=device/smallrouter");
        }
        pgw: PgwStandard {
            @display("p=450.2375,74.625;is=l");
        }
//        eNB1: eNodeB {
//            @display("p=360,330;is=vl");
//            txPower = default(2000mw);
//        }
        eNB2: eNodeB {
            @display("p=530,330;is=vl");
            masterId = default(2);
            macCellId = default(2);
            txPower = default(2000mw);
        }
        ueArea1[numUe]: Ue {
            @display("p=401.48123,351.98126");
        }
        ueArea2[numUe]: Ue {
            @display("p=426.48123,351.98126");
        }
        ueArea3[numUe]: Ue {
            @display("p=451.48123,351.98126");
        }
        ueArea4[numUe]: Ue {
            @display("p=476.48123,351.98126");
        }
        gNB: gNodeB {
            @display("p=359.44376,351.98126");
            gateway = default("pgw");
            masterId = default(1);
            macCellId = default(1);
            txPower = default(2000mw);
        }
    connections:
        server.pppg++ <--> Eth400G <--> router.pppg++;
        router.pppg++ <--> Eth400G <--> pgw.filterGate;
        //pgw.pppg++ <--> Eth400G <--> eNB1.ppp;
        pgw.pppg++ <--> Eth400G <--> eNB2.ppp;
        pgw.pppg++ <--> Eth400G <--> gNB.ppp;
}
