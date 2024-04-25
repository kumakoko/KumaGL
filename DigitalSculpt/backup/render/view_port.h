#ifndef digital_sculpt_view_port_h__
#define digital_sculpt_view_port_h__

namespace DigitalSculpt
{
    struct Viewport
    {
        float clientWidth;// *_pixelRatio;
        float clientHeight;// *_pixelRatio;
        float offsetLeft;
        float offsetTop;
    };
}

#endif // digital_sculpt_view_port_h__