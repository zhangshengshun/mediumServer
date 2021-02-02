/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 10:33:47
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-01 05:03:55
 */
#ifndef __SERVER__
#define __SERVER__

#include<iostream>
#include <sys/epoll.h>
#include<map>
#include<list>
#include<netinet/in.h>
#include<sys/uio.h>
#include<cstring>
#include"EpollEvent.h"
#include"Epoll.h"


class connectfd;
class Server{
    friend class connectfd;
    public:
    Server(){
    }
    void Init();
    int listen();
    int accept(int);
    
    int nListenSocket;

    std::map<uint32_t,connectfd *> fdMap;
    uint32_t map_id=0;
};

#endif