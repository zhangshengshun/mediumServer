/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-01 03:13:57
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 10:34:41
 */
#ifndef __CLIENT__
#define __CLIENT__

#include<string>
#include<iostream>
#include"Connectfd.h"

//class connectfd;
using std::string;

class client: public connectfd
{
    public:
    client(string ip="192.168.80.100",uint32_t p=4000):server_ip(ip),port(p),connectfd(){
    }
    ~client();
    int connect();
    int getMessage();
    int readTCPHead();
    //void run();
    int readBack();

private:
    string message;
    string server_ip;
    uint32_t port;
    //int m_ID;
};


#endif