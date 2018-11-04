/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
/*!
* \file kgl_rectangle.h
* \date 2018/01/14 14:46
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 
*
* TODO: 矩阵类
*
* \note
*/
#ifndef kgl_rectangle_h__
#define kgl_rectangle_h__

namespace kgl
{
    template<typename T>
    class Rectangle
    {
    public:
        Rectangle(){};
        Rectangle(T top_left_x, T top_left_y, T bottom_right_x, T bottom_right_y);
        void Reset(T top_left_x, T top_left_y, T bottom_right_x, T bottom_right_y);
        T Width() const;
        T Height() const;
        T TopLeftX() const;
        T TopLeftY() const;
        T BottomRightX() const;
        T BottomRightY() const;
    protected:
        T top_left_x_ = T(0);
        T top_left_y_ = T(0);
        T bottom_right_x_ = T(0);
        T bottom_right_y_ = T(0);
    };

    template<typename T>
    Rectangle<T>::Rectangle(T top_left_x, T top_left_y, T bottom_right_x, T bottom_right_y)
    {
        top_left_x_ = top_left_x;
        top_left_y_ = top_left_y;
        bottom_right_x_ = bottom_right_x;
        bottom_right_y_ = bottom_right_y;
    }

    template<typename T>
    void Rectangle<T>::Reset(T top_left_x, T top_left_y, T bottom_right_x, T bottom_right_y)
    {
        top_left_x_ = top_left_x;
        top_left_y_ = top_left_y;
        bottom_right_x_ = bottom_right_x;
        bottom_right_y_ = bottom_right_y;
    }

    template<typename T>
    T Rectangle<T>::Width() const
    {
        return bottom_right_x_ - top_left_x_;
    }

    template<typename T>
    T Rectangle<T>::Height() const
    {
        return bottom_right_y_ - top_left_y_;
    }

    template<typename T>
    T Rectangle<T>::TopLeftX() const
    {
        return top_left_x_;
    }

    template<typename T>
    T Rectangle<T>::TopLeftY() const
    {
        return top_left_y_;
    }

    template<typename T>
    T Rectangle<T>::BottomRightX() const
    {
        return bottom_right_x_;
    }

    template<typename T>
    T Rectangle<T>::BottomRightY() const
    {
        return bottom_right_y_;
    }

    typedef Rectangle<int> IRect;
    typedef Rectangle<float> FRect;
}

#endif // kgl_rectangle_h__