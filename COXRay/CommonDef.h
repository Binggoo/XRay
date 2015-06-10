#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_

static const char* Mask_Type[] = {"circle","square"};
static const TCHAR* Units[] = {_T("mm"),_T("cm"),_T("m"),_T("inch")};
#define UNIT_COUNT 4
#define MAX_POS    10

#define RAD(x) (PI * x / 180)
#define DEGREE(x) (180 * x / PI)

#define ANGEL PI

#define EPSINON  0.000001

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
	D2100 = 2100
}PLC_Port;

typedef enum _ENUM_INSPECT_MODE
{
	MANUL_MODE = 0,
	SEMI_AUTO_MODE,
	AUTO_MODE
}InspectMode;

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


/*
id --- 序号
project --- 项目名称
product --- 产品名称
product_spec --- 产品规格
customer --- 客户
department --- 部门或者产线
woker_name --- 检测员姓名或者工号
level --- 检测标准
mode --- 检测模式
pos --- 拍照位置
time --- 拍照时间
voltage --- 管电压
current --- 管电流
original_path --- 原始图片路径
process_path --- 处理后路径
result --- 检测结果
error_msg --- 错误描述
*/
typedef struct _STRUCT_IMG_INFO
{
	int id;
	CString strProjectName;
	CString strProductName;
	CString strProductSpec;
	CString strCustomer;
	CString strDepartment;
	CString strWorkerName;
	int level;
	int mode;
	CString strPos;
	CTime time;
	double dbVolKV;
	double dbCurrentMA;
	CString strOrignalPath;
	CString strProcessPath;
	BOOL bResult;
	CString strErrorMsg;

	_STRUCT_IMG_INFO()
	{
		id = 0;
		level = -1;
		mode = -1;
		dbVolKV = 0.0;
		dbCurrentMA = 0.0;
		bResult = FALSE;
	}

}IMG_INFO,*PIMG_INFO;

#endif