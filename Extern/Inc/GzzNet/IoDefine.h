#pragma once
#include <functional>
#include "NetDefine.h"

#include "TcpHostEX.h"
#include "TcpGuestEX.h"

#include "TcpClient.h"
#include "TcpClientEX.h"

#include "ICompletionItem.h"
#include "CompletionItemDefine.h"

#include "ErrCode.h"

#include "RecvItem.h"
#include "SendItem.h"
#include "SessionItem.h"

namespace gzz
{
	// CONNECT 소켓과 매핑하기위한 Key DATA. 
	constexpr int CONNECT = -77;

	// Wait time = 120 초
	constexpr int DEFAULT_WAIT_MILLITIME = 1200000;

	// Connection time = 5분, 연결이 되었으나 따로 패킷이 5분간 발송되지 않았으면 세션 종료. 
	constexpr int DEFAULT_CONNECTION_TIME = 300;

	// Accept time = 1분, 연결이 되었으나 따로 패킷이 1분간 발생되지 않았으면 세션 종료. 이 간격이 좀 더 짧을 수 도 있다.
	//				 DDOS의 경우, 초단위로 커넥션을 잡아버리기 때문에.
	constexpr int DEFAULT_ACCEPT_TIME = 60;
	// 
	constexpr int CHECKER_CONNECT_CHECK_MILLITIME = 1000;



}