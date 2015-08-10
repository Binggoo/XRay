#include "StdAfx.h"
#include "MyDatabase.h"


CMyDatabase::CMyDatabase(void)
{
}


CMyDatabase::~CMyDatabase(void)
{
}

BOOL CMyDatabase::CleanupTable(CString strTableName)
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s;"),strTableName); // 逐条删除，速度慢
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

BOOL CMyDatabase::DeleteTable(CString strTableName)
{
	CString strDML;
	strDML.Format(_T("TRUNCATE TABLE %s;"),strTableName); // 不记录日志，不可恢复，相当于一个新表，速度快

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
	strDML.Format(_T("CREATE TABLE IF NOT EXISTS %s\
					 (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,\
					 project TEXT NOT NULL,\
					 product TEXT,\
					 serial TEXT,\
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
					 error_msg TEXT);"),CString(main_table_name));


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("创建数据表失败！"));
		return -1;
	}

	// 路径中把"\"改成"/"
	imgInfo->strOrignalPath.Replace(_T('\\'),_T('/'));
	strDML.Format(_T("INSERT INTO %s(project,product,serial,product_spec,customer,department,woker_name,level,mode,pos,time,voltage,current,original_path) \
					 VALUES ('%s','%s','%s','%s','%s','%s','%s','%d','%d','%s','%s','%.1f','%.2f','%s');"),CString(main_table_name)
					 ,imgInfo->strProjectName,imgInfo->strProductName,imgInfo->strSerialNumber
					 ,imgInfo->strProductSpec,imgInfo->strCustomer
					 ,imgInfo->strDepartment,imgInfo->strWorkerName
					 ,imgInfo->level,imgInfo->mode
					 ,imgInfo->strPos,imgInfo->time.Format(_T("%Y-%m-%d %H:%M:%S"))
					 ,imgInfo->dbVolKV,imgInfo->dbCurrentMA
					 ,imgInfo->strOrignalPath);

	dml = W2A(strDML);

	bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("插入数据失败！"));

		return 0;
	}

	// 获取当前数据序号
	// MySQL中没有TOP，用的是LIMIT
	//strDML.Format(_T("SELECT TOP 1 id FROM %s ORDER BY id DESC;"),CString(table_name));
	strDML.Format(_T("SELECT id FROM %s ORDER BY id DESC LIMIT 1;"),CString(main_table_name));
	dml = W2A(strDML);
	int nCount = ExecuteQuery(dml);
	if (nCount != 0)
	{
		return GetIntValue(0,"id");
	}

	return 0;
}

BOOL CMyDatabase::InsertField( CString strTableName,CString strNewField,CString strAfter )
{
	BOOL bRet = TRUE;
	if (!IsExistField(strNewField))
	{
		CString strDML;
		strDML.Format(_T("ALTER TABLE %s ADD %s TEXT AFTER %s;"),strTableName,strNewField,strAfter);

		USES_CONVERSION;
		char *dml = W2A(strDML);

		bRet = ExecuteNoQuery(&dml,1);

		if (!bRet)
		{
			AfxMessageBox(_T("插入字段失败！"));
		}
	}

	return bRet;
}

BOOL CMyDatabase::IsExistField( CString strField )
{
	CString strDML;
	strDML.Format(_T("DESC %s %s;"),CString(main_table_name),strField);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	return ExecuteQuery(dml);
}

BOOL CMyDatabase::UpdateResultData( PIMG_INFO imgInfo )
{
	CString strDML;
	imgInfo->strProcessPath.Replace(_T('\\'),_T('/'));
	strDML.Format(_T("UPDATE %s SET serial='%s',process_path='%s',result='%s',error_msg='%s' WHERE id='%d';"),CString(main_table_name)
		,imgInfo->strSerialNumber,imgInfo->strProcessPath
		,imgInfo->bResult ? _T("OK") : _T("NG"),imgInfo->strErrorMsg,imgInfo->id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新数据失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::DeleteRecord( int id ,CString strTable)
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s WHERE id='%d';"),strTable,id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("删除数据失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::AddUser( UserInfo user )
{
	CString strDML;
	strDML.Format(_T("CREATE TABLE IF NOT EXISTS %s\
					 (user_name VARCHAR(20) NOT NULL PRIMARY KEY,\
					 user_type INT NOT NULL,\
					 password TEXT);"),CString(user_table_name));


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("创建用户表失败！"));

		return bRet;
	}

	strDML.Format(_T("INSERT INTO %s(user_name,user_type,password) VALUES ('%s','%d','%s');")
		,CString(user_table_name),user.strUserName,user.nUserType,user.strPassword);

	dml = W2A(strDML);

	bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("添加用户失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::DeleteUser( CString strUserName )
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s WHERE username='%s';"),CString(user_table_name),strUserName);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("删除用户失败！"));

	}
	return bRet;
}

