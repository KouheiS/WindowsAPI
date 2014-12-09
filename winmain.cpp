#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define MSG(m) {MessageBoxA(NULL,m,NULL,MB_OK);}

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

#define CHILD_ID 1


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

	switch(msg){
	case WM_DESTROY:
		//Windowsにこのプログラムを終了するように伝える
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
        switch(wParam)
		{
                //さっきのボタンの子ウィンドウIDなら
            case CHILD_ID:
                MessageBoxA(NULL,"ボタンが押されました","メッセージ発生",MB_OK);
                return 0;
        }
        break;
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
	HWND hwnd_button;
	
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

	hwnd_button=CreateWindow("button","ボタン",WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50,50,100,100,hwnd,(HMENU)CHILD_ID,hInstance,NULL);


	//ウィンドウの作成でエラー発生した場合
	if(!hwnd || !hwnd_button)
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