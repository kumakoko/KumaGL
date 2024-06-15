//#ifndef graphics_data_h__
//#define graphics_data_h__
#pragma once

#include "graphics_buffer_object.h"
#include "triangle_look_up.h"
#include "transformer.h"

namespace DigitalSculpt
{
    // 对GraphicBufferObject类进行保护继承，即是说GraphicsBufferObject类的公有成员和保护成员，
    // 都以保护成员的方式出现在派生类中，所以GaraphicsData类的实例对象在外部代码中不能直接对
    // GraphicBufferObject类进行调用
    class GraphicsData : protected GraphicsBufferObject, public TriangleLookUp, public ModelMatrix
    {
    public:
        GraphicsData();
        ~GraphicsData();

        /******************************************************************************************************************
         * Desc:
         * Method:    handleDynamicVertexIndexModification
         * Returns:   void
         ****************************************************************************************************************/
        void handleDynamicVertexIndexModification();
        
        /******************************************************************************************************************
         * Desc:
         * Method:    deleteBuffers
         * Returns:   void
         ****************************************************************************************************************/
        void deleteBuffers(); // GPU memory management

        /******************************************************************************************************************
         * Desc:
         * Method:    bind
         * Returns:   void
         ****************************************************************************************************************/
        void bind(); // bind to OpenGL

        /******************************************************************************************************************
         * Desc:
         * Method:    refresh
         * Returns:   void
         ****************************************************************************************************************/
        void refresh(); // reload vertex data to OpenGL.

        /******************************************************************************************************************
         * Desc:
         * Method:    render
         * Returns:   void
         ****************************************************************************************************************/
        void render(); // render and draw vertex data.

        /******************************************************************************************************************
         * Desc:
         * Method:    invertFaces
         * Returns:   void
         ****************************************************************************************************************/
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

//#endif // graphics_data_h__

