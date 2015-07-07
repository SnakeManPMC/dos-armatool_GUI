/*@@ Wedit generated application. Written Sat May 26 06:39:25 2007
 @@header: g:\coding\armatool_gui\armatool_guires.h
 @@resources: g:\coding\armatool_gui\armatool_gui.rc
 Do not edit outside the indicated areas */
/*<---------------------------------------------------------------------->*/
/*<---------------------------------------------------------------------->*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string.h>
#include "armatool_guires.h"
#include <stdio.h>
/* --- The following code comes from c:\lcc\lib\wizard\dlgbased.tpl. */
/*<---------------------------------------------------------------------->*/

void open(char*);
int WRP(HWND, char*);

/*
Template for a dialog based application. The main procedure for this
template is the DialogFunc below. Modify it to suit your needs.
*/
/* prototype for the dialog box function. */
static BOOL CALLBACK DialogFunc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
/*
Win main just registers a class of the same type that the dialog class, and
then calls DialogBox. Then it exits. The return value is the return value of
the dialog procedure.
*/

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	INITCOMMONCONTROLSEX cc;

	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc = DefDlgProc;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hInstance = hinst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszClassName = "armatool_gui";
	RegisterClass(&wc);
	memset(&cc,0,sizeof(cc));
	cc.dwSize = sizeof(cc);
	cc.dwICC = 0xffffffff;
	InitCommonControlsEx(&cc);

	return DialogBox(hinst, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC) DialogFunc);

}

/*
You should add your initialization code here. This function will be called
when the dialog box receives the WM_INITDIALOG message.
*/
static int InitializeApp(HWND hDlg,WPARAM wParam, LPARAM lParam)
{
	SetDlgItemTextA(hDlg, ID_eCellSize, "50");
	SetDlgItemTextA(hDlg, ID_eMapSize, "512");
	return 1;
}

/*
This is the main function for the dialog. It handles all messages. Do what your
application needs to do here.
*/
static BOOL CALLBACK DialogFunc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char fname[MAX_PATH];
	char buffer[5];
	switch (msg) {
	/* This message means the dialog is started but not yet visible.
	   Do All initializations here
        */
	case WM_INITDIALOG:
		InitializeApp(hwndDlg,wParam,lParam);
		return TRUE;
	/* By default, IDOK means close this dialog returning 1, IDCANCEL means
           close this dialog returning zero
        */
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDOK:
				GetDlgItemTextA(hwndDlg, ID_eCellSize, buffer, 5);
				// MessageBoxA(hwndDlg, buffer, 0, MB_OK);
				open((char*)&fname);
				WRP(hwndDlg,fname);
				return 1;
			case IDCANCEL:
				EndDialog(hwndDlg, 0);
				return 1;
		}
		break;
        /* By default, WM_CLOSE is equivalent to CANCEL */
	case WM_CLOSE:
		EndDialog(hwndDlg,0);
		return TRUE;

	}
	return FALSE;
}


void open(char* filename)
{
	OPENFILENAME fname;

	memset(&fname, 0, sizeof(fname));
	filename[0] = '\0';

	fname.lStructSize = sizeof(fname);
	fname.lpstrFilter = "All Files (*.*)\0*.*\0Txt Files (*.txt)\0*.txt\0Nfo Files (*.nfo)\0*.nfo\0Diz Files (*.diz)\0*.diz\0";
	fname.lpstrFile = filename;
	fname.nMaxFile = MAX_PATH;
	fname.Flags = OFN_EXPLORER;
	GetOpenFileName(&fname);
}



