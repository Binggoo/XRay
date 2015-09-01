#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_

static const char* Mask_Type[] = {"circle","square"};
static const TCHAR* Units[] = {_T("mm"),_T("cm"),_T("m"),_T("inch")};
#define UNIT_COUNT 4
#define MAX_POS    10
#define MAX_LEVEL  8

#define RAD(x) (PI * x / 180)
#define DEGREE(x) (180 * x / PI)

#define ANGEL PI

#define EPSINON  0.000001

#define MAX_LEVEL 8

typedef enum _ENUM_MASK_TYPE
{
	MaskType_Circle,
	MaskType_Square
}MaskType;

typedef enum _ENUM_OPTION_TYPE
{
	OP_MOVE,
	OP_SELECT,
	OP_ZOOM,
	OP_LINE,
	OP_RECT,
	OP_ELLIPSE,
	OP_DIST,
	OP_DEGREE,
	OP_TEXT,
	OP_CALIB
}OptionType;

typedef enum _ENUM_UNDO_TYPE
{
	UNDO_IMAGE,
	UNDO_DRAW
}UndoType;

typedef enum _ENUM_ERROR_TYPE
{
	Error_System,
	Error_PLC
}ErrorType;

typedef enum _ENUM_PLC_MASK
{
	Mask_PLC_Ready = 0x0001,
	Mask_PLC_Alarm = 0x1000,
	Mask_PLC_Run = 0x4000
}PLC_Mask;

typedef enum _ENUM_PLC_CMD
{
	CMD_SCAN_END = 0x0001,
	CMD_PRODUCT_OK = 0x0002,
	CMD_PRODUCT_NG = 0x0004,
	CMD_PLC_RESET = 0x0008,
	CMD_PC_RUNNING = 0x4000
}PLC_CMD;

typedef enum _ENUM_PLC_PORT
{
	D2000 = 2000,
	D2001 = 2001,
	D2010 = 2010,
	D2100 = 2100,
	D2101 = 2101
}PLC_Port;

typedef enum _ENUM_INSPECT_MODE
{
	MANUL_MODE = 0,
	SEMI_AUTO_MODE,
	AUTO_MODE
}InspectMode;

typedef enum _ENUM_IMAGE_PROCESS
{
	IMG_ROTATE,
	IMG_ENHANCE,
	IMG_GAMMA,
	IMG_INVERT,
	IMG_GAUSS,
	IMG_MEAN,
	IMG_MEDIAN
}IMG_PROCESS;

typedef struct _STRUCT_DRAW_INFO
{
	int nDrawType;
	HTuple htRow1;
	HTuple htColumn1;
	HTuple htRow2; // htRadius1;
	HTuple htColumn2; //htRadius2;
	HTuple htPhi;
	HTuple htText;
	COLORREF color;
	UINT nLineWidth;
	LOGFONT logFont;
	double dbPerPixel;
	UINT nUnitIndex;
}DrawInfo,*PDrawInfo;



#endif