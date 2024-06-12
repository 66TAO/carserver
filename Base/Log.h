#pragma once
#include <time.h>
#include <string>
#include <vector>

static std::string getFile(std::string file) {
#ifndef WIN32
    std::string pattern = "/";
#else
    std::string pattern = "\\";
#endif // !WIN32


    std::string::size_type pos;
    std::vector<std::string> result;
    file += pattern;                    //��չ�ַ����Է������
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
//  __FILE__ ��ȡԴ�ļ������·��������
//  __LINE__ ��ȡ���д������ļ��е��к�
//  __func__ �� __FUNCTION__ ��ȡ������
//  ##__VA_ARGS__ �������ڽ��ɱ�������ݸ�fprintf���﷨��##���������ڴ���û�пɱ����ʱ�������ȷ����ʹû�пɱ��������Ҳ����ȷչ����

#define LOGI(format, ...)  fprintf(stderr," [%s:%d] " format "\n",__FILE__,__LINE__,##__VA_ARGS__)      //���ʾ����LOGI("Entering the main function"); ��[TrConnection.c:14] Entering the main function
#define LOGE(format, ...)  fprintf(stderr," [%s:%d] " format "\n",__FILE__,__LINE__,##__VA_ARGS__)

