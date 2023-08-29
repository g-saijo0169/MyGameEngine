#include "Stage.h"
#include "Engine/Model.h"

//�R���X�g���N�^
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

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
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
    //���f���f�[�^�̃��[�h
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(fname_base + modelname[i]);
        assert(hModel_[i] >= 0);
    }
 
    //table�Ƀu���b�N�̃^�C�v���Z�b�g���Ă�낤�I
    for (int z = 0; z < XSIZE; z++) {
        for (int x = 0; x < ZSIZE; x++) {
            table_[x][z] = x%5;
        }
    }
}

//�X�V
void Stage::Update()
{
}

//�`��
void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            //table[x][z]
            int type = table_[x][z];
            Transform blockTrans; //Transform�^
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            Model::SetTransform(hModel_[(x+z)%5], blockTrans);
            Model::Draw(hModel_[(x + z) % 5]);
        }
    }

}

//�J��
void Stage::Release()
{
}