#pragma once

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <tchar.h>
#include <stdint.h>
#include "NetErrCode.h"

/// <summary>
/// 패킷 기본 구조를 저장하는 클래스.
/// </summary>

namespace gzz
{
	constexpr int MAX_SEND_BUF_SIZE = 8192;
	constexpr int MAX_RECV_BUF_SIZE = 8192;
	constexpr int MAX_PACKET_SIZE = 1024;
	constexpr int PACKET_HEADER_SIZE = 4;
	constexpr int MAX_CONNECTION_TRY_CNT = 3;

	/// <summary>
	/// 시간 관련 Default 
	/// </summary>

	constexpr int DEFAULT_KEEPALIVED_TIME = 1000; // keepallived time = 1 second
	constexpr int DEFAULT_KEEPALIVED_INTERVAL_TIME = 1000; // keepallived time = 1 second

#pragma pack(push, 1)
	struct PACKET_HEADER
	{
		unsigned short size;
		unsigned short protocol;
	};
#pragma pack(pop)

	union PACKET
	{
		PACKET_HEADER head;
		char memory[MAX_PACKET_SIZE];

		static void Make(PACKET& packet, PACKET_HEADER head, char* data, const int data_size)
		{
			packet.head = head;
			packet.head.size += data_size;
			memcpy_s(packet.memory + PACKET_HEADER_SIZE, MAX_PACKET_SIZE - PACKET_HEADER_SIZE, data, data_size);
		}

		static void Make(PACKET& packet, PACKET_HEADER head, const uint8_t* data, const int data_size)
		{
			packet.head = head;
			packet.head.size += data_size;
			memcpy_s(packet.memory + PACKET_HEADER_SIZE, MAX_PACKET_SIZE - PACKET_HEADER_SIZE, data, data_size);
		}
	};
}