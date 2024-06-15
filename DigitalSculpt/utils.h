#pragma once

#include <thread>
#include "glm/glm.hpp"

namespace DigitalSculpt
{
    /***************************************************
    获取最大可支持的线程数
    @name: DigitalSculpt::getNThreads
    @return: int
    ***************************************************/
    inline int getNThreads()
    {
        // hardware_concurrency() 用于获取当前机器的CPU逻辑核心数,并可根据此值来设置线程池的工作线程数量
        return glm::max(1, (int)std::thread::hardware_concurrency());
    }
}