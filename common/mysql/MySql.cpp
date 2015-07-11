// MySql.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MySql.h"

//һ���ֶνṹ
typedef struct _FIELDS
{
	char field_name[DEFAULT_FIELD_SIZE];//�ֶ���
	void * filed_value; //�ֶ�ֵ��ָ��
	unsigned value_len; //�ֶεĳ��ȣ�ֻ�������ַ�������
}FIELDS,*PFIELDS;

//һ�����ݵĽṹ����
typedef struct _ROWS{
    FIELDS fields[DEFAULT_FIELD];   //�ֶ�,���ݲ�ͬ��Ӧ�ó�������޸Ĵ�ֵ
	int cnt;             //���ٸ��ֶΣ�ָʾfield��Ԫ�ص���Ч����
}ROWS,*PROWS;


MySql::MySql()
{
	m_result=NULL;
	memset(m_host,0,32);
	memset(m_name,0,32);
	memset(m_pwd,0,32);
	m_port=0;
	mysql_init(&m_myData);

	//�������ӳ�ʱ
	unsigned timeout=5*60;
	mysql_options(&m_myData, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout);
	
	//����������
	my_bool reconnect=TRUE;
    mysql_options(&m_myData, MYSQL_OPT_RECONNECT, (char *)&reconnect);

	//��ʼ��HashTable��СΪ100
	m_res.InitHashTable(100);
}
MySql::~MySql()
{
	Close();
}

