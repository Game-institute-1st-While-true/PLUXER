#pragma once
#define WIN32_LEAN_AND_MEAN
#include "../GzzNet/NetDefine.h"
#include "../GzzNet/PackageQueue.h"
#include "../GzzNet/RpcTable.h"
#include "../GzzNet/RpcMap.h"
#include "../GzzNet/Note.h"
#include "../GzzNet/GzzIOCP_EX.h"

using PACKET = gzz::PACKET;
using PACKAGE = gzz::PACKAGE;
using NetLogger = gzz::Note;
using RpcTable = gzz::RpcTable<void, PACKAGE&>;
using RpcMap = gzz::RpcMap<void, PACKAGE&>;
using IOCP = gzz::GzzIOCP_EX;
using SYSTEM_PROTOCOL = gzz::SYSTEM_PROTOCOL;

template<class C>
using CLASS_MEM_PACKAGE_FUNC = void(C::*)(PACKAGE&);

static int PACKET_HEADER_SIZE = gzz::PACKET_HEADER_SIZE;