#pragma once
#include "../common/mysql/mysql.h"

static const char *table_name = "table_xray";

class CMyDatabase : public MySql
{
public:
	CMyDatabase(void);
	~CMyDatabase(void);

	BOOL CreateTable();
	BOOL CleanupTable();

	int InsertData(PIMG_INFO imgInfo);
	BOOL UpdateData(int id,PIMG_INFO imgInfo);
};

