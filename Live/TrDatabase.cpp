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

void TrDatabase::insert_total(struct Car_DATA*& p)							// ���� isr_total ���¼
{
	memset(st_query, 0, sizeof(st_query));										// ��ղ�ѯ����
	// ���� SQL ��ѯ��䣬ʹ�� sprintf ������ֵ��䵽��ѯ�ַ�����
	sprintf((char*)st_query, "INSERT INTO car_total(Car_id,Car_reg_time,Car_com_time) VALUE(\'%s\',\'%s\',\'%s\');", p->car_id, p->car_com_time, p->car_com_time);						//����Ϊ isr_total �ı��в��� ISR �ĸ�����Ϣ// ������� isr_total ���¼�� SQL ���
	temp_sql = st_query;
	//LOGI("SQL: %s", temp_sql);
	std::cout << "SQL:" << temp_sql << std::endl;
	state = mysql_query(&db_g2020, st_query);
	if (0 == state)
	{
		memset(st_query, 0, sizeof(st_query));									// ����ɹ���������Ӧ�� isr_00xx ��
		sprintf((char*)st_query, "CREATE TABLE Car%s_%c%c%c%c(car_index INT NOT NULL AUTO_INCREMENT,radar_path char(255) NULL,inside_path char(255) NOT NULL,outside_path char(255) NULL,car_get_time char(64) NULL,car_com_time char(64) NULL,PRIMARY KEY (car_index));", p->car_id, p->year[0], p->year[1], p->month[0], p->month[1]);//����isr_00��p->isr_id��// ���촴�� isr_00xx ��� SQL ���
		temp_sql = st_query;
		std::cout << "SQL:" << temp_sql << std::endl;
		state = mysql_query(&db_g2020, st_query);

		if (0 == state)
		{
			std::cout << "Insert car_total and creat Car_" << p->car_id << " Success!!" << std::endl;				// ������ɹ�
			insert_data(p);														// ���� car2 ���¼
		}
		else
		{
			std::cout << "Create table Car_" << p->car_id << " failed:" << mysql_error(&db_g2020) << std::endl;		// ������ʧ�ܣ���ӡ������Ϣ
		}
	}
	else
	{
		std::cout << "Insert car_total failed:" << mysql_error(&db_g2020) << std::endl;						// ����ʧ�ܣ���ӡ������Ϣ
	}
}

void TrDatabase::update_total(struct isr_mess*& p)						
{
	
}

void TrDatabase::insert_data(struct Car_DATA*& p)
{
	memset(st_query, 0, sizeof(st_query));
	sprintf((char*)st_query, "INSERT INTO Car%s_%c%c%c%c(radar_path,inside_path,outside_path,car_get_time,car_com_time)VALUE(\'%s\',\'%s\',\'%s\',\'%s\',NOW());", p->car_id, p->year[0], p->year[1], p->month[0], p->month[1], p->radar_file_path, p->inside_file_path, p->outside_file_path, p->car_get_time);

	temp_sql = st_query;
	state = mysql_query(&db_g2020, st_query);

	if (0 == state)
	{
		//���ݲ���ɹ����ٸ���isr_total��isr_00xx�����Ϣ
		std::cout << "INSERT INTO car_" << p->car_id << "_" << p->year[0] << p->year[1] << p->month[0] << p->month[1] << " Success!" << std::endl;
	}
	//����ʧ��
	else
	{
		std::cout << "INSERT INTO " << "car_" << p->car_id << " failed: " << mysql_error(&db_g2020) << std::endl;
	}
	return;
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
	memset(st_query, 0, sizeof(st_query));										// ��ղ�ѯ����
	sprintf((char*)st_query, "SELECT * FROM car_total WHERE car_id = \'%s\';", car_data->car_id);			// ���� SQL ��ѯ��䣬��ѯ�Ƿ������ isr_id ƥ��ļ�¼
	state = mysql_query(&db_g2020, st_query);																	// ִ�� SQL ��ѯ
	temp_sql = st_query;																						// �����ѯ��䣬�������������Ϣ
	std::cout << "SQL: " << temp_sql << std::endl;
	if (0 == state)
	{
		res = mysql_use_result(&db_g2020);																		// ��ȡ��ѯ���
		if (NULL != (row = mysql_fetch_row(res)))																// ��ѯ����ǿգ�˵�������� isr_id ƥ��ļ�¼
		{
			std::cout << "car_id: " << car_data->car_id << " already exists" << std::endl;						// �����ʾ��Ϣ
			mysql_free_result(res);																				// �ͷŲ�ѯ���
			insert_data(car_data);																				// ���� car2 ���¼
		}
		else
		{
			std::cout << "car_id: " << car_data->car_id << " does not exist" << std::endl;						// �����ʾ��Ϣ
			mysql_free_result(res);																				// �ͷŲ�ѯ���
			insert_total(car_data);																				// ���� car1 ���¼
		}
	}
	else
	{
		std::cout << "SELECT car_total failed: " << mysql_error(&db_g2020) << std::endl;						// ��ѯʧ�ܣ����������Ϣ
	}

}