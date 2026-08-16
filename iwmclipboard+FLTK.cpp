//------------------------------------------------------------------------------
#define IWM_COPYRIGHT "(C)2024-2026 iwm-iwama"
#define IWM_VERSION "iwmclipboard+FLTK_20260815"
//------------------------------------------------------------------------------
extern "C"
{
#include "lib_iwmutil2.h"
}
#include <FL/Fl.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>

//--------------------------------------------------------------------------------
// TEXT_BOX
//--------------------------------------------------------------------------------
VOID subClipboard_drop()
{
	WS *wp1 = M2W(Fl::event_text());
	WS **wa1 = iwsa_split(wp1, FALSE, 1, L"\n");
	$struct_iVBW *IVBW = iVBW_alloc();

	for (UINT _u1 = 0; _u1 < iwan_size(wa1); _u1++)
	{
		iVBW_push2(IVBW, wa1[_u1]);
		if (iF_chkDirName(wa1[_u1]))
		{
			if (wa1[_u1][wcslen(wa1[_u1]) - 1] != '\\')
			{
				iVBW_push2(IVBW, L"\\");
			}
		}
		iVBW_push2(IVBW, L"\n");
	}
	iClipboard_setText(iVBW_getStr(IVBW));
	iVBW_free2(IVBW);
	ifree2(wa1); // 配列
	ifree(wp1);
}

class
	TEXT_BOX : public Fl_Text_Display
{
public:
	TEXT_BOX(INT, INT, INT, INT, CONST MS *);

private:
	auto handle(INT) -> INT override;
};

TEXT_BOX::TEXT_BOX(
	INT x, INT y, INT w, INT h, CONST MS *lbl)
	: Fl_Text_Display(x, y, w, h, lbl)
{
	// ★FLTK 1.4対策: 未登録のフォントを指定すると描画時に即落ちるため、
	// 標準フォント（FL_HELVETICAなど）か、明示的なフォント番号（0など）に変えます。
	textfont(FL_HELVETICA);
	textcolor(FL_BLACK);
	textsize(14);
	linenumber_width(48);
}

INT TEXT_BOX::handle(
	INT event)
{
	switch (event)
	{
	case FL_DND_DRAG:
	case FL_DND_ENTER:
	case FL_DND_RELEASE:
	{
		return 1;
	}
	case FL_PASTE:
	{
		subClipboard_drop();
		// TEXT_BOX の表示更新は ChildThread() が行う
		return 1;
	}
	default:
	{
		return Fl_Text_Display::handle(event);
	}
	}
}

//--------------------------------------------------------------------------------
// WINDOW
//--------------------------------------------------------------------------------
Fl_Text_Buffer *TextBuf;
TEXT_BOX *TextBox1;

// メインスレッド側で安全にテキストを更新する関数
VOID UpdateTextCallback(
	VOID *data)
{
	MS *mp1 = (MS *)data;
	if (TextBuf && mp1)
	{
		TextBuf->text(mp1);
	}
	// 必須
	ifree(mp1);
}

// スレッドのループを制御するグローバルフラグ
volatile BOOL GblRunning = TRUE;

DWORD WINAPI ChildThread(VOID *)
{
	WS *oldWp1 = (WS *)L"\0";

	while (GblRunning)
	{
		WS *wp1 = iClipboard_getText();
		if (wcscmp(wp1, oldWp1) == 0)
		{
			ifree(wp1);
		}
		else
		{
			ifree(oldWp1);
			oldWp1 = wp1;
			MS *mp1 = W2M(oldWp1);
			Fl::awake(UpdateTextCallback, mp1);
			/// idebug_map(NULL);
		}
		// まとめて停止
		Sleep(2000);
	}
	ifree(oldWp1);
	return 0;
}

VOID subClipboard_getARGV()
{
	$struct_iVBW *IVBW = iVBW_alloc();
	for (UINT _u1 = 0; _u1 < $ARGC; _u1++)
	{
		iVBW_push2(IVBW, $ARGV[_u1]);
		if (iF_chkDirName($ARGV[_u1]))
		{
			iVBW_push2(IVBW, L"\\");
		}
		iVBW_push2(IVBW, L"\n");
	}
	iClipboard_setText(iVBW_getStr(IVBW));
	iVBW_free2(IVBW);
}

// ★修正: コマンドラインやFLTK内部のキャストを正常にするため「public」継承にします
class WINDOW : public Fl_Window
{
	HANDLE Thread_Handle;
	DWORD Thread_Id;

public:
	WINDOW(INT, INT, INT, INT, CONST MS *);
	~WINDOW();
};

WINDOW::WINDOW(INT x, INT y, INT w, INT h, CONST MS *title)
	: Fl_Window(x, y, w, h, title)
{
	TextBuf = new Fl_Text_Buffer();
	TextBox1 = new TEXT_BOX(0, 0, w, h, "");

	// 最初に一度だけバッファをテキストボックスに紐付ける
	TextBox1->buffer(TextBuf);

	resizable(*TextBox1);
	end();
	show();

	// 最前面に表示
	HWND hwnd = (HWND)fl_xid(this); // ウィンドウハンドルを取得
	if (hwnd)
	{
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	// Main Thread
	if ($ARGC)
	{
		subClipboard_getARGV();
	}

	// スレッド処理
	Thread_Handle = CreateThread(0, 0, ChildThread, 0, 0, &Thread_Id);
	SetThreadPriority(Thread_Handle, THREAD_PRIORITY_IDLE);
}

WINDOW::~WINDOW()
{
	// 1. まずスレッドに終了を通知
	GblRunning = FALSE;

	// 2. スレッドが完全に終わるのを安全に待機
	if (Thread_Handle)
	{
		WaitForSingleObject(Thread_Handle, INFINITE);
		CloseHandle(Thread_Handle);
	}

	// 3. テキストバッファの解放
	if (TextBuf != nullptr)
	{
		delete TextBuf;
	}
}

//--------------------------------------------------------------------------------
// main()
//--------------------------------------------------------------------------------
INT main()
{
	// lib_iwmutil2 初期化
	imain_begin();

	// ★FLTK 1.4対策: ウィンドウを作る前に、メインスレッドの最初でマルチスレッドを有効化する
	Fl::lock();

	CONST INT sizeW = 480;
	CONST INT sizeH = 240;

	// ウィンドウの生成と表示
	WINDOW win(
		((Fl::w() - sizeW) / 2),
		((Fl::h() - sizeH) / 2),
		sizeW,
		sizeH,
		"クリップボードの内容");

	// FLTKのメインループを開始（ここで自動的にロックが管理されます）
	Fl::run();

	// 最終処理
	imain_end();
}