BOOL CMyDatabase::UpdateUser( UserInfo user )
{
	CString strDML;
	strDML.Format(_T("UPDATE %s SET user_type='%d',password='%s' WHERE user_name='%s';")
		,CString(user_table_name),user.nUserType,user.strPassword,user.strUserName);


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新用户失败！"));

	}

	return bRet;
}

BOOL CMyDatabase::UpdateUser( CString strUserName,CString strPassword )
{
	CString strDML;
	strDML.Format(_T("UPDATE %s SET password='%s' WHERE user_name='%s';")
		,CString(user_table_name),strPassword,strUserName);


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新用户失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::UpdateUser( CString strUserName,int nUserType )
{
	CString strDML;
	strDML.Format(_T("UPDATE %s SET user_type='%d' WHERE user_name='%s';")
		,CString(user_table_name),nUserType,strUserName);


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新用户失败！"));
	}

	return bRet;
}

int CMyDatabase::GetUserInfo( CString strUserName, PUserInfo pUser )
{
	CString strDML;
	strDML.Format(_T("SELECT * FROM %s WHERE user_name='%s';")
		,CString(user_table_name),strUserName);


	USES_CONVERSION;
	char *dml = W2A(strDML);

	int nCount = ExecuteQuery(dml);

	if (nCount != 0 && pUser != NULL)
	{
		unsigned int len = 255;
		pUser->strUserName = strUserName;
		pUser->nUserType = GetIntValue(0,1);
		pUser->strPassword = CString(GetStringValue(0,2,&len));
	}

	return nCount;
}

int CMyDatabase::GetAllUser()
{
	CString strDML;
	strDML.Format(_T("SELECT * FROM %s;")
		,CString(user_table_name));


	USES_CONVERSION;
	char *dml = W2A(strDML);

	int nCount = ExecuteQuery(dml);

	return nCount;
}

BOOL CMyDatabase::AddSubTable( SUB_TABLE sub )
{
	CString strDML;
	strDML.Format(_T("CREATE TABLE IF NOT EXISTS %s\
					 (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,\
					 time DATETIME NOT NULL,\
					 product_date TEXT,\
					 module_no TEXT,\
					 defect_len TEXT,\
					 level INT,\
					 user TEXT,\
					 path TEXT);"),CString(sub_table_name));


	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("创建子表失败！"));

		return bRet;
	}

	sub.strPath.Replace(_T('\\'),_T('/'));

	strDML.Format(_T("INSERT INTO %s(time,product_date,module_no,defect_len,level,user,path) \
					 VALUES ('%s','%s','%s','%s','%d','%s','%s');")
		,CString(sub_table_name),sub.time.Format(_T("%Y-%m-%d %H:%M:%S"))
		,sub.strProductDate,sub.strModuleNo,sub.strDefectLen,sub.level,sub.strUser,sub.strPath);

	dml = W2A(strDML);

	bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("添加数据失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::UpdateSubTable( SUB_TABLE sub )
{
	CString strDML;
	sub.strPath.Replace(_T('\\'),_T('/'));

	strDML.Format(_T("UPDATE %s SET time='%s',product_date='%s',module_no='%s',\
					 defect_len='%s',level='%d',user='%s',path='%s' WHERE id='%d';")
					 ,CString(sub_table_name),sub.time.Format(_T("%Y-%m-%d %H:%M:%S"))
					 ,sub.strProductDate,sub.strModuleNo,sub.strDefectLen
					 ,sub.level,sub.strUser,sub.strPath,sub.id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新数据失败！"));
	}

	return bRet;
}

BOOL CMyDatabase::UpdateData( PIMG_INFO imgInfo )
{
	CString strDML;
	imgInfo->strProcessPath.Replace(_T('\\'),_T('/'));

	strDML.Format(_T("UPDATE %s SET project='%s',product='%s',serial='%s',\
					 product_spec='%s',customer='%s',department='%s',woker_name='%s',\
					 level='%d',mode='%d',pos='%s',time='%s',voltage='%f',\
					 current='%f',original_path='%s',process_path='%s',result='%s',\
					 error_msg='%s' WHERE id='%d';")
					 ,CString(sub_table_name)
					 ,imgInfo->strProjectName,imgInfo->strProductName,imgInfo->strSerialNumber
					 ,imgInfo->strProductSpec,imgInfo->strCustomer
					 ,imgInfo->strDepartment,imgInfo->strWorkerName
					 ,imgInfo->level,imgInfo->mode
					 ,imgInfo->strPos,imgInfo->time.Format(_T("%Y-%m-%d %H:%M:%S"))
					 ,imgInfo->dbVolKV,imgInfo->dbCurrentMA
					 ,imgInfo->strOrignalPath
					 ,imgInfo->strProcessPath
					 ,imgInfo->bResult ? _T("OK") : _T("NG")
					 ,imgInfo->strErrorMsg
					 ,imgInfo->id);

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("更新数据失败！"));
	}

	return bRet;
}
