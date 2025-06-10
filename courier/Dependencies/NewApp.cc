
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
#include "NewApp.h"
#include "test.h"
#include "custom_random.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include "distribution_functions.h"

#include "inet/mobility/static/StationaryMobility.h"
#include "LoRa/LoRaTagInfo_m.h"
#include "inet/common/packet/Packet.h"
#include "truck.h"
#include <fstream>


using namespace std;

namespace flora {


Define_Module(NewApp);

void NewApp::initialize(int stage)
{
    if(stage == 0)
    {
        //  initialize node
       node = new Node();
       int id = par("node_id");
       int seed = par("node_seed");
       int parcelMean = par("parcelMean");
       node->init(id, seed, parcelMean);



       //   init rand object
       randObj = new custom_random(0);
       randObj->init(seed);

       //   init truck
       truck = Truck();
       setTruckAttributes();

       string buffer = "node_" + to_string(node->node_id) + "_log.csv";
       resultFile.open(buffer, std::ios::out);
       resultFile << "Time,Event,Details\n";  // Header
       resultFile << simTime() << ",Simulation starting....," << "----" << "\n";


       cout << "truck: " << truck.id << "capacity: " << truck.capacity <<endl;
    }

    //  default code
    cSimpleModule::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        std::pair<double,double> coordsValues = std::make_pair(-1, -1);
        cModule *host = getContainingNode(this);
        // Generate random location for nodes if circle deployment type
        if (strcmp(host->par("deploymentType").stringValue(), "circle")==0) {
           coordsValues = generateUniformCircleCoordinates(host->par("maxGatewayDistance").doubleValue(), host->par("gatewayX").doubleValue(), host->par("gatewayY").doubleValue());
           StationaryMobility *mobility = check_and_cast<StationaryMobility *>(host->getSubmodule("mobility"));
           mobility->par("initialX").setDoubleValue(coordsValues.first);
           mobility->par("initialY").setDoubleValue(coordsValues.second);
        }
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        bool isOperational;
        NodeStatus *nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;
        if (!isOperational)
            throw cRuntimeError("This module doesn't support starting in node DOWN state");
        do {
            timeToFirstPacket = par("timeToFirstPacket");
            EV << "Time to first packet :" << timeToFirstPacket << endl;
            //if(timeToNextPacket < 5) error("Time to next packet must be grater than 3");
        } while(timeToFirstPacket <= 5);

        timeToFirstPacket = par("timeToFirstPacket");
        //  set message to send
        //  create message
        //Test obj = Test();
        //char* mssg = (char*)malloc(sizeof(char)*100);
        //mssg = obj.getMessage();
        sendMeasurements = new cMessage("sendMeasurements");

        //  ------------ my code ------------------
        //  draw from u uniform
        double u = uniformGenerator();
        int firstParcel = (int)randObj->inverseExponential(4, 1700.0, pow(u, 0.05)); //  time to first packet
        cMessage *newParcel = new cMessage("p");
        simtime_t scheduleTime = simTime() + firstParcel;
        scheduleAt(scheduleTime, newParcel);



        //  schedule truck dispatch
        u = uniformGenerator();
        firstParcel = (int)randObj->inverseExponential(10.0, 10000.0, u); //  time to first packet
        newParcel = new cMessage("t");
        scheduleTime = simTime() + firstParcel;
        scheduleAt(scheduleTime, newParcel);


        //scheduleAt(simTime()+timeToFirstPacket, sendMeasurements);

        sentPackets = 0;
        receivedADRCommands = 0;
        numberOfPacketsToSend = par("numberOfPacketsToSend");

        LoRa_AppPacketSent = registerSignal("LoRa_AppPacketSent");

        //LoRa physical layer parameters
        loRaRadio = check_and_cast<LoRaRadio *>(getParentModule()->getSubmodule("LoRaNic")->getSubmodule("radio"));
        loRaRadio->loRaTP = par("initialLoRaTP").doubleValue();
//        setTP(par("initialLoRaTP").doubleValue());
//        loRaCF = units::values::Hz(par("initialLoRaCF").doubleValue());
        loRaRadio->loRaCF = units::values::Hz(par("initialLoRaCF").doubleValue());
//        loRaSF = par("initialLoRaSF");
        loRaRadio->loRaSF = par("initialLoRaSF");
//        loRaBW = inet::units::values::Hz(par("initialLoRaBW").doubleValue());
        loRaRadio->loRaBW = inet::units::values::Hz(par("initialLoRaBW").doubleValue());
//        loRaCR = par("initialLoRaCR");
        loRaRadio->loRaCR = par("initialLoRaCR");
//        loRaUseHeader = par("initialUseHeader");
        loRaRadio->loRaUseHeader = par("initialUseHeader");
        evaluateADRinNode = par("evaluateADRinNode");
        sfVector.setName("SF Vector");
        tpVector.setName("TP Vector");
    }
}