int WRP(HWND hwnd, char* filename)
{
	FILE *map;
	FILE *text;
	FILE *output;
	char sig[33];
	short Elevation=0,LowElev=0,HighElev=0,TexIndex=0;
	int MapSize=0,CellSize=50;

	map=fopen(filename,"rb");
	if (!map) {printf("error at test.wrp file\n");exit(1);}

	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)filename);

	output=fopen("test-ArmA.wrp","wb");
	if (!output) {printf("error at fixed file\n");exit(1);}

	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Opened test-ArmA.wrp");

	text=fopen("test-wrpdebug.txt","wt");
	if (!text) {printf("error at debug txt\n");exit(1);}

	fread(sig,4,1,map);
	sig[4]=0;
	fprintf(text, "signature: %s\n",sig);
	fwrite(sig,4,1,output);

	fread(&MapSize,4,1,map);
	fprintf(text, "MapSize x: %d\n",MapSize);
	fwrite(&MapSize,4,1,output);

	fread(&MapSize,4,1,map);
	fprintf(text, "MapSize y: %d\n",MapSize);
	fwrite(&MapSize,4,1,output);

	char tmp[32];
	sprintf(tmp,"Signature: %s",sig);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
	sprintf(tmp,"MapSize: %i",MapSize);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Reading elevations...");

	/* read elevations */
	fread(&Elevation,2,1,map);
	fwrite(&Elevation,2,1,output);
	for (int ic=0; ic < MapSize*MapSize; ic++) {
	fprintf(text, "elevation [%d]: %d\n",ic,Elevation/22);

		if (LowElev > Elevation) LowElev=Elevation;
		if (HighElev < Elevation) HighElev=Elevation;

		fread(&Elevation,2,1,map);
		fwrite(&Elevation,2,1,output);
	}

	sprintf(tmp,"%i Elevations read.",MapSize*MapSize);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);

	fprintf(text,"pmc1\n");


	/* read textures IDs */
	fread(&TexIndex,2,1,map);
	fwrite(&TexIndex,2,1,output);
	for (int ic=2; ic < MapSize*MapSize; ic++) {
	fprintf(text, "TexIndex [%d]: %d\n",ic,TexIndex);

		fread(&TexIndex,2,1,map);
		fwrite(&TexIndex,2,1,output);
	}

	sprintf(tmp,"%i Textures read.",TexIndex);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);

	fprintf(text,"pmc2\n");

	/* textures 32 char length and total of 512 */
	for (int ix=0; ix < 512; ix++) {
		sig[0]=0;
	fread(sig,32,1,map);
	fwrite(sig,32,1,output);
	fprintf(text, "texture name [%d]: %s\n",ix,sig);
	}

//	sprintf(tmp,"%i Textures read.",TexIndex);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Reading and converting 3dObjects...");

	fprintf(text,"\n****\nlowest elevation; %d, highest elevation: %d\n****\n",LowElev,HighElev);

	sprintf(tmp,"Lowest elevation; %d, highest elevation: %d",LowElev,HighElev);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);

	fprintf(text,"pmc3\n");

