#include "Model.h"
#include "Direct3D.h"


namespace Model
{
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string fileName)
{
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = fileName;
	pData->pFbx_ = nullptr;


	//ファイルネームが同じだったら、読まん！
	for (auto e : modelList)
	{
		if (e->filename_ == fileName) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}

	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);
	}


	modelList.push_back(pData);
	return (modelList.size() - 1);
}

void Model::SetTransform(int hModel, Transform transform) {
	//モデル番号はmodelListのインデックス
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel) {
	//モデル番号はmodelListのインデックス
	//modelList[hModel]->pFbx->Draw(メンバのトランスフォーム)
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false; //参照されてる？
	for (int i = 0; i < modelList.size(); i++)
	{
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->pFbx_ == modelList[j]->pFbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pFbx_)
		}
		SAFE_DELETE(modelList[i])
	}
	modelList.clear();
}

void Model::RayCast(int hModel, RayCastData& rayData)
{

	//?モデルのトランスフォームをカリキュレーション
	modelList[hModel]->transform_.Calclation();
	//①ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr, modelList[hModel]->transform_.GetWorldMatrix());
	//②レイの通過点を求める（モデル空間での例の方向ベクトルを求める
	XMVECTOR vpass{ rayData.start.x + rayData.dir.x,
					rayData.start.y + rayData.dir.y,
					rayData.start.z + rayData.dir.z,
					rayData.start.w + rayData.dir.w };
	//③rayData.startをモデル空間に変換（①を掛ける）
	XMVECTOR vstart = XMLoadFloat4(&rayData.start);
	vstart = XMVector3TransformCoord(vstart, wInv);
	//④（始点から方向ベクトルをちょい伸ばした先）通過する点（②）に①を掛ける
	vpass = XMVector3TransformCoord(vpass, wInv);
	//⑤rayData.dirを③から④に向かうベクトルにする（引き算）
	vpass = vpass - vstart;
	XMStoreFloat4(&rayData.dir, vpass);


	//指定したモデル番号のFBXにレイキャスト！
	modelList[hModel]->pFbx_->RayCast(rayData);
}
