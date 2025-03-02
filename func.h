//
// Created by qdhua on 25-3-1.
//

#pragma once
#include<fstream>
#include<cstdint>
#include<vector>
#include<string>
#include"values.h"
std::ifstream fin;

std::vector<int>intstack;
std::vector<bool>intstack_occupy;
std::vector<bool>boolstack;
std::vector<bool>boolstack_occupy;
std::vector<char>charstack;
std::vector<bool>charstack_occupy;
std::vector<double>doublestack;

std::vector<float>floatstack;

std::vector<long>longstack;

std::vector<std::string>stringstack;


int intstacktop=0;

int baseint;
bool basebool;
char basechar;
double basedouble;
float basefloat;
long baselong;
std::string basestring;


int int_result[64];
int inttmptop=0;

int32_t buffers[1024];
int isrounded=0;
int readtop=0;
int top=0;

int32_t nextbuf(){
    int32_t ret=buffers[top];
    top++;
    if(top==1024){
        isrounded--;
        top=0;
    }
    return ret;
}
uintptr_t value(){
    switch (nextbuf()) {
        case int_val:
            return reinterpret_cast<uintptr_t>(&intstack[nextbuf()]);
    }
}


inline void new_int(){
    for (int i=0;i<nextbuf();i++){
        for(int i=0;i<intstack_occupy.size();i++){
            if(!intstack_occupy[i]){
                intstack_occupy[i]=true;
            }
        }
        intstack.push_back(baseint);
        intstack_occupy.push_back(true);
    }
}

void new_value(){
    switch (nextbuf()) {
        case int_val:
            new_int();
            break;
    }
}

inline void delete_int(){
    intstack_occupy[nextbuf()]=false;
}

void delete_value(){
    switch (nextbuf()) {
        case int_val:
            delete_int();
            break;
    }
}

inline int gettmpint(){
    int a=inttmptop;
    inttmptop++;
    if(inttmptop==64){
        inttmptop=0;
    }
    return a;
}

inline uintptr_t result();

inline uintptr_t add_int(){
    int *a=&int_result[gettmpint()];
    switch (nextbuf()) {
        case int_val:
            *a=nextbuf();
            break;
        case value_val:
            switch (nextbuf()) {
                case int_val:
                    *a=intstack[nextbuf()];
                    break;
                case long_val:
                    *a=longstack[nextbuf()];
                    break;
                case double_val:
                    *a=int(doublestack[nextbuf()]);
                    break;
                case float_val:
                    *a=int(floatstack[nextbuf()]);
                    break;
                case char_val:
                    *a=int(charstack[nextbuf()]);
                    break;
            }
            break;
        /*case result_val:
            *a=result();
            break;*/
    }
    switch (nextbuf()) {
        case int_val:
            *a+=nextbuf();
            break;
        case value_val:
            switch (nextbuf()) {
                case int_val:
                    *a+=intstack[nextbuf()];
                    break;
                case long_val:
                    *a+=longstack[nextbuf()];
                    break;
                case double_val:
                    *a+=int(doublestack[nextbuf()]);
                    break;
                case float_val:
                    *a+=int(floatstack[nextbuf()]);
                    break;
                case char_val:
                    *a+=static_cast<int>(charstack[nextbuf()]);
                    break;
            }
            break;
        case result_val:
            *a+=result();
            break;
    }

    return reinterpret_cast<uintptr_t>(a);

}

uintptr_t input_func(){
    switch (nextbuf()) {
        case int_val:
            int i=gettmpint();
            std::cin>>int_result[i];
            return reinterpret_cast<uintptr_t>(&int_result[i]);
    }
}

inline uintptr_t result_int(){
    switch (nextbuf()) {
        case add_val:
            return add_int();
    }
}

inline uintptr_t result(){
    switch (nextbuf()) {
        case int_val:
            return result_int();
        case print_val:
            return input_func();

    }
}

void equal_int(){
    nextbuf();
    int *a=reinterpret_cast<int *>((value()));
    switch (nextbuf()) {
        case int_val:
            *a=nextbuf();
            break;
        case value_val:
            *a=*(reinterpret_cast<int *>(value()));
            break;
        case result_val:
            *a=*reinterpret_cast<int *>(result());
            break;
    }
}

void equal(){

    switch (nextbuf()) {
        case int_val:
            equal_int();
            break;
    }
}

void print_func(){
    switch(nextbuf()){
        case int_val:
            std::cout<<nextbuf()<<std::endl;
            break;
        case value_val:
            std::cout<<*(reinterpret_cast<int *>(value()))<<std::endl;
            break;
        case result_val:
            std::cout<<*reinterpret_cast<int *>(result())<<std::endl;
            break;
    }
}

