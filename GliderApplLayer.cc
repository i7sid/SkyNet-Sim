#include <fstream>
#include <iostream>
#include <string>


#include "GliderApplLayer.h"

#include "NetwToMacControlInfo.h"
#include "SimpleAddress.h"
#include "ApplPkt_m.h"
#include <NetwToMacControlInfo.h>
//#include <MACAddress.h>

using std::endl;

Define_Module(GliderApplLayer);

void GliderApplLayer::evalLog(std::string line)
{
    std::stringstream lineStream(line);
    std::string word;
    int i=0;        //todo : auto find field position in stream using the index line
    while(getline(lineStream, word, ','))
    {
        switch(i)
        {
        case 0:
            nextLoggedTransmission.timeStamp = ((double)atol(word.c_str())) / 1000.0;
            break;
        case 8:
            nextLoggedTransmission.antenna = word;
            break;
        case 12:
            nextLoggedTransmission.pwrLevel = atoi(word.c_str());
            break;
        case 14:
            nextLoggedTransmission.packetId = word;
            break;
        }
        i++;
    }
}

void GliderApplLayer::updateTransmission(void)
{
    std::string line;

    if(!txStream.good())
    {
        EV << "node[" << getParentModule()->getIndex() << "] tx " << "EOF\n";
        return;
    }


    getline(txStream, line);

    if(line.length() < 1)
    {
        EV << "node[" << getParentModule()->getIndex() << "] tx " << "string too short\n";
        return;
    }

    evalLog(line);

    scheduleAt(nextLoggedTransmission.timeStamp, txTimer);
    EV << "node[" << getParentModule()->getIndex() << "] tx timer: " << nextLoggedTransmission.timeStamp;
}

// do some initialization
void GliderApplLayer::initialize(int stage)
{
    std::string line;

    EV << "Initializing GliderApplLayer stage " << stage << "of node[" << getParentModule()->getIndex() << "]" << endl;


    BaseApplLayer::initialize(stage);

    if(stage==0)
    {
        if(hasPar("burstSize"))
            burstSize = par("burstSize");
        else
            burstSize = 3;
        if(hasPar("burstReply"))
            bSendReply = par("burstReply");
        else
            bSendReply = true;

        EV << "GliderApplLayer of node[" << getParentModule()->getIndex() << "]: burstSize=" << burstSize << " bSendReply=" << bSendReply << endl;

        txTimer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER);

        const char *file = par("txFile");

        txStream.open(file);
        if(!txStream.is_open())
        {
            EV << "Tx file " << file << " for glider[" << getParentModule()->getIndex() << "] not found\n";
            endSimulation();
        }

        getline(txStream, line);       //skip header
    }
    else if(stage==1)
    {
        getline(txStream, line);       //skip another line, due to mobility issues
        updateTransmission();
    }

}

void GliderApplLayer::handleSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case SEND_BROADCAST_TIMER:
        //todo set pwr etc...
        for(int i=0; i<burstSize; i++) {
            sendBroadcast(nextLoggedTransmission.packetId.c_str());
        }
        updateTransmission();

        break;
    default:
        EV <<" Unkown selfmessage! -> delete, kind: "<<msg->getKind()<<endl;
        break;
    }
}

/**
 * There are two kinds of messages that can arrive at this module: The
 * first (kind = BROADCAST_MESSAGE) is a broadcast packet from a
 * neighbor node to which we have to send a reply (if bSendReply is true). The second (kind =
 * BROADCAST_REPLY_MESSAGE) is a reply to a broadcast packet that we
 * have send and just causes some output before it is deleted
 */
void GliderApplLayer::handleLowerMsg( cMessage* msg )
{
    if ( !bSendReply && msg->getKind() == BROADCAST_MESSAGE) {
        ApplPkt *m  = static_cast<ApplPkt *>(msg);
        EV << "Received a broadcast packet from host["<<m->getSrcAddr()<<"]: " << m->getName() << " -> delete message, no reply" << endl;
        //EV << m-> << endl;
        delete msg;
        return;
    }
    BaseApplLayer::handleLowerMsg(msg);
}

void GliderApplLayer::sendBroadcast(const char* name)
{
    ApplPkt *pkt = new ApplPkt(name, BROADCAST_MESSAGE);
    pkt->setDestAddr(LAddress::L3BROADCAST);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( getParentModule()->getIndex() );
    pkt->setBitLength(headerLength);

    // set the control info to tell the network layer the layer 3
    // address;
    NetwToMacControlInfo* ntw = new NetwToMacControlInfo();
    ntw->setDest(LAddress::L2BROADCAST);
    pkt->setControlInfo(ntw);

    EV << "Sending broadcast packet!" << endl;
    sendDown( pkt );
}

