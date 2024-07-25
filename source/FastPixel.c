#include <windows.h>
#include <GL/gl.h>
#include <stdint.h>

HWND hWnd;
HDC hDC;
HGLRC hRC;
GLuint textureId;
uint32_t* pixels;
int x, y, width, height;

typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

static void SetupPixelFormat(HDC hDC) {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    int pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

HGLRC FPInit(HWND windowID, int posX, int posY, int w, int h, uint32_t* pixelArray) {
    hWnd = windowID;
    x = posX;
    y = posY;
    width = w;
    height = h;
    pixels = pixelArray;

    hDC = GetDC(hWnd);
    if (!hDC) {
        return 0;
    }

    SetupPixelFormat(hDC);
    hRC = wglCreateContext(hDC);
    if (!hRC) {
        ReleaseDC(hWnd, hDC);
        return 0;
    }

    if (!wglMakeCurrent(hDC, hRC)) {
        wglDeleteContext(hRC);
        ReleaseDC(hWnd, hDC);
        return 0;
    }
	
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int windowHeight = clientRect.bottom - clientRect.top;

    glViewport(x, windowHeight - y - height, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);  // flipped y-axis
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT) {wglSwapIntervalEXT(0);}  // vsync on/off

    return hRC;
}

void FPFree() {
    if (hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        hRC = NULL;
    }
    if (hDC) {
        ReleaseDC(hWnd, hDC);
        hDC = NULL;
    }
    if (textureId) {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }
}

void FPClear(uint32_t rgba) {
    for (int i = 0; i < width; i++) {pixels[i] = rgba;}
	for (int i = 1; i < height; i++) {
		memcpy(&pixels[i * width], pixels, width * sizeof(uint32_t));
	}
}

void FPDraw() {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(width, 0);
    glTexCoord2f(1, 1); glVertex2f(width, height);
    glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();
    SwapBuffers(hDC);
}