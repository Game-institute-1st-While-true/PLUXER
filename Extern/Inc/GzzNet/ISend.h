#pragma once
#include "NetDefine.h"

/// <summary>
/// [2020-11-02 김효곤]
///	데이터를 보낼 때 사용할 인터페이스.
/// Send와 완료처리를 담당.
/// </summary>

namespace gzz
{
	class ISend abstract
	{
	public:
		ISend() = default;
		virtual ~ISend() = default;

	public:
		virtual int SendMsg(const SOCKET& sock, const LPOVERLAPPED& overraped, PACKET& packet) abstract;
		//virtual int RemainingMsg(const SOCKET& sock, const LPOVERLAPPED& overraped) abstract;
		virtual int SendProcess(const int completionBytes) abstract;
		virtual void Release() abstract;
	};
}