void NewApp::setTruckAttributes()
{
    double u;

    // generate values to fill fields
    truck.id = node->node_id;
    truck.capacity = randObj->randInt(20, 40);
    truck.location = node->node_id;
    truck.carrying = 0;
    truck.direction = 0;
    truck.eta = 0;
    truck.loadingTime = 0;
}

std::pair<double,double> NewApp::generateUniformCircleCoordinates(double radius, double gatewayX, double gatewayY)
{
    double randomValueRadius = uniform(0,(radius*radius));
    double randomTheta = uniform(0,2*M_PI);

    // generate coordinates for circle with origin at 0,0
    double x = sqrt(randomValueRadius) * cos(randomTheta);
    double y = sqrt(randomValueRadius) * sin(randomTheta);
    // Change coordinates based on coordinate system used in OMNeT, with origin at top left
    x = x + gatewayX;
    y = gatewayY - y;
    std::pair<double,double> coordValues = std::make_pair(x,y);
    return coordValues;
}

void NewApp::finish()
{
    string buffer = "node_" + to_string(node->node_id) + "_stats.csv";
    std::ofstream statsFile;
    statsFile.open(buffer, std::ios::out);
    statsFile << "Variable,Value\n";

    //  save statistics
    statsFile << simTime() << "," << "Total time truck idle," << truck.totalIdleTime<<"\n";
    statsFile << simTime() << "," << "Total time broken down," << truck.totalBrokenTime<<"\n";
    statsFile << simTime() << "," << "Time traveling between nodes," << truck.totalTravelTime<<"\n";
    statsFile << simTime() << "," << "Total broken down count," << truck.totalBrokenCount<<"\n";
    statsFile << simTime() << "," << "Time spent unloading," << truck.totalUnLoadingTime<<"\n";
    statsFile << simTime() << "," << "Parcels remaining at base," << node->parcelCount()<<"\n";
    statsFile << simTime() << "," << "Parcels still in truck," << truck.getParcelCount()<<"\n";
    statsFile << simTime() << "," << "Parcels delivered successfully," << node->deliveredTo<<"\n";
    statsFile << simTime() << "," << "Parcels failed to deliver," << node->notDeliveredTo<<"\n";

    resultFile << simTime() << ",Simulation has ended....," << "----" << "\n";

    //  close files
    statsFile.close();
    resultFile.close();

    cModule *host = getContainingNode(this);
    StationaryMobility *mobility = check_and_cast<StationaryMobility *>(host->getSubmodule("mobility"));
    Coord coord = mobility->getCurrentPosition();
    recordScalar("positionX", coord.x);
    recordScalar("positionY", coord.y);
    recordScalar("finalTP", getTP());
    recordScalar("finalSF", getSF());
    recordScalar("sentPackets", sentPackets);
    recordScalar("receivedADRCommands", receivedADRCommands);
}

cMessage *charToMessage(char *temp)
{
    return new cMessage(temp);
}

void initRoute(Truck truck, Node *nodeObj,custom_random *r)
{
    int i=0;
    int flag=1;
    int node;

    while(i<3)
    {
        while(flag==1)
        {
            flag=0;
            node = r->randInt(1, 4);
            if(node==nodeObj->node_id)
            {
                flag =1;
            }
            for(int x=0; x<truck.getRouteSize(); x++)
            {

                if(truck.getRouteNode(x)==node || node==nodeObj->node_id)
                {
                    flag=1;
                    break;
                }
            }
        }
        truck.addToRoute(node);

        i++;
        flag=1;
    }

}

