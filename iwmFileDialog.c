//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024 iwm-iwama"
#define   IWM_VERSION         "iwmFileDialog_20240428"
//------------------------------------------------------------------------------
#include "lib_iwmutil2.h"

INT       main();
VOID      print_version();
VOID      print_help();

#define   PWSH                "pwsh.exe"
#define   PWSHW               L"" PWSH ""

#define   POWERSHELL          "powershell.exe"
#define   POWERSHELLW         L"" POWERSHELL ""

INT
main()
{
	// lib_iwmutil2 初期化
	imain_begin();

	///iCLI_VarList();

	// -h | --help
	if(iCLI_getOptMatch(0, L"-h", L"--help"))
	{
		print_help();
		imain_end();
	}

	// -v | --version
	if(iCLI_getOptMatch(0, L"-v", L"--version"))
	{
		print_version();
		imain_end();
	}

	WS *pwshW = NULL;
	INT pwshFlg = 0; // 0=なし／1=powershell.exe／2=pwsh.exe

	MS *mp1 = 0;
	WS *wp1 = 0;

	// 'PowerShell' Installed ?
	mp1 = ims_popenW(L"where " PWSHW);
		wp1 = M2W(mp1);
			if(*iwp_searchPos(wp1, PWSHW, TRUE))
			{
				pwshFlg = 2;
				pwshW = PWSHW;
			}
		ifree(wp1);
	ifree(mp1);

	if(pwshFlg < 2)
	{
		mp1 = ims_popenW(L"where " POWERSHELLW);
			wp1 = M2W(mp1);
				if(*iwp_searchPos(wp1, POWERSHELLW, TRUE))
				{
					pwshFlg = 1;
					pwshW = POWERSHELLW;
				}
			ifree(wp1);
		ifree(mp1);
	}

	if(pwshFlg < 1)
	{
		// バッチファイル(CP932)で文字化けするのでSTDERRに出力
		fputs(
			"\033[0m"
			"\033[91m"
			"[Err] "
			"\033[37m"
			"このプログラムの実行には"
			"\033[93m"
			" PowerShell ("
			PWSH
			"／"
			POWERSHELL
			") "
			"\033[37m"
			"が必要です。"
			"\033[0m"
			"\n\n"
			, stderr
		);
		imain_end();
	}

	WS GblInitialDir[MAX_PATH] = L".";
	UINT GblCodepage = 65001;
	BOOL GblMultiSelect = TRUE;

	for(UINT _u1 = 0; _u1 < $ARGC; _u1++)
	{
		WS *_wp1 = 0;

		// -d=STR | -dir=STR
		if((_wp1 = iCLI_getOptValue(_u1, L"-d=", L"-dir=")))
		{
			if(iFchk_DirName(_wp1))
			{
				wcscpy(GblInitialDir, _wp1);
			}
			ifree(_wp1);
		}

		// -cp=NUM | -codepage=NUM
		if((_wp1 = iCLI_getOptValue(_u1, L"-cp=", L"-codepage=")))
		{
			GblCodepage = _wtoi(_wp1);
			ifree(_wp1);
		}

		// -ss | -singleselect
		if(iCLI_getOptMatch(_u1, L"-ss", L"-singleselect"))
		{
			GblMultiSelect = FALSE;
		}
	}

	// インライン・スクリプト
	WS *script = (
		L"%S -command \""
		L"[void][System.Reflection.Assembly]::LoadWithPartialName('system.windows.forms');"
		// 日本語メニュー CP932
		L"[Console]::OutputEncoding = [System.Text.Encoding]::GetEncoding(932);"
		L"$dialog = New-Object System.Windows.Forms.OpenFileDialog;"
		L"$dialog.Multiselect = %d;"
		L"$dialog.InitialDirectory = '%S';"
		// 出力 CP指定
		L"[Console]::OutputEncoding = [System.Text.Encoding]::GetEncoding(%d);"
		L"if($dialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::ok){"
		L"  $dialog.FileNames;"
		L"}else{"
		L"  Write-Host -NoNewline '';"
		L"};"
		L"\""
	);

	wp1 = iws_sprintf(script, pwshW, GblMultiSelect, GblInitialDir, GblCodepage);
		imv_systemW(wp1);
	ifree(wp1);

	///idebug_map(); ifree_all(); idebug_map();

	// 最終処理
	imain_end();
}

VOID
print_version()
{
	P1(IESC_STR2);
	LN(80);
	P(
		" %s\n"
		"    %s+%s\n"
		,
		IWM_COPYRIGHT,
		IWM_VERSION,
		LIB_IWMUTIL_VERSION
	);
	LN(80);
	P1(IESC_RESET);
}

VOID
print_help()
{
	MS *_cmd = "iwmFileDialog.exe";

	print_version();
	P(
		IESC_TITLE1	" ファイル選択ダイアログ "	IESC_RESET	"\n\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" [Option]\n\n\n"
		IESC_LBL1	" (例)\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" -dir=\"c:\\windows\" -codepage=932\n\n\n"
		,
		_cmd,
		_cmd
	);
	P1(
		IESC_OPT2	" [Option]\n"
		IESC_OPT21	"    -dir=STR | -d=STR\n"
		IESC_STR1	"        フォルダ指定\n\n"
		IESC_OPT21	"    -codepage=NUM | -cp=NUM\n"
		IESC_STR1	"        コードページ指定\n"
					"            UTF-8: 65001（初期値）／Shift_JIS: 932\n\n"
		IESC_OPT21	"    -singleselect | -ss\n"
		IESC_STR1	"        ファイル選択を１個に限定\n"
		IESC_STR2	"        ※初期値は 複数選択\n\n"
	);
	P1(IESC_STR2);
	LN(80);
	P1(IESC_RESET);
}
