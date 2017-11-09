// 图像读取器

#include "kgl_lib_pch.h"
#include "kgl_image_file_reader.h"

namespace kgl
{
	void GLBitmap::WriteRGB(int first_pixel_component_index, unsigned char r, unsigned char g, unsigned char b)
	{
		buffer_[first_pixel_component_index + 0] = r;
		buffer_[first_pixel_component_index + 1] = g;
		buffer_[first_pixel_component_index + 2] = b;
	}

	void GLBitmap::WriteRGBA(int first_pixel_component_index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		buffer_[first_pixel_component_index + 0] = r;
		buffer_[first_pixel_component_index + 1] = g;
		buffer_[first_pixel_component_index + 2] = b;
		buffer_[first_pixel_component_index + 3] = a;
	}

	ImageFileReader::ImageFileReader()
	{
		FreeImage_Initialise(TRUE);
	}

	ImageFileReader::~ImageFileReader()
	{
		
	}

	GLBitmapSPtr ImageFileReader::GetGLBitmapFromFile(const std::string& file_name, GLuint desired_rgb_mode)
	{
		GLuint tex = 0;
		int i,j,k;
		int w, h;
		int bpp;
		unsigned char* bits = nullptr;
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* fibmp = nullptr;

		FreeImage_Initialise(TRUE);

		// 获取到文件格式
		fif = FreeImage_GetFileType(file_name.c_str(), 0);

		if (FIF_UNKNOWN == fif)
		{
			fif = FreeImage_GetFIFFromFilename(file_name.c_str());

			if (FIF_UNKNOWN == fif)
				return std::make_shared<GLBitmap>(); // 返回一个空指针
		}

		if (FreeImage_FIFSupportsReading(fif))
			fibmp = FreeImage_Load(fif, file_name.c_str(), 0);

		int pitch = FreeImage_GetPitch(fibmp);// 从freeimage bitmap info中取出图片跨度
		bits = FreeImage_GetBits(fibmp); // 从freeimage bitmap info中取出字节数据
		bpp = FreeImage_GetBPP(fibmp);// 从freeimage bitmap info中取出字节数据取出bit per pixel数据
		RGBQUAD *palette = nullptr;

		w = FreeImage_GetWidth(fibmp);
		h = FreeImage_GetHeight(fibmp);

		switch (bpp)
		{
		case 8:
		{
			if (!(palette = FreeImage_GetPalette(fibmp)))
				return std::make_shared<GLBitmap>();

			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGB(index, palette[i*pitch + j].rgbRed,
							palette[i*pitch + j].rgbGreen,palette[i*pitch + j].rgbBlue);
						/*glbmp->buf[(i*w + j) * 3 + 0] = palette[k].rgbRed;
						glbmp->buf[(i*w + j) * 3 + 1] = palette[k].rgbGreen;
						glbmp->buf[(i*w + j) * 3 + 2] = palette[k].rgbBlue;*/
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						k = bits[i*pitch + j];
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGBA(index, palette[k].rgbRed, palette[k].rgbGreen, palette[k].rgbBlue, 0xFF);
					}
				}
			}

			FreeImage_Unload(fibmp);
			return gl_bmp;
		}
		break;
		case 24:
		{
			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGB(index, bits[i*pitch + j * 3 + 2],
							bits[i*pitch + j * 3 + 1], bits[i*pitch + j * 3 + 0]);
						/*
						glbmp->buf[(i*glbmp->w + j) * 3 + 0] = bits[i*pitch + j * 3 + 2];
						glbmp->buf[(i*glbmp->w + j) * 3 + 1] = bits[i*pitch + j * 3 + 1];
						glbmp->buf[(i*glbmp->w + j) * 3 + 2] = bits[i*pitch + j * 3 + 0];
						*/
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (w + j) * 3;
						gl_bmp->WriteRGBA(index, bits[i*pitch + j * 3 + 2],
							bits[i*pitch + j * 3 + 1], bits[i*pitch + j * 3 + 0], 0xFF);
					}
				}
			}

	//		FreeImage_Unload(fibmp);
			return gl_bmp;
		}
		break;
		case 32:
		{
			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 4;
						gl_bmp->WriteRGB(index, bits[i*pitch + j * 4 + 2],
							bits[i*pitch + j * 4 + 1], bits[i*pitch + j * 4 + 0]);
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 4;
						gl_bmp->WriteRGBA(index,
							bits[i*pitch + j * 4 + 2], bits[i*pitch + j * 4 + 1],
							bits[i*pitch + j * 4 + 0], bits[i*pitch + j * 4 + 3]);
					}
				}
			}

			FreeImage_Unload(fibmp);
			return gl_bmp;
			/*
			for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
			glbmp->buf[(i*glbmp->w + j) * 4 + 0] = bits[i*pitch + j * 4 + 2];
			glbmp->buf[(i*glbmp->w + j) * 4 + 1] = bits[i*pitch + j * 4 + 1];
			glbmp->buf[(i*glbmp->w + j) * 4 + 2] = bits[i*pitch + j * 4 + 0];
			glbmp->buf[(i*glbmp->w + j) * 4 + 3] = bits[i*pitch + j * 4 + 3];
			}*/
		}
		break;
		default:
			FreeImage_Unload(fibmp);
			return  std::make_shared<GLBitmap>();;
		}

		FreeImage_Unload(fibmp);
		return std::make_shared<GLBitmap>();;
	}
}

