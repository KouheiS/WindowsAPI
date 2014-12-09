#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();
//グローバル変数
HINSTANCE hinst;
HDC hdc;			//デバイスコンテキストへのハンドル
TCHAR ch = ' ';		//入力された文字
RECT rect;			//Rectangle構造体
PAINTSTRUCT ps;		//WM_PAINTで使用される
bool vkKeys[256];	//仮想キーの状態　true or false

//変数
const char CLASS_NAME[] = "keyboard";
const char APP_TITLE[] = "KeysDown";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;


//=============================================================
//Windowsアプリケーションの開始店
//パラメータ
//	hInstance:		アプリケーションの現在のインスタンスへのハンドル
//	hPrevInstance:  常にＮＵＬＬ（廃止されたパラメータ）
//	lpCmdLine:		コマンドライン引数のNULL終端文字列へのポインタ
//	nCmdShow:		ウィンドウをどのように表示するかを指定
//=============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	// これより前にインスタンスが生成されていないか調べる
    if (AnotherInstance())
        return false;

	//ウィンドウを作成
	if(!CreateMainWindow(hInstance, nCmdShow))
		return false;

	int done = 0;
	while (!done)
	{
		//PeelMessageはメッセージがない場合は処理を待たずにループさせていく
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
//ウィンドウコールバック関数
//=====================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	short nVirtKey;                 //仮想キーコード 
    const short SHIFTED = (short)0x8000; 
    DWORD chWidth = 20;             // width of characters
    DWORD chHeight = 20;            // height of characters


	switch(msg){
	case WM_CHAR:				//キーボードから文字が入力された場合
		switch(wParam)			//wParamに文字が格納されている
		{
		case 0x08:				//バックスペース
		case 0x09:				//タブ
		case 0x0A:				//ラインフィード
		case 0x0D:				//キャリッジリターン
		case 0x1B:				//エスケープ
			MessageBeep((UINT)-1);	//ビープ音を鳴らす、表示はしない
			return 0;
		default:				//表示可能な文字
			ch = (TCHAR)wParam;			//文字を取得
			//WM_PAINTを強制的に発生させる
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		}
	
	case WM_KEYDOWN:					//キーが押された場合
		vkKeys[wParam] = true;
		switch(wParam)
		{
		case VK_SHIFT:					//Shiftキー
			nVirtKey = GetKeyState(VK_LSHIFT);	//左シフトキーの状態を取得
			if(nVirtKey & SHIFTED)				//左シフトキーの場合
				vkKeys[VK_LSHIFT] = true;
			nVirtKey = GetKeyState(VK_RSHIFT);	//右シフトキーの状態を取得
			if(nVirtKey & SHIFTED)				//右シフトキーの場合
				vkKeys[VK_RSHIFT] = true;
			break;

		case VK_CONTROL:				//CTRキー
			nVirtKey = GetKeyState(VK_LCONTROL);//左CTRLキーの場合
			if(nVirtKey & SHIFTED)				//左CTRLーの場合
				vkKeys[VK_LCONTROL] = true;
			nVirtKey = GetKeyState(VK_RCONTROL);//右のCTRLキーの場合
			if(nVirtKey & SHIFTED)				//右CTRLキーの場合
				vkKeys[VK_RSHIFT] = true;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);		//WM_PAINTを強制的に発生させる
		return 0;
		break;

	case WM_KEYUP:								//キーが離された場合
		vkKeys[wParam] = false;
		switch(wParam)
		{
		case VK_SHIFT:					//Shiftキー
			nVirtKey = GetKeyState(VK_LSHIFT);	//左シフトキーの状態を取得
			if((nVirtKey & SHIFTED) == 0)				//左シフトキーの場合
				vkKeys[VK_LSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);	//右シフトキーの状態を取得
			if((nVirtKey & SHIFTED) == 0)				//右シフトキーの場合
				vkKeys[VK_RSHIFT] = false;
			break;

		case VK_CONTROL:				//CTRキー
			nVirtKey = GetKeyState(VK_LCONTROL);//左CTRLキーの場合
			if((nVirtKey & SHIFTED) == 0)				//左CTRLーの場合
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);//右のCTRLキーの場合
			if((nVirtKey & SHIFTED) == 0)				//右CTRLキーの場合
				vkKeys[VK_RSHIFT] = false;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);		//WM_PAINTを強制的に発生させる
		return 0;
		break;

		case WM_PAINT:	//ウィンドウを再描画する必要がある場合
					//デバイスコンテキストへのハンドルを取得
		hdc = BeginPaint(hwnd, &ps);
		//GetClientRect(hwnd, &rect);			//ウィンドウの矩形を取得


		//文字を表示
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
		//Windowsにこのプログラムを終了するように伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//============================================================
//ウィンドウを作成
//戻り値:エラーの場合、false
//============================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow) 
{
	WNDCLASSEX wcx;
	HWND hwnd;
	
	//ウィンドウクラスの構造体をメインウィンドウを記述するパラメータで設定する。
	wcx.cbSize = sizeof(wcx);						//構造体のサイズ
	wcx.style = CS_HREDRAW | CS_VREDRAW;			//ウィンドウサイズ変更時に再描画
	wcx.lpfnWndProc = WinProc;						//ウィンドウプロシージャを指す
	wcx.cbClsExtra = 0;								//拡張クラスメモリ　なし
	wcx.cbWndExtra = 0;								//拡張ウィンドウメモリ　なし
	wcx.hInstance = hInstance;						//インスタンスへのハンドル
	wcx.hIcon = NULL;								//
	wcx.hCursor = LoadCursor(NULL,IDC_ARROW);		//矢印カーソルの設定
	wcx.hbrBackground =
		(HBRUSH)GetStockObject(GRAY_BRUSH);		//背景ブラシ
	wcx.lpszMenuName = NULL;						//メニューリソースの名前
	wcx.lpszClassName = CLASS_NAME;					//ウィンドウクラスの名前
	wcx.hIconSm = NULL;								//小さいアイコン　なし
	

	//ウィンドウクラスの登録
	//エラー時、RegisterClassExは０を戻す
	if(RegisterClassEx(&wcx) == 0)
		return false;

	hwnd = CreateWindow(
		CLASS_NAME,					//ウィンドウクラスの名前
		APP_TITLE,					//タイトルバーのテキスト
		WS_OVERLAPPEDWINDOW,		//ウィンドウのスタイル
		0,				//ウィンドウの水平位置　デフォルト
		0,				//ウィンドウの垂直位置　デフォルト
		WINDOW_WIDTH,				//ウィンドウの幅
		WINDOW_HEIGHT,				//ウィンドウの高さ
		NULL,					//親ウィンドウ　なし
		NULL,				//メニュー　なし
		hInstance,					//アプリケーションインスタンスへのハンドル
		NULL);				//ウィンドウパラメータなし

	//ウィンドウの作成でエラー発生した場合
	if(!hwnd)
		return false;

	//ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);

	//ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);

	return true;
}

//===============================================================
//現在のアプリケーションの別のインスタンスがないかをチェック
//戻り値:別のインスタンスが見つかった場合 = true
//		 自身が唯一のインスタンスである場合 = false
//===============================================================
bool AnotherInstance()
{
	HANDLE ourMutex;
	//固有の文字列を使ってミューテックスの作成を試みる
	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161-XYZZY");

	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	//別のインスタンスが見つかった場合
	return false;		//自身が唯一のインスタンスである場合
}