void handleTruck(Truck truck, Node *node, custom_random *r)
{
    //  if truck at owner node
    if(truck.location == node->node_id)
    {
        //  dispatch truck
        initRoute(truck, node,r);
    }
}

void getRoute(Truck truck, Node *node)
{
    cout << "node " << node->node_id << " route: ";
    for(int i=0; i<truck.getRouteSize(); i++)
    {
        cout << truck.getRouteNode(i) << " - ";
    }
    cout << "" << endl;
}

void NewApp::unloadParcels(Truck truck, Node *node, custom_random *r)
{
    if(truck.getParcelCount()<1)
    {
        resultFile << simTime() << "," << "unloading" <<",Nothing to unload at node " << truck.direction<< "\n";
        return;
    }
    cout << "unloading truck: " << truck.id << " at node " << truck.direction << endl;
    int unloadCount=0;
    int countDelivered = 0;
    int countNotDelivered = 0;
    vector<int> *temp_p= new vector<int>();
    //  unload parcels
    for(int i=0; i<truck.getParcelCount(); i++)
    {
        Parcel p = truck.getParcel(i);
        //  if parcel address same as current node
        //  drop parcel
        if(p.destination == truck.direction)
        {
            bool isDelivered = r->biasedBinary(0.2);
            //  probability of parcel not delivered
            if(isDelivered)
            {
                node->addDeliveredTo();
                countDelivered++;
            }   else{
                countNotDelivered++;
            }

            unloadCount++;
            //  remove parcel from truck
            temp_p->push_back(i);
        }
    }
    truck.unloadParcels(temp_p);
    node->notDeliveredTo += countNotDelivered;
    node->deliveredTo += countDelivered;
    cout << countNotDelivered << "/" << countDelivered << ": not delivered!" << endl;
    resultFile << simTime() << "," << "delivery" << ",Delivered " << countDelivered << " of " << countNotDelivered+countDelivered<< "parcels\n";
    delete temp_p;
}

int NewApp::loadParcels(Truck truck, Node *node)
{
    if(node->parcelCount()<1)
    {
        resultFile << simTime() << "," << "loading" << ",Nothing to load. No parcels at node " << node->node_id << "\n";
        return 0;
    }

    //  load parcels
    for(int i=0; i<truck.capacity && node->parcelCount()>0; i++)
    {
        // add parcel to truck
        truck.loadParcel(node->popParcel());
    }
    resultFile << simTime() << "," << "loading" << ",Loaded " << truck.getParcelCount() << " parcels. Capacity " << truck.getParcelCount() << "/" << truck.capacity << "parcels\n";
    return 1;
}

void displayParcels(Truck truck)
{
    for(int i=0; i<truck.getParcelCount(); i++)
    {
        cout << truck.getParcel(i).destination << " ,";
    }
}

