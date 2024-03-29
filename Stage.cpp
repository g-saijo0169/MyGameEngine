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
    int posY = -1;
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
                        posY = y;
                        posZ = z;
                    }
                    break;
                }
            }
        }
    }

    int current = table_[posX][posZ].type;

    switch (mode_)
    {
        //  地形を上げる
    case 0:
        if(table_[posX][posZ].height < 15)
        table_[posX][posZ].height++;
        break;
        //  地形を下げる
    case 1:
        if (table_[posX][posZ].height > 0)
        table_[posX][posZ].height--;
        break;
        //  種類を変える
    case 2:
        table_[posX][posZ].type = select_;
        break;
        //  塗りつぶし
    case 3:
        for (int x = 0;  x < 15; x++)
        {
            for (int z = 0; z < 15; z++)
            {
                if (table_[posX][posZ].height == table_[x][z].height)
                {
                    if (current == table_[x][z].type)
                    table_[x][z].type = select_;

                }
                
            }
        }
        break;
        //  全消し
    case 4:
        for (int x = 0; x < 15; x++)
        {
            for (int z = 0; z < 15; z++)
    
            {
                SetBlock(x, z, DEFAULT);
                SetBlockHeght(x, z, 0);
            }
        }
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

        SendMessage(GetDlgItem(hDlg, IDC_RADIO_FILL), BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_DELETE), BM_SETCHECK, BST_UNCHECKED, 0);

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
        case IDC_RADIO_FILL:
            mode_ = 3;
            break;
        case IDC_RADIO_DELETE:
            mode_ = 4;
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
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");
    ofn.lpstrFile = fileName;               	            //ファイル名
    ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
    ofn.Flags = OFN_OVERWRITEPROMPT;   		                //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,               //ファイル名
        GENERIC_WRITE,          //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        CREATE_ALWAYS,          //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）
    string mapData;
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            string mapHeight = std::to_string(table_[x][z].height);
            string mapType = std::to_string(static_cast<int>(table_[x][z].type)); // ブロックの種類を数値に変換
            mapData += mapHeight + "," + mapType; // 高さと種類をカンマ区切りで追加
            if (x != 14 || z != 14) {  // マップの最後でない場合、カンマを追加
                mapData += "　";
            }
        }
    }

    const char* cstr = mapData.c_str();

    DWORD dwBytes = 0;  //書き込み位置
    WriteFile(
        hFile,              //ファイルハンドル
        cstr,                  //保存するデータ（文字列）
        (DWORD)strlen(cstr),   //書き込む文字数
        &dwBytes,           //書き込んだサイズを入れる変数
        NULL);              //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);
}



void Stage::Load()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");
    ofn.lpstrFile = fileName;               	            //ファイル名
    ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
    ofn.Flags = OFN_FILEMUSTEXIST;   		                //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_READ,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        OPEN_EXISTING,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）

    //ファイルのサイズを取得
    DWORD fileSize = GetFileSize(hFile, NULL);

    //ファイルのサイズ分メモリを確保
    char* data;
    data = new char[fileSize];

    DWORD dwBytes = 0; //読み込み位置

    ReadFile(
        hFile,     //ファイルハンドル
        data,      //データを入れる変数
        fileSize,  //読み込むサイズ
        &dwBytes,  //読み込んだサイズ
        NULL);     //オーバーラップド構造体（今回は使わない）

    char* nextToken;
    char* token = strtok_s(data, ",", &nextToken); // カンマで文字列を分割

    int count = 0;
    while (token != NULL)
    {
        if (count >= 15 * 15) {
            break;  // データが多すぎる場合は終了
        }
        int height_ = atoi(token);
        token = strtok_s(NULL, ",", &nextToken);

        if (token != NULL) {
            int type_ = atoi(token); // ブロックの種類の数値を読み込む
            table_[count / 15][count % 15].height = height_;
            table_[count / 15][count % 15].type = static_cast<BLOCKTYPE>(type_); // 数値からブロックの種類に変換
        }
        token = strtok_s(NULL, ",", &nextToken); // 次のトークンを取得
        count++;
    }
    CloseHandle(hFile);
}

