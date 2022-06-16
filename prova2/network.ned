//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import simu5g.nodes.NR.gNodeB;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import inet.node.ethernet.Eth10M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.Ue;
import simu5g.nodes.eNodeB;
import simu5g.nodes.PgwStandard;
import simu5g.world.radio.LteChannelControl;

simple Client
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
        input radioIn @directIn;
}
simple Brouter
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate_net[];
        inout gate_servers[];
}
simple Nrouter
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate_hosts[];
        inout gate_backbone[];
}
simple Server
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
        input radioIn @directIn;
}

network Tictoc13
{
    @display("bgb=750,450");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 2Gbps;
            per = 1e-3; // ERRORI PACCHETTI
            //delay = 100ms;
        }
    submodules:
        server[5]: Server {
            @display("p=200,50,m,5,100,50");
            @display("i=device/server");
        }
        bRouter: Brouter {
            @display("i=old/srouter;p=400,125");
        }
        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
        }
        binder: Binder {
            @display("p=50,175;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        gNodeB: gNodeB {
            @display("p=508,173");
        }
        router[4]: Nrouter {
            @display("p=250,200,m,4,125,50");
            @display("i=abstract/router");
        }
        hostsarea1[10]: Client {
            @display("p=50,100,m,2,50,50");
            @display("i=device/pc2");
        }
        hostsarea2[10]: Client {
            @display("p=175,350,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea3[5]: Client {
            @display("p=475,275,m,3,50,50");
            @display("i=device/device");
        }
        hostsarea4[3]: Client {
            @display("p=700,250,m,1,50,50");
            @display("i=block/wheelbarrow");
        }
    connections:
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[0].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[1].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[2].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[3].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[4].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[5].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[6].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[7].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[8].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[9].gate++;

        router[1].gate_hosts++ <--> Channel <--> hostsarea2[0].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[1].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[2].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[3].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[4].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[5].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[6].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[7].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[8].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[9].gate++;

        router[2].gate_hosts++ <--> Channel <--> hostsarea3[0].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea3[1].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea3[2].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea3[3].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea3[4].gate++;

        router[3].gate_hosts++ <--> Channel <--> hostsarea4[0].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea4[1].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea4[2].gate++;


        server[0].gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;
        server[1].gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;
        server[2].gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;
        server[3].gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;
        server[4].gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;

        bRouter.gate_net++ <--> Channel <--> router[0].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[1].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[2].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[3].gate_backbone++;

        gNodeB.ppp <--> Channel <--> bRouter.gate_net++;
}
