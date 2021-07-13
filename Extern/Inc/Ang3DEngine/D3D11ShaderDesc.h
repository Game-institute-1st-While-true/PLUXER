#pragma once

#include <d3d11_4.h>

namespace hos::cg
{
	// 쉐이더 종류
	enum class SHADER_LIST
	{
		SHADER_PRIMITIVE,
		SHADER_LINE,
		SHADER_ERROR,

		SHADER_PBR,
		SHADER_SKIN_PBR,

		SHADER_TEXOFF_UI,
		SHADER_TEXON_UI,

		SHADER_SKYBOX,

		SHADER_MAP_DEPTH,
		SHADER_MAP_DEPTH_SKIN,

		SHADER_MAP_SHADOW,
		SHADER_MAP_SHADOW_SKIN,
		
		SHADER_MAP_EMISS,
		SHADER_MAP_EMISS_SKIN,

		SHADER_GBLUR_X,
		SHADER_GBLUR_Y,

		SHADER_MAP_VELOCITY,

		SHADER_BLOOMCURVE,
		SHADER_COMBINE,

		SHADER_SOFTSHADOW,
		SHADER_SOFTSHADOW_SKIN,

		SHADER_TOSWAPCHAIN,
		SHADER_TOSWAPCHAIN_MOTIONNBLUR,
		SHADER_TOSWAPCHAIN_RADIALBLUR,
		SHADER_TOSWAPCHAIN_ALLBLUR,

		// 최대 갯수 체크용
		SHADER_MAX,

		SHADER_NONE = 65535, // 없는 경우
	};

	// VertexBuffer Layout 설정.
	static D3D11_INPUT_ELEMENT_DESC VBlayoutList[(int)SHADER_LIST::SHADER_MAX + 1][10] =
	{
		//SHADER_TEXOFF_LAMBERT[0]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER LINE[2]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER ERROR[4]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_PBR[5]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_SKIN_PBR[6]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 정점 가중치
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 뼈대 참조 인덱스
		},

		// SHADER_TEXOFF_UI[7]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_TEXON_UI[8]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_SKYBOX[9]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_DEFERRED_DEPTH[14]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_DEFERRED_DEPTH_SKIN[15]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 정점 가중치
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 뼈대 참조 인덱스
		},

		// SHADER_SHADOWMAP[21]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_SHADOWMAP_SKIN[22]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 정점 가중치
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 뼈대 참조 인덱스
		},

		//SHADER_EMISS[25]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_EMISS_SKIN[26]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 정점 가중치
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 뼈대 참조 인덱스
		},

		// SHADER_EMISS_HBULR[27]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_EMISS_VBULR[28]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_EMISS_VBULR[29]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_EMISS_VBULR[30]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_COMBINE[31]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_PBR[5]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_SKIN_PBR[6]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 정점 가중치
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 뼈대 참조 인덱스
		},

		// SHADER_TOSWAPCHAIN[31]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_TOSWAPCHAIN_MORTIONBLUR[]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_TOSWAPCHAIN_RADIALBLUR[]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},

		// SHADER_TOSWAPCHAIN_ALLBLUR[]
		{
			//  Sementic          format                    offset         classification             
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		},
	};

	enum class TEXMODE_LIST
	{
		SS_CLAMP,			//텍스처 어드레스 모드 : 클램핑 (DX 기본값) 
		SS_WRAP,			//텍스처 어드레스 모드 : 렙핑 
		SS_BORDER,
		SS_SHADOW,

		SS_MAX,
		SS_DEFAULT = SS_WRAP,	//기본 샘플러
		SS_NONE = 65535,
	};


	static D3D11_SAMPLER_DESC SamplerOptionList[(int)TEXMODE_LIST::SS_MAX] =
	{
		// 0번 샘플러(클램프: DX기본)
		{
			D3D11_FILTER_MIN_MAG_MIP_LINEAR,	// FILTER Filter;
			D3D11_TEXTURE_ADDRESS_CLAMP,		// TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_CLAMP,		// TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_CLAMP,		// TEXTURE_ADDRESS_MODE AddressW;
			0,									// MipLODBias;
			16,									// MaxAnisotropy;
			D3D11_COMPARISON_NEVER,				// COMPARISON_FUNC ComparisonFunc;
			1,									// BorderColor[0];
			1,									// BorderColor[1];
			1,									// BorderColor[2];
			1,									// BorderColor[3];
			0,									// MinLOD;
			D3D11_FLOAT32_MAX					// MaxLOD;
		},

		// 1번 샘플러(WRAP)
		{
			D3D11_FILTER_ANISOTROPIC,			// FILTER Filter;
			D3D11_TEXTURE_ADDRESS_WRAP,			// TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_WRAP,			// TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_WRAP,			// TEXTURE_ADDRESS_MODE AddressW;
			0,									// MipLODBias;
			16,									// MaxAnisotropy;
			D3D11_COMPARISON_NEVER,				// COMPARISON_FUNC ComparisonFunc;
			1,									// BorderColor[0];
			1,									// BorderColor[1];
			1,									// BorderColor[2];
			1,									// BorderColor[3];
			0,									// MinLOD;
			D3D11_FLOAT32_MAX					// MaxLOD;
		},

		// 2번 샘플러(흰색)
		{
			D3D11_FILTER_ANISOTROPIC,			// FILTER Filter;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressW;
			0,									// MipLODBias;
			16,									// MaxAnisotropy;
			D3D11_COMPARISON_NEVER,				// COMPARISON_FUNC ComparisonFunc;
			1,									// BorderColor[0];
			1,									// BorderColor[1];
			1,									// BorderColor[2];
			1,									// BorderColor[3];
			0,									// MinLOD;
			0									// MaxLOD;
		},

		// 3번 샘플러(그림자)
		{
			D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,			// FILTER Filter;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_BORDER,		// TEXTURE_ADDRESS_MODE AddressW;
			0,									// MipLODBias;
			0,									// MaxAnisotropy;
			D3D11_COMPARISON_LESS_EQUAL,		// COMPARISON_FUNC ComparisonFunc;
			1,									// BorderColor[0];
			1,									// BorderColor[1];
			1,									// BorderColor[2];
			1,									// BorderColor[3];
			0,									// MinLOD;
			0									// MaxLOD;
		},
	};

	// 쉐이더의 샘플러 객체 인덱스 리스트
	static TEXMODE_LIST samplerListOnShader[(int)SHADER_LIST::SHADER_MAX][10] =
	{
		// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},


		// 1번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

		// 2번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

		// 3번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_BORDER,
		TEXMODE_LIST::SS_SHADOW,
		TEXMODE_LIST::SS_NONE
		},

		// 4번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_BORDER,
		TEXMODE_LIST::SS_SHADOW,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

				// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

			// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

			// 3번 쉐이더
		{
		TEXMODE_LIST::SS_SHADOW,
		TEXMODE_LIST::SS_NONE
		},

			// 4번 쉐이더
		{
		TEXMODE_LIST::SS_SHADOW,
		TEXMODE_LIST::SS_NONE
		},

			// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

			// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

		// 0번 쉐이더
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},

		// ALL BLUR
		{
		TEXMODE_LIST::SS_CLAMP,
		TEXMODE_LIST::SS_NONE
		},
	};
}