#ifndef _AUDIO_LISTENER_H
#define _AUDIO_LISTENER_H

// [2020/12/10 노종원]
// 오디오 리스너
// 오디오 소스의 소리를 듣는 청취자 컴포넌트

// LOG
// [2020/12/31] 노종원 : com네임스페이스 추가
// [2021/03/26] 노종원 : 복사 생성자 추가, 가상 복사 생성 함수 추가, DirectX::AudioListenser를 상속하고있는 오타 수정

#include "Component.h"
#include "DXTK\DirectXTK.h"

namespace hos::com
{
	class AudioListener : public Component
	{
	private:
		DirectX::AudioListener Listener;
	public:
		AudioListener();
		~AudioListener();

	protected:
		//Only Used GameObject Copy
		AudioListener(const AudioListener& dest);

	public:
		DirectX::AudioListener& Get();

		void Bind(GameObject* gameObject) override;
		void UnBind(int index) override;
		const std::vector<U8> Serialize() override;
		bool Deserialize(mbstring_view);

		AudioListener* Clone() const override;
		void Reset() override;
		void Awake() override;

		void Update() override;

		void OnEnable() override;
		void OnDisable() override;
	};
}

#endif // !_AUDIO_LISTENER_H