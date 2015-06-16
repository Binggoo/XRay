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
		AfxMessageBox(_T("�������ݱ�ʧ�ܣ�"));
	}

	return bRet;
}

BOOL CMyDatabase::CleanupTable()
{
	CString strDML;
	strDML.Format(_T("DELETE FROM %s;"),CString(table_name)); // ����ɾ�����ٶ���
//	strDML.Format(_T("TRUNCATE TABLE %s;"),strTableName); // ����¼��־�����ɻָ����൱��һ���±��ٶȿ�

	USES_CONVERSION;
	char *dml = W2A(strDML);

	BOOL bRet = ExecuteNoQuery(&dml,1);

	if (!bRet)
	{
		AfxMessageBox(_T("������ݱ�ʧ�ܣ�"));
	}

	return bRet;
}

int CMyDatabase::InsertData( PIMG_INFO imgInfo )
{
	CString strDML;

	// ·���а�"\"�ĳ�"/"
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
		AfxMessageBox(_T("��������ʧ�ܣ�"));

		return 0;
	}

	// ��ȡ��ǰ�������
	// MySQL��û��TOP���õ���LIMIT
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
		AfxMessageBox(_T("��������ʧ�ܣ�"));
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
		AfxMessageBox(_T("ɾ������ʧ�ܣ�"));
	}

	return bRet;
}