//  what happens when message is sent
void NewApp::handleMessage(cMessage *msg)
{
    char* mssg = (char*)malloc(sizeof(char)*100);
    mssg = (char*)"p";
    cMessage *cParcel = new cMessage(mssg); // node command,


    if (msg->isSelfMessage()) {
        if(strcmp(charToMessage((char*)"p")->getName(), msg->getName()) == 0)
        {
            //cout << "new parcels!!" << endl;
            //  generate new parcels arriving
            //  send new parcel count
            double u = uniformGenerator();
            int parcelsCount = node->parcelArrival(node->parcelMean, 8);
            int parcelCount = node->parcelCount();
            std::string str = std::to_string(parcelCount);

            string buffer = "parcels: " + to_string(parcelCount);
            //strcat(buffer, (char *) "parcels: ");
            if(!node->getInit())
            {
                node->setInit();
                lastServerUpdate = simTime();
                //cout << "init first update to server" << endl;
            }   else{

                if(simTime()>lastServerUpdate + (int)randObj->inverseExponential(4, 1700.0, pow(u, 0.05)))
                {
                    //cout << "sending update to server" << endl;
                    lastServerUpdate = simTime();
                    string buffer = "Node: " + to_string(node->node_id) + "," + to_string(parcelCount);
                    sendJoinRequest(buffer.c_str());
                }

            }

            //  log
            resultFile << simTime() << "," << "parcels arrived," << parcelsCount << " parcels have arrived." <<"\n";

            //cout << "node: " << node->node_id << "parcels: " << node->parcelCount() << endl;
            EV << "parcels: " << node->parcelCount() << endl;

            int nextParcel = (int)randObj->inverseExponential(4, 1700.0, pow(u, 0.05));
            EV << "nextParcel" << nextParcel << endl;
            cMessage *newParcel = new cMessage("p");
            simtime_t scheduleTime = simTime() + nextParcel;
            scheduleAt(scheduleTime, newParcel);
        }   else if(strcmp(charToMessage((char*)"t")->getName(), msg->getName()) == 0)
        {
            //  if truck finished repairing
            if(truck.isRepairing)
            {
                truck.setRepairing(false);
                resultFile << simTime() << "," << "truck finished repairing," << " heading to node " << truck.direction <<"\n";
            }
            //cout << "in t0" << endl;
            if(truck.status==0) // is init stage, pick up parcel and dispatch
            {
                //  init truck
                if(!truck.getInit())
                {
                    truck.setInit();
                }


                //  load parcels
                //cout <<"attempting load" << endl;
                int parcelAvailable = loadParcels(truck, node);


                if(parcelAvailable)
                {
                    handleTruck(truck, node, randObj);
                    getRoute(truck, node);
                    resultFile << simTime() << "," << "Route" << ",Generated route: " << truck.route->at(2) << "->" << truck.route->at(1) << "->"
                            << truck.route->at(0) << "\n";
                    truck.direction = truck.getNextNode();
                    truck.status = 1; // enroute
                    //  schedule truck arrival at next node
                    double u = uniformGenerator();
                    int nextParcel = (int)randObj->inverseExponential(4.0, 2700.0, pow(u, 0.1));
                    cMessage *newParcel = new cMessage("t");
                    simtime_t scheduleTime = simTime() + nextParcel;
                    scheduleAt(scheduleTime, newParcel);
                    resultFile << simTime() << "," << "Truck" << ",Dispatched, arrival at next node: " << scheduleTime <<"\n";
                    truck.totalTravelTime+= nextParcel;
                }   else
                {
                    double u = uniformGenerator();
                    int nextParcel = (int)randObj->inverseExponential(4.0, 1700.0, pow(u, 0.05));
                    cMessage *newParcel = new cMessage("t");
                    simtime_t scheduleTime = simTime() + nextParcel;
                    scheduleAt(scheduleTime, newParcel);
                    truck.totalIdleTime += nextParcel;
                }
            }   else if(truck.status==1)    //  has arrived at destination node
            {
                    bool brokenDown = !randObj->biasedBinary(0.5);
                    if(brokenDown)
                    {
                        double u = uniformGenerator();
                        int timeToRepair = (int)randObj->inverseExponential(2.0, 400.0, pow(u, 0.01)); // 0.01 is bias
                        cMessage *newParcel = new cMessage("t");
                        simtime_t scheduleTime = simTime() + timeToRepair;
                        scheduleAt(scheduleTime, newParcel);
                        truck.setRepairing(true);
                        truck.totalBrokenCount += 1;
                        truck.totalBrokenTime += (int)timeToRepair;
                        //cout << "truck " << truck.id << " has broken down. It will take approx " << timeToRepair << "to fix" << endl;
                        resultFile << simTime() << "," << "Truck broken down," << timeToRepair << " to repair." <<"\n";
                        truck.totalTravelTime+= timeToRepair;
                    }

                    if(truck.direction!=node->node_id &&!brokenDown)
                    {
                        //  if node not equal to owner node, unload and load parcels
                        //cout << "unloading truck " << truck.id << " at node " << truck.direction<<endl;
                        resultFile << simTime() << "," << "truck " << truck.id << ",Arrived at node " << truck.direction << ". Unloading" <<"\n";
                        truck.status = 2;

                        //  find packages with address
                        unloadParcels(truck, node, randObj);

                        double u = uniformGenerator();
                        int nextParcel = (int)randObj->inverseExponential(2.0, 500.0, pow(u, 0.05)); // time till finished unloading
                        cMessage *newParcel = new cMessage("t");
                        simtime_t scheduleTime = simTime() + nextParcel;
                        scheduleAt(scheduleTime, newParcel);
                        truck.totalUnLoadingTime += nextParcel;
                    }   else if(truck.direction==node->node_id && !brokenDown)
                    {
                        cout << "Truck: " << truck.id << " arrived at base. Scheduling dispatch..." << endl;
                        displayParcels(truck);
                        truck.status = 0;
                        //  schedule truck arrival at next node
                        double u = uniformGenerator();
                        int nextParcel = (int)randObj->inverseExponential(10.0, 4000.0, u);
                        cMessage *newParcel = new cMessage("t");
                        simtime_t scheduleTime = simTime() + nextParcel;
                        scheduleAt(scheduleTime, newParcel);
                        resultFile << simTime() << "," << "truck " << truck.id << ",Arrived at base. Next dispatch at " << scheduleTime <<"\n";
                    }   else if(truck.getRouteSize()==0 && !brokenDown)        //  no more node on routes, return to base
                    {
                        truck.direction = node->node_id;
                        truck.status = 1;
                        //  return home
                        double u = uniformGenerator();
                        int nextParcel = (int)randObj->inverseExponential(4.0, 2700.0, pow(u, 0.1));
                        cMessage *newParcel = new cMessage("t");
                        simtime_t scheduleTime = simTime() + nextParcel;
                        scheduleAt(scheduleTime, newParcel);
                        truck.totalTravelTime+= nextParcel;
                    }
                } else if(truck.status==2)
                {

                    //  was unloading and packing
                    //  dispatch truck to next node in route
                    //  continue on route
                    if(truck.getRouteSize()==0)
                    {
                        resultFile << simTime() << "," << "truck " << truck.id << ",Route finished. Returning to base." <<"\n";
                        truck.direction = node->node_id;
                        truck.status = 1;
                    }   else{
                        truck.direction = truck.getNextNode();
                        truck.status = 1;
                        resultFile << simTime() << "," << "truck " << truck.id << ",Finished loading. Proceeding to node " << truck.direction <<"\n";
                    }


                    double u = uniformGenerator();
                    int nextParcel = (int)randObj->inverseExponential(4.0, 2700.0, pow(u, 0.1));
                    cMessage *newParcel = new cMessage("t");
                    simtime_t scheduleTime = simTime() + nextParcel;
                    scheduleAt(scheduleTime, newParcel);
                    truck.totalTravelTime+= nextParcel;
                }


        }
        if (strcmp(cParcel->getName(), msg->getName()) == 0)
        {

            if (simTime() >= getSimulation()->getWarmupPeriod())
                sentPackets++;
            delete msg;
            if(numberOfPacketsToSend == 0 || sentPackets < numberOfPacketsToSend)
            {
                double time;
                int loRaSF = getSF();
                if(loRaSF == 7) time = 7.808;
                if(loRaSF == 8) time = 13.9776;
                if(loRaSF == 9) time = 24.6784;
                if(loRaSF == 10) time = 49.3568;
                if(loRaSF == 11) time = 85.6064;
                if(loRaSF == 12) time = 171.2128;
            }
        }
    }
    else
    {
        handleMessageFromLowerLayer(msg);
        delete msg;
        //cancelAndDelete(sendMeasurements);
        //sendMeasurements = new cMessage("sendMeasurements");
        //scheduleAt(simTime(), sendMeasurements);
    }
}

