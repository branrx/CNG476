package courier.simulations;

@license(LGPL);

import flora.LoRaPhy.LoRaMedium;
import flora.LoraNode.LoRaGW;
import flora.LoraNode.LoRaNode;
import inet.examples.inet.ipv4hook.MyRouter;
import inet.networklayer.configurator.ipv4.Ipv4FlatNetworkConfigurator;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth1G;

@license(LGPL);

network package_delivery
{
    @display("bgb=1044,648");
    submodules:
        loRaGW: LoRaGW {
            @display("p=546,297");
        }
        loRaMedium: LoRaMedium {
            @display("p=977,37");
        }
        myRouter: MyRouter {
            @display("p=480,268");
        }
        server: StandardHost {
            @display("p=546,212");
        }
        networkConfigurator: Ipv4FlatNetworkConfigurator {
            @display("p=848,34");
        }
        node1: LoRaNode {
            @display("p=537,519;is=l");
        }
        node2: LoRaNode {
            @display("p=839,322;is=l");
        }
        node3: LoRaNode {
            @display("p=222,206;is=l");
        }
        node4: LoRaNode {
            @display("p=772,182;is=l");
        }
    connections:
        server.ethg++ <--> Eth1G <--> myRouter.ethg++;
        loRaGW.ethg++ <--> Eth1G <--> myRouter.ethg++;
}
