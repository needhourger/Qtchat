#include <QCoreApplication>
#include"Mytcpserver.h"



quint16 setPort(){
    /*
    quint16 port;
    std::cout<<"[INFO] Please set service port(default:1234):";
    std::cin>>port;
    */

    std::string port;
    std::cout<<"[INFO] Please set service port(default:1234):";
    std::getline(std::cin,port);

    if (port.empty()){
        std::cout<<"[INFO] error input,using default setting"<<std::endl;
        return 1234;
    }

    for (int i=0;i<port.length();i++)
        if (port[i]-'0'<0 || port[i]-'0'>9) {
            std::cout<<"[INFO] error input,using default setting"<<std::endl;
            return 1234;
        }

    if ( std::stoi(port)<0 || std::stoi(port)>65535 ) {
        std::cout<<"[INFO] error input, using default setting"<<std::endl;
        return 1234;
    }

    quint16 PORT=stoi(port);
    return PORT;

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Mytcpserver server;
    server.start_server(setPort());
    return a.exec();
}
