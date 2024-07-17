#pragma once

class NetworkObject
{
	virtual void RunObject() abstract;
	virtual void SendPacket(int8* _data, int32 _length) abstract;
	virtual void OnSendPacket(int32 _length) abstract;
};