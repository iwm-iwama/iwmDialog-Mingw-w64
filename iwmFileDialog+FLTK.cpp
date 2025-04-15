//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024-2025 iwm-iwama"
#define   IWM_FILENAME        "iwmFileDialog+FLTK"
#define   IWM_UPDATE          "20250413"
//------------------------------------------------------------------------------
extern "C"
{
	#include "lib_iwmutil2.h"
}
#include <FL/Fl_Native_File_Chooser.H>

INT       main();
VOID      subFLTK(CONST MS *mpTitle, UINT uType, CONST MS *mpDir, UINT uCP);
VOID      subPrintVersion();
VOID      subPrintHelp();

#define   TITLE_BROWSE_DIRECTORY        "単一フォルダ選択"
#define   TITLE_BROWSE_SAVE_DIRECTORY   "保存フォルダ選択"
#define   TITLE_BROWSE_FILE             "単一ファイル選択"
#define   TITLE_BROWSE_MULTI_FILE       "複数ファイル選択"
#define   TITLE_BROWSE_SAVE_FILE        "保存ファイル選択"

INT
main()
{
	// lib_iwmutil2 初期化
	imain_begin();

	///iCLI_VarList();

	// -h | --help
	if(! $ARGC || iCLI_getOptMatch(0, L"-h", L"--help"))
	{
		subPrintVersion();
		subPrintHelp();
		Sleep(2000);
		imain_end();
	}

	// -v | --version
	if(iCLI_getOptMatch(0, L"-v", L"--version"))
	{
		subPrintVersion();
		imain_end();
	}

	// 初期化
	MS *GblTitle = 0;
	INT GblType = 0;
	MS *GblDir = icalloc_MS(MAX_PATH);
		GetCurrentDirectory(MAX_PATH, GblDir);
	INT GblCodepage = 65001;

	for(UINT _u1 = 0; _u1 < $ARGC ; _u1++)
	{
		WS *_wp1 = 0;

		// -type=STR
		if((_wp1 = iCLI_getOptValue(_u1, L"-t=", L"-type=")))
		{
			// BROWSE_DIRECTORY
			if(! wcscmp(_wp1, L"d"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_DIRECTORY);
				GblType = Fl_Native_File_Chooser::BROWSE_DIRECTORY;
			}
			// BROWSE_SAVE_DIRECTORY
			else if(! wcscmp(_wp1, L"D"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_SAVE_DIRECTORY);
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY;
			}
			// BROWSE_FILE
			else if(! wcscmp(_wp1, L"f"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_FILE;
			}
			// BROWSE_MULTI_FILE
			else if(! wcscmp(_wp1, L"m"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_MULTI_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_MULTI_FILE;
			}
			// BROWSE_SAVE_FILE
			else if(! wcscmp(_wp1, L"F"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_SAVE_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_FILE;
			}
		}
		// -dir=STR
		else if((_wp1 = iCLI_getOptValue(_u1, L"-d=", L"-dir=")))
		{
			if(iF_chkDirName(_wp1))
			{
				GblDir = W2M(_wp1);
			}
		}
		// -cp=NUM
		else if((_wp1 = iCLI_getOptValue(_u1, L"-cp=", L"-codepage=")))
		{
			GblCodepage = _wtoi(_wp1);
		}
	}

	if(GblType)
	{
		subFLTK(GblTitle, GblType, GblDir, GblCodepage);
	}
	else
	{
		subPrintHelp();
	}

	// Debug
	///idebug_map();
	///ifree_all();
	///idebug_map();

	// 最終処理
	imain_end();
}

VOID
subFLTK(
	CONST MS *mpTitle,
	UINT uType,
	CONST MS *mpDir,
	UINT uCP
)
{
	Fl_Native_File_Chooser fnfc;
	fnfc.title(mpTitle);
	fnfc.type(uType);
	fnfc.directory(mpDir);
	fnfc.filter(
		"All Files\t*.*"
		///"\n"
		///"Image\t*{.png,*.jpg,*.jpeg}"
	);

	switch(fnfc.show())
	{
		case -1:
		case  1:
			P1("");
			break;

		default:
			$struct_iVBM *iVBM = iVBM_alloc();
				for(int _i1 = 0; _i1 < fnfc.count(); _i1++)
				{
					iVBM_push2(iVBM, fnfc.filename(_i1));
					// Dirのとき
					if(uType == Fl_Native_File_Chooser::BROWSE_DIRECTORY || uType == Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY)
					{
						// "c:" のとき "c:\" が返されるため
						if(*(iVBM_getStr(iVBM) + iVBM_getLength(iVBM) - 1) != '\\')
						{
							iVBM_push(iVBM, "\\", 1);
						}
					}
					iVBM_push(iVBM, "\n", 1);
				}
				if(uCP == 932)
				{
					WS *wp1 = M2W(iVBM_getStr(iVBM));
					MS *mp1 = icnv_W2M(wp1, uCP);
					ifree(wp1);
						QP2(mp1);
					ifree(mp1);
				}
				else
				{
					QP(iVBM_getStr(iVBM), iVBM_getLength(iVBM));
				}
			iVBM_freeAll(iVBM);
			break;
	}
}

VOID
subPrintVersion()
{
	P1(IESC_STR2);
	LN(80);
	P1(
		"\033[2G"	IWM_COPYRIGHT	"\n"
		"\033[5G"	IWM_FILENAME	"_"	IWM_UPDATE	" + "	LIB_IWMUTIL_FILENAME	"\n"
	);
	LN(80);
	P1(IESC_RESET);
}

VOID
subPrintHelp()
{
	P1(
		"\033[1G"	IESC_TITLE1	" フォルダ／ファイル選択ダイアログ "	IESC_RESET	"\n"
		"\n"
		"\033[5G"	IESC_STR1	IWM_FILENAME	IESC_OPT2	" [Option]"	"\n"
		"\n"
		"\033[2G"	IESC_LBL1	"(例)"	"\n"
		"\033[5G"	IESC_STR1	IWM_FILENAME	IESC_OPT2	" -type=m -dir=\"c:\\windows\" -codepage=932"	"\n"
		"\n"
		"\033[2G"	IESC_OPT2	"[Option]"	"\n"
		"\033[5G"	IESC_OPT21	"-type=STR | -t=STR"	"\n"
		"\033[9G"	IESC_STR1	"ダイアログ"	IESC_LBL1	"（必須）"	"\n"
		IESC_STR2
		"\033[13G"	"d : "	TITLE_BROWSE_DIRECTORY	"\n"
		"\033[13G"	"D : "	TITLE_BROWSE_SAVE_DIRECTORY	"\n"
		"\033[13G"	"f : "	TITLE_BROWSE_FILE	"\n"
		"\033[13G"	"m : "	TITLE_BROWSE_MULTI_FILE	"\n"
		"\033[13G"	"F : "	TITLE_BROWSE_SAVE_FILE	"\n"
		"\n"
		"\033[5G"	IESC_OPT21	"-dir=STR | -d=STR"	"\n"
		"\033[9G"	IESC_STR1	"初期フォルダ指定"	IESC_LBL1	"（初期値 : Current Directory）"	"\n"
		"\n"
		"\033[5G"	IESC_OPT21	"-codepage=NUM | -cp=NUM"	"\n"
		"\033[9G"	IESC_STR1	"コードページ指定"	IESC_LBL1	"（初期値 : 65001）"	"\n"
		IESC_STR2
		"\033[13G"	"65001 : UTF-8"	"\n"
		"\033[13G"	"932   : Shift_JIS"	"\n"
		"\n"
	);
	P1(IESC_STR2);
	LN(80);
	P1(IESC_RESET);
}
