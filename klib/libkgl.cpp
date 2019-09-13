//
//  libkgl.cpp
//  libkgl
//
//  Created by apple on 2019/5/31.
//  Copyright © 2019年 apple. All rights reserved.
//

#include <iostream>
#include "libkgl.hpp"
#include "libkglPriv.hpp"

void libkgl::HelloWorld(const char * s)
{
    libkglPriv *theObj = new libkglPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void libkglPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

