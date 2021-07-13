#ifndef _COLOR_H
#define _COLOR_H

// [2020/12/31 노종원]
// 색을 표현하는 클래스
// 여러가지 컬러 타입의 컨버터 역할을 할 Color클래스
// 일부러 ut네임스페이스를 정의하지않았다.

// LOG
// [2020/12/31] 노종원 : SimpleMath의 Color을 그대로 상속만 한다.
// [2021/03/24] 노종원 : using 으로 변경

#include "BaseTypes.h"
#include "DXTK\DirectXTK.h"

namespace hos
{
	using Color = DirectX::SimpleMath::Color;
	/*class Color :public DirectX::SimpleMath::Color
	{
	public:
		Color(F32 r, F32 g, F32 b, F32 a = 1.f);
		~Color();
	};*/
}

#endif // !_COLOR_H