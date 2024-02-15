//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024 iwm-iwama"
#define   IWM_VERSION         "iwmDirDialog_20240213"
//------------------------------------------------------------------------------
#include "lib_iwmutil2.h"

INT       main();
VOID      print_version();
VOID      print_help();

#define   PowerShell          L"powershell.exe"

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

	WS *wp1 = 0;

	// 'powershell.exe' or 'pwsh.exe' Installed ?
	wp1 = iws_popen(L"where " PowerShell);
		///PL2W(wp1);
		if(! iwn_searchi(wp1, PowerShell))
		{
			P1(
				"\033[0m"
				"\n"
				"\033[91m"
				"[Err] "
				"\033[37m"
				"このプログラムの実行には"
				"\033[93m"
				" Windows PowerShell (powershell.exe) "
				"\033[37m"
				"が必要です。"
				"\033[0m"
				"\n\n"
			);
			imain_end();
		}
	ifree(wp1);

	WS GblInitialDir[MAX_PATH] = { L"." };

	for(UINT _u1 = 0; _u1 < $ARGC; _u1++)
	{
		// -d=STR | -dir=STR
		WS *_wp1 = NULL;
		if((_wp1 = iCLI_getOptValue(_u1, L"-d=", L"-dir=")))
		{
			if(iFchk_DirName(_wp1))
			{
				wcscpy(GblInitialDir, _wp1);
			}
			ifree(_wp1);
		}
	}

	// インライン・スクリプト
	WS *script = \
		PowerShell \
		L" -command \"" \
		L"[void][System.Reflection.Assembly]::LoadWithPartialName('system.windows.forms');" \
		L"<# メニュー CP932 #>;" \
		L"[Console]::OutputEncoding = [System.Text.Encoding]::GetEncoding(932);" \
		L"$dialog = New-Object System.Windows.Forms.FolderBrowserDialog;" \
		L"$dialog.SelectedPath = '%S';" \
		L"$showdialog = $dialog.ShowDialog();" \
		L"<# 出力 CP65001 #>;" \
		L"[Console]::OutputEncoding = [System.Text.Encoding]::GetEncoding(65001);" \
		L"if($showdialog -eq [System.Windows.Forms.DialogResult]::ok){" \
		L"  Write-Host ($dialog.SelectedPath.TrimEnd('\\') + '\\');" \
		L"}else{" \
		L"  Write-Host -NoNewline '';" \
		L"};" \
		L"\"";

	wp1 = iws_sprintf(script, GblInitialDir);
		imv_system(wp1, TRUE);
	ifree(wp1);

	Sleep(1000);

	///icalloc_mapPrint(); ifree_all(); icalloc_mapPrint();

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
	MS *_cmd = "iwmDirDialog.exe";

	print_version();
	P(
		IESC_TITLE1	" フォルダ選択ダイアログ "	IESC_RESET	"\n\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" [Option]\n\n\n"
		IESC_LBL1	" (例)\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" -dir=\"c:\\windows\"\n\n\n"
		,
		_cmd,
		_cmd
	);
	P1(
		IESC_OPT2	" [Option]\n"
		IESC_OPT21	"    -dir=STR | -d=STR\n"
		IESC_STR1	"        フォルダ指定\n\n"
	);
	P1(IESC_STR2);
	LN(80);
	P1(IESC_RESET);
}
