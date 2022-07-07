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

simple Client
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
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
}

network Network
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
        server: Server {
            @display("i=device/server;p=350,150");
        }
        bRouter: Brouter {
            @display("i=old/srouter;p=350,250");
        }
        router[4]: Nrouter {
            @display("p=150,100,m,2,450,250");
            @display("i=abstract/router");
        }

        hostsarea0[5]: Client {
            @display("p=50,50,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea1[5]: Client {
            @display("p=50,150,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea2[5]: Client {
            @display("p=500,50,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea3[5]: Client {
            @display("p=500,150,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea4[5]: Client {
            @display("p=50,300,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea5[5]: Client {
            @display("p=50,400,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea6[5]: Client {
            @display("p=500,300,m,5,50,50");
            @display("i=device/pc2");
        }
        hostsarea7[5]: Client {
            @display("p=500,400,m,5,50,50");
            @display("i=device/pc2");
        }
    connections:
        router[0].gate_hosts++ <--> Channel <--> hostsarea0[0].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea0[1].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea0[2].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea0[3].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea0[4].gate++;

        router[0].gate_hosts++ <--> Channel <--> hostsarea1[0].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[1].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[2].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[3].gate++;
        router[0].gate_hosts++ <--> Channel <--> hostsarea1[4].gate++;

        router[1].gate_hosts++ <--> Channel <--> hostsarea2[0].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[1].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[2].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[3].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea2[4].gate++;

        router[1].gate_hosts++ <--> Channel <--> hostsarea3[0].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea3[1].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea3[2].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea3[3].gate++;
        router[1].gate_hosts++ <--> Channel <--> hostsarea3[4].gate++;

        router[2].gate_hosts++ <--> Channel <--> hostsarea4[0].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea4[1].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea4[2].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea4[3].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea4[4].gate++;

        router[2].gate_hosts++ <--> Channel <--> hostsarea5[0].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea5[1].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea5[2].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea5[3].gate++;
        router[2].gate_hosts++ <--> Channel <--> hostsarea5[4].gate++;

        router[3].gate_hosts++ <--> Channel <--> hostsarea6[0].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea6[1].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea6[2].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea6[3].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea6[4].gate++;

        router[3].gate_hosts++ <--> Channel <--> hostsarea7[0].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea7[1].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea7[2].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea7[3].gate++;
        router[3].gate_hosts++ <--> Channel <--> hostsarea7[4].gate++;


        server.gate++ <--> Channel {  delay = 20ms; } <--> bRouter.gate_servers++;

        bRouter.gate_net++ <--> Channel <--> router[0].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[1].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[2].gate_backbone++;
        bRouter.gate_net++ <--> Channel <--> router[3].gate_backbone++;

}
