/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-01 04:33:16
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 11:00:37
 */
#include"client.h"
#include"Epoll.h"
#include"EpollEvent.h"
const int link=1024;

int main(){
    Epoll::getInstance()->initialize(1);
    client cli;
    cli.connect();
    cli.getMessage();
    Epoll::getInstance()->runInClient(cli);
    // cout<<cli.getMessage()<<endl;
}