/*
	3d objects!
	9 floats, x coord, elevation, z coord, 3d object index, 3d object filename
	1,0,0,0,1,0,0,0,1 floats
	x/z coords 0 to 12800 in 256 map
	elevation 32768 and its "fit to ground".
*/
	char dObjName[76];
	float dDir,dDirX=0,dDirZ=0;
	long dObjIndex=0;

	while (!feof(map)) {

	dDir=1;
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	dDir=0;
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	dDir=1;
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	dDir=0;
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);
	dDir=1;
	fread(&dDir,4,1,map);
	fwrite(&dDir,4,1,output);

	/* X coord */
	fread(&dDirX,4,1,map);
	fwrite(&dDirX,4,1,output);

	/* here we do our magic for aling to ground */
	fread(&dDir,4,1,map);

	/* Z coord */
	fread(&dDirZ,4,1,map);

	dObjIndex++;
	fread(&dObjIndex,4,1,map);
	/* clear the objname variable */
	for (int i=0; i<76; i++)
        	dObjName[i]=0;

	fread(dObjName,76,1,map);
	/* start */
	strlwr(dObjName);
	if (strcmp (dObjName,"data3d\\afbarabizna.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\afbarabizna.p3d"); };
	if (strcmp (dObjName,"data3d\\afdum_mesto2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\afdum_mesto2.p3d"); };
	if (strcmp (dObjName,"data3d\\afdum_mesto2l.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\afdum_mesto2l.p3d"); };
	if (strcmp (dObjName,"data3d\\afdum_mesto3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\afdum_mesto3.p3d"); };
	if (strcmp (dObjName,"data3d\\afhospoda_mesto.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\afhospoda_mesto.p3d"); };
	if (strcmp (dObjName,"data3d\\dulni_bs.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dulni_bs.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_zboreny.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum_zboreny.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_zboreny_lidice.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum_zboreny_Lidice.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_zboreny_total.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum_zboreny_total.p3d"); };
	if (strcmp (dObjName,"data3d\\skala.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala.p3d"); };
	if (strcmp (dObjName,"data3d\\skala_new.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala_new.p3d"); };
	if (strcmp (dObjName,"data3d\\skala_newk.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala_newk.p3d"); };
	if (strcmp (dObjName,"data3d\\bouda1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\bouda1.p3d"); };
	if (strcmp (dObjName,"data3d\\bouda2_vnitrek.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\bouda2_vnitrek.p3d"); };
	if (strcmp (dObjName,"data3d\\bouda3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\bouda3.p3d"); };
	if (strcmp (dObjName,"data3d\\budova1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova1.p3d"); };
	if (strcmp (dObjName,"data3d\\budova2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova2.p3d"); };
	if (strcmp (dObjName,"data3d\\budova3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova3.p3d"); };
	if (strcmp (dObjName,"data3d\\budova4.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova4.p3d"); };
	if (strcmp (dObjName,"data3d\\budova4_in.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova4_in.p3d"); };
	if (strcmp (dObjName,"data3d\\budova5.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova5.p3d"); };
	if (strcmp (dObjName,"data3d\\cihlovej_dum.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Cihlovej_dum.p3d"); };
	if (strcmp (dObjName,"data3d\\cihlovej_dum_in.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Cihlovej_dum_in.p3d"); };
	if (strcmp (dObjName,"data3d\\cihlovej_dum_mini.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Cihlovej_dum_mini.p3d"); };
	if (strcmp (dObjName,"data3d\\deutshe.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Deutshe.p3d"); };
	if (strcmp (dObjName,"data3d\\deutshe_mini.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Deutshe_mini.p3d"); };
	if (strcmp (dObjName,"data3d\\dum01.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum01.p3d"); };
	if (strcmp (dObjName,"data3d\\dum02.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum02.p3d"); };
	if (strcmp (dObjName,"data3d\\dumruina.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dumruina.p3d"); };
	if (strcmp (dObjName,"data3d\\dumruina_mini.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dumruina_mini.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_m2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dum_m2.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_mesto.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dum_mesto.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_mesto2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dum_mesto2.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_mesto2L.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dum_mesto2L.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_mesto3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\Dum_mesto3.p3d"); };
	if (strcmp (dObjName,"data3d\\dum_olezlina.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\dum_olezlina.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala1_1.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala1_1.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala1_2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala1_2.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala1_3.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala1_3.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala1_4.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala1_4.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala1_5.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala1_5.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala2_1.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala2_1.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala2_2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala2_2.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala2_3.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala2_3.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala2_4.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala2_4.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala2_5.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala2_5.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala3_1.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala3_1.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala3_2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala3_2.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala3_3.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala3_3.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala3_4.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala3_4.p3d"); };
	if (strcmp (dObjName,"o\\skal\\skala3_5.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\skala3_5.p3d"); };
	if (strcmp (dObjName,"o\\tree\\akat02s.p3d") == 0) { strcpy (dObjName,"ca\\plants\\Akat02S.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces10 100.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 100.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces10 50.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 50.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces10 75.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 75.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces10 25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 25.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces25.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces12.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces12.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces6.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces6.p3d"); };
	if (strcmp (dObjName,"o\\road\\ces6konec.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces6konec.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil10 100.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil10 100.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil10 50.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil10 50.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil10 75.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil10 75.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil10 25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil10 25.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil25.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil12.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil12.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil6.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil6.p3d"); };
	if (strcmp (dObjName,"o\\road\\sil6konec.p3d") == 0) { strcpy (dObjName,"ca\\roads\\sil6konec.p3d"); };
	if (strcmp (dObjName,"o\\hous\\hangar_2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\hangar_2.p3d"); };
	if (strcmp (dObjName,"o\\tree\\smrk_maly.p3d") == 0) { strcpy (dObjName,"ca\\plants\\smrk_maly.p3d"); };
	if (strcmp (dObjName,"o\\tree\\smrk_siroky.p3d") == 0) { strcpy (dObjName,"ca\\plants\\smrk_siroky.p3d"); };
	if (strcmp (dObjName,"o\\tree\\smrk_velky.p3d") == 0) { strcpy (dObjName,"ca\\plants\\smrk_velky.p3d"); };
	if (strcmp (dObjName,"o\\tree\\dd_borovice.p3d") == 0) { strcpy (dObjName,"ca\\plants\\dd_borovice.p3d"); };
	if (strcmp (dObjName,"o\\tree\\dd_borovice02.p3d") == 0) { strcpy (dObjName,"ca\\plants\\dd_borovice02.p3d"); };
	if (strcmp (dObjName,"o\\tree\\les_singlestrom.p3d") == 0) { strcpy (dObjName,"ca\\plants\\les_singlestrom.p3d"); };
	if (strcmp (dObjName,"data3d\\hromada_kameni.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\hromada_kameni.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen1_piskove2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen1_piskove2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen1_piskovec.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen1_piskovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen1_piskovec2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen1_piskovec2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen1_zula.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen1_zula.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen2_piskovec.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen2_piskovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen2_piskovec2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen2_piskovec2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen3_piskovec.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen3_piskovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen3_piskovec2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen3_piskovec2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen3_zula.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen3_zula.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen4_piskovec.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen4_piskovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen4_piskovec2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen4_piskovec2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen4_zula.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen4_zula.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen5_piskovec.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen5_piskovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen5_piskovec2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen5_piskovec2.p3d"); };
	if (strcmp (dObjName,"data3d\\kamen5_zula.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kamen5_zula.p3d"); };
	if (strcmp (dObjName,"data3d\\kopa_kameni.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kopa_kameni.p3d"); };
	if (strcmp (dObjName,"data3d\\kopa_kameni2.p3d") == 0) { strcpy (dObjName,"ca\\rocks\\kopa_kameni2.p3d"); };
	if (strcmp (dObjName,"data3d\\hospital.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\hospital.p3d"); };
	if (strcmp (dObjName,"data3d\\fuelstation.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\fuelstation.p3d"); };
	if (strcmp (dObjName,"data3d\\fuelstation_army.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\fuelstation_army.p3d"); };
	if (strcmp (dObjName,"data3d\\garaz.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\garaz.p3d"); };
	if (strcmp (dObjName,"data3d\\garaz_bez_tanku.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\garaz_bez_tanku.p3d"); };
	if (strcmp (dObjName,"data3d\\garaz_s_tankem.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\garaz_s_tankem.p3d"); };
	if (strcmp (dObjName,"data3d\\pristresek.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\tents\\pristresek.p3d"); };
	if (strcmp (dObjName,"data3d\\pristresek_mensi.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\tents\\pristresek_mensi.p3d"); };
	if (strcmp (dObjName,"data3d\\strazni_vez.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\strazni_vez.p3d"); };
	if (strcmp (dObjName,"data3d\\vez.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\vez.p3d"); };
	if (strcmp (dObjName,"data3d\\pletivo_wired.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\pletivo_wired.p3d"); };
	if (strcmp (dObjName,"data3d\\pletivo_wired_branal.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\pletivo_wired_branal.p3d"); };
	if (strcmp (dObjName,"data3d\\pletivo_wired_branap.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\pletivo_wired_branap.p3d"); };
	if (strcmp (dObjName,"o\\misc\\plot_vlnplech1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\plot_vlnplech1.p3d"); };
	if (strcmp (dObjName,"o\\misc\\plot_vlnplech2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\plot_vlnplech2.p3d"); };
	if (strcmp (dObjName,"data3d\\budova1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova1.p3d"); };
	if (strcmp (dObjName,"data3d\\budova2.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova2.p3d"); };
	if (strcmp (dObjName,"data3d\\budova3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova3.p3d"); };
	if (strcmp (dObjName,"data3d\\budova4.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova4.p3d"); };
	if (strcmp (dObjName,"data3d\\budova4_in.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova4_in.p3d"); };
	if (strcmp (dObjName,"data3d\\budova5.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\budova5.p3d"); };
	if (strcmp (dObjName,"data3d\\repair_center.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\repair_center.p3d"); };
	if (strcmp (dObjName,"o\\hous\\bouda_plech.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\bouda_plech.p3d"); };
	if (strcmp (dObjName,"o\\hous\\tovarna1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\tovarna1.p3d"); };
	if (strcmp (dObjName,"o\\hous\\ruiny_obvod_3.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\ruiny_obvod_3.p3d"); };
	if (strcmp (dObjName,"data3d\\str jalovec.p3d") == 0) { strcpy (dObjName,"ca\\plants\\str jalovec.p3d"); };
	if (strcmp (dObjName,"data3d\\kasna.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\kasna.p3d"); };
	if (strcmp (dObjName,"o\\misc\\lavicka_1.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\misc\\lavicka_1.p3d"); };
	if (strcmp (dObjName,"data3d\\ker trs travy3.p3d") == 0) { strcpy (dObjName,"ca\\plants\\ker trs travy3.p3d"); };
	if (strcmp (dObjName,"data3d\\ker trs travy5.p3d") == 0) { strcpy (dObjName,"ca\\plants\\ker trs travy5.p3d"); };

