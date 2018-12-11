#include <iostream>
#include "SimpleSerial.h"

using namespace std;
using namespace boost;

//arm-linux-gnueabihf-g++ main.cpp -L/home/home/src/boost/lib -lboost_system -lpthread -I/home/home/src/boost/include

int main(int argc, char* argv[])
{
    try {

        SimpleSerial serial("/dev/ttyS0",9600);

        serial.writeString("Hello world\n");

        cout<<serial.readLine()<<endl;

    } catch(boost::system::system_error& e)
    {
        cout<<"Error: "<<e.what()<<endl;
        return 1;
    }
}