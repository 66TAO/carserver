#include "TrDatabase.h"



TrDatabase* TrDatabase::createNew()
{
	return new TrDatabase();
}

TrDatabase::TrDatabase()
{
	//LOGI("TrDatabase");
}

//TrDatabase::~TrDatabase() {};

bool TrDatabase::init_db()																			// 初始化数据库连接
{
	mysql_init(&db_g2020);
	if (!(mysql_real_connect(&db_g2020, MYSQL_IP, MYSQL_USER, MYSQL_PWD, MYSQL_NAME, 0, NULL, 0)))		// 尝试连接到数据库
	{
		printf("Error connecting to database:%s\n", mysql_error(&db_g2020));
		return false;
	}
	else
	{
		printf("Database connected...\n");
		memset(st_query, 0, sizeof(st_query));										// 检查是否存在 car_total 表
		strcpy(st_query, "select * from car_total;");
		state = mysql_query(&db_g2020, st_query);				//向与指定的连接标识符关联的服务器中的当前活动数据库发送一条查询
		if (0 == state)																// 查询成功，说明 isr_total 表存在，打印提示信息，释放查询结果并返回 true
		{
			printf("car_total is in Car!!\n");
			res = mysql_use_result(&db_g2020);						//获取查询的结果集
			mysql_free_result(res);									//释放查询的结果集
			return true;
		}
		else
		{
			printf("car_total does not exist：%s\n", mysql_error(&db_g2020));		// 查询失败，说明 isr_total 表不存在，打印错误信息并返回 false
			return false;
		}
	}
}

void TrDatabase::insert_total(struct isr_mess*& p)							// 插入 isr_total 表记录
{
	memset(st_query, 0, sizeof(st_query));										// 清空查询缓存
	// 构造 SQL 查询语句，使用 sprintf 将变量值填充到查询字符串中
	sprintf((char*)st_query, "INSERT INTO isr_total(isr_id,net_id,isr_ip,isr_mac,isr_gps,isr_regist_time,isr_com_time,isr_cpu_rate,isr_ram_rate) VALUE(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\');", p->isr_id, p->isr_net_id, p->isr_ip, p->isr_mac, p->isr_id, p->isr_reg_time, p->isr_reg_time, p->isr_cpu, p->isr_ram);						//向名为 isr_total 的表中插入 ISR 的各种信息// 构造插入 isr_total 表记录的 SQL 语句
	temp_sql = st_query;
	//LOGI("SQL: %s", temp_sql);
	std::cout << "SQL:" << temp_sql << std::endl;
	state = mysql_query(&db_g2020, st_query);
	if (0 == state)
	{
		memset(st_query, 0, sizeof(st_query));									// 插入成功，创建对应的 isr_00xx 表
		sprintf((char*)st_query, "CREATE TABLE isr_00%s(sap_id char(2) NOT NULL,sap_reg_ym char(4) NOT NULL,sap_mac char(16) NULL,sap_isr_id char(2) NULL,sap_isr_mac char(16) NULL,net_id char(4) NULL,sap_gps char(11) NULL,sap_register_time varchar(24) NULL,sap_com_time varchar(24) NULL,com_type char(4) NULL,port_type char(3) NULL,sap_cpu_rate char(2) NULL,sap_ram_rate char(2) NULL,PRIMARY KEY(sap_id,sap_reg_ym)) ENGINE=InnoDB;", p->isr_id);//建表：isr_00（p->isr_id）// 构造创建 isr_00xx 表的 SQL 语句

		temp_sql = st_query;
		std::cout << "SQL:" << temp_sql << std::endl;
		state = mysql_query(&db_g2020, st_query);

		if (0 == state)
		{
			std::cout << "Insert isr_total and creat isr_00" << p->isr_id << " Success!!" << std::endl;				// 创建表成功
		}
		else
		{
			std::cout << "Create table isr_00" << p->isr_id << " failed:" << mysql_error(&db_g2020) << std::endl;		// 创建表失败，打印错误信息
		}
	}
	else
	{
		std::cout << "Insert isr_total failed:" << mysql_error(&db_g2020) << std::endl;						// 插入失败，打印错误信息
	}
}

void TrDatabase::update_total(struct isr_mess*& p)						
{
	
}

void TrDatabase::insert_isr(struct sap_mess*& p)								
{
	
}

void TrDatabase::update_isr(struct sap_mess*& p)						
{
	
}

void TrDatabase::insert_sap(struct SAP_DATA*& q)			
{
	
}

void TrDatabase::handle_04(struct isr_mess*& isr_mess_reg)
{
	
}

void TrDatabase::handle_05(struct sap_mess*& sap_mess_reg)
{
	
}

void TrDatabase::handle_06(struct SAP_DATA*& sap_data)
{

}

void TrDatabase::handle(struct Car_DATA*& car_data)
{
	memset(st_query, 0, sizeof(st_query));										
	sprintf((char*)st_query, "INSERT INTO car1(name_length,file_path,file_size) VALUE(\'%s\',\'%s\',\'%s\');", car_data->name_length, car_data->file_path, car_data->file_size);
	
	state = mysql_query(&db_g2020, st_query);
	temp_sql = st_query;
	std::cout << "sql: " << temp_sql << std::endl;

	if (0 == state)
	{
		std::cout << "INSERT car1 success" << std::endl;
	}
	else
	{
		std::cout << "INSERT car1 Failed: " << mysql_error(&db_g2020) << std::endl;
	}
}