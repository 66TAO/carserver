#include "InetAddress.h"

Ipv4Address::Ipv4Address()
{

}

Ipv4Address::Ipv4Address(std::string ip, uint16_t port) :
    mIp(ip),                                        // ������� IP ��ַ�洢����Ա���� mIp ��
    mPort(port)                                     // ������Ķ˿ںŴ洢����Ա���� mPort ��
{
    mAddr.sin_family = AF_INET;                         // ���õ�ַ��Ϊ IPv4
    mAddr.sin_addr.s_addr = inet_addr(ip.c_str());      // �� IP ��ַת��Ϊ�����ֽ��򲢴洢�� sockaddr_in �ṹ��� sin_addr ��Ա��
    mAddr.sin_port = htons(port);                       // ���˿ں�ת��Ϊ�����ֽ��򲢴洢�� sockaddr_in �ṹ��� sin_port ��Ա��
}

void Ipv4Address::setAddr(std::string ip, uint16_t port)
{
    mIp = ip;
    mPort = port;
    mAddr.sin_family = AF_INET;
    mAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    mAddr.sin_port = htons(port);
}

std::string Ipv4Address::getIp()
{
    return mIp;
}

uint16_t Ipv4Address::getPort()
{
    return mPort;
}

struct sockaddr* Ipv4Address::getAddr()
{
    return (struct sockaddr*)&mAddr;
}
