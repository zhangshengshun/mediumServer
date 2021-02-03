#include <string.h>
#include <errno.h>
#include<iostream>

#include"Epoll.h"
#include"EpollEvent.h"
#include"server.h"
#include"Error.h"
#include"client.h"

#define EPOLL_TIMEOUT_LEN 5

using std::cout;

Epoll::Epoll():m_epollEvents(nullptr),m_epollFd(-1){
    
}

Epoll::~Epoll(void){
    if(m_epollEvents!=nullptr){
        delete[] m_epollEvents;
        m_epollEvents=nullptr;
    }
}

int Epoll::getEpollFd()const{
    return m_epollFd;
}

int Epoll::initialize(int fdsize){
    m_eventSize=fdsize;
    m_epollFd=epoll_create1(::EPOLL_CLOEXEC);

    if(m_epollFd<0){
        cout<<"Epoll:initialize error"<<endl;
        return FAILED;
    }
    m_epollEvents=new epoll_event[fdsize];
    memset(m_epollEvents, 0, sizeof( epoll_event )*fdsize);
    return SUCCESSFUL;
}

int Epoll::doEvent(EpollEvent *ptr,int fd,int op,unsigned int events){
    struct epoll_event ev;
    memset( &ev, 0, sizeof( struct epoll_event ) );
    ev.events=events;
    ev.data.ptr=ptr;
    if(epoll_ctl(m_epollFd, op, fd,&ev)<0){
        cout<<"Epoll:doEvent epoll_ctl() error "<<strerror(errno)<<endl;
        return FAILED;
    }
    return SUCCESSFUL;
}

void Epoll::run(Server& server){
    int nfds=0;
    EpollEvent *event=nullptr;

    while(1){
        nfds=::epoll_wait(m_epollFd, m_epollEvents, m_eventSize, -1);
        // if(EINTR==errno){
        //     continue;
        // }
        // else{
        //     cout<<"Epoll:epoll_wait"<<endl;
        // }
        for(int i=0;i<nfds;i++){
            event=(EpollEvent *)m_epollEvents[i].data.ptr;
            if(event->m_epollEvent.fd==server.nListenSocket){
                //cout<<event->m_epollEvent.m_id<<endl;
                server.accept(server.nListenSocket);
                cout<<server.fdMap.size()<<endl;
                continue;
            }
            connectfd *connect=server.fdMap[event->m_epollEvent.m_id];
            if(connect==nullptr){
                cout<<"connect == NULL"<<endl;
                continue;
            }
            else if(m_epollEvents[i].events & EPOLLOUT){

                if(connect->sendData()<=0){
                    connect->event.closeWevent();
                }
                continue;
            }
            else if(m_epollEvents[i].events & EPOLLIN){
                if(connect->readData()<0){
                    
                }
                continue;
            }
            else if(m_epollEvents[i].events &EPOLLERR || m_epollEvents[i].events & EPOLLHUP){
                cout<<"EPOLLERR"<<endl;
                connect->releaseSendBuffer();
                if(connect->m_InReq.ioBuf!=nullptr){
                    delete[] connect->m_InReq.ioBuf;
                    connect->m_InReq.ioBuf=nullptr;
                }
                memset(&connect->m_InReq.m_msgHeader,0,HEADER_SIZE);
                connect->server->fdMap.erase(event->m_epollEvent.m_id);
                continue;
            }
        }
    }
}

void Epoll::runInClient(){
    int nfds=0;
    EpollEvent *event=nullptr;
    
    while(1){
        
        nfds=::epoll_wait(m_epollFd, m_epollEvents, m_eventSize, -1);
        
        for(int i=0;i<nfds;i++){
            
            //cout<<"nfds:"<<nfds<<endl;
            
            event=(EpollEvent *)m_epollEvents[i].data.ptr;
            client * cli=clientManager::getInstance()->map_client[event->m_epollEvent.id_in_client];
            //cout<<cli->id_in_client<<endl;

            if(m_epollEvents[i].events & EPOLLOUT){
                if((*cli).sendData()<=0){
                    cli->event.closeWevent();
                }
                continue;
            }
            else if(m_epollEvents[i].events & EPOLLIN){
                
                if((*cli).readData()<0){
                }
                continue;
            }
        }
    }
}