/*

	CONVERSION PART, THIS IS WERE ISLANDS ARE "CHANGED"!
	remove this part if you do not want to change objects

*/
	if (strcmp (dObjName,"data3d\\cesta10 100.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 100.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta10 50.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 50.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta10 75.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 75.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta10 25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces10 25.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta25.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces25.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta12.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces12.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta6.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces6.p3d"); };
	if (strcmp (dObjName,"data3d\\cesta6konec.p3d") == 0) { strcpy (dObjName,"ca\\roads\\ces6konec.p3d"); };
	if (strcmp (dObjName,"data3d\\hangar.p3d") == 0) { strcpy (dObjName,"ca\\buildings\\hangar_2.p3d"); };
	if (strcmp (dObjName,"data3d\\ker trs travy.p3d") == 0) { strcpy (dObjName,"ca\\plants\\ker trs travy3.p3d"); };
	if (strcmp (dObjName,"data3d\\ker trs travy2.p3d") == 0) { strcpy (dObjName,"ca\\plants\\ker trs travy3.p3d"); };
	if (strcmp (dObjName,"o\\tree\\dd_bush01.p3d") == 0) { strcpy (dObjName,"ca\\plants\\dd_bush01.p3d"); };
	if (strcmp (dObjName,"o\\tree\\dd_bush02.p3d") == 0) { strcpy (dObjName,"ca\\plants\\dd_bush02.p3d"); };
	if (strcmp (dObjName,"data3d\\barel1.p3d") == 0) { strcpy (dObjName,"ca\\misc\\barel1.p3d"); };
	if (strcmp (dObjName,"data3d\\barel2.p3d") == 0) { strcpy (dObjName,"ca\\misc\\barel2.p3d"); };
	if (strcmp (dObjName,"data3d\\barel3.p3d") == 0) { strcpy (dObjName,"ca\\misc\\barel3.p3d"); };
	if (strcmp (dObjName,"data3d\\barel4.p3d") == 0) { strcpy (dObjName,"ca\\misc\\barel4.p3d"); };
	if (strcmp (dObjName,"data3d\\barels.p3d") == 0) { strcpy (dObjName,"ca\\misc\\barels.p3d"); };
	if (strcmp (dObjName,"data3d\\heli_h.p3d") == 0) { strcpy (dObjName,"ca\\misc\\heli_h.p3d"); };
	if (strcmp (dObjName,"data3d\\jezek kov.p3d") == 0) { strcpy (dObjName,"ca\\misc\\jezek kov.p3d"); };
	if (strcmp (dObjName,"data3d\\jezekbeton.p3d") == 0) { strcpy (dObjName,"ca\\misc\\jezekbeton.p3d"); };
	if (strcmp (dObjName,"data3d\\obihacka.p3d") == 0) { strcpy (dObjName,"ca\\misc\\obihacka.p3d"); };
	if (strcmp (dObjName,"data3d\\podlejzacka.p3d") == 0) { strcpy (dObjName,"ca\\misc\\podlejzacka.p3d"); };
	if (strcmp (dObjName,"data3d\\prebehlavka.p3d") == 0) { strcpy (dObjName,"ca\\misc\\prebehlavka.p3d"); };
	if (strcmp (dObjName,"data3d\\prolejzacka.p3d") == 0) { strcpy (dObjName,"ca\\misc\\prolejzacka.p3d"); };
	if (strcmp (dObjName,"data3d\\pytle.p3d") == 0) { strcpy (dObjName,"ca\\misc\\pytle.p3d"); };
	if (strcmp (dObjName,"data3d\\pytle_bsl.p3d") == 0) { strcpy (dObjName,"ca\\misc\\pytle_bsl.p3d"); };
	if (strcmp (dObjName,"data3d\\paletaa.p3d") == 0) { strcpy (dObjName,"ca\\misc\\paletaa.p3d"); };
	if (strcmp (dObjName,"data3d\\paletyc.p3d") == 0) { strcpy (dObjName,"ca\\misc\\paletyc.p3d"); };
	if (strcmp (dObjName,"data3d\\drevo_hromada.p3d") == 0) { strcpy (dObjName,"ca\\misc\\drevo_hromada.p3d"); };
	if (strcmp (dObjName,"data3d\\deka.p3d") == 0) { strcpy (dObjName,"ca\\misc\\deka.p3d"); };
	if (strcmp (dObjName,"data3d\\drat.p3d") == 0) { strcpy (dObjName,"ca\\misc\\drat.p3d"); };

