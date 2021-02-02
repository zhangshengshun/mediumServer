/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-01 03:22:25
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 06:57:34
 */
#include"client.h"
#include"Error.h"
#include<sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string>
#include<fstream>
#include <iostream>

client::~client(){
    
}

int client::connect(){
    this->sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        std::cout << "socket error" << std::endl;
        return 0;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    if(::inet_pton(AF_INET, server_ip.c_str(), &ServerAddress.sin_addr) != 1)
    {
        std::cout << "inet_pton error" << std::endl;
        return 0;
    }

    ServerAddress.sin_port = htons(port);

    if(::connect(sockfd, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
    {
        std::cout << "connect error" << std::endl;
        return 0;
    }
    //将得到的端口注册到epoll中
    event_st epollevent;
    epollevent.fd=this->sockfd;
    this->event.m_epollEvent=epollevent;

    this->event.registerREvent();
    
    
    disableLinger();
    enableReuseaddr();
    disableNagle();
    return 0;
}

int client::getMessage(){
    std::ifstream out;
    out.open("package");
    std::string str;
    while(!out.eof()){
        getline(out,str);
        message+=str;
    }
    return message.length();
}

int client::readBack(){
    //cout<<"1m_InReq.m_msgHeader.cmd:"<<m_InReq.m_msgHeader.cmd<<endl;
    //cout<<"client readBack";
    if(m_InReq.m_msgHeader.cmd==1){
        m_connectionID=m_InReq.m_msgHeader.recvfrom;
        InReq req;
        req.ioBuf=nullptr;
        req.m_msgHeader.cmd=2;
        req.m_msgHeader.legnth=message.length();
        req.m_msgHeader.recvfrom=m_connectionID;
        req.m_msgHeader.sendform=m_connectionID+1;
        
        char* sendMessage=new char[HEADER_SIZE+message.length()];
        memset(sendMessage,0,HEADER_SIZE+message.length());
        memcpy(sendMessage, &req.m_msgHeader, sizeof(MsgHeader));

        memcpy(sendMessage+HEADER_SIZE,message.c_str(),message.length());
        m_sendIovList.push_back(Iov(sendMessage,HEADER_SIZE+message.length()));
    }
    else if(m_InReq.m_msgHeader.cmd==2){
        m_InReq.m_msgHeader.recvfrom=m_connectionID;
        m_InReq.m_msgHeader.sendform=m_connectionID+1;

        char* sendbufer=new char[HEADER_SIZE+m_InReq.m_msgHeader.legnth];
        memset(sendbufer,0,HEADER_SIZE+m_InReq.m_msgHeader.legnth);

        memcpy(sendbufer, &m_InReq.m_msgHeader, sizeof(MsgHeader));
        memcpy(sendbufer+HEADER_SIZE,m_InReq.ioBuf,m_InReq.m_msgHeader.legnth);
        m_sendIovList.push_back(Iov(sendbufer,HEADER_SIZE+m_InReq.m_msgHeader.legnth));
    }
    if(this->event.openWevent()<0){
        return -1;
    }
    return 0;
}

// int client::readTCPHead(){
//     //cout<<"3.readData()"<<endl;
//     int rt=read((char*)( &(m_InReq.m_msgHeader))+m_nReadOffset,m_nHeadSize-m_nReadOffset);
//     if(rt<=0){
//         return FAILED;
//     }

//     cout<<"vdkhtg"<<endl;
//     m_nReadOffset+=(uint32_t)rt;
//     if(m_nReadOffset==m_nHeadSize){
//         m_nReadOffset=0;
//         m_getReadHeader=false;
//         m_nContentLength=m_InReq.m_msgHeader.legnth;
//         if(m_InReq.m_msgHeader.cmd==1){
//             readBack();
//             delete[] m_InReq.ioBuf;
//             m_getNewPackage = true;
//         }
//         else if(m_InReq.m_msgHeader.cmd==2){
//             m_InReq.ioBuf=new char[m_nContentLength];
//             memset(m_InReq.ioBuf,0,m_nContentLength);
//         }
//     }
//     return SUCCESSFUL;
// }
