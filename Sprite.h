#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

#define SAFE_DELETE_ARRAY(p) if(p!=nullptr){delete p;p=nullptr;}
class Sprite
{
	//コンスタントバッファー

 struct CONSTANT_BUFFER
 {
	XMMATRIX	matW;
 };

 struct VERTEX
 {
	XMVECTOR position;
	XMVECTOR uv;

 };
protected:
	UINT64 vertexNum_;
	std::vector<VERTEX> vertices_;
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ

	UINT64 indexNum;
	std::vector<int> index_;

	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;

public:
	Sprite();
	~Sprite();
	HRESULT Initialize();
	void Draw(XMMATRIX& worldMatrix);
	void Release();

private:
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();

	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstanBuffer();
	HRESULT LoadTexture();

	void PassDataToCB(DirectX::XMMATRIX& worldMatrix);
	void SetBufferToPipeline();
};