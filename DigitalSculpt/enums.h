#ifndef enums_h__
#define enums_h__

#include <map>
#include <string>
#include <utility>

namespace Enums 
{
    // Enum for Actions
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

    // Enum for Sculpt Tools
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

    // Enum for Shader Types
    enum Shader
    {
        PBR = 0,         // Shouldn't change (serialized in sgl file)
        FLAT = 1,
        NORMAL = 2,      // Shouldn't change
        WIREFRAME = 3,
        UV = 4,          // Shouldn't change
        MATCAP = 5,      // Shouldn't change
        SELECTION = 6,
        BACKGROUND = 7,
        MERGE = 8,
        FXAA = 9,
        CONTOUR = 10,
        PAINTUV = 11,
        BLUR = 12
    };

    // Enum for Camera Projection
    enum Projection
    {
        PERSPECTIVE = 0, // Shouldn't change
        ORTHOGRAPHIC = 1 // Shouldn't change
    };

    // Enum for Camera Mode
    enum CameraMode 
    {
        ORBIT = 0,       // Shouldn't change
        SPHERICAL = 1,   // Shouldn't change
        PLANE = 2        // Shouldn't change
    };

    // Enum for Multi-Resolution State
    enum MultiState 
    {
        NONE = 0,
        SCULPT = 1,
        CAMERA = 2,
        PICKING = 3
    };

    std::map<std::string, int> KeyAction = [] {
        std::map<std::string, int> keyAction;
        int acc = 0;

        // Insert tools into KeyAction
        for (int i = 0; i <= TRANSFORM; ++i) {
            keyAction["Tool" + std::to_string(i)] = acc++;
        }

        // Additional actions
        keyAction["INTENSITY"] = acc++;
        keyAction["RADIUS"] = acc++;
        keyAction["NEGATIVE"] = acc++;
        keyAction["PICKER"] = acc++;
        keyAction["DELETE"] = acc++;
        keyAction["CAMERA_FRONT"] = acc++;
        keyAction["CAMERA_TOP"] = acc++;
        keyAction["CAMERA_LEFT"] = acc++;
        keyAction["CAMERA_RESET"] = acc++;
        keyAction["STRIFE_LEFT"] = acc++;
        keyAction["STRIFE_RIGHT"] = acc++;
        keyAction["STRIFE_UP"] = acc++;
        keyAction["STRIFE_DOWN"] = acc++;
        keyAction["WIREFRAME"] = acc++;
        keyAction["REMESH"] = acc++;

        return keyAction;
        }();

} // namespace Enums

#endif // enums_h__