//����MySql������
BOOL MySql::Connect(char * host,char * name,char *pwd,char * db,unsigned port)
{
	BOOL bRet=TRUE;
	strcpy(m_host,host);
	strcpy(m_name,name);
	strcpy(m_pwd,pwd);
	m_port=port;
	do
	{
		//���ӷ�����
		if(mysql_real_connect(&m_myData,host,name,pwd,db,port,NULL,0)==NULL)
		{
			bRet=FALSE;
			break;
		}
		
		//���������ַ�����Ĭ��Ϊgb2312
		if(mysql_set_character_set(&m_myData, "gb2312"))
		{
			bRet=FALSE;
			//��������ַ������ɹ�����ر�����
			Close();
			break;
		}
		//������������֧��
		mysql_set_server_option(&m_myData, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	}while(0);
	return bRet;
}

//ִ�зǲ�ѯ���
BOOL MySql::ExecuteNoQuery(char ** sqlArray,int count)
{
	BOOL uRet=TRUE;

	// ˢ��
	mysql_reload(&m_myData);

	//�����Զ��ύģʽ
     mysql_autocommit(&m_myData, 0); 
	 for(int i=0;i<count;i++)
	 {
		 if (mysql_query(&m_myData,sqlArray[i]) != 0)
		 {
			 return FALSE;
		 }
		 //��ѯ��Ӱ�������
		 //uRet=uRet+(unsigned)mysql_affected_rows(&myData); 
	 }
	 //���û�гɹ�
	 if(mysql_commit(&m_myData))
	 {
		 //�ع�����
		 mysql_rollback(&m_myData);
		 uRet=FALSE;
	 }
     //�ָ��Զ��ύģʽ
     mysql_autocommit(&m_myData, 1); 
	 return uRet;
}
//ִ�в�ѯ���
unsigned MySql::ExecuteQuery(char * sql)
{
	unsigned uLine=0,uField=0;
	PROWS rows;
	MYSQL_ROW row;
	MYSQL_FIELD *pField;
    //�ͷ�ǰһ�εĽ����
	FreeResult();

	// ˢ��
	mysql_reload(&m_myData);
	//��ѯ
    mysql_query(&m_myData,sql);
    //�洢�����
    m_result=mysql_store_result(&m_myData);
    //��������Ϊ�գ��򷵻�0
    if(m_result==NULL)return 0;
    //�������
    uLine=(unsigned)mysql_num_rows(m_result);
    //����ֶ���
    uField=(unsigned)mysql_num_fields(m_result);
   
   //�������
   for(int i=0;i<(int)uLine;i++)
   {
	   //Ϊ�洢һ�����ݷ����ڴ�
      rows=(PROWS)malloc(sizeof(ROWS));
	  //һ���ж��ٸ��ֶ�
	  rows->cnt=uField;
	  //���һ������
	  row=mysql_fetch_row(m_result);
	  for(int j=0;j<(int)uField;j++)
	  {
		 //��ø��е������Ϣ
		  pField=mysql_fetch_field_direct(m_result, j);
		  //������
		  strcpy(rows->fields[j].field_name,pField->name);
		  //��ֵ
		  rows->fields[j].filed_value=row[j];
		  //�п��
		  rows->fields[j].value_len=pField->max_length;
	  }
	  //��һ�����ݴ洢��HashTable��
	  m_res.SetAt(i,(void *)rows);
   }
   //��������
   return uLine;
}

//�ͷŲ�ѯ�Ľ����
void MySql::FreeResult()
{
	int len=0;
	PROWS rows=NULL;

	len=m_res.GetCount();
	//�ͷŽ�������ڴ�
	for(int i=0;i<len;i++)
	{
		rows=(PROWS)m_res[i];
		if(rows!=NULL)
		{
			free(rows);
			rows=NULL;
		}
	}
   //���HashTable
	m_res.RemoveAll();
	//�ͷ�result
    if(m_result!=NULL)
	{
	   mysql_free_result(m_result);
	}
	m_result=NULL;
}

//ִ�д�������SQL��亯��
BOOL MySql::ExecutePrepare(char * sql)
{
	BOOL bRet=TRUE;

	do
	{
		//��ʼ�����
		m_myStmt=mysql_stmt_init(&m_myData);
		if (!m_myStmt)
		{
			bRet=FALSE;
			break;
		}
		//׼��SQL
		if (mysql_stmt_prepare(m_myStmt, sql, strlen(sql)))
		{
			bRet=FALSE;
			break;
		}
	}while(0);
	return bRet;
}
BOOL MySql::BindParam(MYSQL_BIND * bind)
{
	BOOL bRet=TRUE;
	if (mysql_stmt_bind_param(m_myStmt, bind))
	{
       bRet=FALSE;
	}
	return bRet;

}
unsigned MySql::Execute()
{
	unsigned uRet=0;
	do
	{
		if (mysql_stmt_execute(m_myStmt))
		{
			uRet=0;
			break;
		}
		//�����Ӱ������� 
		uRet=(unsigned)mysql_stmt_affected_rows(m_myStmt);
	}while(0);
    return uRet;
}
BOOL MySql::ClosePrepare()
{
	BOOL bRet=TRUE;
	//�رվ��
	if (mysql_stmt_close(m_myStmt))
	{
       bRet=FALSE;
	}
	return bRet;
}

//�ı��û�
BOOL MySql::ChangeUser(char * name,char *pwd,char *db)
{
	BOOL bRet=TRUE;
	//���ʧ�ܣ�����FALSE
	if(mysql_change_user(&m_myData, name, pwd, db) )
	{
		bRet=FALSE;
	}
	return bRet;
}
//ѡ�����ݿ�
BOOL MySql::SelectDB(char * db)
{
	BOOL bRet=TRUE;
	if(mysql_select_db(&m_myData, db))
	{
		bRet=FALSE;
	}
	return bRet;
}


//�ر�
void MySql::Close()
{
   //��ս����
    FreeResult();
	//�ر�����
	mysql_close(&m_myData);
}


//��ȡ��ѯ��ֵ
int MySql::GetIntValue(int row, char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	int iRet=0;
	char * tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			//������ݵ��ַ�����ʾ
			tmp=(char *)rows->fields[i].filed_value;
			//ת��������
			iRet=atoi(tmp);
			break;
		}
	}
	return iRet;
}
int MySql::GetIntValue(int row, int fd_num)
{
    PROWS rows=NULL;
	int len=0,iRet=0;
	char * tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];

    //������ֵ��ַ�����ʾ
	tmp=(char *)rows->fields[fd_num].filed_value;
	//ת��������
	iRet=atoi(tmp);
	return iRet;
}

