/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 11:33:18
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-01 15:35:30
 */
#include"server.h"
#include"Error.h"
#include"Connectfd.h"
#include"EpollEvent.h"

#include<iostream>
#include<limits.h>
#include<unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>



class connectfd;
void Server::Init(){
    nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == nListenSocket)
    {
        std::cout << "socket error" << std::endl;
    }
    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(4000);
    if(::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
    {
        std::cout << "bind error" << std::endl;
        ::close(nListenSocket);
    }
}

int Server::listen(){
    if(::listen(nListenSocket,23)<0){
        cerr<<"TCPSocket::listen::listen"<<std::endl;
        return FAILED;
    }
    int i;
    event_st epollevent;
    epollevent.fd=nListenSocket;
    epollevent.m_id=map_id;
    
    connectfd *connect=new connectfd(this);
    if(connect==nullptr){
        cout<<"分配失败"<<endl;
    }
    connect->event.m_epollEvent=epollevent;
    
    connect->event.registerREvent();
    connect->sockfd=nListenSocket;
    connect->m_connectionID=map_id;
    
    fdMap[map_id]=connect;
    map_id++;
    
    
    connect->disableLinger();
    connect->enableReuseaddr();
    connect->disableNagle();
    return SUCCESSFUL;
}

int Server::accept(int fd){
    sockaddr_in ClientAddress;
    socklen_t LengthOfClientAddress = sizeof(sockaddr_in);
    int clientfd = ::accept(fd, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
    // int num=10;
    // ::write(clientfd,&num,sizeof(int));
    if(-1 == clientfd)
    {
        std::cout << "accept error" << std::endl;
        ::close(nListenSocket);
        return 0;
    }
    
    connectfd *connect=new connectfd(this);
    connect->sockfd=clientfd;
    connect->m_connectionID=map_id;

    //将ID发回给client
    event_st epollevent;
    epollevent.fd=clientfd;
    epollevent.m_id=map_id;

    connect->event.m_epollEvent=epollevent;
    fdMap[map_id]=connect;
    //cout<<"fdMap.size():"<<fdMap.size()<<endl;
    map_id++;
    //fdMap[m_id++]=connect;
    connect->event.registerREvent();
    connect->disableLinger();
    connect->enableReuseaddr();
    connect->disableNagle();

    connect->sendPackage(epollevent.m_id);
    return SUCCESSFUL;
}