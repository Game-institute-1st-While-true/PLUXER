#ifndef _AUDIO_CLIP_H
#define _AUDIO_CLIP_H

// [2021/1/15 노종원]
// 오디오 클립(음원)

// LOG
// [2021/03/11] 노종원 : 생성자 string에서 string_veiw로 수정

#include "hosUtility\hosUtility.h"
#include "DXTK\DirectXTK.h"

namespace hos
{
	class AudioClip
	{
	public:
		static constexpr string_literal WAV_FILE = L".wav";
		static constexpr string_literal FILE_EXTENSION = L".sclip";
		static constexpr string_literal FILE_PATH = L"\\Assets\\Sound\\";

	public:
		DirectX::SoundEffect* Sound;

	private:
		string Name;

	public:
		AudioClip();
		~AudioClip();

		string GetName() const { return Name; }
		void SetName(string_view name);

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};
}

#endif // !_AUDIO_CLIP_H