/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
#include "kgl_lib_pch.h"
#include "kgl_vertex_buffer_object.h"

namespace kgl
{
//#include "common_header.h"
//
//#include "vertexBufferObject.h"
//
//	CVertexBufferObject::CVertexBufferObject()
//	{
//		bDataUploaded = false;
//		uiBuffer = 0;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	CreateVBO
//
//	Params:	a_iSize - initial size of buffer
//
//	Result:	Creates vertex buffer object.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::CreateVBO(int a_iSize)
//	{
//		glGenBuffers(1, &uiBuffer);
//		data.reserve(a_iSize);
//		iSize = a_iSize;
//		iCurrentSize = 0;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	DeleteVBO
//
//	Params:	none
//
//	Result:	Releases VBO and frees all memory.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::DeleteVBO()
//	{
//		glDeleteBuffers(1, &uiBuffer);
//		bDataUploaded = false;
//		data.clear();
//	}
//
//	/*-----------------------------------------------
//
//	Name:	mapBufferToMemory
//
//	Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
//
//	Result:	Maps whole buffer data to memory and
//	returns pointer to data.
//
//	/*---------------------------------------------*/
//
//	void* CVertexBufferObject::MapBufferToMemory(int iUsageHint)
//	{
//		if (!bDataUploaded)return NULL;
//		void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
//		return ptrRes;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	MapSubBufferToMemory
//
//	Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
//	uiOffset - data offset (from where should
//	data be mapped).
//	uiLength - length of data
//
//	Result:	Maps specified part of buffer to memory.
//
//	/*---------------------------------------------*/
//
//	void* CVertexBufferObject::MapSubBufferToMemory(int iUsageHint, UINT uiOffset, UINT uiLength)
//	{
//		if (!bDataUploaded)return NULL;
//		void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
//		return ptrRes;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	UnmapBuffer
//
//	Params:	none
//
//	Result:	Unmaps previously mapped buffer.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::UnmapBuffer()
//	{
//		glUnmapBuffer(iBufferType);
//	}
//
//	/*-----------------------------------------------
//
//	Name:	BindVBO
//
//	Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)
//
//	Result:	Binds this VBO.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::BindVBO(int a_iBufferType)
//	{
//		iBufferType = a_iBufferType;
//		glBindBuffer(iBufferType, uiBuffer);
//	}
//
//	/*-----------------------------------------------
//
//	Name:	UploadDataToGPU
//
//	Params:	iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
//
//	Result:	Sends data to GPU.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::UploadDataToGPU(int iDrawingHint)
//	{
//		glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
//		bDataUploaded = true;
//		data.clear();
//	}
//
//	/*-----------------------------------------------
//
//	Name:	AddData
//
//	Params:	ptrData - pointer to arbitrary data
//	uiDataSize - data size in bytes
//
//	Result:	Adds arbitrary data to VBO.
//
//	/*---------------------------------------------*/
//
//	void CVertexBufferObject::AddData(void* ptrData, UINT uiDataSize)
//	{
//		data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData + uiDataSize);
//		iCurrentSize += uiDataSize;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	GetDataPointer
//
//	Params:	none
//
//	Result:	Returns data pointer (only before uploading).
//
//	/*---------------------------------------------*/
//
//	void* CVertexBufferObject::GetDataPointer()
//	{
//		if (bDataUploaded)return NULL;
//		return (void*)data[0];
//	}
//
//	/*-----------------------------------------------
//
//	Name:	GetBufferID
//
//	Params:	none
//
//	Result:	Returns VBO ID.
//
//	/*---------------------------------------------*/
//
//	UINT CVertexBufferObject::GetBufferID()
//	{
//		return uiBuffer;
//	}
//
//	/*-----------------------------------------------
//
//	Name:	GetCurrentSize
//
//	Params:	none
//
//	Result: Returns size of data that has been added to
//	the buffer object.
//
//	/*---------------------------------------------*/
//
//	int CVertexBufferObject::GetCurrentSize()
//	{
//		return iCurrentSize;
//	}
}