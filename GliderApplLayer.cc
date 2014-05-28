#include <fstream>
#include <iostream>
#include <string>

#include <NetwToMacControlInfo.h>
#include <MacToNetwControlInfo.h>
#include <FWMath.h>

#include "GliderApplLayer.h"

#include "PgNetwToMacControlInfo.h"
#include "SimpleAddress.h"
#include "ApplPkt_m.h"
#include "FindModule.h"
#include "GliderMobility.h"

//#include <MACAddress.h>

using std::endl;

Define_Module(GliderApplLayer);

void GliderApplLayer::evalLog(std::string line)
{
	std::stringstream lineStream(line);
	std::string word;
	int i = 0;        //todo : auto find field position in stream using the index line
	while (getline(lineStream, word, ','))
	{
		switch (i)
		{
		case 0:
			nextLoggedTransmission.timeStamp = ((double) atol(word.c_str())) / 1000.0;
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

	if (!txStream.good())
	{
		EV << "node[" << getParentModule()->getIndex() << "] tx " << "EOF\n";
		return;
	}

	getline(txStream, line);

	if (line.length() < 1)
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

	if (stage == 0)
	{
		if (hasPar("burstSize"))
			burstSize = par("burstSize");
		else
			burstSize = 1;
		if (hasPar("burstReply"))
			bSendReply = par("burstReply");
		else
			bSendReply = true;

		EV << "GliderApplLayer of node[" << getParentModule()->getIndex() << "]: burstSize=" << burstSize << " bSendReply=" << bSendReply << endl;

		txTimer = new cMessage("delay-timer", SEND_BROADCAST_TIMER);

		const char *fileTx = par("txFile");

		txStream.open(fileTx);
		if (!txStream.is_open())
		{
			EV << "Tx file " << fileTx << " for glider[" << getParentModule()->getIndex() << "] not found\n";
			endSimulation();
		}

		getline(txStream, line);       //skip header

		//rx stuff
		fillRxDatabase(par("rxFile"));

		//results
		EV << "Open for write: " << (const char *) par("resultRssiErrFile") << endl;
 		rssiErr.open((const char *)par("resultRssiErrFile"));
		rssiErr << "rssiErr" << endl;

		EV << "Open for write: " << (const char *) par("results") << endl;
 		results.open((const char *)par("results"));
		results << "Right,Hight,Altitude,Bearing,Rssi,PaketID" << endl;

		EV << "Open for write: " << (const char *) par("resultsTx") << endl;
 		resultsTx.open((const char *)par("resultsTx"));
		resultsTx << "Right,Hight,Altitude,Bearing,Powerlevel,PaketID" << endl;


	}
	else if (stage == 1)
	{
		getline(txStream, line);       //skip another line, due to mobility issues
		updateTransmission();
	}

}

void GliderApplLayer::handleSelfMsg(cMessage *msg)
{
	switch (msg->getKind())
	{
	case SEND_BROADCAST_TIMER:
		//todo set pwr etc...
		for (int i = 0; i < burstSize; i++)
		{
			//change tx power

			//send pkt
			sendBroadcast(nextLoggedTransmission.packetId.c_str(), FWMath::dBm2mW(nextLoggedTransmission.pwrLevel));

			GliderMobility* mobi = FindModule<GliderMobility*>::findSubModule(getParentModule());
			ASSERT(mobi);

			Coord pos = mobi->getCurrentPosition();

			//EV<< "@@@@@@ pos: " << pos.x << " "<< pos.y << " "<< pos.z << " " << endl;

			//Right,Hight,Altitude,Bearing,Powerlevel,PaketID
			resultsTx << pos.x << "," << pos.y << "," << pos.z << "," << mobi->getDirection() << "," << nextLoggedTransmission.pwrLevel << "," <<  nextLoggedTransmission.packetId.c_str() << endl;

		}
		updateTransmission();

		break;
	default:
		EV << "node[" << getParentModule()->getIndex() << "] Unkown selfmessage! -> delete, kind: " << msg->getKind() << endl;
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
void GliderApplLayer::handleLowerMsg(cMessage* msg)
{
	if (!bSendReply && msg->getKind() == BROADCAST_MESSAGE)
	{
		ApplPkt *m = static_cast<ApplPkt *>(msg);
		EV << "node[" << getParentModule()->getIndex() << "] Received a broadcast packet from host[" << m->getSrcAddr() << "]: " << m->getName()
					<< " -> delete message, no reply" << endl;

		//not working
		double rssi = (static_cast<MacToNetwControlInfo*>(m->getControlInfo()))->getRSSI();
		EV << "node[" << getParentModule()->getIndex() << "] RSSI: " << FWMath::mW2dBm(rssi) << "dBm " << rssi << "mW" << endl;

		if (rxDb.find(m->getName()) == rxDb.end())
		{
			// not found
			EV << "!!!!!!!!!!!! Received packet was not found in real measurement records !!!!!!!!!!"  << endl;
		}
		else
		{
			// found
			double realMeasuredRssi = rxDb[m->getName()];
			rxDb.erase(m->getName());							//remove datum from db
			EV << "Real RSSI: " << realMeasuredRssi << "size: " << rxDb.size() << endl;

			double err = realMeasuredRssi - FWMath::mW2dBm(rssi);
			rssiErr << err << endl;
		}

		//EVAL

		GliderMobility* mobi = FindModule<GliderMobility*>::findSubModule(getParentModule());
		ASSERT(mobi);

		Coord pos = mobi->getCurrentPosition();

		//EV<< "@@@@@@ pos: " << pos.x << " "<< pos.y << " "<< pos.z << " " << endl;

		//Right,Hight,Altitude,Bearing,Rssi,PaketID
		results << pos.x << "," << pos.y << "," << pos.z << "," << mobi->getDirection() << "," << FWMath::mW2dBm(rssi) << "," <<  m->getName() << endl;

		delete msg;
		return;
	}
	else
		BaseApplLayer::handleLowerMsg(msg);
}

void GliderApplLayer::sendBroadcast(const char* name, double txPower_mW)
{
	ApplPkt *pkt = new ApplPkt(name, BROADCAST_MESSAGE);
	pkt->setDestAddr(LAddress::L3BROADCAST);
	// we use the host modules getIndex() as a appl address
	pkt->setSrcAddr(getParentModule()->getIndex());
	pkt->setBitLength(headerLength);

	// set the control info to tell the network layer the layer 3
	// address;
	PgNetwToMacControlInfo* ntw = new PgNetwToMacControlInfo();
	ntw->setDest(LAddress::L2BROADCAST);
	ntw->setTxPower(txPower_mW);
	pkt->setControlInfo(ntw);

	EV << "Sending broadcast packet!" << endl;
	sendDown(pkt);
}

void GliderApplLayer::fillRxDatabase(const char *fileRx)
{
	std::ifstream rxStream;
	rxStream.open(fileRx);
	if (!rxStream.is_open())
	{
		EV << "Rx file " << fileRx << " for glider[" << getParentModule()->getIndex() << "] not found\n";
		endSimulation();
	}

	while (rxStream.good())
	{
		std::string line;
		getline(rxStream, line);
		std::stringstream lineStream(line);
		std::string word;

		double rssi;
		std::string packetId;

		int i = 0;  						      //todo : auto find field position in stream using the index line
		while (getline(lineStream, word, ','))
		{
			switch (i)
			{
			case 12:
				rssi = atof(word.c_str());
				break;
			case 14:
				packetId = word;
				break;
			}
			i++;
		}

		//EV << "found: " << packetId << " @ " << rssi << endl;
		rxDb[packetId] = rssi;
	}

	rxStream.close();

}

void GliderApplLayer::finish(void)
{
	EV << "node[" << getParentModule()->getIndex() << "] App Layer finish" << endl;
	EV << "Unused rssi measurements: " << rxDb.size() << endl;
	cancelAndDelete(txTimer);
	rssiErr.close();
	results.close();
	resultsTx.close();
}
