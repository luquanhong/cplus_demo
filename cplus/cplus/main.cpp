//
//  main.cpp
//  cplus
//
//  Created by 卢泉鸿 on 16/7/16.
//  Copyright © 2016年 卢泉鸿. All rights reserved.
//

#include <iostream>
#include <string>
#include <unistd.h>

#include "refcount_ptr.h"
#include "normal_ptr.h"
#include "MessageLoop.h"


using namespace std;
using namespace turbo;



class CBase;
class Base;

class Listen{
public:
    virtual refcount_ptr<Base>& lockObj() = 0;
};


class Base : public MessageLoop{
public:
    Base()
    :MessageLoop()
    , count(10)
    {
        MessageLoop::start(true);
        cout << "Base constor" << endl;
    }
    
    virtual ~Base()
    {
        MessageLoop::stop();
        cout << "~Base destrutor" << endl;
    }
    
    
    void func();
    
private:
    Listen* listen;
    refcount_ptr<Base> xxxx;
    
public:
    class BaseMessage : public MessageLoop::Message {
    public:
        BaseMessage(normal_ptr<Base> base,  refcount_ptr<Base>& b) {
            this->pb = base;
            this->b = b;
        }
        
        virtual void onMessage() {
            pb->onBaseMessage(b);
        }
        
    private:
        normal_ptr<Base> pb;
        refcount_ptr<Base> b;
    };
    
    //refcount_ptr<BaseMessage> bMessage;
    void onBaseMessage( refcount_ptr<Base>& base);
    
    virtual void func1();
    
    void func2(const refcount_ptr<CBase>& cb);
    
    static void* test_thread(void* args);
    
    
    void setListen(Listen* listen) {
        this->listen = listen;
    }
    
public:
    class Base1Message : public MessageLoop::Message {
    public:
        Base1Message(normal_ptr<Base> base) {
            this->pb = base;
        }
        
        virtual void onMessage() {
            pb->onBase1Message();
        }
        
    private:
        normal_ptr<Base> pb;
    };
    
    void onBase1Message();
    
private:
    int count;
    
    refcount_ptr<CBase> c;
    
};

void Base::onBaseMessage( refcount_ptr<Base>& base){
    cout << __FUNCTION__ << endl;
    
    cout << __FUNCTION__ << " 1 "<< endl;
    
    sleep(5);
    
    cout << "onBaseMessage count: " << count << endl;
    
    base = NULL;
    
    cout << __FUNCTION__ << " end "<< endl;
}


void Base::func() {
    cout << __FUNCTION__ << endl;
    
    refcount_ptr<Base> bb = listen->lockObj();
    refcount_ptr<MessageLoop::Message> bMessage = new BaseMessage(this, bb);
    
    MessageLoop::postMessage(bMessage);
    
    sleep(1);
    cout << __FUNCTION__ << " end "<< endl;
    
}

void Base::onBase1Message()
{
    cout << __FUNCTION__ << endl;
    
    sleep(5);
    
    //count = 100;
    
    cout << "count: " << count << endl;
    
    cout << __FUNCTION__ << " end "<< endl;
}

void Base::func1()
{
    cout << __FUNCTION__ << endl;
    
    refcount_ptr<MessageLoop::Message> bMessage = new Base1Message(this);
    
    MessageLoop::postMessage(bMessage);
    
     xxxx = listen->lockObj();
    
    //func1(bMessage);
    
    cout << __FUNCTION__ << " end "<< endl;
}

void Base::func2(const refcount_ptr<CBase>& cb)
{
    

    
    
    cout << __FUNCTION__ << " 4 end "<< endl;
}



class CBase : public Listen{
    
public:
    CBase() {
        cout << __FUNCTION__ << endl;
        b = new Base();
        b->setListen(this);
    }
    
    ~CBase() {
        cout << __FUNCTION__ << endl;
    }
    
    void func2();
    
    virtual refcount_ptr<Base>& lockObj(){
        cout << __FUNCTION__ << " entry"<< endl;
        
        return b;
        
        //cout << __FUNCTION__ << endl;
    }
    
private:
    int count;
    
    refcount_ptr<Base> b;
    
};

void CBase::func2()
{
    cout << __FUNCTION__ << " enter "<< endl;
//    pthread_t thread;
//    pthread_create(&thread, NULL, test_thread, this);
    b->func();
    
    
    cout << __FUNCTION__ << " end "<< endl;
}



void function1()
{
    refcount_ptr<CBase> cb;
    
    if (1) {
        cout << __FUNCTION__ << "===== end " << endl;
        cb = new CBase();
    
        cb->func2();
        cout << __FUNCTION__ << " 2 ======end " << endl;
    }
    
    
    cout << __FUNCTION__ << " 2 end " << endl;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    function1();
    
    
    
    getchar();
    return 0;
}

