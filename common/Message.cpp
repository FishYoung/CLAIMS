/*
 * Message.cpp
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#include "Message.h"

THERON_DEFINE_REGISTERED_MESSAGE(BlockStatusRespond)
THERON_DEFINE_REGISTERED_MESSAGE(BlockStatusMessage)
THERON_DEFINE_REGISTERED_MESSAGE(StorageBudgetMessage)
THERON_DEFINE_REGISTERED_MESSAGE(RegisterStorageRespond)
THERON_DEFINE_REGISTERED_MESSAGE(HeartBeatMessage)
THERON_DEFINE_REGISTERED_MESSAGE(HeartBeatRespond)
THERON_DEFINE_REGISTERED_MESSAGE(MatcherMessage)
THERON_DEFINE_REGISTERED_MESSAGE(MatcherRespond)
THERON_DEFINE_REGISTERED_MESSAGE(Message256)
THERON_DEFINE_REGISTERED_MESSAGE(Message4K)
THERON_DEFINE_REGISTERED_MESSAGE(int)
THERON_DEFINE_REGISTERED_MESSAGE(unsigned long long int)
THERON_DEFINE_REGISTERED_MESSAGE(NodeRegisterMessage)
THERON_DEFINE_REGISTERED_MESSAGE(PartitionBindingMessage)
THERON_DEFINE_REGISTERED_MESSAGE(ExchangeID)
THERON_DEFINE_REGISTERED_MESSAGE(PartitionUnbindingMessage)

void PhysicalQueryPlan::run()
{
	block_stream_iterator_root_->Open();
	while(block_stream_iterator_root_->Next(0));
	block_stream_iterator_root_->Close();

}
