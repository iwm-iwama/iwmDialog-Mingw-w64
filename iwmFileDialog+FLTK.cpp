//------------------------------------------------------------------------------
#define   IWM_COPYRIGHT       "(C)2024 iwm-iwama"
#define   IWM_VERSION         "iwmFileDialog+FLTK_202405018"
//------------------------------------------------------------------------------
extern "C"
{
	#include "lib_iwmutil2.h"
}
#include <FL/Fl_Native_File_Chooser.H>

INT       main();
VOID      subFLTK(UINT uType, CONST WS *wpDir, UINT uCP);
VOID      print_version();
VOID      print_help();

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
	INT GblType = Fl_Native_File_Chooser::BROWSE_MULTI_FILE;
	WS  GblInitialDir[MAX_PATH] = L".";
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
				GblType = Fl_Native_File_Chooser::BROWSE_FILE;
			}
			// BROWSE_MULTI_FILE
			else if(iwb_cmpp(_wp1, L"mf"))
			{
				GblType = Fl_Native_File_Chooser::BROWSE_MULTI_FILE;
			}
			// BROWSE_DIRECTORY
			else if(iwb_cmpp(_wp1, L"d"))
			{
				GblType = Fl_Native_File_Chooser::BROWSE_DIRECTORY;
			}
			// BROWSE_SAVE_FILE
			else if(iwb_cmpp(_wp1, L"sf"))
			{
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_FILE;
			}
			// BROWSE_SAVE_DIRECTORY
			else if(iwb_cmpp(_wp1, L"sd"))
			{
				GblType = Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY;
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
				wcscpy(GblInitialDir, _wp1);
			}
			else
			{
				GblOptFlg = FALSE;
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
		subFLTK(GblType, GblInitialDir, GblCodepage);
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
	UINT uType,
	CONST WS *wpDir,
	UINT uCP
)
{
	MS *mpDir = W2M(wpDir);
		Fl_Native_File_Chooser fnfc;
		fnfc.title("");
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
	ifree(mpDir);
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
	CONST MS *_cmd = "iwmFileDialog+FLTK.exe";

	print_version();
	P(
		IESC_TITLE1	" フォルダ／ファイル選択ダイアログ "	IESC_RESET	"\n\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" [Option]\n\n\n"
		IESC_LBL1	" (例１) オプション指定\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" -type=f -dir=\"c:\\windows\" -cp=932\n\n"
		IESC_LBL1	" (例２) オプション初期値で実行\n"
		IESC_STR1	"    %s"
		IESC_OPT2	" -x\n\n\n"
		,
		_cmd,
		_cmd,
		_cmd
	);
	P1(
		IESC_OPT2	" [Option]\n"
		IESC_OPT21	"    -type=STR\n"
		IESC_STR1	"        ダイアログ種（初期値：mf）\n"
		IESC_STR2	"            f  = ファイル選択(単一)\n"
					"            mf = ファイル選択(複数)\n"
					"            d  = フォルダ選択\n"
					"            sf = 保存先ファイル選択\n"
					"            sd = 保存先フォルダ選択\n\n"
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
