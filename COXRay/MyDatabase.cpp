#include "StdAfx.h"
#include "MyDatabase.h"


CMyDatabase::CMyDatabase(void)
{
}


CMyDatabase::~CMyDatabase(void)
{
}

BOOL CMyDatabase::CreateTable()
{
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
	CString strDML;
	strDML.Format(_T("CREATE TABLE IF NOT EXISTS %s\
					 (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,\
					 project TEXT NOT NULL,\
					 product TEXT,\
					 product_spec TEXT, \
					 customer TEXT,\
					 department TEXT,\
					 woker_name TEXT,\
					 level INT,\
					 mode INT,\
					 pos TEXT NOT NULL,\
					 time DATETIME NOT NULL,\
					 voltage FLOAT NOT NULL,\
					 current FLOAT NOT NULL,\
					 original_path TEXT NOT NULL,\
					 process_path TEXT,\
					 result TEXT,\
					 error_msg TEXT);"),CString(table_name));


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("创建数据表失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::CleanupTable()
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s;"),CString(table_name)); // 逐条删除，速度慢
//	strDML.Format(_T("TRUNCATE TABLE %s;"),strTableName); // 不记录日志，不可恢复，相当于一个新表，速度快

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("清空数据表失败！"));
	}

	return bRet;
}

int CMyDatabase::InsertData( PIMG_INFO imgInfo )
{
	CString strDML;

	// 路径中把"\"改成"/"
	imgInfo->strOrignalPath.Replace(_T('\\'),_T('/'));
	strDML.Format(_T("INSERT INTO %s(project,product,product_spec,customer,department,woker_name,level,mode,pos,time,voltage,current,original_path) \
					 VALUES ('%s','%s','%s','%s','%s','%s','%d','%d','%s','%s','%.1f','%.2f','%s');"),CString(table_name)
					 ,imgInfo->strProjectName,imgInfo->strProductName
					 ,imgInfo->strProductSpec,imgInfo->strCustomer
					 ,imgInfo->strDepartment,imgInfo->strWorkerName
					 ,imgInfo->level,imgInfo->mode
					 ,imgInfo->strPos,imgInfo->time.Format(_T("%Y-%m-%d %H:%M:%S"))
					 ,imgInfo->dbVolKV,imgInfo->dbCurrentMA
					 ,imgInfo->strOrignalPath);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("插入数据失败！"));

		return 0;
	}

	// 获取当前数据序号
	// MySQL中没有TOP，用的是LIMIT
	//strDML.Format(_T("SELECT TOP 1 id FROM %s ORDER BY id DESC;"),CString(table_name));
	strDML.Format(_T("SELECT id FROM %s ORDER BY id DESC LIMIT 1;"),CString(table_name));
	dml = W2A(strDML);
	int nCount = ExecuteQuery(dml);
	if (nCount != 0)
	{
		return GetIntValue(0,"id");
	}

	return 0;
}

BOOL CMyDatabase::UpdateData( int id,PIMG_INFO imgInfo )
{
	CString strDML;
	imgInfo->strProcessPath.Replace(_T('\\'),_T('/'));
	strDML.Format(_T("UPDATE %s SET process_path='%s',result='%s',error_msg='%s' WHERE id='%d';"),CString(table_name)
		,imgInfo->strProcessPath,imgInfo->bResult ? _T("OK") : _T("NG"),imgInfo->strErrorMsg,imgInfo->id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新数据失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::DeleteRecord( int id )
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s WHERE id='%d';"),CString(table_name),id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("删除数据失败！"));
	}

	return bRet;
}
