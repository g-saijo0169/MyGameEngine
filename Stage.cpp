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

//�R���X�g���N�^
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
    for (int z = 0; z < ZSIZE; z++) {
        for (int x = 0; x < XSIZE; x++) {
           SetBlock(x, z, (BLOCKTYPE)(0));

        }
    }SetBlockHeght(0, 0, 0);
}

//�X�V
void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0))
    {
        
        return;
    }
    float w = (float)(Direct3D::scrWidth/2.0f);//��ʃT�C�Y�̔���
    float h = (float)(Direct3D::scrHeight/2.0f);//��ʂ̍����̔���
    //Offsetx,y��0
    //minZ = 0, maxZ = 1

    XMMATRIX vp =
    {
        w,  0,   0,    0,
        0, -h,   0,    0,
        0,  0,   1,    0,
        w,  h,   0,    1
    };
    //�r���[�|�[�g
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
    //�v���W�F�N�V�����ϊ�
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    //�r���[�ϊ�
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;
    //�@�@mousePosFront���x�N�g���ɕϊ�
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
    //�A�@�@��invVP�AinvPrj�AinvView��������
    vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
    //�B�@mousePosBack���x�N�g���ɕϊ�
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
    //�C�@�B��invVP�AinvPrj�AinvView��������
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    float distance = 100000; //���������Ă����ϐ�
    int posX = -1;
    int posZ = -1;


    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //�D�@�A����C�Ɍ������ă��C�����i�Ƃ肠�������f���ԍ���hModel_[0]�j
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[0], trans);

                Model::RayCast(hModel_[0], data);

                //�E�@���C������������u���[�N�|�C���g�Ŏ~�߂�
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

//�`��
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
    char fileName[MAX_PATH] = "����.map";           //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                               //���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));                  //�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);         //�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")    //�t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");               //
    ofn.lpstrFile = fileName;                       //�t�@�C����
    ofn.nMaxFile = MAX_PATH;                        //�p�X�̍ő啶����
    ofn.Flags = OFN_OVERWRITEPROMPT;                //�t���O(�����t�@�C�������݂�����㏑���m�F)
    ofn.lpstrDefExt = "map";                        //�f�t�H���g�g���q


    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    //�Z�[�u�̃��[�`��
    HANDLE hFile;
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        CREATE_ALWAYS,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j


        //std::string Stagedata = std::to_string(XSIZE) + "," + std::to_string(ZSIZE) + "\n"; // �X�e�[�W�̃T�C�Y��ǉ�
    std::string Stagedata;

    for (int z = 0; z < ZSIZE; z++)
    {
        for (int x = 0; x < XSIZE; x++)
        {
            // �u���b�N�̍����Ǝ�ނ𕶎���ɕϊ����ăR���}�ŋ�؂��ĘA��
            Stagedata += std::to_string(table_[x][z].height) + "," + std::to_string(table_[x][z].height);

            // XSIZE - 1 �̂Ƃ��̓R���}��ǉ�
            if (x < XSIZE - 1)
                Stagedata += ",";
        }

        // ZSIZE - 1 �̂Ƃ��͉��s������ǉ�
        if (z < ZSIZE - 1)
            Stagedata += "\n";
    }

    DWORD dwBytes = 0;                  //�������݈ʒu
    WriteFile(
        hFile,                          //�t�@�C���n���h��
        Stagedata.c_str(),                   //�ۑ�����f�[�^�i������j
        (DWORD)strlen(Stagedata.c_str()),    //�������ޕ�����
        &dwBytes,                       //�������񂾃T�C�Y������ϐ�
        NULL);                          //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);
}

void Stage::Load()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

    //�u�t�@�C�����J���v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĊJ���_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");                   //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_FILEMUSTEXIST;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

    //�u�t�@�C�����J���v�_�C�A���O
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_READ,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j

    

    //�t�@�C���̃T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);

    //�t�@�C���̃T�C�Y�����������m��
    char* Data;
    Data = new char[fileSize];

    DWORD dwBytes = 0; //�ǂݍ��݈ʒu

    ReadFile(
        hFile,     //�t�@�C���n���h��
        Data,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &dwBytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);
}