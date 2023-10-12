#include "Stage.h"
#include "Engine/Model.h"
#include "resource.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"

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
    for (int z = 0; z < ZSIZE; z++) {
        for (int x = 0; x < XSIZE; x++) {
            SetBlock(x, z, DEFAULT);
            SetBlockHeght(x, z, 0);
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
    for (int z = 0; z < ZSIZE; z++) {
        for (int x = 0; x < XSIZE; x++) {
           SetBlock(x, z, (BLOCKTYPE)(0));

        }
    }SetBlockHeght(0, 0, 0);
}

//更新
void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0))
    {
        
        return;
    }
    float w = (float)(Direct3D::scrWidth/2.0f);//画面サイズの半分
    float h = (float)(Direct3D::scrHeight/2.0f);//画面の高さの半分
    //Offsetx,yは0
    //minZ = 0, maxZ = 1

    XMMATRIX vp =
    {
        w,  0,   0,    0,
        0, -h,   0,    0,
        0,  0,   1,    0,
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
    //�@　mousePosFrontをベクトルに変換
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
    //�A　�@にinvVP、invPrj、invViewをかける
    vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
    //�B　mousePosBackをベクトルに変換
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
    //�C　�BにinvVP、invPrj、invViewをかける
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    float distance = 100000; //距離を入れておく変数
    int posX = -1;
    int posZ = -1;


    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //�D　�Aから�Cに向かってレイをうつ（とりあえずモデル番号はhModel_[0]）
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[0], trans);

                Model::RayCast(hModel_[0], data);

                //�E　レイが当たったらブレークポイントで止める
                if (data.hit)
                {
                    if (distance > data.dist) {
                        distance = data.dist;
                        posX = x;
                        posZ = z;
                    }
                    break;
                }
            }
        }
    }



    switch (mode_)
    {
    case 0:
        if(table_[posX][posZ].height < 15)
        table_[posX][posZ].height++;
        break;
    case 1:
        if (table_[posX][posZ].height > 0)
        table_[posX][posZ].height--;
        break;
    case 2:
        table_[posX][posZ].type = select_;
        break;
    };
   

}

//描画
void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y <=  table_[x][z].height; y++)
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

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDC_RADIO_UP:
            mode_ = 0;
            break;
        case IDC_RADIO_DOWN:
            mode_ = 1;
            break;
        case IDC_RADIO_CHANGE:
            mode_ = 2;
            break;
        case IDC_COMBO:
            select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO), CB_GETCURSEL, 0, 0);
            break;
            
        }


    }
    return FALSE;
}

void Stage::Save()
{
    char fileName[MAX_PATH] = "無題.map";           //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                               //名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));                  //構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);         //構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")    //ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");               //
    ofn.lpstrFile = fileName;                       //ファイル名
    ofn.nMaxFile = MAX_PATH;                        //パスの最大文字数
    ofn.Flags = OFN_OVERWRITEPROMPT;                //フラグ(同名ファイルが存在したら上書き確認)
    ofn.lpstrDefExt = "map";                        //デフォルト拡張子


    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    //セーブのルーチン
    HANDLE hFile;
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_WRITE,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        CREATE_ALWAYS,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）

    std::string data = "";

    DWORD dwBytes = 0;                  //書き込み位置
    WriteFile(
        hFile,                          //ファイルハンドル
        data.c_str(),                   //保存するデータ（文字列）
        (DWORD)strlen(data.c_str()),    //書き込む文字数
        &dwBytes,                       //書き込んだサイズを入れる変数
        NULL);                          //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);
}

void Stage::Load()
{
}