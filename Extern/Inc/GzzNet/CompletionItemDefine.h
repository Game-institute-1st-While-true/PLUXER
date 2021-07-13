#pragma once
#include "NetDefine.h"
#include "ICompletionItem.h"
#include <Windows.h>

// 시스템 패킷은 Net Form에서 Logic쪽에 전달하는 Message이다. 
// 보통 서버의 시작과 끝, 그리고 어떤 소켓 ID가 어떤 행동을 했는지 (접속, 종료, 연결)을 지정한다.
// 그러므로 패킷 구조는 헤더, ID로 나뉜다.
// 가변길이 패킷으로 전달하지만 패킷의 사이즈는 항상 PACKETHEADERSIZE + SOCK_ID_SIZE 이다.
// [--------- HEADER ---------][--- SOCKID ---]

namespace gzz
{
	constexpr int SYSTEM_PACKET_SIZE = PACKET_HEADER_SIZE + 2;
	constexpr int MAX_CONNECT_TRY_COUNT = 3;

#pragma pack(push, 1)
	struct PACKAGE
	{
		PACKAGE() = default;
		PACKAGE(int id, PACKET pack) { this->id = id; packet = pack; }
		PACKET packet;
		int id;
	};
#pragma pack(pop)

	enum class SYSTEM_PROTOCOL : unsigned char
	{
		NONE = 0,

		ACCEPT,
		CONNECT,
		CONNECT_TRY,
		CONNECT_FAIL,
		DISCONNECT_ACCEPT,
		DISCONNECT_CONNECT,
		JOB,

		END,

		MAX = 0xff
	};
}