#ifndef graphics_data_h__
#define graphics_data_h__

#include "graphics_buffer_object.h"
#include "triangle_look_up.h"
#include "transformer.h"

namespace DigitalSculpt
{
    // ��GraphicBufferObject����б����̳У�����˵GraphicsBufferObject��Ĺ��г�Ա�ͱ�����Ա��
    // ���Ա�����Ա�ķ�ʽ�������������У�����GaraphicsData���ʵ���������ⲿ�����в���ֱ�Ӷ�
    // GraphicBufferObject����е���
    class GraphicsData : protected GraphicsBufferObject, public TriangleLookUp, public ModelMatrix
    {
    public:
         GraphicsData();
         ~GraphicsData();

        void handleDynamicVertexIndexModification();
        void deleteBuffers(); // GPU memory management
        void bind(); // bind to OpenGL

        void refresh(); // reload vertex data to OpenGL.

        void render(); // render and draw vertex data.

        void invertFaces();

        bool needToRefresh = true; // need to refresh GPU binding

        bool needToDumpHistory = false;
    };

    // preload any constants used for binding and refreshing
    class OpenGLVertexAttributes
    {
    public:
        static const int SizeOfV3D;
        static const int SizeOfv3;
        static const int SizeOfv4;
        static const int SizeOfKeyData;
        static const int SizeOfVertex;
        static const int SizeOfIndice;
    };
}

#endif // graphics_data_h__

