#include <iostream>
#include<thread>
#include "func.h"

using namespace std;

void read(){

    while(true){
        if (readtop=top-1&&isrounded==1){
            continue;
        }
        int32_t a;
        fin.read(reinterpret_cast<char *>(&a),4);
        buffers[readtop]=a;
        if(a==Mainend){
            break;
        }
        readtop++;
        if(readtop==1024){
            isrounded++;
            readtop=0;
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc>1){
        fin.open(argv[1]);
    }
    else fin.open("test.cap",std::ios::binary);
    thread t1(read);
    t1.join();
    while(true){
        switch (nextbuf()) {
            case equal_val:
                equal();
                break;
            case print_val:
                print_func();
                break;
        }
    }

    return 0;
}
