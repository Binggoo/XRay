#pragma once
#include "../common/mysql/mysql.h"

#define QUERY_FIELDS_COUNT 13
#define TABLE_FIELDS_COUNT 18

static const char *table_name = "table_xray";
static const char *query_fields[] = {
	"project",
	"product",
	"serial",
	"product_spec",
	"customer",
	"department",
	"woker_name",
	"level",
	"mode",
	"pos",
	"voltage",
	"current",
	"result"
};

static const char *table_fields[] = {
	"id",
	"project",
	"product",
	"serial",
	"product_spec",
	"customer",
	"department",
	"woker_name",
	"level",
	"mode",
	"pos",
	"time",
	"voltage",
	"current",
	"original_path",
	"process_path",
	"result",
	"error_msg"
};

static const int fileds_width[] = {
	60,
	100,
	100,
	100,
	100,
	100,
	100,
	100,
	60,
	60,
	60,
	130,
	60,
	60,
	200,
	200,
	60,
	200
};

typedef enum _ENUM_TABLE_COL
{
	COL_ID = 0,
	COL_PROJECT,
	COL_PRODUCT,
	COL_SERIAL,
	COL_SPEC,
	COL_CUSTOMER,
	COL_DEPT,
	COL_WORKER,
	COL_LEVEL,
	COL_MODE,
	COL_POS,
	COL_TIME,
	COL_VOLTAGE,
	COL_CURRENT,
	COL_ORIGIN,
	COL_PROCESS,
	COL_RESULT,
	COL_MSG
}TableCol;

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
	CString strSerialNumber;
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


static const char *user_table_name = "table_user";

typedef struct _STRUCT_USER_INFO
{
	CString strUserName;
	int nUserType; // 0 - 高级用户， 1 - 普通用户
	CString strPassword; // 经过加密之后的密码

	_STRUCT_USER_INFO()
	{
		strUserName = _T("");
		nUserType = 1;
		strPassword = _T("");
	};
}UserInfo,*PUserInfo;

class CMyDatabase : public MySql
{
public:
	CMyDatabase(void);
	~CMyDatabase(void);

	BOOL CreateTable();
	BOOL CleanupTable();

	int InsertData(PIMG_INFO imgInfo);
	BOOL UpdateData(PIMG_INFO imgInfo);

	BOOL IsExistField(CString strField);
	BOOL InsertField(CString strNewField,CString strAfter);

	BOOL DeleteRecord(int id);

	// 用户
	BOOL AddUser(UserInfo user);
	BOOL DeleteUser(CString strUserName);

	BOOL UpdateUser(UserInfo user);
	BOOL UpdateUser(CString strUserName,CString strPassword);
	BOOL UpdateUser(CString strUserName,int nUserType);

	int GetUserInfo(CString strUserName, PUserInfo pUser);
	int GetAllUser();
};

