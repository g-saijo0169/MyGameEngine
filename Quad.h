//#pragma once
//
//#include <DirectXMath.h>
//#include "Direct3D.h"
//#include "Texture.h"
//#include "Dice.h"
//
//using namespace DirectX;
//
////コンスタントバッファー
//
//struct CONSTANT_BUFFER
//{
//	XMMATRIX	matWVP;
//	XMMATRIX	matW;
//};
//
//struct VERTEX
//{
//	XMVECTOR position;
//	XMVECTOR uv;
//	XMVECTOR normal;
//};
//
//class Quad
//{
//	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
//	ID3D11Buffer* pIndexBuffer_;
//	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
//
//	Texture* pTexture_;
//
//public:
//	/// <summary>
//	/// 四角形を表すクラスのコンス
//	/// </summary>
//	Quad();
//	~Quad();
//	HRESULT Initialize();
//	void Draw(XMMATRIX& worldMatrix);
//	void Release();
//};