//��÷�����ֵ
float MySql::GetFloatValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	float iRet=0.0;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
			iRet=(float)atof(tmp);
			break;
		}
	}
	return iRet;
}
float MySql::GetFloatValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	float iRet=0.0;
	char * tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];

    tmp=(char *)rows->fields[fd_num].filed_value;
    iRet=(float)atof(tmp);
	return iRet;
}

//��ø߾��ȷ�����ֵ
double MySql::GetDoubleValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	double iRet=0.0;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
		    iRet=atof(tmp);
			break;
		}
	}
	return iRet;
}
double MySql::GetDoubleValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	double iRet=0.0;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];

	tmp=(char *)rows->fields[fd_num].filed_value;
	iRet=atof(tmp);
	return iRet;
}

//��ò�����ֵ
BOOL MySql::GetBoolValue(int row,char * fd_name)
{
   	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	BOOL iRet=0;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
			iRet=(BOOL)atoi(tmp);
			break;
		}
	}
	return iRet;
}
BOOL MySql::GetBoolValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	BOOL iRet=0;
	char * tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];

	tmp=(char *)rows->fields[fd_num].filed_value;
	iRet=(BOOL)atoi(tmp);
	return iRet;
}

char * MySql::GetStringValue(int row,char * fd_name,unsigned * length)
{
	PROWS rows=NULL;
	int len=0;
	char *p_Tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			*length=(unsigned)(rows->fields[i].value_len);
			p_Tmp=(char *)(rows->fields[i].filed_value);
			break;
		}
	}
	return p_Tmp;
}
char * MySql::GetStringValue(int row,int fd_num, unsigned * length)
{
	PROWS rows=NULL;
	int len=0;
	char * p_Tmp=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];

	//����г���
	*length=(unsigned)rows->fields[fd_num].value_len;
	//ֱ�ӷ��ش��е�ֵ
	p_Tmp=(char *)(rows->fields[fd_num].filed_value);
	return p_Tmp;
}

//�������ʱ��������
char * MySql::GetDataTimeValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * pTime=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			pTime=(char *)(rows->fields[i].filed_value);
			break;
		}
	}
	return pTime;
}
char * MySql::GetDataTimeValue(int row, int fd_num)
{
   	PROWS rows=NULL;
	int len=0;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	rows=(PROWS)m_res[row];

	//ֱ�ӷ��ش��е�ֵ
	return (char *)(rows->fields[fd_num].filed_value);
}

//��ö��������ݵĻ�����
int MySql::GetBinaryValue(int row,char * fd_name,char * pBuf)
{
	PROWS rows=NULL;
	int len=0;
	char * p=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	rows=(PROWS)m_res[row];
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
		    len=(int)rows->fields[i].value_len;
			if(pBuf==NULL)
				break;
			p=(char *)(rows->fields[i].filed_value);
			memcpy(pBuf,p,len);
			break;
		}
	}
	return len;
}
int MySql::GetBinaryValue(int row,int fd_num,char * pBuf)
{
	PROWS rows=NULL;
	int len=0;
	char * p=NULL;
	len=m_res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	rows=(PROWS)m_res[row];
    
	len=rows->fields[fd_num].value_len;

	if(pBuf==NULL)
		return len;
  
	//ֱ�ӷ��ش��е�ֵ
	 p=(char *)(rows->fields[fd_num].filed_value);
	 memcpy(pBuf,p,len);
	 return len;
}

BOOL MySql::IsTableExist( const char * tablename )
{
	char sql[1024] = {NULL};
	sprintf_s(sql,"SELECT table_name FROM information_schema.TABLES WHERE table_name = '%s'; ",tablename);

	return ExecuteQuery(sql);
}
