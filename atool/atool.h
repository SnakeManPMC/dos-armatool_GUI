HINSTANCE hInstance;

#define IdButtonOpen	30000
#define IdButtonQuit	30001
#define IdButtonConvert	30002
#define IDEditCellSize	30003
#define IDCheckDebug	30004
#define IDStaticCellSize	30005
#define IDListbox	30006
#define IdButtonElevation 30007
#define IdButtonTextureId 30008
#define IDStaticFile	30009
#define IdButtonTextureNames 30010
#define IdButtonObjects 30011

unsigned long gfilepos;
const char szAppname[9] = "Armatool\0";
const char szClassname[8] = "oClass\0";
static char texturenames[512][33];
static int gc;

typedef struct {
	short Elevation;
	short TexIndex;
} wrpformat[4096][4096];

typedef struct {
	char dObjName[76];
	float dDir[9];
	float dDirX;
	float elevation;
	float dDirZ;
} object[750000];
	object obj;

int readMapSize(char* filename);
unsigned char check4WVR(char*);
void openWrpFile(char*);
char readElevations(char*, int,int);
char readTextureIndex(char*, int,int);
char readTextureNames(char*, int);
char readObjectNames(char*, int, int);
char saveWrpAs(HWND, int, int, char*);
float elevationAtPoint(float, float, int, int);

#define ERR_SUCCESS	200
#define ERR_FALSE	205
