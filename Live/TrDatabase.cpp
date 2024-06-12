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

bool TrDatabase::init_db()																			// ��ʼ�����ݿ�����
{
	mysql_init(&db_g2020);
	if (!(mysql_real_connect(&db_g2020, MYSQL_IP, MYSQL_USER, MYSQL_PWD, MYSQL_NAME, 0, NULL, 0)))		// �������ӵ����ݿ�
	{
		printf("Error connecting to database:%s\n", mysql_error(&db_g2020));
		return false;
	}
	else
	{
		printf("Database connected...\n");
		memset(st_query, 0, sizeof(st_query));										// ����Ƿ���� car_total ��
		strcpy(st_query, "select * from car_total;");
		state = mysql_query(&db_g2020, st_query);				//����ָ�������ӱ�ʶ�������ķ������еĵ�ǰ����ݿⷢ��һ����ѯ
		if (0 == state)																// ��ѯ�ɹ���˵�� isr_total ����ڣ���ӡ��ʾ��Ϣ���ͷŲ�ѯ��������� true
		{
			printf("car_total is in Car!!\n");
			res = mysql_use_result(&db_g2020);						//��ȡ��ѯ�Ľ����
			mysql_free_result(res);									//�ͷŲ�ѯ�Ľ����
			return true;
		}
		else
		{
			printf("car_total does not exist��%s\n", mysql_error(&db_g2020));		// ��ѯʧ�ܣ�˵�� isr_total �����ڣ���ӡ������Ϣ������ false
			return false;
		}
	}
}

void TrDatabase::insert_total(struct isr_mess*& p)							// ���� isr_total ���¼
{
	memset(st_query, 0, sizeof(st_query));										// ��ղ�ѯ����
	// ���� SQL ��ѯ��䣬ʹ�� sprintf ������ֵ��䵽��ѯ�ַ�����
	sprintf((char*)st_query, "INSERT INTO isr_total(isr_id,net_id,isr_ip,isr_mac,isr_gps,isr_regist_time,isr_com_time,isr_cpu_rate,isr_ram_rate) VALUE(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\');", p->isr_id, p->isr_net_id, p->isr_ip, p->isr_mac, p->isr_id, p->isr_reg_time, p->isr_reg_time, p->isr_cpu, p->isr_ram);						//����Ϊ isr_total �ı��в��� ISR �ĸ�����Ϣ// ������� isr_total ���¼�� SQL ���
	temp_sql = st_query;
	//LOGI("SQL: %s", temp_sql);
	std::cout << "SQL:" << temp_sql << std::endl;
	state = mysql_query(&db_g2020, st_query);
	if (0 == state)
	{
		memset(st_query, 0, sizeof(st_query));									// ����ɹ���������Ӧ�� isr_00xx ��
		sprintf((char*)st_query, "CREATE TABLE isr_00%s(sap_id char(2) NOT NULL,sap_reg_ym char(4) NOT NULL,sap_mac char(16) NULL,sap_isr_id char(2) NULL,sap_isr_mac char(16) NULL,net_id char(4) NULL,sap_gps char(11) NULL,sap_register_time varchar(24) NULL,sap_com_time varchar(24) NULL,com_type char(4) NULL,port_type char(3) NULL,sap_cpu_rate char(2) NULL,sap_ram_rate char(2) NULL,PRIMARY KEY(sap_id,sap_reg_ym)) ENGINE=InnoDB;", p->isr_id);//����isr_00��p->isr_id��// ���촴�� isr_00xx ��� SQL ���

		temp_sql = st_query;
		std::cout << "SQL:" << temp_sql << std::endl;
		state = mysql_query(&db_g2020, st_query);

		if (0 == state)
		{
			std::cout << "Insert isr_total and creat isr_00" << p->isr_id << " Success!!" << std::endl;				// ������ɹ�
		}
		else
		{
			std::cout << "Create table isr_00" << p->isr_id << " failed:" << mysql_error(&db_g2020) << std::endl;		// ������ʧ�ܣ���ӡ������Ϣ
		}
	}
	else
	{
		std::cout << "Insert isr_total failed:" << mysql_error(&db_g2020) << std::endl;						// ����ʧ�ܣ���ӡ������Ϣ
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