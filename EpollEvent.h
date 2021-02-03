/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 10:57:48
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 12:26:35
 */

#ifndef __EPOLLEVENT__
#define __EPOLLEVENT__

#include <stdio.h>
#include <stdint.h>
#include<sys/epoll.h>

typedef struct event_st
{
    int         m_id;
    int         fd;
    bool        read;
    bool        write;
    unsigned int events;
    int id_in_client;

    struct event_st& operator=(event_st st){
        this->fd=st.fd;
        this->m_id=st.m_id;
        this->read=st.read;
        this->write=st.write;
        this->events=st.events;
        this->id_in_client=st.id_in_client;
        return *this;
    }
} event_st;

class EpollEvent
{
public:
    EpollEvent(){
    }
    ~EpollEvent( void )
    {
    }

    int openRevent( void );
    int openWevent( void );
    int closeRevent( void );
    int closeWevent( void );
    int registerREvent( void );
    int registerWEvent( void );
    int registerRWEvents( void );
    int unregisterRWEvents( void );
    event_st m_epollEvent;
};

#endif