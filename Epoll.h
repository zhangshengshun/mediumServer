/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 11:04:42
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 10:35:08
 */

#ifndef __EPOLL__
#define __EPOLL__

#include <sys/epoll.h>

#include"EpollEvent.h"
#include"server.h"
#include"SingleTon.h"
#include"Connectfd.h"
using namespace std;

class Server;
class client;
class Epoll:public Singleton<Epoll>
{
public:
    friend class Singleton<Epoll>;

    Epoll();
    ~Epoll( void );
    int getEpollFd()const;
    int initialize( int );
    int doEvent(EpollEvent*ptr,int fd, int op, unsigned int events );
    void run( Server& server);
    void runInClient();
private:

    struct epoll_event*   m_epollEvents;
    int                   m_epollFd;
    int                   m_eventSize;
};

#endif