/*
 * NewApp.h
 *
 *  Created on: May 22, 2025
 *      Author: branrx
 */

#ifndef OMNET_NEWAPP_H_
#define OMNET_NEWAPP_H_


#include <omnetpp.h>
#include "././inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/LifecycleOperation.h"

#include "LoRaApp/LoRaAppPacket_m.h"
#include "LoRa/LoRaMacControlInfo_m.h"
#include "LoRa/LoRaRadio.h"
#include "test.h"
#include "distribution_functions.h"
#include "node.h"
#include "truck.h"
#include "custom_random.h"

using namespace omnetpp;
using namespace inet;
using namespace std;
#include <fstream>


namespace flora {


class NewApp : public cSimpleModule, public ILifecycle
{
    protected:
        void initialize(int stage) override;
        void finish() override;
        int numInitStages() const override { return NUM_INIT_STAGES; }
        void handleMessage(cMessage *msg) override;
        virtual bool handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback) override;

        void handleMessageFromLowerLayer(cMessage *msg);
        std::pair<double,double> generateUniformCircleCoordinates(double radius, double gatewayX, double gatewayY);
        void sendJoinRequest(const char *);
        void sendDownMgmtPacket();

        int numberOfPacketsToSend;
        int sentPackets;
        int receivedADRCommands;
        int lastSentMeasurement;
        simtime_t timeToFirstPacket;
        simtime_t timeToNextPacket;

        cMessage *configureLoRaParameters;
        cMessage *sendMeasurements;

        //history of sent packets;
        cOutVector sfVector;
        cOutVector tpVector;

        //LoRa parameters control
        LoRaRadio *loRaRadio;

        void setSF(int SF);
        int getSF();
        void setTP(int TP);
        double getTP();
        void setCR(int CR);
        int getCR();
        void setCF(units::values::Hz CF);
        units::values::Hz getCF();
        void setBW(units::values::Hz BW);
        units::values::Hz getBW();


        //variables to control ADR
        bool evaluateADRinNode;
        int ADR_ACK_CNT = 0;
        int ADR_ACK_LIMIT = 64; //64;
        int ADR_ACK_DELAY = 32; //32;
        bool sendNextPacketWithADRACKReq = false;
        void increaseSFIfPossible();

        //own
        void setTruckAttributes();

        Node *node = new Node();

    public:
        Test testObj;
        simtime_t lastServerUpdate;
        Truck truck;
        int loadParcels(Truck, Node *);
        void unloadParcels(Truck , Node *, custom_random *);
        std::ofstream resultFile;
        custom_random *randObj;
        NewApp() {}
        simsignal_t LoRa_AppPacketSent;

};
Register_Class(NewApp);
}


#endif /* NEWAPP_H_ */
