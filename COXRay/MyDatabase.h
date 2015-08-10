#pragma once
#include "../common/mysql/mysql.h"

static const char *main_table_name = "table_xray";
static const char *sub_table_name = "table_sub";
static const char *user_table_name = "table_user";

static const int main_fileds_width[] = {
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

static const int sub_fileds_width[] = {
	60,
	150,
	100,
	100,
	100,
	100,
	100,
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

typedef enum _ENUM_SUB_TABLE_COL
{
	SUB_ID = 0,
	SUB_TIME,
	SUB_PRODUCT_TIME,
	SUB_MODULE_NO,
	SUB_DEFECT_LEN,
	SUB_LEVEL,
	SUB_USER,
	SUB_PATH
}SubTableCol;

/*
id --- ���
project --- ��Ŀ����
product --- ��Ʒ����
product_spec --- ��Ʒ���
customer --- �ͻ�
department --- ���Ż��߲���
woker_name --- ���Ա�������߹���
level --- ����׼
mode --- ���ģʽ
pos --- ����λ��
time --- ����ʱ��
voltage --- �ܵ�ѹ
current --- �ܵ���
original_path --- ԭʼͼƬ·��
process_path --- �����·��
result --- �����
error_msg --- ��������
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

typedef struct _STRUCT_SUB_TABLE
{
	int id;
	CTime time;
	CString strProductDate;
	CString strModuleNo;
	CString strDefectLen;
	int level;
	CString strUser;
	CString strPath;

	_STRUCT_SUB_TABLE()
	{
		id = 0;
		time = 0;
		level = 0;
	}
}SUB_TABLE,*PSUB_TABLE;

typedef struct _STRUCT_USER_INFO
{
	CString strUserName;
	int nUserType; // 0 - �߼��û��� 1 - ��ͨ�û�
	CString strPassword; // ��������֮�������

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

	BOOL CleanupTable(CString strTableName);
	BOOL DeleteTable(CString strTableName);

	int InsertData(PIMG_INFO imgInfo);
	BOOL UpdateResultData(PIMG_INFO imgInfo);
	BOOL UpdateData(PIMG_INFO imgInfo);

	BOOL IsExistField(CString strField);
	BOOL InsertField(CString strTableName,CString strNewField,CString strAfter);

	BOOL DeleteRecord(int id,CString strTable);

	// �û�
	BOOL AddUser(UserInfo user);
	BOOL DeleteUser(CString strUserName);

	BOOL UpdateUser(UserInfo user);
	BOOL UpdateUser(CString strUserName,CString strPassword);
	BOOL UpdateUser(CString strUserName,int nUserType);

	int GetUserInfo(CString strUserName, PUserInfo pUser);
	int GetAllUser();

	// SUBTable
	BOOL AddSubTable(SUB_TABLE sub);
	BOOL UpdateSubTable(SUB_TABLE sub);
};

