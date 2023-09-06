#include "Stage.h"
#include "Engine/Model.h"
#include "resource.h"
#include "Engine/Direct3D.h"


void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_x][_z].type = _type;
}

void Stage::SetBlockHeght(int _x, int _z, int _height)
{
    table_[_x][_z].height = _height;
}

//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = -1;
    }
    for (int z = 0; z < XSIZE; z++) {
        for (int x = 0; x < ZSIZE; x++) {
            SetBlock(x, z, DEFAULT);
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
           SetBlock(x, z, (BLOCKTYPE)(z%5));
        }
    }
}

//�X�V
void Stage::Update()
{
    float w = (float)(Direct3D::scrWidth/2.0f);//��ʃT�C�Y�̔���
    float h = (float)(Direct3D::scrHeight/2.0f);//��ʂ̍����̔���
    //Offsetx,y��0
    //minZ = 0, maxZ = 1

    XMMATRIX vp =
    {
        w, 0,  0,   0,
        0, -h, 0,  0,
        0, 0, 1 - 0, 0,
        w, h,  0,   1
    };
    //�r���[�|�[�g
    XMMATRIX invVP = ;
    //�v���W�F�N�V�����ϊ�
    XMMATRIX invProj = ;
    //�r���[�ϊ�
    XMMATRIX invView = ;
    XMFLOAT3 mousePosFront = ;//�}�E�X�|�W�Q�b�g
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = ;
    mousePosBack.z = 1.0f;
    //�@�@mousePosFront���x�N�g���ɕϊ�
    //�A�@�@��invVP,intPrj,invView��������
    //�B�@mousePosBack���x�N�g���ɕϊ�
    //�C�@�B��invVP,invPrj,invView��������
    //�D�@�A����C�Ɍ������ă��C��łi�Ƃ肠�������f���ԍ���hModel_[0])
    //�E�@���C������������u���[�N�|�C���g�Ŏ~�߂�
}

//�`��
void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < 15; y++)
            {
                //table[x][z]
                int type = table_[x][z].type;
                Transform blockTrans; //Transform�^
                blockTrans.position_.x = x;
                blockTrans.position_.y = y;
                blockTrans.position_.z = z;

                Model::SetTransform(hModel_[type], blockTrans);
                Model::Draw(hModel_[type]);
            }
            
        }
    }

}

//�J��
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) 
{
    switch (msg)
    {
    //�_�C�A���O�ł���
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK,BST_CHECKED, 0);

        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"����");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"���n");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_SETCURSEL, 0, 0);

        return TRUE;

    }
    return FALSE;
}