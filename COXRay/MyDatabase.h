#pragma once
#include "../common/mysql/mysql.h"

#define QUERY_FIELDS_COUNT 12
#define TABLE_FIELDS_COUNT 17

static const char *table_name = "table_xray";
static const char *query_fields[] = {
	"project",
	"product",
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

class CMyDatabase : public MySql
{
public:
	CMyDatabase(void);
	~CMyDatabase(void);

	BOOL CreateTable();
	BOOL CleanupTable();

	int InsertData(PIMG_INFO imgInfo);
	BOOL UpdateData(int id,PIMG_INFO imgInfo);

	BOOL DeleteRecord(int id);
};

