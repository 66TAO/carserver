//#pragma once
//#include <time.h>
//#include <string>
//#include <vector>
//
//static std::string getFile(std::string file) {
//#ifndef WIN32
//    std::string pattern = "/";
//#else
//    std::string pattern = "\\";
//#endif // !WIN32
//
//
//    std::string::size_type pos;
//    std::vector<std::string> result;
//    file += pattern;                    //��չ�ַ����Է������
//    int size = file.size();
//    for (int i = 0; i < size; i++) {
//        pos = file.find(pattern, i);
//        if (pos < size) {
//            std::string s = file.substr(i, pos - i);
//            result.push_back(s);
//            i = pos + pattern.size() - 1;
//        }
//    }
//    return result.back();
//}
////  __FILE__ ��ȡԴ�ļ������·��������
////  __LINE__ ��ȡ���д������ļ��е��к�
////  __func__ �� __FUNCTION__ ��ȡ������
////  ##__VA_ARGS__ �������ڽ��ɱ�������ݸ�fprintf���﷨��##���������ڴ���û�пɱ����ʱ�������ȷ����ʹû�пɱ��������Ҳ����ȷչ����
//
//#define LOGI(format, ...)  fprintf(stderr," [%s:%d] " format "\n",__FILE__,__LINE__,##__VA_ARGS__)      //���ʾ����LOGI("Entering the main function"); ��[TrConnection.c:14] Entering the main function
//#define LOGE(format, ...)  fprintf(stderr," [%s:%d] " format "\n",__FILE__,__LINE__,##__VA_ARGS__)

#pragma once
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// ��ȡ�ļ����ĺ���
static std::string getFile(std::string file) {
#ifndef WIN32
    std::string pattern = "/";
#else
    std::string pattern = "\\";
#endif // !WIN32

    std::string::size_type pos;
    std::vector<std::string> result;
    file += pattern; // ��չ�ַ����Է������
    int size = file.size();
    for (int i = 0; i < size; i++) {
        pos = file.find(pattern, i);
        if (pos < size) {
            std::string s = file.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result.back();
}

// ��ȡ��ǰʱ����ַ���
static std::string getCurrentTime() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtm);
    return std::string(buf);
}

// ��־��¼���ļ�
static void logToFile(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios_base::app); // ��׷��ģʽ���ļ�
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Unable to open log file!" << std::endl;
    }
}

// ��־�궨��
#define LOGI(format, ...)  do { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    std::string logMessage = "[" + getCurrentTime() + "] [INFO] [" + getFile(__FILE__) + ":" + std::to_string(__LINE__) + "] " + buffer; \
    fprintf(stderr, "%s\n", logMessage.c_str()); \
    logToFile(logMessage); \
} while (0)

#define LOGE(format, ...)  do { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    std::string logMessage = "[" + getCurrentTime() + "] [ERROR] [" + getFile(__FILE__) + ":" + std::to_string(__LINE__) + "] " + buffer; \
    fprintf(stderr, "%s\n", logMessage.c_str()); \
    logToFile(logMessage); \
} while (0)