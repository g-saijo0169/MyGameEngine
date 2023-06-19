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
	UINT64 vertexNum_;              //頂点数
	std::vector<VERTEX> vertices_;  //頂点情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ

	UINT64 indexNum;                //インデックス数
	std::vector<int> index_;        //インデックス情報

	ID3D11Buffer* pIndexBuffer_;    //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;             //テクスチャ

public:
	Sprite();
	~Sprite();
	HRESULT Initialize();
	void Draw(XMMATRIX& worldMatrix);
	void Release();

private:
	//-------------------Initialize関数から呼ばれる関数------------------
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();   //インデックスバッファを作成

	HRESULT CreateConstanBuffer(); //コンスタントバッファを作成

	HRESULT LoadTexture();         //テクスチャをロード

	//-------------------Draw関数から呼ばれる関数------------------

	void PassDataToCB(DirectX::XMMATRIX& worldMatrix);  //コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();                         //各バッファをパイプラインにセット
};