/*
 * OneToManyProcessor.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: pedro
 */

#include "OneToManyProcessor.h"
#include "WebRtcConnection.h"

namespace erizo{
OneToManyProcessor::OneToManyProcessor() : MediaReceiver(){

	sendVideoBuffer_ = (char*)malloc(2000);
	sendAudioBuffer_ = (char*)malloc(2000);
	publisher = NULL;

}

OneToManyProcessor::~OneToManyProcessor(){

	if(sendVideoBuffer_)
		delete sendVideoBuffer_;
	if(sendAudioBuffer_)
		delete sendAudioBuffer_;
}

int OneToManyProcessor::receiveAudioData(char* buf, int len){

	if (subscribers.empty()||len<=0)
		return 0;

	std::map<int,WebRtcConnection*>::iterator it;
	for (it = subscribers.begin(); it!=subscribers.end(); it++) {
		memset(sendAudioBuffer_,0,len);
		memcpy(sendAudioBuffer_,buf, len);
		(*it).second->receiveAudioData(sendAudioBuffer_, len);
	}

	return 0;
}

int OneToManyProcessor::receiveVideoData(char* buf, int len){

	if (subscribers.empty()||len<=0)
		return 0;

	std::map<int,WebRtcConnection*>::iterator it;
	for (it = subscribers.begin(); it!=subscribers.end(); it++) {
		memset(sendVideoBuffer_,0,len);
		memcpy(sendVideoBuffer_,buf, len);
		(*it).second->receiveVideoData(sendVideoBuffer_, len);
	}
	return 0;
}

void OneToManyProcessor::setPublisher(WebRtcConnection* webRtcConn){

	this->publisher = webRtcConn;
}

void OneToManyProcessor::addSubscriber(WebRtcConnection* webRtcConn, int peerId){

	this->subscribers[peerId]=webRtcConn;
}

void OneToManyProcessor::removeSubscriber(int peerId){

	if (this->subscribers.find(peerId)!=subscribers.end()){
		this->subscribers[peerId]->close();
		this->subscribers.erase(peerId);
	}
}

}/* namespace erizo */



