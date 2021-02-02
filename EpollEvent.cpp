/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 11:17:02
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-01-31 03:28:06
 */
#include"EpollEvent.h"
#include"Epoll.h"
#include"Error.h"

#define LEVELREVENT                  EPOLLIN
#define LEVELWEVENT                  EPOLLOUT
#define LEVELEVENTS                  (EPOLLIN |EPOLLOUT| EPOLLERR | EPOLLHUP)
#define EVENTADD                     EPOLL_CTL_ADD
#define EVENTDEL                     EPOLL_CTL_DEL
#define EVENTCHANGE                  EPOLL_CTL_MOD


int EpollEvent::closeRevent( void ){
    if(this->m_epollEvent.fd==-1){
        return SUCCESSFUL;
    }
    m_epollEvent.read=false;
    m_epollEvent.events=m_epollEvent.events&~LEVELREVENT;
    if((Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTCHANGE, m_epollEvent.events)<0){       
        return FAILED; 
    }
    
    return SUCCESSFUL;
}

int EpollEvent::closeWevent( void ){
    if(this->m_epollEvent.fd==-1){
        return SUCCESSFUL;
    }
    m_epollEvent.events=m_epollEvent.events&~LEVELWEVENT;
    m_epollEvent.write=false;
    if((Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTCHANGE,m_epollEvent.events)<0){
        return FAILED;
    }
    
    return SUCCESSFUL;
}

int EpollEvent::openWevent( void )
{
    if ( -1 == this->m_epollEvent.fd )
    {
        return SUCCESSFUL;
    }

    if(!m_epollEvent.write){
        m_epollEvent.write=true;
        m_epollEvent.events = m_epollEvent.events | LEVELWEVENT;

        if ( (Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTCHANGE,m_epollEvent.events) < 0 )
        {
            return FAILED;
        }
    }
    
    return SUCCESSFUL;
}
int EpollEvent::openRevent(){
    if ( -1 == this->m_epollEvent.fd )
    {
        return SUCCESSFUL;
    }
    m_epollEvent.events = m_epollEvent.events | LEVELREVENT;
    m_epollEvent.read=true;
    if ( (Epoll::getInstance())->doEvent(this, m_epollEvent.fd, \
                        EVENTCHANGE, m_epollEvent.events ) < 0 )
    {
        return FAILED;
    }
    
    return SUCCESSFUL;
    
}

int EpollEvent::registerREvent( void )
{
    if ( -1 == this->m_epollEvent.fd)
    {
        return SUCCESSFUL;
    }

    
    m_epollEvent.events = LEVELREVENT;
    m_epollEvent.read=true;
    m_epollEvent.write=false;
    
    if ( (Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTADD, m_epollEvent.events) < 0 )
    {
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::registerWEvent( void )
{
    if ( -1 == this->m_epollEvent.fd)
    {
        return SUCCESSFUL;
    }

    m_epollEvent.events = LEVELWEVENT;
    m_epollEvent.write=true;
    m_epollEvent.read=false;

    if ( (Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTADD, m_epollEvent.events) < 0 )
    {
        return FAILED;
    }
    return SUCCESSFUL;
}

int EpollEvent::registerRWEvents( void )
{
    if ( -1 == this->m_epollEvent.fd )
    {
        return SUCCESSFUL;
    }

    m_epollEvent.events = LEVELEVENTS;
    m_epollEvent.write=true;
    m_epollEvent.read=true;

    if ( (Epoll::getInstance())->doEvent(this,m_epollEvent.fd,EVENTADD, m_epollEvent.events) < 0 )
    {
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::unregisterRWEvents( void )
{
    if ( -1 == this->m_epollEvent.fd )
    {
        return SUCCESSFUL;
    }

    m_epollEvent.events= LEVELEVENTS;
    m_epollEvent.read=false;
    m_epollEvent.read=false;

    if ((Epoll::getInstance())->doEvent(this,m_epollEvent.fd, \
                           EVENTDEL, m_epollEvent.events ) < 0 )
    {
        return FAILED;
    }
    return SUCCESSFUL;
}