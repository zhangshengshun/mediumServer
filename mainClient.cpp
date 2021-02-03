/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-01 04:33:16
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-02 15:38:52
 */
#include"client.h"
#include"Epoll.h"
#include"EpollEvent.h"
#include<fstream>
#include <iostream>
#include<thread>
const int link=10000;

string getMessage(){
    std::ifstream out;
    std::string message;
    out.open("package");
    std::string str;
    while(!out.eof()){
        getline(out,str);
        message+=str;
    }
    return message;
}

void openEpoll(){
    Epoll::getInstance()->initialize(1);
    Epoll::getInstance()->runInClient();
}

int main(){
    std::thread t(openEpoll);
    t.detach();

    clientManager::getInstance()->initializer();
    std::string message=getMessage();
    int i=0;
    while(1){
        for(;i<link;i++){
            client *cli=new client();
            cli->connect();
            cli->setMessage(message);
            i++;
        }
    }
    

    //cout<<clientManager::getInstance()->map_client.size()<<endl;
    
    // cout<<cli.getMessage()<<endl;
}