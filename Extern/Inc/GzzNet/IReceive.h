#pragma once
#include "NetDefine.h"

/// <summary>
/// [2020-11-02 김효곤]
///	데이터를 받을 때 사용 할 인터페이스
/// Receive와 완료처리를 담당.
/// </summary>

namespace gzz
{
	class IReceive abstract
	{
	public:
		IReceive() = default;
		virtual ~IReceive() = default;

	public:
		virtual int Receive(const SOCKET& sock, const LPOVERLAPPED& overraped) abstract;
		virtual PACKET GetPacket(const int completionBytes, const int recvBytes) abstract;
		virtual char* GetBuf() abstract;
		virtual void Release() abstract;
	};
}