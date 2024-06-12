#pragma once

#include <mysql.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "MegParse.h"
#include "../Base/Log.h"
#define MYSQL_IP  "127.0.0.1"    	   	   // ���ݿ�IP
#define MYSQL_PWD "Tr708708708"            // ���ݿ�����
#define MYSQL_USER "Car" 			   // ���ݿ��˺�
#define MYSQL_NAME "car"      		   // ���ݿ�����

class TrDatabase
{
public:
	static TrDatabase* createNew();
	TrDatabase();
	bool init_db();
	void handle_04(struct isr_mess*& isr_mess_reg);
	void handle_05(struct sap_mess*& sap_mess_reg);
	void handle_06(struct SAP_DATA*& sap_data);
	void handle_33(struct sap_data_33*& sap_data);
	void handle(struct Car_DATA*& car_data);
	

private:
	void insert_total(struct isr_mess*& p);
	void update_total(struct isr_mess*& p);
	void insert_isr(struct sap_mess*& p);
	void update_isr(struct sap_mess*& p);
	void insert_sap(struct SAP_DATA*& q);

private:
	int state;
	char st_query[4096] = {};
	int flag = 0;
	std::string temp_sql;
	MYSQL db_g2020;
	MYSQL* connection;
	MYSQL_RES* res;
	MYSQL_ROW row;

};
