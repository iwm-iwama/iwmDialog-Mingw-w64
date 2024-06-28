//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024 iwm-iwama"
#define   IWM_FILENAME        "iwmFileDialog+FLTK"
#define   IWM_UPDATE          "20240621"
//------------------------------------------------------------------------------
extern "C"
{
	#include "lib_iwmutil2.h"
}
#include <FL/Fl_Native_File_Chooser.H>

INT       main();
VOID      subFLTK(CONST MS *mpTitle, UINT uType, CONST MS *mpDir, UINT uCP);
VOID      print_version();
VOID      print_help();

#define   TITLE_BROWSE_FILE             "ファイル選択（単一）"
#define   TITLE_BROWSE_MULTI_FILE       "ファイル選択（複数）"
#define   TITLE_BROWSE_DIRECTORY        "フォルダ選択（単一）"
#define   TITLE_BROWSE_SAVE_FILE        "ファイルを指定"
#define   TITLE_BROWSE_SAVE_DIRECTORY   "フォルダを指定"

INT
main()
{
	// lib_iwmutil2 初期化
	imain_begin();

	///iCLI_VarList();

	// -h | --help
	if(! $ARGC || iCLI_getOptMatch(0, L"-h", L"--help"))
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

	// 初期化
	BOOL GblOptFlg = FALSE;
	MS *GblTitle = 0;
	INT GblType = 0;
	MS *GblDir = 0;
	INT GblCodepage = 65001;

	for(UINT _u1 = 0; _u1 < $ARGC ; _u1++)
	{
		WS *_wp1 = 0;

		// -x
		if(_u1 == 0 && iCLI_getOptMatch(0, L"-x", NULL))
		{
			GblOptFlg = TRUE;
			break;
		}
		// -type=STR
		else if((_wp1 = iCLI_getOptValue(_u1, L"-type=", NULL)))
		{
			GblOptFlg = TRUE;

			// BROWSE_FILE
			if(iwb_cmpp(_wp1, L"f"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_FILE;
			}
			// BROWSE_DIRECTORY
			else if(iwb_cmpp(_wp1, L"d"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_DIRECTORY);
				GblType = Fl_Native_File_Chooser::BROWSE_DIRECTORY;
			}
			// BROWSE_SAVE_FILE
			else if(iwb_cmpp(_wp1, L"sf"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_SAVE_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_FILE;
			}
			// BROWSE_SAVE_DIRECTORY
			else if(iwb_cmpp(_wp1, L"sd"))
			{
				GblTitle = ims_clone(TITLE_BROWSE_SAVE_DIRECTORY);
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY;
			}
			// BROWSE_MULTI_FILE
			else
			{
				GblTitle = ims_clone(TITLE_BROWSE_MULTI_FILE);
				GblType = Fl_Native_File_Chooser::BROWSE_MULTI_FILE;
			}

			ifree(_wp1);
		}
		// -dir=STR
		else if((_wp1 = iCLI_getOptValue(_u1, L"-dir=", NULL)))
		{
			GblOptFlg = TRUE;

			if(iFchk_DirName(_wp1))
			{
				GblOptFlg = TRUE;
				GblDir = W2M(_wp1);
			}
			else
			{
				GblOptFlg = FALSE;
				GblDir = ims_clone(".");
			}

			ifree(_wp1);
		}
		// -cp=NUM
		else if((_wp1 = iCLI_getOptValue(_u1, L"-cp=", NULL)))
		{
			GblOptFlg = TRUE;
			GblCodepage = _wtoi(_wp1);
			ifree(_wp1);
		}
	}

	if(GblOptFlg)
	{
		subFLTK(GblTitle, GblType, GblDir, GblCodepage);
	}
	else
	{
		print_help();
	}

	// 処理時間
	///P("-- %.3fsec\n\n", iExecSec_next());

	///idebug_map(); ifree_all(); idebug_map();

	// 最終処理
	imain_end();
}

VOID subFLTK(
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
			$struct_iVBM *IVBM = iVBM_alloc();
				for(int _i1 = 0; _i1 < fnfc.count(); _i1++)
				{
					iVBM_add(IVBM, fnfc.filename(_i1));
					// Dirのとき
					if(uType == Fl_Native_File_Chooser::BROWSE_DIRECTORY || uType == Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY)
					{
						// "c:" のとき "c:\" が返されるため
						if(*(iVBM_getStr(IVBM) + iVBM_getLength(IVBM) - 1) != '\\')
						{
							iVBM_add(IVBM, "\\");
						}
					}
					iVBM_add(IVBM, "\n");
				}
				if(uCP == 65001)
				{
					QP(iVBM_getStr(IVBM), iVBM_getLength(IVBM));
				}
				else
				{
					WS *wp1 = M2W(iVBM_getStr(IVBM));
						MS *mp1 = icnv_W2M(wp1, uCP);
							QP1(mp1);
						ifree(mp1);
					ifree(wp1);
				}
			iVBM_free(IVBM);
			break;
	}
}

VOID
print_version()
{
	P1(IESC_STR2);
	LN(80);
	P1(
		" " IWM_COPYRIGHT "\n"
		"    " IWM_FILENAME "_" IWM_UPDATE " + " LIB_IWMUTIL_FILENAME "\n"
	);
	LN(80);
	P1(IESC_RESET);
}

VOID
print_help()
{
	print_version();
	P1(
		IESC_TITLE1	" フォルダ／ファイル選択ダイアログ " IESC_RESET "\n\n"
		IESC_STR1	"    " IWM_FILENAME
		IESC_OPT2	" [Option]\n\n\n"
		IESC_LBL1	" (例１) オプション指定\n"
		IESC_STR1	"    " IWM_FILENAME
		IESC_OPT2	" -type=f -dir=\"c:\\windows\" -cp=932\n\n"
		IESC_LBL1	" (例２) オプション初期値で実行\n"
		IESC_STR1	"    " IWM_FILENAME
		IESC_OPT2	" -x\n\n\n"
	);
	P1(
		IESC_OPT2	" [Option]\n"
		IESC_OPT21	"    -type=STR\n"
		IESC_STR1	"        ダイアログ（初期値：mf）\n"
	);
	P1(
		IESC_STR2	"            f  = " TITLE_BROWSE_FILE "\n"
					"            mf = " TITLE_BROWSE_MULTI_FILE "\n"
					"            d  = " TITLE_BROWSE_DIRECTORY "\n"
					"            sf = " TITLE_BROWSE_SAVE_FILE "\n"
					"            sd = " TITLE_BROWSE_SAVE_DIRECTORY "\n\n"
	);
	P1(
		IESC_OPT21	"    -dir=STR\n"
		IESC_STR1	"        初期フォルダ指定（初期値：\".\"）\n\n"
		IESC_OPT21	"    -cp=NUM\n"
		IESC_STR1	"        コードページ指定（初期値：65001）\n"
		IESC_STR2	"            65001 = UTF-8\n"
					"            932   = Shift_JIS\n\n"
	);
	P1(IESC_STR2);
	LN(80);
	P1(IESC_RESET);

	Sleep(2000);
}
