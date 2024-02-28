//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024 iwm-iwama"
#define   IWM_VERSION         "iwmFileDialog_20240226"
//------------------------------------------------------------------------------
#include "lib_iwmutil2.h"

INT       main();
VOID      print_version();
VOID      print_help();

#define   BufSize   (MAX_PATH*1024)

INT
main()
{
	// lib_iwmutil2 初期化
	imain_begin();

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

	WS GblInitialDir[MAX_PATH] = { L"." };
	INT GblOptFlags = OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER;
	BOOL GblOptLongName = FALSE;

	for(UINT _u1 = 0; _u1 < $ARGC; _u1++)
	{
		// -d=STR | -dir=STR
		WS *wp1 = NULL;
		if((wp1 = iCLI_getOptValue(_u1, L"-d=", L"-dir=")))
		{
			if(iFchk_DirName(wp1))
			{
				wcscpy(GblInitialDir, wp1);
			}
			ifree(wp1);
		}

		// -s | -single
		if(iCLI_getOptMatch(_u1, L"-s", L"-single"))
		{
			GblOptFlags = OFN_HIDEREADONLY | OFN_EXPLORER;
		}

		// -l | -long
		if(iCLI_getOptMatch(_u1, L"-l", L"-long"))
		{
			GblOptLongName = TRUE;
		}
	}

	// PowerShellによるサンプル
	/*
		WS *script = \
			L"powershell -command \"" \
			L"[void][System.Reflection.Assembly]::LoadWithPartialName('system.windows.forms');" \
			L"<# メニュー CP932 #>;" \
			L"[Console]::OutputEncoding = [System.Text.Encoding]::GetEncoding(932);" \
			L"$dialog = New-Object System.Windows.Forms.OpenFileDialog;" \
			L"$dialog.Multiselect = %S;" \
			L"$dialog.InitialDirectory = '%S';" \
			L"if($dialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::ok){" \
			L"  $dialog.FileNames;" \
			L"}else{" \
			L"  Write-Host -NoNewline '';" \
			L"};" \
			L"\"";
		WS *wp1 = iws_sprintf(script, L"$true", L"c:\\windows");
			imv_system(wp1, TRUE);
		ifree(wp1);
	*/
	WS *wpFiles = icalloc_WS(BufSize);

	OPENFILENAMEW oFn = { 0 };
	oFn.lStructSize = sizeof(OPENFILENAMEW);
	oFn.hwndOwner = NULL;
	oFn.Flags = GblOptFlags;
	oFn.lpstrInitialDir = GblInitialDir;
	oFn.nFilterIndex = 0;
	oFn.lpstrFile = wpFiles;
	oFn.nMaxFile = BufSize;

	GetOpenFileNameW(&oFn);

	WS *wpEnd = wpFiles;
	UINT uResult = 0;

	if(*wpEnd)
	{
		// '\0' を '\n' に置換
		while(TRUE)
		{
			if(*wpEnd == '\0')
			{
				++uResult;
				*wpEnd = (INT)'\n';
				++wpEnd;
				if(*wpEnd == '\0')
				{
					break;
				}
			}
			++wpEnd;
		}

		// WS を MS に変換
		MS *mpFiles = icnv_W2M(wpFiles, GetACP());
		MS *mpEnd = mpFiles;
		ifree(wpFiles);

		// 結果 >= 2 のとき
		if(uResult >= 2)
		{
			// １行目は [Dir\]
			for(; *mpEnd != '\n'; mpEnd++);
			MS *mpDir = ims_pclone(mpFiles, mpEnd);
				++mpEnd;
				/*
				//	[Dir]\[File1]
				//	[Dir]\[File2]
				//	...
				//		と出力
				*/
				if(GblOptLongName)
				{
					MS *_mpBgn = mpEnd;
					while(*mpEnd)
					{
						if(*mpEnd == '\n')
						{
							P1(mpDir);
							P1("\\");
							*mpEnd = '\0';
							P2(_mpBgn);
							_mpBgn = mpEnd + 1;
						}
						++mpEnd;
					}
				}
				/*
				//	[Dir]\
				//	[File1]
				//	[File2]
				//	...
				//		と出力
				*/
				else
				{
					P1(mpDir);
					P2("\\");
					P1(mpEnd);
				}
			ifree(mpDir);
		}
		// 結果 == 1 のとき
		else if(uResult == 1)
		{
			if(GblOptLongName)
			{
				P1(mpEnd);
			}
			else
			{
				/*
				//	[Dir]\[File]
				//		を
				//	[Dir]\
				//	[File]
				//		に分割出力
				*/
				INT i1 = strlen(mpEnd) - 1;
				for(; i1 >= 0 && mpEnd[i1] != '\\'; i1--);
				mpEnd[i1] = '\0';
				P1(mpEnd);
				P2("\\");
				P1(mpEnd + i1 + 1);
			}
		}
		ifree(mpFiles);
	}
	// 結果 == 0 のとき
	else
	{
		P1("");
	}

	Sleep(1000);

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
		IESC_OPT2	" -dir=\".\" -single -long\n\n\n"
		,
		_cmd,
		_cmd
	);
	P1(
		IESC_OPT2	" [Option]\n"
		IESC_OPT21	"    -dir=STR | -d=STR\n"
		IESC_STR1	"        フォルダ指定\n\n"
		IESC_OPT21	"    -single | -s\n"
		IESC_STR1	"        ファイル選択を１個に限定\n"
		IESC_STR2	"        ※初期値は 複数選択\n\n"
		IESC_OPT21	"    -long | -l\n"
		IESC_STR1	"        フルパス名を出力\n"
		IESC_STR2	"        ※初期値は １行目：フォルダ名／２行目以降：ファイル名 を出力\n\n"
	);
	P1(IESC_STR2);
	LN(80);
	P1(IESC_RESET);
}