void NewApp::handleMessageFromLowerLayer(cMessage *msg)
{

//    LoRaAppPacket *packet = check_and_cast<LoRaAppPacket *>(msg);
    auto pkt = check_and_cast<Packet *>(msg);
    const auto & packet = pkt->peekAtFront<LoRaAppPacket>();
    if (simTime() >= getSimulation()->getWarmupPeriod())
        receivedADRCommands++;
    if(evaluateADRinNode)
    {
        ADR_ACK_CNT = 0;
        if(packet->getMsgType() == TXCONFIG)
        {
            if(packet->getOptions().getLoRaTP() != -1)
            {
                setTP(packet->getOptions().getLoRaTP());
            }
            if(packet->getOptions().getLoRaSF() != -1)
            {
                setSF(packet->getOptions().getLoRaSF());
            }

            EV << "New TP " << getTP() << endl;
            EV << "New SF " << getSF() << endl;
        }
    }
}

bool NewApp::handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();

    throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName());
    return true;
}

void NewApp::sendJoinRequest(const char * mssg)
{
    auto pktRequest = new Packet(mssg);

    pktRequest->setKind(DATA);

    auto payload = makeShared<LoRaAppPacket>();
    payload->setChunkLength(B(par("dataSize").intValue()));

    lastSentMeasurement = rand();
    payload->setSampleMeasurement(lastSentMeasurement);

    if(evaluateADRinNode && sendNextPacketWithADRACKReq)
    {
        auto opt = payload->getOptions();
        opt.setADRACKReq(true);
        payload->setOptions(opt);
        //request->getOptions().setADRACKReq(true);
        sendNextPacketWithADRACKReq = false;
    }


    auto loraTag = pktRequest->addTagIfAbsent<LoRaTag>();
    loraTag->setBandwidth(getBW());
    loraTag->setCenterFrequency(getCF());
    loraTag->setSpreadFactor(getSF());
    loraTag->setCodeRendundance(getCR());
    loraTag->setPower(mW(math::dBmW2mW(getTP())));

    //add LoRa control info
  /*  LoRaMacControlInfo *cInfo = new LoRaMacControlInfo();
    cInfo->setLoRaTP(loRaTP);
    cInfo->setLoRaCF(loRaCF);
    cInfo->setLoRaSF(loRaSF);
    cInfo->setLoRaBW(loRaBW);
    cInfo->setLoRaCR(loRaCR);
    pktRequest->setControlInfo(cInfo);*/

    sfVector.record(getSF());
    tpVector.record(getTP());
    EV << "Wysylam pakiet z TP: " << getTP() << endl;
    EV << "Wysylam pakiet z SF: " << getSF() << endl;
    pktRequest->insertAtBack(payload);
    send(pktRequest, "socketOut");
    if(evaluateADRinNode)
    {
        ADR_ACK_CNT++;
        if(ADR_ACK_CNT == ADR_ACK_LIMIT) sendNextPacketWithADRACKReq = true;
        if(ADR_ACK_CNT >= ADR_ACK_LIMIT + ADR_ACK_DELAY)
        {
            ADR_ACK_CNT = 0;
            increaseSFIfPossible();
        }
    }
    emit(LoRa_AppPacketSent, getSF());
}

void NewApp::increaseSFIfPossible()
{
//    if(loRaSF < 12) loRaSF++;
    if(getSF() < 12) setSF(getSF() + 1);
}

void NewApp::setSF(int SF) {
    loRaRadio->loRaSF = SF;
}

int NewApp::getSF() {
    return loRaRadio->loRaSF;
}

void NewApp::setTP(int TP) {
    loRaRadio->loRaTP = TP;
}

double NewApp::getTP() {
    return loRaRadio->loRaTP;
}

void NewApp::setCF(units::values::Hz CF) {
    loRaRadio->loRaCF = CF;
}

units::values::Hz NewApp::getCF() {
    return loRaRadio->loRaCF;
}

void NewApp::setBW(units::values::Hz BW) {
    loRaRadio->loRaBW = BW;
}

units::values::Hz NewApp::getBW() {
    return loRaRadio->loRaBW;
}

void NewApp::setCR(int CR) {
    loRaRadio->loRaCR = CR;
}

int NewApp::getCR() {
    return loRaRadio->loRaCR;
}

} //end namespace inet
