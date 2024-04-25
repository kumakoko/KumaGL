#ifndef digital_sculpt_enums_h__
#define digital_sculpt_enums_h__

#include "utils.h" // Include the header file containing Utils class

namespace DigitalSculpt
{
    class Enums
    {
    public:
        enum Action
        {
            NOTHING = 0,
            MASK_EDIT = 1,
            SCULPT_EDIT = 2,
            CAMERA_ZOOM = 3,
            CAMERA_ROTATE = 4,
            CAMERA_PAN = 5,
            CAMERA_PAN_ZOOM_ALT = 6
        };

        enum Tools
        {
            BRUSH = 0,
            INFLATE = 1,
            TWIST = 2,
            SMOOTH = 3,
            FLATTEN = 4,
            PINCH = 5,
            CREASE = 6,
            DRAG = 7,
            PAINT = 8,
            MOVE = 9,
            MASKING = 10,
            LOCALSCALE = 11,
            TRANSFORM = 12
        };

        enum Shader
        {
            PBR = 0,
            FLAT = 1,
            NORMAL = 2,
            WIREFRAME = 3,
            UV = 4,
            MATCAP = 5,
            SELECTION = 6,
            BACKGROUND = 7,
            MERGE = 8,
            FXAA = 9,
            CONTOUR = 10,
            PAINTUV = 11,
            BLUR = 12
        };

        enum Projection
        {
            PERSPECTIVE = 0,
            ORTHOGRAPHIC = 1
        };

        enum CameraMode
        {
            ORBIT = 0,
            SPHERICAL = 1,
            PLANE = 2
        };

        enum MultiState
        {
            NONE = 0,
            SCULPT = 1,
            CAMERA = 2,
            PICKING = 3
        };

        enum KeyAction
        {
            INTENSITY,
            RADIUS,
            NEGATIVE,
            PICKER,
            DELETE,
            CAMERA_FRONT,
            CAMERA_TOP,
            CAMERA_LEFT,
            CAMERA_RESET,
            STRIFE_LEFT,
            STRIFE_RIGHT,
            STRIFE_UP,
            STRIFE_DOWN,
            WIREFRAME,
            REMESH
        };
    };
    // Optionally, you may need to export Enums as a header file or make it accessible in your application.
}

#endif // digital_sculpt_enums_h__