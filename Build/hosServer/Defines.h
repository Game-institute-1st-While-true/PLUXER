#pragma once
#include "hosEngine/NetDefine.h"
#include "hosEngine/NetworkManager.h"
#include "GzzNet/BehaviorTree.h"
#include "GzzNet/Monitoring.h"
#include "hosEngine/Component.h"	
#include "hosEngine/Scene.h"
#include "hosEngine/NetworkToken.h"
#include "hosEngine/GameObject.h"

//Components
#include "hosEngine/Transform.h"
#include "hosEngine/BoxCollision.h"
#include "hosEngine/CapsuleCollision.h"
#include "hosEngine/MeshCollision.h"
#include "hosEngine/SphereCollision.h"
#include "hosEngine/Rigidbody.h"
#include "hosEngine/Camera.h"
#include "hosEngine/Light.h"
#include "hosEngine/AudioListener.h"
#include "hosEngine/AudioSource.h"
#include "hosEngine/Script.h"
#include "hosEngine/MeshFilter.h"
#include "hosEngine/MeshRenderer.h"
#include "hosEngine/SkinnedMeshRenderer.h"
#include "hosEngine/NavAgent.h"
#include "hosEngine/Networkcomponent.h"
#include "hosEngine/Animation.h"
#include "ScriptFactory.h"

#define assert_msg(exp, msg) assert(((void)msg, exp))

constexpr int MAX_GROUP_COUNT = 1;
constexpr int MAX_GROUP_USER_COUNT = 3;
constexpr int MAX_SESSION_COUNT = 1000;
constexpr int SERVER_THREAD_COUNT = 10;
constexpr int LISTEN_PORT = 9000;
constexpr short DBA_PORT = 9123;

using BehaviorTree = gzz::BehaviorTree;

using BehaviorNode = gzz::BehaviorNode;
using BehaviorTask = gzz::BehaviorNode;

using SelectNode = gzz::SelectNode;
using SequenceNode = gzz::SequenceNode;

using DecoratorNode = gzz::DecoratorNode;
using ServiceNode = gzz::ServiceNode;



