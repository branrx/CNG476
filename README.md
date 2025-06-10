# Smart Courier Service
## Introduction
We propose a smart courier service for a small city, consisting of a central hub (head office) and
multiple sub-hubs connected by delivery routes. Packages can be dropped off at any node, where
they are scanned to acknowledge arrival. A delivery vehicle, tracked via GPS, then picks up
the package, delivers it to the next node, till it reaches the destination node. Since this project is
constrained to using FLORA, which has limited range, we propose a mesh topology of sub-stations.
These substations act as routing nodes for parcel delivery. Each parcel will have a QR code that
is scanned at every node to track its location. All information will be relayed to the central hub
via the mesh network, this includes number of parcels at each node, location of vehicles and more.
The central hub will aggregate this information and use it to optimize the network and delivery
routes, also it will be responsible for dispatching aide in the form of personnel for repairs or more
trucks and this all depends on the current state of the system.
In the below section we have tried to lament on a few stochastic processes we shall utilize in
order achieve the set goals and objectives. It is worth noting that we are not yet fully aware of
the capabilities of the target software (FLORA and OMNET++), therefore we might add more
deliverables not included in the initial proposal during the development of the proposed works.

##  How to use
NB: Ignore all folders in root directory except one called, "courier".

### Clone the repository
Using git clone --link-- to this repository.

###  Structure
The package is structured as follows:
courier:
        - Binaries
        -   Dependencies
        -   Includes
        -   simulations
            -   results
        -   src
        -   out

- network file .ned and intialisation files are located in simulations directory
- dependecies are stored in Dependencies folder

## HOW TO RUN
-  open omnet++, (project was developed with v6.0)
-  go to import existing project
-  select the courier directory and click okay.
-  now the tricky part,
-  from the Dependencies copy all files
-  paste the files in your flora-> src -> LoRa directory
-  clean project and build
-  now go to courier -> simulations directory
-  open omnetpp.init file
-  you can change variables from there
-  click run
-  once done, click finish
-  simulations logs are stored in the simulations directory, according to module id, e.g node_1_log.csv
        
