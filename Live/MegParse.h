#pragma once
#include <map>
#include <iostream>
#include <string>

extern std::map<std::string, int> device_match;

//������ͷ
#define ISR_REGIST     	 "04" 
#define AP_REGIST 	     "05"
#define SAP_TRANS_DATA	 "06"
#define ISR_TRANS_DATA	 "07"
#define AP_HISTORY_DATA  "08"
#define ISR_HEARTBEAT	 "09"
#define ISR_REPLY1		 "30"
#define ISR_REPLY2		 "31"
#define ISR_UPD_REGIST   "22"
#define ISR_CHANGE       "33"

// ISR��SAP�������ֵ����ݣ��и��ַ���Ϣ
const int pct_st = 1, pct_len = 2;
const int netid_st = 6, netid_len = 4;
const int datalen_st = 12, datalen_len = 4;

// 04��-ISR�豸ע�����ݣ��и��ַ���Ϣ
const int IsrMac_st = 16, IsrMac_len = 16;
const int IsrId_st = 19, IsrId_len = 2;
const int IsrGps_st = 32, IsrGps_len = 11;
const int IsrCpu_st = 43, IsrCpu_len = 2;
const int IsrRam_st = 45, IsrRam_len = 2;
const int IsrIp_st = 47;

// 05��-sap�豸ע�����ݣ��и��ַ���Ϣ
// 05����isr_mac��04��λ��Ŀǰ����һ��
const int SapMac_st = 32, SapMac_len = 16;
const int SapId_st = 35, SapId_len = 2;
const int SapGps_st = 48, SapGps_len = 11;
const int SapCpu_st = 59, SapCpu_len = 2;
const int SapRam_st = 61, SapRam_len = 2;
const int SapComtype_st = 63, SapComtype_len = 4;
const int SapPort_st = 67, SapPort_len = 3;

//06��-sap�豸��������ݣ��и��ַ���Ϣ
const int IsrMac_06_st = 16, IsrMac_06_len = 16;
const int IsrId_06_st = 19, IsrId_06_len = 2;
const int SapMac_06_st = 32, SapMac_06_len = 16;
const int SapId_06_st = 35, SapId_06_len = 2;
const int SapPort_06_st = 48, SapPort_06_len = 3;
const int SapGps_06_st = 51, SapGps_06_len = 11;
const int SapCpu_06_st = 62, SapCpu_06_len = 2;
const int SapRam_06_st = 64, SapRam_06_len = 2;
const int SapComtype_06_st = 66, SapComtype_06_len = 4;
const int HjGass_06_st = 70, HjGass_06_len = 0;
const int HjSoil_06_st = 0, HjSoil_06_len = 0;
const int Hjwater_06_st = 0, Hjwater_06_len = 0;

//HJ212��Gps������
const std::string TIME_DEV = "QN=";
const std::string GPS_DEV = "Gps=";
const std::string MAC_DEV = "MN=";

// ��������HJ212����ͷ
const std::string SO2_SENSOR = "a21026-Rtd=";
const std::string O2_SENSOR = "a19001-Rtd=";
const std::string O3_SENSOR = "a19002-Rtd=";
const std::string CH4_SENSOR = "a05002-Rtd=";
const std::string NO_SENSOR = "a21003-Rtd=";
const std::string NH3_SENSOR = "a21001-Rtd=";
const std::string NO2_SENSOR = "a21004-Rtd=";
const std::string H2S_SENSOR = "a21028-Rtd=";
const std::string CO2_SENSOR = "a05001-Rtd=";
const std::string CO_SENSOR = "a21005-Rtd=";
const std::string TEMPER_SENSOR = "a01001-Rtd=";
const std::string HUMID_SENSOR = "a01002-Rtd=";
const std::string PM10_SENSOR = "a34002-Rtd=";
const std::string PM25_SENSOR = "a34004-Rtd=";
const std::string PM1_SENSOR = "a34005-Rtd=";

// ˮ������HJ212����ͷ
const std::string WATER_TEMPER = "w01010-Rtd=";	//ˮ��
const std::string PH_WATER = "w01001-Rtd=";		//PH
const std::string COND_WATER = "w01014-Rtd=";	//�絼��
const std::string TURB_WATER = "w01012-Rtd=";	//�Ƕ�
const std::string DISSOLVED_OX = "w01009-Rtd=";	//�ܽ���
const std::string COD_WATER = "w00001-Rtd=";		//ˮ�ʺ�����
const std::string TOC_WATER = "w01006-Rtd=";		//���л�̼  TDS(ˮ�����ܽ������ʵ�Ũ��)
const std::string NH3N_WATER = "w21003-Rtd=";	//����
const std::string PHYII_WATER = "w00004-Rtd=";	//Ҷ����
const std::string BGPI_WATER = "w00003-Rtd=";	//������
const std::string CUPRIC_ION = "w20138-Rtd=";    //ͭ����
const std::string CADMIUM_ION = "w20143-Rtd=";   //������
const std::string COD = "w01018-Rtd=";           //��ѧ������

// ˮ������HJ212����ͷ
const std::string VELO_WATER = "w00006-Rtd=";	//����
const std::string DEPTH_WATER = "w00007-Rtd=";	//���

