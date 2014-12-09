#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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

	short nVirtKey;                 //���z�L�[�R�[�h 
    const short SHIFTED = (short)0x8000; 
    DWORD chWidth = 20;             // width of characters
    DWORD chHeight = 20;            // height of characters


	switch(msg){
	case WM_CHAR:				//�L�[�{�[�h���當�������͂��ꂽ�ꍇ
		switch(wParam)			//wParam�ɕ������i�[����Ă���
		{
		case 0x08:				//�o�b�N�X�y�[�X
		case 0x09:				//�^�u
		case 0x0A:				//���C���t�B�[�h
		case 0x0D:				//�L�����b�W���^�[��
		case 0x1B:				//�G�X�P�[�v
			MessageBeep((UINT)-1);	//�r�[�v����炷�A�\���͂��Ȃ�
			return 0;
		default:				//�\���\�ȕ���
			ch = (TCHAR)wParam;			//�������擾
			//WM_PAINT�������I�ɔ���������
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		}
	
	case WM_KEYDOWN:					//�L�[�������ꂽ�ꍇ
		vkKeys[wParam] = true;
		switch(wParam)
		{
		case VK_SHIFT:					//Shift�L�[
			nVirtKey = GetKeyState(VK_LSHIFT);	//���V�t�g�L�[�̏�Ԃ��擾
			if(nVirtKey & SHIFTED)				//���V�t�g�L�[�̏ꍇ
				vkKeys[VK_LSHIFT] = true;
			nVirtKey = GetKeyState(VK_RSHIFT);	//�E�V�t�g�L�[�̏�Ԃ��擾
			if(nVirtKey & SHIFTED)				//�E�V�t�g�L�[�̏ꍇ
				vkKeys[VK_RSHIFT] = true;
			break;

		case VK_CONTROL:				//CTR�L�[
			nVirtKey = GetKeyState(VK_LCONTROL);//��CTRL�L�[�̏ꍇ
			if(nVirtKey & SHIFTED)				//��CTRL�[�̏ꍇ
				vkKeys[VK_LCONTROL] = true;
			nVirtKey = GetKeyState(VK_RCONTROL);//�E��CTRL�L�[�̏ꍇ
			if(nVirtKey & SHIFTED)				//�ECTRL�L�[�̏ꍇ
				vkKeys[VK_RSHIFT] = true;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);		//WM_PAINT�������I�ɔ���������
		return 0;
		break;

	case WM_KEYUP:								//�L�[�������ꂽ�ꍇ
		vkKeys[wParam] = false;
		switch(wParam)
		{
		case VK_SHIFT:					//Shift�L�[
			nVirtKey = GetKeyState(VK_LSHIFT);	//���V�t�g�L�[�̏�Ԃ��擾
			if((nVirtKey & SHIFTED) == 0)				//���V�t�g�L�[�̏ꍇ
				vkKeys[VK_LSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);	//�E�V�t�g�L�[�̏�Ԃ��擾
			if((nVirtKey & SHIFTED) == 0)				//�E�V�t�g�L�[�̏ꍇ
				vkKeys[VK_RSHIFT] = false;
			break;

		case VK_CONTROL:				//CTR�L�[
			nVirtKey = GetKeyState(VK_LCONTROL);//��CTRL�L�[�̏ꍇ
			if((nVirtKey & SHIFTED) == 0)				//��CTRL�[�̏ꍇ
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);//�E��CTRL�L�[�̏ꍇ
			if((nVirtKey & SHIFTED) == 0)				//�ECTRL�L�[�̏ꍇ
				vkKeys[VK_RSHIFT] = false;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);		//WM_PAINT�������I�ɔ���������
		return 0;
		break;

		case WM_PAINT:	//�E�B���h�E���ĕ`�悷��K�v������ꍇ
					//�f�o�C�X�R���e�L�X�g�ւ̃n���h�����擾
		hdc = BeginPaint(hwnd, &ps);
		//GetClientRect(hwnd, &rect);			//�E�B���h�E�̋�`���擾


		//������\��
		TextOut(hdc, 0, 0, &ch, 1);

		for (int r=0; r<16; r++)
            {
                for (int c=0; c<16; c++)
                {
                    if (vkKeys[r*16+c])
                    {
                        SetBkMode(hdc, OPAQUE);         // opaque text background
                        TextOut(hdc,c*chWidth+chWidth*2,r*chHeight+chHeight*2,"T ", 2);
                    } else {
                        SetBkMode(hdc, TRANSPARENT);    // transparent text background
                        TextOut(hdc,c*chWidth+chWidth*2,r*chHeight+chHeight*2,"F ", 2);
                    }
                }
            }

		EndPaint(hwnd, &ps);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);



	case WM_DESTROY:
		//Windows�ɂ��̃v���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;
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

	//�E�B���h�E�̍쐬�ŃG���[���������ꍇ
	if(!hwnd)
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