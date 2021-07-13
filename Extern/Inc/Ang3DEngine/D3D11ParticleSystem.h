// <Concept>
// Source Name		: D3D11ParticleSystem.h
// Creator			: Hosik_Shin / shin0394g@gmail.com
// Create Date		: 2021.06.25

// <Explanation>
// D3D11의 Particle System 클래스
// 임시로 Particle의 생성과 제거를 관리한다.

// Working Log
// 2021.06.25.Hosik	: 생성

#pragma once

#include <d3d11_4.h>
#include "DXTK\SimpleMath.h"

namespace hos::cg
{

	class D3D11ParticleSystem
	{
		// 클래스 멤버 인스턴스
	private:
		struct ParticleType
		{
			DirectX::SimpleMath::Vector3 vPos = { 0.f, 0.f, 0.f };
			DirectX::SimpleMath::Vector3 Color = { 1.f, 1.f, 1.f };;
			float velocity = 0.0f;
			bool active = false;
		};

		struct VertexType
		{
			DirectX::SimpleMath::Vector3 position;
			DirectX::SimpleMath::Vector2 uv;
			DirectX::SimpleMath::Vector4 color;
		};

		// Device
		ID3D11Device& DeviceRef;
		ID3D11DeviceContext& ContextRef;

		// Texture
		ID3D11ShaderResourceView* ParticalTexture;

		int m_vertexCount;
		int m_indexCount;
		VertexType* m_vertices;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;

		// Particle Value
		DirectX::SimpleMath::Vector3 Deviation;

		float m_particleVelocity;
		float m_particleVelocityVariation;

		float m_particleSize;
		float m_accumulatedTime;
		float m_particlesPerSecond;

		unsigned int m_maxParticles;
		unsigned int m_currentParticleCount;

		// Particle List
		ParticleType* m_particleList;


		// 생성자 소멸자
	public:
		D3D11ParticleSystem() = delete;
		D3D11ParticleSystem(ID3D11Device* device, ID3D11DeviceContext* context);
		~D3D11ParticleSystem();

		// 외부 사용 함수
	public:
		void Frame(float dTime);
		void Render();

		// 내부 함수
	private:
		bool SetParticle();
		HRESULT CreateTexture();
		bool CreateVertexBuffer();

		void EmitParticles(float frameTime);
		void UpdateParticles(float frameTime);
		void KillParticles();

		bool UpdateBuffer();

	};

}