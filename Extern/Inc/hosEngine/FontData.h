#ifndef _FONT_DATA_H
#define _FONT_DATA_H

// [2021/03/11 노종원]
// 폰트

// LOG

#include "hosUtility\hosUtility.h"
#include "DXTK\DirectXTK.h"

namespace hos
{
	class FontData
	{
	public:
		static constexpr string_literal SPRITE_FONT_FILE = L".spritefont";
		static constexpr string_literal FILE_EXTENSION = L".font";
	public:
		mbstring Blob;
	private:
		string Name;

	public:
		FontData();
		~FontData();

		string GetName()const;
		void SetName(string_view name);

		const std::vector<U8> Serialize();
		bool Deserialize(mbstring_view data);
	};
}

#endif // !_FONT_DATA_H