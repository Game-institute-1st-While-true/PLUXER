#pragma once

#include "ISocketEX.h"
#include "IReceive.h"
#include "ISend.h"

namespace gzz
{
	class SocketInterfaceHelper
	{
	public:
		SocketInterfaceHelper() = default;
		virtual ~SocketInterfaceHelper() = default;
	
	public:
		virtual ISend* GetInterfaceSend() { return nullptr; }
		virtual IReceive* GetInterfaceRecv() { return nullptr; };
		virtual ISocketEX* GetInterfaceSock() { return nullptr; }
	};
}