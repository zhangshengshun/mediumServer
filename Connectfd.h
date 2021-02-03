/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-31 05:22:23
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 13:39:45
 */
#ifndef __CONNECTFD__
#define __CONNECTFD__

#include<iostream>
#include <sys/epoll.h>
#include<map>
#include<list>
#include<netinet/in.h>
#include<sys/uio.h>
#include<cstring>
#include"server.h"

const unsigned int HEADER_SIZE=4*sizeof(uint32_t);


class Server;
struct MsgHeader{
    uint32_t cmd;//消息类型
    uint32_t legnth;//消息长度
    uint32_t recvfrom;//要发送给谁的ID
    uint32_t sendform;//自己的ID
};

typedef struct Iov{
    Iov(){
        m_pCompleteBuffer=nullptr;
        memset(&m_Iovec,0,sizeof(m_Iovec));
    }

    Iov(char* buf,size_t len){
        m_pCompleteBuffer=buf;
        m_Iovec.iov_base=buf;
        m_Iovec.iov_len=len;
    }

    ~Iov(){
        m_Iovec.iov_base=nullptr;
        m_Iovec.iov_len=0;
        m_pCompleteBuffer=nullptr;
    }

    struct iovec m_Iovec;
    char* m_pCompleteBuffer;
}Iov;

struct InReq
{
    MsgHeader m_msgHeader;
    char* ioBuf;
    //sockaddr_in oppoAddr;
};



class connectfd{
    public:
    connectfd();
    connectfd(Server *ser);
    ~connectfd();
    void releaseSendBuffer();
    void init();
    int readData();
    int sendData();
    virtual int readTCP();
    int readTCPHead();
    int readTCPContent();
    virtual int readBack();
    //virtual int read_in_client();


    //设置sockfd的属性
    int read( char* buf, size_t len );
    int write( const char* buf, size_t len );
    int close( void );
    
    int setNonblock( void );
    int disableLinger( void );
    int disableNagle( void );
    int enableReuseaddr( void );
    int sendPackage(int id);


    int sockfd;//对应的文件描述符
    //缓冲区
    std::list<Iov> m_sendIovList;

    uint32_t m_nReadOffset;//读取的偏移量
    uint32_t m_nHeadSize;//头大小
    uint32_t m_nContentLength;//数据内容的大小
    
    int m_connectionID;//ID
    bool m_getReadHeader;//是否读头
    bool m_getNewPackage;//是否是新的包

    InReq m_InReq;

    //控制epoll中的fd
    EpollEvent event;
    Server *server;
};

#endif