/*
	if (strcmp (dObjName,"") == 0) { strcpy (dObjName,""); };

if we have the special ArmA models that need special values, this is
where they are setupped properly. very ugly thing I know, but I'm looser
so dont complain :)

*/

	if (strcmp (dObjName,"ca\\plants\\palm_01.p3d") == 0) { dDir += 5.7; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_02.p3d") == 0) { dDir += 6.9; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_03.p3d") == 0) { dDir += 6.9; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_04.p3d") == 0) { dDir += 6.9; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_08small.p3d") == 0) { dDir += 1.5; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_09.p3d") == 0) { dDir += 7.7; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\palm_10.p3d") == 0) { dDir += 7.9; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\akat02s.p3d") == 0) { dDir += 4.2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\banana_1.p3d") == 0) { dDir += 2.2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\banana_2.p3d") == 0) { dDir += 2.2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\banana_3.p3d") == 0) { dDir += 2.2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\banana_4.p3d") == 0) { dDir += 2.2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\letistni_hala.p3d") == 0) { dDir += 8.5; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\str briza.p3d") == 0) { dDir += 3.5; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\str_briza_kriva.p3d") == 0) { dDir += 3.5; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\plants\\str_briza_rovna.p3d") == 0) { dDir += 3.3; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan2.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan2_04a.p3d") == 0) { dDir += 4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan2b.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan3.p3d") == 0) { dDir += 2.6; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan3_hromada.p3d") == 0) { dDir += 3.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan3_hromada2.p3d") == 0) { dDir += 4.7; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan3_pumpa.p3d") == 0) { dDir += 2; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan4.p3d") == 0) { dDir += 7.3; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan4_big.p3d") == 0) { dDir += 10.3; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_istan4_detaily1.p3d") == 0) { dDir += 7.3; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\Dum_mesto3_istan.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_olez_istan1.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_olez_istan2.p3d") == 0) { dDir += 1; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_olez_istan2_maly.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\dum_olez_istan2_maly2.p3d") == 0) { dDir += 2.4; dDir -= 1000; };
	if (strcmp (dObjName,"ca\\buildings\\hotel.p3d") == 0) { dDir += 8; dDir -= 1000; };

	fwrite(&dDir,4,1,output);
	fwrite(&dDirZ,4,1,output);
	fwrite(&dObjIndex,4,1,output);

	fprintf(text,"%s, %d %f\n",dObjName,dObjIndex,dDir);
	fwrite(dObjName,sizeof(dObjName),1,output);

	};

	sprintf(tmp,"%d Object Indexes read.",dObjIndex);
	SendDlgItemMessage(hwnd, ID_eListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);

	fclose(map);
	fclose(output);
	fclose(text);

	return 0;
}
