#include "Stage.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = -1;
    }
    for (int z = 0; z < XSIZE; z++) {
        for (int x = 0; x < ZSIZE; x++) {
            table_[x][z] = 0;
        }
    }
    
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    string modelname[] = {
        "BoxDefault.fbx",
        "BoxBrick.fbx",
        "BoxGrass.fbx",
        "BoxSand.fbx",
        "BoxWater.fbx"
    };

    string fname_base = "Assets/";
    //モデルデータのロード
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(fname_base + modelname[i]);
        assert(hModel_[i] >= 0);
    }
 
    //tableにブロックのタイプをセットしてやろう！
    for (int z = 0; z < XSIZE; z++) {
        for (int x = 0; x < ZSIZE; x++) {
            table_[x][z] = x%5;
        }
    }
}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            //table[x][z]
            int type = table_[x][z];
            Transform blockTrans; //Transform型
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            Model::SetTransform(hModel_[(x+z)%5], blockTrans);
            Model::Draw(hModel_[(x + z) % 5]);
        }
    }

}

//開放
void Stage::Release()
{
}