// ��������HJ212����ͷ
const std::string WIND_DIR = "a01008-Rtd=";		//����
const std::string WIND_SPE = "a01007-Rtd=";		//����
const std::string WIND_SPE2M = "q00003-Rtd=";	//2���ӷ���
const std::string WIND_SPE10M = "LA-Rtd=";		//����
const std::string AMBI_TEMP = "q00005-Rtd=";		//�����¶�
const std::string MAX_TEMP = "q00006-Rtd=";		//����¶�
const std::string MIN_TEMP = "q00007-Rtd=";		//����¶�
const std::string AMBI_HUMI = "a01002-Rtd=";		//����ʪ��
const std::string DEWP_HUMI = "q00009-Rtd=";		//¶��ʪ��
const std::string AIR_PRES = "a01006-Rtd=";		//��ѹ
const std::string ILUM = "a01004-Rtd=";			//��ǿ��(����)
const std::string RAIN_FALL = "a01003-Rtd=";		//����

//·������ͷ
const std::string PATH = "from:";

//����HJ212���ݰ�
const int sensor_lenth = 10;			//���˴������õ�21λ
const int gas_lenth = 8;				//���������õ�19λ
const int sensor_flag = 11;
const int time_lenth = 17;
const int time_mac_flag = 3;
const int gps_lenth = 20;
const int gps_flag = 4;
const int mac_lenth = 27;
const int water_lenth = 10;             //����ˮ�����ݳ���

struct SAP_DATA {
    char air_mac[30];           //�����ɼ��豸��Ψһ��ʶ��
    char air_id[4];             //�����ɼ��豸��ID
    char air_sap_id[4];         //SAP�豸��ID
    char air_sap_mac[20];       //SAP�豸��MAC
    char air_isr_id[4];         //������isr�豸ID
    char air_isr_mac[20];       //������isr�豸��Ψһ��ʶ��
    char air_net_id[10];        //����ID
    char air_data_len[10];      //���ݳ���
    char air_com_time[64];      //�����ɼ��豸��ͨ��ʱ��
    char air_real_time[64];     //ϵͳĿǰ��ʵ��ʱ�䣨��ǰ����ʱ�䣩
    char air_com_type[6];       //ͨ�Ųɼ��豸��SAP�豸������
    char air_com_port[6];       //RS485ͨ�Ŷ˿�
    char data_type[6];			//��������
    char sap_gps[20];           //�����豸��GPS��Ϣ
    char sap_dev_gps[20];       //�豸��GPS��Ϣ(��ȡHJ212���������)
    char sap_cpu_rate[4];       //�����豸��cpu��������Ϣ
    char sap_ram_rate[4];       //�����豸���ڴ���������Ϣ
    char gas_data[2048];        //�������ݵ����ݰ�
    char ship_data[2048];       //�����豸���ݵ����ݰ�
    char hj212_crc[10];			//HJ212���ݰ���ѭ��������
    char year[3];				//ע�����
    char month[3];				//ע���·�
    double so2, h2s, nh3, no2, no, co, co2, o2, o3, ch4, ethanol1, ethanol2, ethanol3, pm10, pmD4, temper, hum, pm1;	//��������
    double WATER_TEMPER, ph, conductivity, turbidity, dissolved_ox, cod, toc, nh3n, chlorophyll, bg_algae, cupric_ion, cadmium_ion;//ˮ��
    double flow_velocity, depth_water;//ˮ��
    double wind_direction, wind_speed, wind_speed_2m, wind_speed_10m, ambient_temp, max_temp, min_temp, ambient_humi, dewp_humi, air_press, luminous, rain_fall;//����
    char file_path[64];
};

struct sap_mess {
    char sap_id[4];				//�����豸ID
    char sap_isr_id[4];			//�����豸������isrID
    char sap_net_id[10];		//����ID
    char sap_data_len[10];  	//���ݳ���,˽��Э����
    char sap_mac[20];			//�����豸��mac��Ϣ
    char sap_isr_mac[20];		//�����������豸��mac��Ϣ
    char sap_gps[20];			//�����豸��GPS��Ϣ
    char sap_cpu[4];			//�����豸��cpu��������Ϣ
    char sap_ram[4];			//�����豸���ڴ���������Ϣ
    char sap_reg_time[64];		//�����豸��ע��ʱ��
    char sap_com_type[6];		//ͨ�ŷ�ʽ
    char sap_port[6];			//�˿���Ϣ
    char year_mon[6];			//ע������
};

struct isr_mess
{
    char isr_id[4] = "0";				//����ID
    char isr_net_id[10] = "0";		//����ID
    char isr_data_len[10] = "0";  	//���ݳ���
    char isr_mac[20] = "0";			//ISR��mac��Ϣ
    char isr_gps[20] = "0";			//ISR��GPS��Ϣ
    char isr_cpu[4] = "0";			//ISR��cpu��������Ϣ
    char isr_ram[4] = "0";			//ISR���ڴ���������Ϣ
    char isr_ip[20] = "0";			//ISR��IP��ַ
    char isr_reg_time[64] = "0";		//ISR��ע��ʱ��
};

struct sap_data_33
{
    char isr_id_new[20] = {};
    char sap_id_new[20] = {};
    const char* path_new;
    const char* co2_new;
    const char* hum_new;
    const char* temper_new;
};

struct Car_DATA
{
    char name_length[255];
    char file_path[255];
    char file_size[255];
};

