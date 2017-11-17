//TODO: TEST THIS MESSAGE CLASS's OPERATORS ERROR PRONE

#include "Message.h"

const int OFFSET_TIME_MILLIS = 10;

Message::Message(int post, Message_Type type, GameEntity * send, GameEntity * rec){
	postTime = post;
	messageType = type;
	sender = send;
	receiver = rec;
}

bool Message::operator==(Message m)const{

	int a = postTime - m.postTime;
	if(a < 0)a=-a;
	if(a<=OFFSET_TIME_MILLIS && (m.messageType == messageType)) return true;
	return false;
}

bool Message::operator>(Message m)const{
	//earlier time is greater time?
	return (!operator==(m) && postTime > m.postTime);
}


bool Message::operator<(Message m)const{
	return (!operator==(m) && postTime < m.postTime);
}


bool Message::operator>=(Message m)const{

	if( operator==(m) ) return true;
	
	if( postTime - m.postTime > 0)
		return true;
	
	return false;
}


bool Message::operator<=(Message m)const{

	if( operator==(m) ) return true;
	
	if( postTime - m.postTime < 0)
		return true;
	
	return false;

}