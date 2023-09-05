#include "Stage.h"
#include "Engine/Model.h"
#include "resource.h"


void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_x][_z].type = _type;
}

void Stage::SetBlockHeght(int _x, int _z, int _height)
{
    table_[_x][_z].height = _height;
}

//コンストラクタ
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
           SetBlock(x, z, (BLOCKTYPE)(z%5));
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
            for (int y = 0; y < 15; y++)
            {
                //table[x][z]
                int type = table_[x][z].type;
                Transform blockTrans; //Transform型
                blockTrans.position_.x = x;
                blockTrans.position_.y = y;
                blockTrans.position_.z = z;

                Model::SetTransform(hModel_[type], blockTrans);
                Model::Draw(hModel_[type]);
            }
            
        }
    }

}

//開放
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) 
{
    switch (msg)
    {
    //ダイアログできた
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK,BST_CHECKED, 0);

        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"草原");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"砂地");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_ADDSTRING, 0, (LPARAM)"水");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_SETCURSEL, 0, 0);

        return TRUE;

    }
    return FALSE;
}