#include "Stage.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_(-1), table_(nullptr)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform blockTrans; //Transform型
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            Model::SetTransform(hModel_, blockTrans);
            Model::Draw(hModel_);
        }
    }

}

//開放
void Stage::Release()
{
}