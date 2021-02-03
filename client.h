/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-01 03:13:57
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 13:38:40
 */
#ifndef __CLIENT__
#define __CLIENT__

#include<string>
#include<iostream>
#include<map>
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
    void setMessage(string m_mesage);
    //void run();
    int readBack() override;
    int id_in_client;
private:
    string server_ip;
    uint32_t port;
    std::string message;
};


class clientManager:public Singleton<clientManager>{
    friend class Singleton<Epoll>;
    public:
    int id_in_client;
    std::map<uint32_t,client*> map_client;
    void initializer(){
        map_client.clear();
        id_in_client=0;
    }
};


#endif