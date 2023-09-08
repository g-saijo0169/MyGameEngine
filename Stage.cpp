#include "Stage.h"
#include "Engine/Model.h"
#include "resource.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

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
    float w = (float)(Direct3D::scrWidth/2.0f);//画面サイズの半分
    float h = (float)(Direct3D::scrHeight/2.0f);//画面の高さの半分
    //Offsetx,yは0
    //minZ = 0, maxZ = 1

    XMMATRIX vp =
    {
        w,  0,   0,    0,
        0, -h,   0,    0,
        0,  0, 1 - 0,  0,
        w,  h,   0,    1
    };
    //ビューポート
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
    //プロジェクション変換
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    //ビュー変換
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;

    //①　mousePosFrontをベクトルに変換
    XMVECTOR vMousePosFront = XMLoadFloat3(&mousePosFront);

    //②　①にinvVP,intPrj,invViewをかける
    vMousePosFront = XMVector3TransformCoord(vMousePosFront,invVP * invProj * invView );

    //③　mousePosBackをベクトルに変換
    XMVECTOR vMousePosBack = XMLoadFloat3(&mousePosBack);
    //④　③にinvVP,invPrj,invViewをかける
    vMousePosBack = XMVector3TransformCoord(vMousePosBack, invVP * invProj * invView);

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //⑤　②から④に向かってレイを打つ（とりあえずモデル番号はhModel_[0])
                RayCastData data;
                XMStoreFloat4(&data.start, vMousePosFront);
                XMStoreFloat4(&data.dir, vMousePosBack - vMousePosFront);
                Transform trans;

                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;

                Model::SetTransform(hModel_[0], trans);
                Model::RayCast(hModel_[0], data);

                //⑥　レイが当たったらブレークポイントで止める
                if (data.hit)
                {
                    break;
                }
            }
        }
    }

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