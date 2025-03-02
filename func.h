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

struct int_result_t{
    int id;
    uintptr_t addr;
};

int int_result[32];
bool int_result_occupy[32];


int32_t buffers[1024];
int top=0;

int32_t nextbuf(){
    int32_t ret=buffers[top];
    top++;
    if(top==1024){
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

int getavaliableint(){

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
    for(int i=0;i<32;i++){
        if(!int_result_occupy[i]){
            int_result_occupy[i]=true;
            return i;
        }
    }
}

inline void releasetmpint(int i){
    int_result_occupy[i]=false;
}

inline uintptr_t result();

inline int_result_t add_int(){
    int i=gettmpint();
    int *a=&int_result[i];
    switch (nextbuf()) {
        case int_val:
            *a=nextbuf();
            break;
        case value_val:
            *a=*(reinterpret_cast<int *>(value()));
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
            *a+=*(reinterpret_cast<int *>(value()));
            break;
        case result_val:
            *a+=result();
            break;
    }
    int_result_t ret;
    ret.id=i;
    ret.addr=reinterpret_cast<uintptr_t>(a);
    return ret;
}

inline uintptr_t result_int(){
    switch (nextbuf()) {
        case add_val:
            int_result_t a;
            a=add_int();
            releasetmpint(a.id);
            return a.addr;
    }
}

inline uintptr_t result(){
    switch (nextbuf()) {
        case int_val:
            return result_int();
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