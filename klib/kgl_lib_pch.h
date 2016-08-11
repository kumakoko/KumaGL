// 预编译头文件
#ifndef kgl_lib_pch_h__
#define kgl_lib_pch_h__

#include <windows.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <array>
#include <memory>
#include <exception>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <bitset>
#include <cstdint>
#include <functional>
#include <set>

#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

extern "C"
{
#include "ft2build.h"
#include "freetype.h"
#include "iconv.h"
#include "SOIL.h"
}

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "boost/noncopyable.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"

#endif // kgl_lib_pch_h__