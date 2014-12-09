#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define MSG(m) {MessageBoxA(NULL,m,NULL,MB_OK);}

//�֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();
//�O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;			//�f�o�C�X�R���e�L�X�g�ւ̃n���h��
TCHAR ch = ' ';		//���͂��ꂽ����
RECT rect;			//Rectangle�\����
PAINTSTRUCT ps;		//WM_PAINT�Ŏg�p�����
bool vkKeys[256];	//���z�L�[�̏�ԁ@true or false

//�ϐ�
const char CLASS_NAME[] = "keyboard";
const char APP_TITLE[] = "KeysDown";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

#define CHILD_ID 1


//=============================================================
//Windows�A�v���P�[�V�����̊J�n�X
//�p�����[�^
//	hInstance:		�A�v���P�[�V�����̌��݂̃C���X�^���X�ւ̃n���h��
//	hPrevInstance:  ��ɂm�t�k�k�i�p�~���ꂽ�p�����[�^�j
//	lpCmdLine:		�R�}���h���C��������NULL�I�[������ւ̃|�C���^
//	nCmdShow:		�E�B���h�E���ǂ̂悤�ɕ\�����邩���w��
//=============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	// ������O�ɃC���X�^���X����������Ă��Ȃ������ׂ�
    if (AnotherInstance())
        return false;

	//�E�B���h�E���쐬
	if(!CreateMainWindow(hInstance, nCmdShow))
		return false;


	int done = 0;
	while (!done)
	{
		//PeelMessage�̓��b�Z�[�W���Ȃ��ꍇ�͏�����҂����Ƀ��[�v�����Ă���
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT)
				done = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

//=====================================================
//�E�B���h�E�R�[���o�b�N�֐�
//=====================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
	case WM_DESTROY:
		//Windows�ɂ��̃v���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
        switch(wParam)
		{
                //�������̃{�^���̎q�E�B���h�EID�Ȃ�
            case CHILD_ID:
                MessageBoxA(NULL,"�{�^����������܂���","���b�Z�[�W����",MB_OK);
                return 0;
        }
        break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//============================================================
//�E�B���h�E���쐬
//�߂�l:�G���[�̏ꍇ�Afalse
//============================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow) 
{
	WNDCLASSEX wcx;
	HWND hwnd;
	HWND hwnd_button;
	
	//�E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肷��B
	wcx.cbSize = sizeof(wcx);						//�\���̂̃T�C�Y
	wcx.style = CS_HREDRAW | CS_VREDRAW;			//�E�B���h�E�T�C�Y�ύX���ɍĕ`��
	wcx.lpfnWndProc = WinProc;						//�E�B���h�E�v���V�[�W�����w��
	wcx.cbClsExtra = 0;								//�g���N���X�������@�Ȃ�
	wcx.cbWndExtra = 0;								//�g���E�B���h�E�������@�Ȃ�
	wcx.hInstance = hInstance;						//�C���X�^���X�ւ̃n���h��
	wcx.hIcon = NULL;								//
	wcx.hCursor = LoadCursor(NULL,IDC_ARROW);		//���J�[�\���̐ݒ�
	wcx.hbrBackground =
		(HBRUSH)GetStockObject(GRAY_BRUSH);		//�w�i�u���V
	wcx.lpszMenuName = NULL;						//���j���[���\�[�X�̖��O
	wcx.lpszClassName = CLASS_NAME;					//�E�B���h�E�N���X�̖��O
	wcx.hIconSm = NULL;								//�������A�C�R���@�Ȃ�
	

	//�E�B���h�E�N���X�̓o�^
	//�G���[���ARegisterClassEx�͂O��߂�
	if(RegisterClassEx(&wcx) == 0)
		return false;

	hwnd = CreateWindow(
		CLASS_NAME,					//�E�B���h�E�N���X�̖��O
		APP_TITLE,					//�^�C�g���o�[�̃e�L�X�g
		WS_OVERLAPPEDWINDOW,		//�E�B���h�E�̃X�^�C��
		0,				//�E�B���h�E�̐����ʒu�@�f�t�H���g
		0,				//�E�B���h�E�̐����ʒu�@�f�t�H���g
		WINDOW_WIDTH,				//�E�B���h�E�̕�
		WINDOW_HEIGHT,				//�E�B���h�E�̍���
		NULL,					//�e�E�B���h�E�@�Ȃ�
		NULL,				//���j���[�@�Ȃ�
		hInstance,					//�A�v���P�[�V�����C���X�^���X�ւ̃n���h��
		NULL);				//�E�B���h�E�p�����[�^�Ȃ�

	hwnd_button=CreateWindow("button","�{�^��",WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50,50,100,100,hwnd,(HMENU)CHILD_ID,hInstance,NULL);


	//�E�B���h�E�̍쐬�ŃG���[���������ꍇ
	if(!hwnd || !hwnd_button)
		return false;

	//�E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);

	//�E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);

	return true;
}

//===============================================================
//���݂̃A�v���P�[�V�����̕ʂ̃C���X�^���X���Ȃ������`�F�b�N
//�߂�l:�ʂ̃C���X�^���X�����������ꍇ = true
//		 ���g���B��̃C���X�^���X�ł���ꍇ = false
//===============================================================
bool AnotherInstance()
{
	HANDLE ourMutex;
	//�ŗL�̕�������g���ă~���[�e�b�N�X�̍쐬�����݂�
	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161-XYZZY");

	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	//�ʂ̃C���X�^���X�����������ꍇ
	return false;		//���g���B��̃C���X�^���X�ł���ꍇ
}