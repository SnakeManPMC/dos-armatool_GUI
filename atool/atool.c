#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "atool.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

wrpformat wrp;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    MSG msg ;
    WNDCLASSEX wndclass ;

    wndclass.cbSize = sizeof (wndclass) ;
    wndclass.style = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc = WndProc ;
    wndclass.cbClsExtra = 0 ;
    wndclass.cbWndExtra = 0 ;
    wndclass.hInstance = hInstance ;
    wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
    wndclass.lpszMenuName = NULL ;
    wndclass.lpszClassName = szClassname ;
    wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION) ;

    RegisterClassEx (&wndclass) ;

    HWND hwnd = CreateWindow (szClassname,
        szAppname,
        WS_DLGFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        415,
        400,
        NULL,
        NULL,
        hInstance,
        NULL) ;
    ShowWindow (hwnd, iCmdShow) ;

    while (GetMessage (&msg, NULL, 0, 0) == TRUE)
    {
        TranslateMessage (&msg) ;
        DispatchMessage (&msg) ;
    }

    return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static char errormsg[128];
	static int MapSize;

	static HWND hwndButtonOpen;
	static HWND hwndButtonQuit;
	static HWND hwndButtonConvert;
	static HWND hwndEditCellSize;
	static HWND hwndCheckDebug;
	static HWND hwndStaticCellSize;
	static HWND hwndListbox;
	static HWND hwndButtonElevation;
	static HWND hwndButtonTextureId;
	static HWND hwndStaticFile;
	static HWND hwndButtonTextureNames;
	static HWND hwndButtonObjects;

	static char filename[MAX_PATH];

	HFONT hFont;
	hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT) ;

	switch (iMsg)
    {

		case WM_CREATE:

			hwndButtonOpen = CreateWindowEx(0, "button", "Open", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 30, 120, 20, hwnd, (HMENU)IdButtonOpen, hInstance, NULL);
			SendMessage(hwndButtonOpen, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonElevation = CreateWindowEx(0, "button", "Read Elevatations", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 55, 120, 20, hwnd, (HMENU)IdButtonElevation, hInstance, NULL);
			SendMessage(hwndButtonElevation, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonTextureId = CreateWindowEx(0, "button", "Read Texture IDs", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 80, 120, 20, hwnd, (HMENU)IdButtonTextureId, hInstance, NULL);
			SendMessage(hwndButtonTextureId, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonTextureNames = CreateWindowEx(0, "button", "Read Texture Names", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 105, 120, 20, hwnd, (HMENU)IdButtonTextureNames, hInstance, NULL);
			SendMessage(hwndButtonTextureNames, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonObjects = CreateWindowEx(0, "button", "Read Objects", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 130, 120, 20, hwnd, (HMENU)IdButtonObjects, hInstance, NULL);
			SendMessage(hwndButtonObjects, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonConvert = CreateWindowEx(0, "button", "Convert", WS_CHILD | BS_PUSHBUTTON |WS_VISIBLE, 5, 155, 120, 20, hwnd, (HMENU)IdButtonConvert, hInstance, NULL);
            SendMessage(hwndButtonConvert, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndButtonQuit = CreateWindowEx(0, "button", "Close", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 5, 180, 120, 20, hwnd, (HMENU)IdButtonQuit, hInstance, NULL);
			SendMessage(hwndButtonQuit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndStaticFile = CreateWindowEx(WS_EX_CLIENTEDGE, "static", "", WS_CHILD | WS_VISIBLE, 5, 5, 400, 20, hwnd, (HMENU)IDStaticFile, hInstance, NULL);
			SendMessage(hwndStaticFile, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndEditCellSize = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "50", WS_CHILD | WS_VISIBLE | ES_READONLY, 365,30, 40, 20, hwnd, (HMENU)IDEditCellSize, hInstance, NULL);
			SendMessage(hwndEditCellSize, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndStaticCellSize = CreateWindowEx(0, "static", "Cellsize", WS_CHILD | WS_VISIBLE , 320,30, 40, 20, hwnd, (HMENU)IDStaticCellSize, hInstance, NULL);
			SendMessage(hwndStaticCellSize, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndCheckDebug = CreateWindowEx(0, "button", "Debug", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 140, 30, 80, 20, hwnd, (HMENU)IDCheckDebug, hInstance, NULL);
			SendMessage(hwndCheckDebug, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			hwndListbox = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL, 5,210, 400, 160, hwnd, (HMENU)IDListbox, hInstance, NULL);
			SendMessage(hwndListbox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

			EnableWindow(hwndButtonElevation, FALSE);
			EnableWindow(hwndButtonTextureId, FALSE);
			EnableWindow(hwndButtonConvert, FALSE);
			EnableWindow(hwndButtonTextureNames, FALSE);
			EnableWindow(hwndButtonObjects, FALSE);

			return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {


				case IdButtonOpen:
					openWrpFile((char*)&filename);
					if( check4WVR(filename) == ERR_FALSE ) {
						SetDlgItemTextA(hwnd, IDStaticFile, "");
						strcpy(errormsg, "Error in reading the signature!");
						SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
						break;
					}
					else {
						strcpy(errormsg, "Found signature");
						SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
					}

					if(strlen(filename) < 1) {
						break;
					}

					else {
						SetDlgItemTextA(hwnd, IDStaticFile, filename);
						EnableWindow(hwndButtonElevation, TRUE);
						EnableWindow(hwndButtonTextureId, FALSE);
						EnableWindow(hwndButtonConvert, FALSE);
					}

					MapSize = readMapSize(filename);
					if( MapSize == ERR_FALSE ) {
						SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Error in mapsizeblahblah!");
						break;
					}
					else {
						sprintf(errormsg, "Mapsize: %d x %d", MapSize, MapSize);
						SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
					}

					break;

				case IdButtonElevation:

					if(SendDlgItemMessage(hwnd, IDCheckDebug, BM_GETCHECK, 0,0) == BST_CHECKED) {
						if( readElevations(filename, MapSize,1) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading elevations! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "elevations ok! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonElevation, FALSE);
							EnableWindow(hwndButtonTextureId, TRUE);

						}
					}

					else {
						if( readElevations(filename, MapSize,0) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading elevations!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "elevations ok!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonElevation, FALSE);
							EnableWindow(hwndButtonTextureId, TRUE);
						}
					}

					break;

				case IdButtonTextureId:
					if(SendDlgItemMessage(hwnd, IDCheckDebug, BM_GETCHECK, 0,0) == BST_CHECKED) {
						if( readTextureIndex(filename, MapSize,1) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading texture id's! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "texture ids read ok! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonTextureId, FALSE);
							EnableWindow(hwndButtonTextureNames, TRUE);
						}
					}

					else {
						if( readTextureIndex(filename, MapSize,0) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading textures!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "texture ids read ok!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonTextureId, FALSE);
							EnableWindow(hwndButtonTextureNames, TRUE);
						}
					}

					break;

				case IdButtonTextureNames:
					if(SendDlgItemMessage(hwnd, IDCheckDebug, BM_GETCHECK, 0,0) == BST_CHECKED) {
						if( readTextureNames(filename, 1) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading texture names! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "texture names read ok! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonObjects, TRUE);
							EnableWindow(hwndButtonTextureNames, FALSE);

						}

					}

					else {
						if( readTextureNames(filename, 0) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading texture names!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "texture names read ok!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonObjects, TRUE);
							EnableWindow(hwndButtonTextureNames, FALSE);
						}
					}

					break;

				case IdButtonObjects:
					if(SendDlgItemMessage(hwnd, IDCheckDebug, BM_GETCHECK, 0,0) == BST_CHECKED) {
						if( readObjectNames(filename, 1, MapSize) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading objects! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "objects read ok! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonConvert, TRUE);
							EnableWindow(hwndButtonObjects, FALSE);
						}

					}

					else {
						if( readObjectNames(filename, 0, MapSize) == ERR_FALSE) {
							strcpy(errormsg, "Error in reading objects!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							break;
						}
						else {
							strcpy(errormsg, "objects read ok!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonConvert, TRUE);
							EnableWindow(hwndButtonObjects, FALSE);
						}
					}
					break;

				case IdButtonQuit:
					if( MessageBoxA(hwnd, "Exit?", szAppname, MB_YESNO) == IDYES ) {
						PostQuitMessage(0);
					}
					break;

				case IdButtonConvert:
					if(SendDlgItemMessage(hwnd, IDCheckDebug, BM_GETCHECK, 0,0) == BST_CHECKED) {
						if( saveWrpAs(hwnd, MapSize, 1, filename) == ERR_FALSE) {
							strcpy(errormsg, "Error in converting wrp! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
						}
						else {
							strcpy(errormsg, "yeah! convert worked ok! (debug mode)");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonConvert, FALSE);
						}
					}
					else {
						if( saveWrpAs(hwnd, MapSize, 0, filename) == ERR_FALSE) {
							strcpy(errormsg, "Error in converting wrp!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
						}
						else {
							strcpy(errormsg, "yeah! convert worked ok!");
							SendDlgItemMessage(hwnd, IDListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)errormsg);
							EnableWindow(hwndButtonConvert, FALSE);
						}

					}

					break;

			}

			return 0;

	    case WM_DESTROY:
			PostQuitMessage(0);
	    	return 0;
    }

    return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

void openWrpFile(char* filename)
{
	OPENFILENAME fname;
	memset(&fname, 0, sizeof(fname));
	filename[0] = 0;

	fname.lStructSize = sizeof(fname);
	fname.lpstrFilter = "All Files (*.*)\0*.*\0WRP Files (*.wrp)\0*.txt\0";
	fname.lpstrFile = filename;
	fname.nMaxFile = MAX_PATH;
	fname.Flags = OFN_EXPLORER;
	GetOpenFileName(&fname);
}

unsigned char check4WVR(char* filename)
{
	FILE *fp;

	char sig[5];

	fp = fopen(filename, "rb");

	if(!fp) {
		return ERR_FALSE;
	}

	gfilepos = 0;
	fsetpos(fp, &gfilepos);

	fread(sig, 4, 1, fp);
	sig[4] = 0;

	if( strcmp(sig, "4WVR") != 0 ) {
		fclose(fp);
		return ERR_FALSE;
	}

	fclose(fp);

	return ERR_SUCCESS;
}

int readMapSize(char* filename)
{
	FILE *fp;
	int MapSize;

	fp = fopen(filename, "rb");

	if(!fp) {
		return ERR_FALSE;
	}

	gfilepos = 4;
	fsetpos(fp, &gfilepos);
	fread(&MapSize, 4, 1, fp);

	if(MapSize > 4096) {
		fclose(fp);
		return ERR_FALSE;
	}

	fclose(fp);
	return MapSize;
}

char readElevations(char* filename, int MapSize, int debug)
{
	FILE *text;
	FILE *map;

	int x=0, z=0;
	short Elevation=0,LowElev=0,HighElev=0;

	unsigned char buffer[MAX_PATH+7];

	map = fopen(filename,"rb");
	if(!map) {
		return ERR_FALSE;
	}

	sprintf(buffer, "%s-debug\0", filename);

	text = fopen(buffer, "wt");
	if(!text) {
		fclose(map);
		return ERR_FALSE;
	}

	gfilepos = 12;
	fsetpos(map, &gfilepos);

		for (int zx=0; zx < MapSize*MapSize; zx++) {

			fread(&wrp[x][z].Elevation,sizeof(wrp[x][z].Elevation),1,map);

			if (LowElev > Elevation) LowElev=Elevation;
			if (HighElev < Elevation) HighElev=Elevation;

			x++;
			if (x == MapSize) {z++; x=0;}
			if (z == MapSize) {z=0;}

	}
	fgetpos(map, &gfilepos);


x=1475;
z=1525;
fprintf(text, "elevation %i,%i: %d === %f\n",x,z,wrp[x][z].Elevation,elevationAtPoint(x,z,50,MapSize));



	fclose(text);
	fclose(map);
	if(debug == 0) {
		remove(buffer);
	}

	return ERR_SUCCESS;
}

char readTextureIndex(char* filename, int MapSize, int debug)
{
	FILE *map;
	FILE *text;
	int x=0,z=0,TexIndex=0;

	unsigned char buffer[MAX_PATH+7];

	map = fopen(filename,"rb");
	if(!map) {
		return ERR_FALSE;
	}

	sprintf(buffer, "%s-debug\0", filename);

	text = fopen(buffer, "at");
	if(!text) {
		fclose(map);
		return ERR_FALSE;
	}

	fsetpos(map, &gfilepos);

	for (int tx=0; tx < MapSize*MapSize; tx++) {
		wrp[x][z].TexIndex = 0;
		fread(&wrp[x][z].TexIndex,sizeof(wrp[x][z].TexIndex),1,map);

		if(debug == 1) {
			fprintf(text, "TexIndex %i,%i: %d\n",x,z,wrp[x][z].TexIndex);
		}

		x++;
		if (x == MapSize) {z++; x=0;}
		if (z == MapSize) {z=0;}

	}

	fgetpos(map, &gfilepos);
	fclose(map);
	fclose(text);
	return ERR_SUCCESS;
}

char readTextureNames(char* filename, int debug)
{
	FILE *map;
	FILE *text;
	unsigned char buffer[MAX_PATH+7];

	map = fopen(filename,"rb");
	if(!map) {
		return ERR_FALSE;
	}

	sprintf(buffer, "%s-debug\0", filename);

	text = fopen(buffer, "at");
	if(!text) {
		fclose(map);
		return ERR_FALSE;
	}

	fsetpos(map, &gfilepos);
	for (int ix=0; ix < 512; ix++) {
		texturenames[ix][0]=0;
		fread(texturenames[ix],32,1,map);

		if(debug == 1) {
			fprintf(text, "texture name [%d]: %s\n",ix,texturenames[ix]);
		}
	}

	fgetpos(map, &gfilepos);
	fclose(text);
	fclose(map);
	return ERR_SUCCESS;
}

char readObjectNames(char* filename, int debug, int MapSize)
{
	FILE *map;
	FILE *text;
	unsigned char buffer[MAX_PATH+7];
	unsigned int dObjIndex=0,c = 0;
	int CellSize = 50;

	map = fopen(filename,"rb");

	if(!map) {
		return ERR_FALSE;
	}

	sprintf(buffer, "%s-debug\0", filename);

	text = fopen(buffer, "at");
	if(!text) {
		fclose(map);
		return ERR_FALSE;
	}

	fsetpos(map, &gfilepos);

	while(!feof(map)) {

		fread(&obj[c].dDir[0], 4, 1, map);
		fread(&obj[c].dDir[1], 4, 1, map);
		fread(&obj[c].dDir[2], 4, 1, map);
		fread(&obj[c].dDir[3], 4, 1, map);
		fread(&obj[c].dDir[4], 4, 1, map);
		fread(&obj[c].dDir[5], 4, 1, map);
		fread(&obj[c].dDir[6], 4, 1, map);
		fread(&obj[c].dDir[7], 4, 1, map);
		fread(&obj[c].dDir[8], 4, 1, map);

		fread(&obj[c].dDirX, 4, 1, map);
		fread(&obj[c].elevation, 4, 1, map);
		obj[c].elevation = elevationAtPoint(obj[c].dDirX, obj[c].dDirZ, CellSize, MapSize);

		fread(&obj[c].dDirZ, 4, 1, map);

		fread(&dObjIndex, 4, 1, map);

		memset(obj[c].dObjName, 0, sizeof(obj[c].dObjName));

		fread(obj[c].dObjName, 76, 1, map);
		strlwr(obj[c].dObjName);

/* -----------------------------------*/
		if (strcmp (obj[c].dObjName,"data3d\\afbarabizna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\afbarabizna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\afdum_mesto2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\afdum_mesto2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\afdum_mesto2l.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\afdum_mesto2l.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\afdum_mesto3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\afdum_mesto3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\afhospoda_mesto.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\afhospoda_mesto.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dulni_bs.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dulni_bs.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_zboreny.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_zboreny.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_zboreny_lidice.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_zboreny_Lidice.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_zboreny_total.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_zboreny_total.p3d"); };
	// not work	if (strcmp (obj[c].dObjName,"data3d\\skala.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\skala_new.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala_new.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\skala_newk.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala_newk.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\bouda1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bouda1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\bouda2_vnitrek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bouda2_vnitrek.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\bouda3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bouda3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova4.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova4_in.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova4_in.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova5.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cihlovej_dum.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\cihlovej_dum.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cihlovej_dum_in.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\cihlovej_dum_in.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cihlovej_dum_mini.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\cihlovej_dum_mini.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\deutshe.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\deutshe.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\deutshe_mini.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\deutshe_mini.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum01.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum01.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum02.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dumruina.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dumruina.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dumruina_mini.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dumruina_mini.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_m2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_m2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_mesto.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_mesto.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_mesto2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_mesto2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_mesto2l.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_mesto2l.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_mesto3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_mesto3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_olezlina.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_olezlina.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala1_1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala1_1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala1_2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala1_2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala1_3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala1_3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala1_4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala1_4.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala1_5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala1_5.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala2_1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala2_1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala2_2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala2_2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala2_3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala2_3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala2_4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala2_4.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala2_5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala2_5.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala3_1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala3_1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala3_2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala3_2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala3_3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala3_3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala3_4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala3_4.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\skal\\skala3_5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala3_5.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\akat02s.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\akat02s.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces12.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces6.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\ces6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil12.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil6.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\sil6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\sil6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\hangar_2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hangar_2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\smrk_maly.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\smrk_maly.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\smrk_siroky.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\smrk_siroky.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\smrk_velky.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\smrk_velky.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\dd_borovice.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\dd_borovice.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\dd_borovice02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\dd_borovice02.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\les_singlestrom.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\les_singlestrom.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hromada_kameni.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\hromada_kameni.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen1_piskove2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen1_piskove2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen1_piskovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen1_piskovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen1_piskovec2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen1_piskovec2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen1_zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen1_zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen2_piskovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen2_piskovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen2_piskovec2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen2_piskovec2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen2_zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen2_zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen3_piskovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen3_piskovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen3_piskovec2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen3_piskovec2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen3_zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen3_zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen4_piskovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen4_piskovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen4_piskovec2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen4_piskovec2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen4_zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen4_zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen5_piskovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen5_piskovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen5_piskovec2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen5_piskovec2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kamen5_zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kamen5_zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kopa_kameni.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kopa_kameni.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kopa_kameni2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\kopa_kameni2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hospital.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hospital.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\fuelstation.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\fuelstation.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\fuelstation_army.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\fuelstation_army.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\garaz.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\garaz.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\garaz_bez_tanku.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\garaz_bez_tanku.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\garaz_s_tankem.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\garaz_s_tankem.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pristresek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\pristresek.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pristresek_mensi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\pristresek_mensi.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\strazni_vez.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\strazni_vez.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\vez.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\vez.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pletivo_wired.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pletivo_wired.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pletivo_wired_branal.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pletivo_wired_branal.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pletivo_wired_branap.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pletivo_wired_branap.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\plot_vlnplech1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_vlnplech1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\plot_vlnplech2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_vlnplech2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova4.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova4_in.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova4_in.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\budova5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\budova5.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\repair_center.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\repair_center.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\bouda_plech.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bouda_plech.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\tovarna1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tovarna1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_obvod_3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_obvod_3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str jalovec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str jalovec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kasna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kasna.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\lavicka_1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\lavicka_1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy5.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\dd_bush01.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\dd_bush01.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\tree\\dd_bush02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\dd_bush02.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barel1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barel1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barel2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barel2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barel3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barel3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barel4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barel4.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barels.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barels.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\heli_h.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\heli_h.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\jezek kov.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\jezek kov.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\jezekbeton.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\jezekbeton.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\obihacka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\obihacka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\podlejzacka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\podlejzacka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\prebehlavka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\prebehlavka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\prolejzacka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\prolejzacka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pytle.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\pytle.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pytle_bsl.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\pytle_bsl.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\paletaa.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\paletaa.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\paletyc.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\paletyc.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\drevo_hromada.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\drevo_hromada.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\deka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\deka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\drat.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\drat.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\barbedwire.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\barbedwire.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\betacam.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\betacam.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\statek_plot.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\statek_plot.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\strel_post.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\strel_post.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\strel_post_panel.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\strel_post_panel.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zed_kamenna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zed_kamenna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\krovi.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\krovi2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\krovi4.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi_bigest.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\krovi_bigest.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi_long.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\krovi_long.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\afnewplot2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\afnewplot2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\newplot.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\newplot.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\newplot2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\newplot2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pletivo.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pletivo.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\pletivo_dira.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pletivo_dira.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_branka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_branka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_brank_o.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_brank_o.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_cihlsloup.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_cihlsloup.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_draty.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_draty.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_vrata.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_vrata.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_vrat_l.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_vrat_l.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_vrat_o.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_vrat_o.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_green_vrat_r.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_green_vrat_r.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_provizorni.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_provizorni.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_rust_branka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_rust_branka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_rust_brank_o.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_rust_brank_o.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_rust_cihlsloup.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_rust_cihlsloup.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_rust_draty.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_rust_draty.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\plot_vlnplech1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_vlnplech1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\plot_vlnplech2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_vlnplech2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plot_zboreny.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_zboreny.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\plot_zed-drevo1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_zed-drevo1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\plot_zed-drevo2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_zed-drevo2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\plot_zed-drevo3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plot_zed-drevo3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\plutek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\plutek.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\pumpa.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\pumpa.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_rasovna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_rasovna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\fuelstation_new.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\fuelstation.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kasna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kasna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kostel.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kostel.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kostel2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kostel2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kostel3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kostel3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kostelik.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kostelik.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kostel_trosky.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kostel_trosky.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\majak.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\majak.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\majak_podesta.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\majak_podesta.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\majak_v_celku.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\majak_v_celku.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\molo_beton.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\molo_beton.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\molo_drevo.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\molo_drevo.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\molo_drevo_bs.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\molo_drevo_bs.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\molo_drevo_end.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\molo_drevo_end.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\molo_krychle.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\molo_krychle.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice12.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice6.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\silnice6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\silnice6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfatlka10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfatlka10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfatlka10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfatlka10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka12.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka6.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\asfaltka6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asfaltka6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_asfaltka_asfaltka_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_asfaltka_asfaltka_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_asfaltka_cesta_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_asfaltka_cesta_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_asfaltka_sil_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_asfaltka_sil_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_asf_asf_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_asf_asf_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_asf_ces_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_asf_ces_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_asf_sil_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_asf_sil_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_kos.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_kos.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_kos_kos_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_kos_kos_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_kos_sil_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_kos_sil_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_silxsil.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_silxsil.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_sil_asf_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_sil_asf_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_sil_ces_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_sil_ces_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_sil_kos_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_sil_kos_t.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kr_new_sil_sil_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_new_sil_sil_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_silnicexsilnice.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_silnicexsilnice.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_silnice_asfaltka_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_silnice_asfaltka_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_silnice_cesta_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_silnice_cesta_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\kr_silnice_silnice_t.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kr_silnice_silnice_t.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\jablon.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\jablon.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker buxus.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker buxus.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker deravej.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker deravej.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker pichlavej.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker pichlavej.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker s bobulema.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker s bobulema.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\oliva.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\oliva.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\parez.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\parez.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str briza.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str briza.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str buk.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str buk.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str dub jiny.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str dub jiny.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str dub.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str dub.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str habr.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str habr.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str javor.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str javor.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str jerabina.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str jerabina.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str kastan.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str kastan.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str krovisko vysoke.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str krovisko vysoke.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str lipa.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str lipa.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str osika.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str osika.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str trnka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str trnka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str vrba.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str vrba.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_fikovnik.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_fikovnik.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_fikovnik2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_fikovnik2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_fikovnik_ker.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_fikovnik_ker.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hruzdum.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hruzdum.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\statek_brana.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\statek_brana.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\statek_brana_open.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\statek_brana_open.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\statek_hl_bud.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\statek_hl_bud.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\statek_kulna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\statek_kulna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stodola_open.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\stodola_open.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zvonice.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\zvonice.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\lampa strop.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\lampa strop.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\lampadrevo.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\lampadrevo.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\lampazel.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\lampazel.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stoh.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\stoh.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_briza_kriva.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_briza_kriva.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_briza_rovna.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_briza_rovna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_liskac.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_liskac.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_pinie.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_pinie.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ammostore2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\ammostore2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_mesto_in.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_mesto_in.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hospoda_mesto.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hospoda_mesto.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stanice.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\stanice.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\helfenburk.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\castle\\helfenburk.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\helfenburk_brana.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\castle\\helfenburk_brana.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\helfenburk_budova2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\castle\\helfenburk_budova2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\helfenburk_cimburi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\castle\\helfenburk_cimburi.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\helfenburk_zed.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\castle\\helfenburk_zed.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_topol.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_topol.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\str_topol2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\str_topol2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\aut_zast.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\aut_zast.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\bordel_zidka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bordel_zidka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\bozi_muka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\bozi_muka.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\brana02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\brana02.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\brana02nodoor.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\brana02nodoor.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\domek_rosa.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\domek_rosa.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hlaska.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hlaska.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\hlidac_budka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hlidac_budka.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\kap02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kap02.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\kapl.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\kapl.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nabrezi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\nabrezi.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nabrezi_najezd.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\nabrezi_najezd.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\orlhot.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\orlhot.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\panelak.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\panelak.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\panelak2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\panelak2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\panelak3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\panelak3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\posed.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\posed.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\psi_bouda.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\psi_bouda.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ryb_domek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\ryb_domek.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\skola.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\skola.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\vysilac_fm.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\vysilac_fm.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zalchata.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\zalchata.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\aut_z_st.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\aut_z_st.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\dd_pletivo.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\dd_pletivo.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\dd_pletivo_dira.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\dd_pletivo_dira.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\dd_pletivo_sl.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\dd_pletivo_sl.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\hrob1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrob1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\hrob2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrob2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrobecek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrobecek.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrobecek2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrobecek2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrobecek_krizek1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrobecek_krizek1.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrobecek_krizek2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrobecek_krizek2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrobecek_krizekhelma.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\hrobecek_krizekhelma.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\kbud.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\kbud.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\leseni2x.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\leseni2x.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\leseni4x.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\leseni4x.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nahrobek1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nahrobek1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nahrobek2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nahrobek2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nahrobek3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nahrobek3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nahrobek4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nahrobek4.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nahrobek5.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nahrobek5.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nam_dlazba.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nam_dlazba.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\nam_okruzi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nam_okruzi.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\nastenka2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nastenka2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\nastenka3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\nastenka3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_3_dvere.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_3_dvere.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_3_prasklina.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_3_prasklina.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_3_roh.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_3_roh.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_3_stena.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_3_stena.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\ruiny_3_stenazbor.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\ruiny_3_stenazbor.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\stozarvn_1.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\stozarvn_1.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\stozarvn_2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\stozarvn_2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\stozarvn_3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\stozarvn_3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zavora.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zavora.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\misc\\zavora_sloupek.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zavora_sloupek.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zed.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zed.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zed2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zed2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zed3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zed3.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zidka01.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zidka01.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zidka02.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zidka02.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zidka03.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zidka03.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zidka04.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zidka04.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\zidka_branka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zidka_branka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\astan.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\astan.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\mash.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\mash.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stan.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\stan.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stan_east.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\tents\\stan_east.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hrusen2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\hrusen2.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf12.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf6.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\asf6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\asf6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos12.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos25.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos6.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\road\\kos6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\kos6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"o\\hous\\most_stred30.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\most_stred30.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zula.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\zula.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\jehlan cs.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\jehlan cs.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\patnik.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\patnik.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\patniky.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\patniky.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\sloupyela.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\sloupyela.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\sloupyeli.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\sloupyeli.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\terc.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\terc.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\uzaverka.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\uzaverka.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\zed_podplaz.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\misc\\zed_podplaz.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\podprapornik.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\misc\\podprapornik.p3d"); };
	/* -----------------------------------*/
	/*
		if (strcmp (obj[c].dObjName,"") == 0) { strcpy (obj[c].dObjName,""); };


		CONVERSION PART, THIS IS WERE ISLANDS ARE "CHANGED"!
		remove this part if you do not want to change objects

	*/
		if (strcmp (obj[c].dObjName,"data3d\\cesta10 100.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 100.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta10 50.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 50.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta10 75.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 75.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta10 25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces10 25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta25.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces25.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta12.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces12.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces6.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\cesta6konec.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\roads\\ces6konec.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\hangar.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\hangar_2.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy2.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\skala.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\rocks\\skala_new.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker listnac.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker buxus.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker rakosi.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker buxus.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy kvetina.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy3.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy4.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy5.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy6.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy5.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\ker trs travy7.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\ker trs travy5.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\krovi3.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\plants\\Krovi4.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\dum_rasovna_in.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\dum_rasovna.p3d"); };
		if (strcmp (obj[c].dObjName,"data3d\\stodola.p3d") == 0) { strcpy (obj[c].dObjName,"ca\\buildings\\stodola_old.p3d"); };

	/*

	if we have the special ArmA models that need special values, this is
	where they are setupped properly. very ugly thing I know, but I'm looser
	so dont complain :)

	*/
		if(debug == 1) {
			fprintf(text,"%s, %d, [%f,%f], old: %f, new: ", obj[c].dObjName,dObjIndex,obj[c].dDirX,obj[c].dDirZ,obj[c].elevation);
			fprintf(text,"%f fixed: ",obj[c].elevation);
		}

		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_01.p3d") == 0) { obj[c].elevation += 5.7; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_02.p3d") == 0) { obj[c].elevation += 6.9; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_03.p3d") == 0) { obj[c].elevation += 6.9; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_04.p3d") == 0) { obj[c].elevation += 6.9; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_08small.p3d") == 0) { obj[c].elevation += 1.5; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_09.p3d") == 0) { obj[c].elevation += 7.7; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\palm_10.p3d") == 0) { obj[c].elevation += 7.9; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\akat02s.p3d") == 0) { obj[c].elevation += 4.2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\banana_1.p3d") == 0) { obj[c].elevation += 2.2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\banana_2.p3d") == 0) { obj[c].elevation += 2.2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\banana_3.p3d") == 0) { obj[c].elevation += 2.2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\banana_4.p3d") == 0) { obj[c].elevation += 2.2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\letistni_hala.p3d") == 0) { obj[c].elevation += 8.5; obj[c].elevation -= 1000; };
	// this is not converted object?	if (strcmp (obj[c].dObjName,"ca\\plants\\str briza.p3d") == 0) { obj[c].elevation += 3.5; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\str_briza_kriva.p3d") == 0) { obj[c].elevation += 3.5; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\plants\\str_briza_rovna.p3d") == 0) { obj[c].elevation += 3.3; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan2.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan2_04a.p3d") == 0) { obj[c].elevation += 4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan2b.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan3.p3d") == 0) { obj[c].elevation += 2.6; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan3_hromada.p3d") == 0) { obj[c].elevation += 3.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan3_hromada2.p3d") == 0) { obj[c].elevation += 4.7; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan3_pumpa.p3d") == 0) { obj[c].elevation += 2; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan4.p3d") == 0) { obj[c].elevation += 7.3; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan4_big.p3d") == 0) { obj[c].elevation += 10.3; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_istan4_detaily1.p3d") == 0) { obj[c].elevation += 7.3; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\Dum_mesto3_istan.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_olez_istan1.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_olez_istan2.p3d") == 0) { obj[c].elevation += 1; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_olez_istan2_maly.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\dum_olez_istan2_maly2.p3d") == 0) { obj[c].elevation += 2.4; obj[c].elevation -= 1000; };
		if (strcmp (obj[c].dObjName,"ca\\buildings\\hotel.p3d") == 0) { obj[c].elevation += 8; obj[c].elevation -= 1000; };

		if(debug == 1) {
			fprintf(text,"%f\n",obj[c].elevation);
		}
		gc = c;
		c++;
	}

	fgetpos(map, &gfilepos);
	fclose(text);
	fclose(map);

	return ERR_SUCCESS;
}

float elevationAtPoint(float x, float z, int CellSize, int MapSize)
{

	int row, col, maxrow=MapSize, maxcol=MapSize;
	float y, x0, z0, y0, y1, y2;

	col = (int)floor(x / CellSize);
	row = (int)floor(z / CellSize);

	if ((fmod(x,CellSize)+fmod(z,CellSize)) < CellSize)
	{
		x0 = (float)(col * CellSize);
		z0 = (float)(row * CellSize);
		y0 = wrp[row][col].Elevation;

		if (row == maxrow)
		{
			y1 = 0;
		}
		else
		{
			y1 = wrp[row+1][col].Elevation;
		}
		if (col == maxcol)
		{
			y2 = 0;
		}
		else
		{
			y2 = wrp[row][col+1].Elevation;
		}

		y = (y0+((z-z0)/CellSize*(y1-y0))+((x-x0)/CellSize*(y2-y0)));
	}
	else
	{
		x0 = (float) (col * CellSize);
		z0 = (float) ((row+1) * CellSize);

		if (col == maxcol)
		{
			y0 = 0;
		}
		else
		{
			y0 = wrp[row][col+1].Elevation;
		}
		if (row == maxrow)
		{
			y1 = 0;
			y2 = 0;
		}
		else
		{
			y1 = wrp[row+1][col].Elevation;
			y2 = wrp[row+1][col+1].Elevation;
		}

		y = (y0+(((x-x0)/(-CellSize)*(y1-y0))+((z-z0+x0-x)/CellSize*(y2-y0))));
	}

	return y * 9./200.;
}

char saveWrpAs(HWND hwnd, int MapSize, int debug, char* debuglog)
{
	OPENFILENAME fname;
	memset(&fname, 0, sizeof(fname));
	char filename[MAX_PATH] = {0};
	FILE* output;
	FILE *text;
	int z,x,zx;
	unsigned char buffer[MAX_PATH+7];

	fname.lStructSize = sizeof(fname);
	fname.lpstrFilter = "All Files (*.*)\0*.*\0WRP Files (*.wrp)\0*.txt\0";
	fname.lpstrFile = filename;
	fname.nMaxFile = MAX_PATH;
	fname.Flags = OFN_EXPLORER;
	GetSaveFileName(&fname);

	output = fopen(filename, "wb");

	if(!output) {
		return ERR_FALSE;
	}

	sprintf(buffer, "%s-debug\0", debuglog);

	text = fopen(buffer, "at");

	if(!text) {
		fclose(output);
		return ERR_FALSE;
	}

// Write Signature
	fwrite("4WVR", 4, sizeof(char) , output);

// Write Map Size
	fwrite(&MapSize, 4, 1, output);
	fwrite(&MapSize, 4, 1, output);

	z = 0;
	x = 0;

// Write Elevations
	for (zx=0; zx < (MapSize*MapSize); zx++) {
			fwrite(&wrp[x][z].Elevation,sizeof(wrp[x][z].Elevation),1,output);
			x++;
			if (x == MapSize) {z++; x=0;}
			if (z == MapSize) {z=0;}
	}


// Write Texture Indexes
	x=0,z=0;
	for (zx=0; zx < MapSize*MapSize; zx++) {

			fwrite(&wrp[x][z].TexIndex,sizeof(wrp[x][z].TexIndex),1,output);
			x++;
			if (x == MapSize) {z++; x=0;}
			if (z == MapSize) {z=0;}

	}

// Write Texture Names
	for (zx=0; zx < 512; zx++) {
		fwrite(texturenames[zx],32,1,output);
		if(debug==1) {
			fprintf(text, "texture name [%d]: %s\n",zx,texturenames[zx]);
		}
	}

// Write 9 floats, X, Z, elev, objID, Objectname
	for(unsigned long i=0; i<gc; i++) {
		fwrite(&obj[i].dDir[0], 4, 1, output);
		fwrite(&obj[i].dDir[1], 4, 1, output);
		fwrite(&obj[i].dDir[2], 4, 1, output);
		fwrite(&obj[i].dDir[3], 4, 1, output);
		fwrite(&obj[i].dDir[4], 4, 1, output);
		fwrite(&obj[i].dDir[5], 4, 1, output);
		fwrite(&obj[i].dDir[6], 4, 1, output);
		fwrite(&obj[i].dDir[7], 4, 1, output);
		fwrite(&obj[i].dDir[8], 4, 1, output);
		fwrite(&obj[i].dDirX, 4, 1, output);
		fwrite(&obj[i].elevation, 4, 1, output);
		fwrite(&obj[i].dDirZ, 4, 1, output);
		fwrite(&i, 4, 1, output);
		fwrite(obj[i].dObjName, 76, 1, output);
	}

	if(debug==0) {
		remove(buffer);
	}
	fclose(text);
	fclose(output);
	return ERR_SUCCESS;
}
