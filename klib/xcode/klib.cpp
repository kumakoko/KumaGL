/*
 *  klib.cpp
 *  klib
 *
 *  Created by xiongxinke on 2017/10/4.
 *  Copyright © 2017年 xiongxinke. All rights reserved.
 *
 */

#include <iostream>
#include "klib.hpp"
#include "klibPriv.hpp"

void klib::HelloWorld(const char * s)
{
    klibPriv *theObj = new klibPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void klibPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

