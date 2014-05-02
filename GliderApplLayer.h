/* -*- mode:c++ -*- ********************************************************
 * file:        BurstApplLayer.h
 *
 * author:      Daniel Willkomm
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: application layer like the TestApplLayer but sends burst of
 *               messages
 **************************************************************************/

#ifndef GLIDER_APPL_LAYER_H
#define GLIDER_APPL_LAYER_H

#include "MiXiMDefs.h"
#include <BaseApplLayer.h>

#include "Transmission.h"

class MIXIM_API GliderApplLayer : public BaseApplLayer
{
    private:
        std::ifstream txStream;
        Transmission nextLoggedTransmission;

        void evalLog(std::string line);
        void updateTransmission(void);

    public:
        GliderApplLayer() :
                BaseApplLayer(), burstSize(3), bSendReply(false)
        {
        }

        virtual ~GliderApplLayer()
        {
        }

        /** @brief Initialize module parameters*/
        virtual void initialize(int);

        /** @brief Message kinds used by this layer.*/
        enum TestApplMessageKinds {
            SEND_BROADCAST_TIMER = LAST_BASE_APPL_MESSAGE_KIND,
            BROADCAST_MESSAGE,
            BROADCAST_REPLY_MESSAGE,
            LAST_TEST_APPL_MESSAGE_KIND
        };

    protected:
        /** @brief Timer message for scheduling next message.*/
        cMessage *txTimer;

        /** @brief Handle self messages such as timer... */
        virtual void handleSelfMsg(cMessage*);

        /** @brief Handle messages from lower layer */
        virtual void handleLowerMsg(cMessage*);
        /** @brief Number of messages to send in a burst*/
        int burstSize;
        /** @brief If true, send a unicast BROADCAST_REPLY message to each
         * received BROADCAST message. */
        bool bSendReply;

        /** @brief send a broadcast packet to all connected neighbors */
        void sendBroadcast(const char* name);
};

#endif