/*
GLBITMAP * FIBitmap2GLBitmap(FIBITMAP *fibmp)
{
	int i, j, k;
	int pitch = FreeImage_GetPitch(fibmp);
	unsigned char *bits = FreeImage_GetBits(fibmp);
	int bpp = FreeImage_GetBPP(fibmp);
	GLBITMAP *glbmp = (GLBITMAP *)malloc(sizeof(GLBITMAP));
	RGBQUAD *palette = NULL;

	if (!glbmp) return NULL;

	glbmp->w = FreeImage_GetWidth(fibmp);
	glbmp->h = FreeImage_GetHeight(fibmp);

	switch (bpp) {
	case 8:
		if (!(palette = FreeImage_GetPalette(fibmp))) return NULL;
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
		glbmp->rgb_mode = GL_RGB;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				k = bits[i*pitch + j];
				glbmp->buf[(i*glbmp->w + j) * 3 + 0] = palette[k].rgbRed;
				glbmp->buf[(i*glbmp->w + j) * 3 + 1] = palette[k].rgbGreen;
				glbmp->buf[(i*glbmp->w + j) * 3 + 2] = palette[k].rgbBlue;
			}
		}
		break;
	case 24:
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
		glbmp->rgb_mode = GL_RGB;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				glbmp->buf[(i*glbmp->w + j) * 3 + 0] = bits[i*pitch + j * 3 + 2];
				glbmp->buf[(i*glbmp->w + j) * 3 + 1] = bits[i*pitch + j * 3 + 1];
				glbmp->buf[(i*glbmp->w + j) * 3 + 2] = bits[i*pitch + j * 3 + 0];
			}
		}
		break;
	case 32:
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 4))) return NULL;
		glbmp->rgb_mode = GL_RGBA;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				glbmp->buf[(i*glbmp->w + j) * 4 + 0] = bits[i*pitch + j * 4 + 2];
				glbmp->buf[(i*glbmp->w + j) * 4 + 1] = bits[i*pitch + j * 4 + 1];
				glbmp->buf[(i*glbmp->w + j) * 4 + 2] = bits[i*pitch + j * 4 + 0];
				glbmp->buf[(i*glbmp->w + j) * 4 + 3] = bits[i*pitch + j * 4 + 3];
			}
		}
		break;
	default: return NULL;
	}

	return glbmp;
}

void FreeGLBitmap(GLBITMAP *glbmp)
{
	if (glbmp) {
		if (glbmp->buf) free(glbmp->buf);
		free(glbmp);
	}
}


GLuint loadtexture(const char *filename)
{
	GLuint tex = 0;
	int tmp_bit;
	int i;
	int w, h;
	int bpp;
	unsigned char *bits = NULL;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *bitmap = NULL;
	GLBITMAP *glbmp = NULL;

	fif = FreeImage_GetFileType(filename, 0);
	if (FIF_UNKNOWN == fif) {
		fif = FreeImage_GetFIFFromFilename(filename);
		if (FIF_UNKNOWN == fif)
			return 0;
	}
	if (FreeImage_FIFSupportsReading(fif))
		bitmap = FreeImage_Load(fif, filename, 0);

	if (!bitmap)
		return 0;

	printf("bit: %d\n", FreeImage_GetBPP(bitmap));

	glbmp = FIBitmap2GLBitmap(bitmap);
	if (!glbmp)
		return 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, glbmp->rgb_mode, glbmp->w, glbmp->h, 0, glbmp->rgb_mode, GL_UNSIGNED_BYTE, glbmp->buf);

	FreeGLBitmap(glbmp);
	FreeImage_Unload(bitmap);

	return tex;
}


static GLfloat rot_z = 0.0f;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glTranslatef(0.0f, 2.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, S_bmp_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	glTranslatef(-2.0, -2.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, S_png_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	glTranslatef(2.0, -2.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, S_jpg_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	glTranslatef(2.0, 2.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, S_tga_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	rot_z += 0.1f;

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	if (0 == h) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


#define LOADTEXTURE(t, f)   do{ if(!((t) = loadtexture(f))){ ERROR("load texture(%s) failed!\n", f); } }while(0)  
void init(void)
{
#if defined(FREEIMAGE_LIB)  
	FreeImage_Initialise(0);
#endif  

	LOADTEXTURE(S_bmp_tex, "images/BG.bmp");
	LOADTEXTURE(S_png_tex, "images/brown.png");
	LOADTEXTURE(S_jpg_tex, "images/apple.jpg");
	LOADTEXTURE(S_tga_tex, "images/Font.tga");

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("OpenGL - FreeImage!");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
#if defined(FREEIMAGE_LIB)  
	FreeImage_DeInitialise();
#endif  
	return 0;
}
}*/