//void Stage::Save(){
//    char fileName[MAX_PATH] = "無題.map";           //ファイル名を入れる変数
//
//    //「ファイルを保存」ダイアログの設定
//    OPENFILENAME ofn;                               //名前をつけて保存ダイアログの設定用構造体
//    ZeroMemory(&ofn, sizeof(ofn));                  //構造体初期化
//    ofn.lStructSize = sizeof(OPENFILENAME);         //構造体のサイズ
//    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")    //ファイルの種類
//        TEXT("すべてのファイル(*.*)\0*.*\0\0");               //
//    ofn.lpstrFile = fileName;                       //ファイル名
//    ofn.nMaxFile = MAX_PATH;                        //パスの最大文字数
//    ofn.Flags = OFN_OVERWRITEPROMPT;                //フラグ(同名ファイルが存在したら上書き確認)
//    ofn.lpstrDefExt = "map";                        //デフォルト拡張子
//
//
//    //「ファイルを保存」ダイアログ
//    BOOL selFile;
//    selFile = GetSaveFileName(&ofn);
//
//    //キャンセルしたら中断
//    if (selFile == FALSE) return;
//
//    //セーブのルーチン
//    HANDLE hFile;
//    hFile = CreateFile(
//        fileName,                 //ファイル名
//        GENERIC_WRITE,           //アクセスモード（書き込み用）
//        0,                      //共有（なし）
//        NULL,                   //セキュリティ属性（継承しない）
//        CREATE_ALWAYS,           //作成方法
//        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
//        NULL);                  //拡張属性（なし）
//
//
//    //std::string Stagedata = std::to_string(XSIZE) + "," + std::to_string(ZSIZE) + "\n"; // ステージのサイズを追加
//    //std::string Stagedata;
//    //for (int x = 0; x < XSIZE; x++)
//    //{
//    //    for (int z = 0; z < ZSIZE; z++)
//    //    {
//    //        // ブロックの高さと種類を文字列に変換してコンマで区切って連結
//    //        Stagedata += std::to_string(table_[x][z].height) + "," + std::to_string(table_[x][z].type) + " ";
//
//    //        // XSIZE - 1 のときはコンマを追加
//    //        if (z < ZSIZE - 1)
//    //            Stagedata += ",";
//    //    }
//
//    //    // ZSIZE - 1 のときは改行文字を追加
//    //    if (x < XSIZE - 1)
//    //        Stagedata += "\n";
//    //}
//
//    std::string mapData;
//
//    //for (int x = 0; x < XSIZE; x++)
//    //{
//    //    for (int z = 0; z < ZSIZE; z++)
//    //    {
//    //        string map = std::to_string(table_[x][z].height) + " ," + std::to_string(table_[x][z].type);
//    //        mapData += map;
//    //        if (x != 14 || z != 14) {  // マップの最後でない場合、カンマを追加
//    //            mapData += ",";
//    //        }
//    //        //if (z < ZSIZE - 1)
//    //        //   mapData += ",";
//    //    }
//    //    //if (x < XSIZE - 1)
//    //    //mapData += "\n";
//    //    
//    //}
//
//    for (int x = 0; x < 15; x++)
//    {
//        for (int z = 0; z < 15; z++)
//        {
//            string mapHeight = std::to_string(table_[x][z].height);
//            string mapType = std::to_string(static_cast<int>(table_[x][z].type)); // ブロックの種類を数値に変換
//            mapData += mapHeight + "," + mapType; // 高さと種類をカンマ区切りで追加
//            if (x != 14 || z != 14) {  // マップの最後でない場合、カンマを追加
//                mapData += ",";
//            }
//        }
//    }
//
//    DWORD dwBytes = 0;                  //書き込み位置
//    WriteFile(
//        hFile,                          //ファイルハンドル
//        mapData.c_str(),                   //保存するデータ（文字列）
//        (DWORD)strlen(mapData.c_str()),    //書き込む文字数
//        &dwBytes,                       //書き込んだサイズを入れる変数
//        NULL);                          //オーバーラップド構造体（今回は使わない）
//
//    CloseHandle(hFile);
//}
//
//void Stage::Load()
//{
//    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数
//
//    //「ファイルを開く」ダイアログの設定
//    OPENFILENAME ofn;                         	//名前をつけて開くダイアログの設定用構造体
//    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
//    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
//    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
//        TEXT("すべてのファイル(*.*)\0*.*\0\0");                   //─┘
//    ofn.lpstrFile = fileName;               	//ファイル名
//    ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
//    ofn.Flags = OFN_FILEMUSTEXIST;   		//フラグ（同名ファイルが存在したら上書き確認）
//    ofn.lpstrDefExt = "map";                  	//デフォルト拡張子
//
//    //「ファイルを開く」ダイアログ
//    BOOL selFile;
//    selFile = GetOpenFileName(&ofn);
//
//    //キャンセルしたら中断
//    if (selFile == FALSE) return;
//
//    HANDLE hFile;        //ファイルのハンドル
//    hFile = CreateFile(
//        fileName,                 //ファイル名
//        GENERIC_READ,           //アクセスモード（書き込み用）
//        0,                      //共有（なし）
//        NULL,                   //セキュリティ属性（継承しない）
//        OPEN_EXISTING,           //作成方法
//        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
//        NULL);                  //拡張属性（なし）
//
//    std::string LoadData;
//
//    //ファイルのサイズを取得
//    DWORD fileSize = GetFileSize(hFile, NULL);
//
//    //ファイルのサイズ分メモリを確保
//    char* Data;
//    Data = new char[fileSize];
//
//    DWORD dwBytes = 0; //読み込み位置
//
//    ReadFile(
//        hFile,     //ファイルハンドル
//        Data,      //データを入れる変数
//        fileSize,  //読み込むサイズ
//        &dwBytes,  //読み込んだサイズ
//        NULL);     //オーバーラップド構造体（今回は使わない）
//
//    char* nextToken;
//    char* token = strtok_s(Data, ",", &nextToken); // カンマで文字列を分割
//
//    int count = 0;
//    while (token != NULL)
//    {
//        if (count >= 15 * 15) {
//            break;  // データが多すぎる場合は終了
//        }
//
//        int height_ = atoi(token);
//        table_[count / 15][count % 15].height = height_;
//
//        if (token != NULL) {
//            int type_ = atoi(token); // ブロックの種類の数値を読み込む
//            table_[count / 15][count % 15].height = height_;
//            table_[count / 15][count % 15].type = static_cast<BLOCKTYPE>(type_); // 数値からブロックの種類に変換
//        }
//        token = strtok_s(NULL, ",", &nextToken); // 次のトークンを取得
//        count++;
//    }
//
//    CloseHandle(hFile);
//}
