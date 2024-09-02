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

std::string TrDatabase::handle_request(const std::string& request) {
	std::string clean_request = request;
	clean_request.erase(std::remove(clean_request.begin(), clean_request.end(), '/'), clean_request.end());
	memset(st_query, 0, sizeof(st_query));  // ��ղ�ѯ����
	sprintf((char*)st_query, "SELECT radar_path,inside_path,outside_path FROM Car01_2408 WHERE car_get_time LIKE '%s%%';", clean_request.c_str());

	state = mysql_query(&db_g2020, st_query);
	temp_sql = st_query;
	std::cout << "SQL: " << temp_sql << std::endl;

	nlohmann::json json_response;

	if (0 == state) {
		res = mysql_use_result(&db_g2020);
		if (res == NULL) {
			LOGE(mysql_error(&db_g2020));
			return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		}

		if (mysql_num_rows(res) == 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				nlohmann::json record;
				record["radar_path"] = row[0] ? row[0] : "";
				record["inside_path"] = row[1] ? row[1] : "";
				record["outside_path"] = row[2] ? row[2] : "";
				json_response.push_back(record);
			}
			mysql_free_result(res);
		}
		else {
			mysql_free_result(res);
			return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
		}
	}
	else {
		std::cout << "select error��" << mysql_error(&db_g2020) << std::endl;

		if (0 != mysql_ping(&db_g2020)) {
			mysql_close(&db_g2020);
			init_db();
		}
		return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
	}

	// ������Ϊ�գ�Ҳ����404
	if (json_response.empty()) {
		return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
	}

	// ��JSON����ת��Ϊ�ַ���
	std::string response = json_response.dump();

	// ���HTTPͷ��Ϣ
	std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type\r\nContent-Length: " + std::to_string(response.size()) + "\r\n\r\n" + response;
	return http_response;
}

//std::string TrDatabase::handle_request(const std::string& request) {
//	// ��������������Ӧ
//	if (request == "/hello") {
//		return "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
//	}
//	else {
//		return "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
//	}
//}

//std::string TrDatabase::handle_request(const std::string& request) {
//	std::string clean_request = request;
//	clean_request.erase(std::remove(clean_request.begin(), clean_request.end(), '/'), clean_request.end());
//	memset(st_query, 0, sizeof(st_query));  // ��ղ�ѯ����
//	sprintf((char*)st_query, "SELECT radar_path,inside_path,outside_path FROM Car01_2408 WHERE car_get_time LIKE '%s%%';", clean_request.c_str());
//	//sprintf((char*)st_query, "SELECT radar_path,inside_path,outside_path FROM Car01_2408 WHERE car_get_time = '%s';", clean_request.c_str());
//	//sprintf((char*)st_query, "SELECT radar_path,inside_path,outside_path FROM Car01_2408 WHERE car_index = '%s';", clean_request.c_str());
//	state = mysql_query(&db_g2020, st_query);
//	temp_sql = st_query;
//	std::cout << "SQL: " << temp_sql << std::endl;
//
//	std::string result;
//
//	if (0 == state) {
//		res = mysql_use_result(&db_g2020);
//		if (res == NULL) {
//			LOGE(mysql_error(&db_g2020));
//			return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
//		}
//
//		//if (mysql_num_rows(res) == 0) {
//		//	mysql_free_result(res);
//		//	return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
//		//}
//		//std::cout << mysql_num_rows(res) << std::endl;
//		//std::cout << res << std::endl;
//		//while ((row = mysql_fetch_row(res)) != NULL) {
//		//	std::string radar_path = row[0] ? row[0] : "";
//		//	std::string inside_path = row[1] ? row[1] : "";
//		//	std::string outside_path = row[2] ? row[2] : "";
//
//		//	// �����ƴ�ӳ�һ���ַ������账��
//		//	result += "Radar Path: " + radar_path + ", Inside Path: " + inside_path + ", Outside Path: " + outside_path + "\n";
//		//}
//
//		//mysql_free_result(res);
//
//		if (mysql_num_rows(res) == 0) {
//			while ((row = mysql_fetch_row(res)) != NULL) {
//				std::string radar_path = row[0] ? row[0] : "";
//				std::string inside_path = row[1] ? row[1] : "";
//				std::string outside_path = row[2] ? row[2] : "";
//
//				// �����ƴ�ӳ�һ���ַ������账��
//				result += "Radar Path: " + radar_path + ", Inside Path: " + inside_path + ", Outside Path: " + outside_path + "\n";
//			}
//		}
//		else
//		{
//			mysql_free_result(res);
//			return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
//		}
//	}
//	else {
//		std::cout << "select error��" << mysql_error(&db_g2020) << std::endl;
//
//		if (0 != mysql_ping(&db_g2020)) {
//			mysql_close(&db_g2020);
//			init_db();
//		}
//		return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
//	}
//
//	if (result.empty()) {
//		return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
//	}
//
//	// ���HTTPͷ��Ϣ
//	std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(result.size()) + "\r\n\r\n" + result;
//	return http_response;
//}