/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 14:53:27
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 10:08:48
 */
#include"server.h"
#include"Epoll.h"
#include"EpollEvent.h"
const int EPOLLSIZE = 1024;

int main(){
    Epoll::getInstance()->initialize(1024);
    Server server;
    server.Init();
    server.listen();
    Epoll::getInstance()->run(server);
}