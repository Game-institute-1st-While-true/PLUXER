#include "GroupCallBackcomponent.h"
#include "Group.h"	

using namespace hos;

hos::com::GroupCallBackcomponent::GroupCallBackcomponent(Group* group)
	:Component(L"GroupCallBackcomponent"), GroupCallBack(group), SendTime(0), CheckTime(0)
{
	ExecutionOrder = -1;
}

hos::com::GroupCallBackcomponent::~GroupCallBackcomponent()
{
}

void hos::com::GroupCallBackcomponent::OnEnable()
{
}

void hos::com::GroupCallBackcomponent::OnDisable()
{
}

void hos::com::GroupCallBackcomponent::Reset()
{
}

void hos::com::GroupCallBackcomponent::LateUpdate()
{
	if (SendTime < 0) return;

	CheckTime += m_GameObject->m_Scene->GetTimer()->DeltaTime();

	if (CheckTime >= SendTime)
	{
		OnUpateFunc();
		CheckTime = 0.f;
	}
}

void hos::com::GroupCallBackcomponent::Bind(GameObject* gameObject)
{
	if (m_GameObject)
	{
		throw std::invalid_argument("This Component is bound");
	}

	if (gameObject == nullptr)
	{
		throw std::invalid_argument("NullObject try to bind Component");
	}

	if (gameObject->network)
	{
		throw std::invalid_argument("this Object has Group");
	}
	m_GameObject = gameObject;
}

const std::vector<U8> hos::com::GroupCallBackcomponent::Serialize()
{
	string t = GetName();
	mbstring name(t.begin(), t.end());

	flexbuffers::Builder builder;
	builder.Map([&]
		{
			builder.String("Name", name.c_str());
			builder.Float("CheckTime", CheckTime);
		});
	builder.Finish();
	return builder.GetBuffer();
}

bool hos::com::GroupCallBackcomponent::Deserialize(mbstring_view data)
{
	if (data.size() <= 0)
	{
		return false;
	}
	auto m = flexbuffers::GetRoot(reinterpret_cast<const uint8_t*>(data.data()), data.size()).AsMap();
	mbstring sn = m["Name"].AsString().str();
	string name(sn.begin(), sn.end());

	if (name != GetName())
	{
		std::invalid_argument("NavAgent try Deserialize to" + sn);
		return false;
	}
	CheckTime = m["CheckTime"].AsFloat();
	return true;
}
