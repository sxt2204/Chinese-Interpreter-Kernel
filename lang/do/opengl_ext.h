#ifndef CUSTOM_OPENGL_EXT_H
#define CUSTOM_OPENGL_EXT_H

#include "../../source/interpreter/native_registry.h"
#include <vector>
#include <functional>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <unistd.h>
#endif

// 绘制队列：存储所有的 OpenGL 绘制闭包
static std::vector<std::function<void()>> gl_render_queue;

// 全局颜色状态
static float gl_current_r = 0.0f;
static float gl_current_g = 0.0f;
static float gl_current_b = 0.0f;

// 窗口标题
static std::string gl_window_title = "中文编程 - OpenGL绘图窗口";

// 背景颜色状态
static float gl_bg_r = 1.0f;
static float gl_bg_g = 1.0f;
static float gl_bg_b = 1.0f;

// 1. 设置颜色
REGISTER_NATIVE_FUNC(opengl_set_color, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 3) return 0.0;
    float r = static_cast<float>(valueToDouble(args[0]));
    float g = static_cast<float>(valueToDouble(args[1]));
    float b = static_cast<float>(valueToDouble(args[2]));
    
    gl_render_queue.push_back([=]() {
        glColor3f(r, g, b);
    });
    return 1.0;
});

// 2. 绘制点
REGISTER_NATIVE_FUNC(opengl_draw_point, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    float x = static_cast<float>(valueToDouble(args[0]));
    float y = static_cast<float>(valueToDouble(args[1]));
    
    gl_render_queue.push_back([=]() {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    });
    return 1.0;
});

// 3. 绘制线
REGISTER_NATIVE_FUNC(opengl_draw_line, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 4) return 0.0;
    float x1 = static_cast<float>(valueToDouble(args[0]));
    float y1 = static_cast<float>(valueToDouble(args[1]));
    float x2 = static_cast<float>(valueToDouble(args[2]));
    float y2 = static_cast<float>(valueToDouble(args[3]));
    
    gl_render_queue.push_back([=]() {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    });
    return 1.0;
});

// 4. 绘制矩形
REGISTER_NATIVE_FUNC(opengl_draw_rect, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 4) return 0.0;
    float x1 = static_cast<float>(valueToDouble(args[0]));
    float y1 = static_cast<float>(valueToDouble(args[1]));
    float x2 = static_cast<float>(valueToDouble(args[2]));
    float y2 = static_cast<float>(valueToDouble(args[3]));
    
    gl_render_queue.push_back([=]() {
        glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
    });
    return 1.0;
});

// 新增功能 1: 设置背景颜色
REGISTER_NATIVE_FUNC(opengl_set_bg_color, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 3) return 0.0;
    gl_bg_r = static_cast<float>(valueToDouble(args[0]));
    gl_bg_g = static_cast<float>(valueToDouble(args[1]));
    gl_bg_b = static_cast<float>(valueToDouble(args[2]));
    return 1.0;
});

// 新增功能 2: 清空画布
REGISTER_NATIVE_FUNC(opengl_clear_canvas, [](const std::vector<Value>& args) -> Value {
    gl_render_queue.push_back([=]() {
        glClear(GL_COLOR_BUFFER_BIT);
    });
    return 1.0;
});

// 新增功能 3: 设置画笔粗细
REGISTER_NATIVE_FUNC(opengl_set_line_width, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1) return 0.0;
    float width = static_cast<float>(valueToDouble(args[0]));
    gl_render_queue.push_back([=]() {
        glLineWidth(width);
    });
    return 1.0;
});

// 新增功能 4: 设置点大小
REGISTER_NATIVE_FUNC(opengl_set_point_size, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1) return 0.0;
    float size = static_cast<float>(valueToDouble(args[0]));
    gl_render_queue.push_back([=]() {
        glPointSize(size);
    });
    return 1.0;
});

// 新增功能 5: 绘制三角形
REGISTER_NATIVE_FUNC(opengl_draw_triangle, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 6) return 0.0;
    float x1 = static_cast<float>(valueToDouble(args[0]));
    float y1 = static_cast<float>(valueToDouble(args[1]));
    float x2 = static_cast<float>(valueToDouble(args[2]));
    float y2 = static_cast<float>(valueToDouble(args[3]));
    float x3 = static_cast<float>(valueToDouble(args[4]));
    float y3 = static_cast<float>(valueToDouble(args[5]));
    
    gl_render_queue.push_back([=]() {
        glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glEnd();
    });
    return 1.0;
});

// 新增功能 6: 绘制圆形
#include <math.h>
REGISTER_NATIVE_FUNC(opengl_draw_circle, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 3) return 0.0;
    float x = static_cast<float>(valueToDouble(args[0]));
    float y = static_cast<float>(valueToDouble(args[1]));
    float radius = static_cast<float>(valueToDouble(args[2]));
    
    gl_render_queue.push_back([=]() {
        glBegin(GL_POLYGON);
        for(int i = 0; i < 100; i++) {
            float theta = 2.0f * 3.1415926f * float(i) / float(100);
            float cx = radius * cosf(theta);
            float cy = radius * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    });
    return 1.0;
});

// 新增功能 7: 开始绘制多边形
REGISTER_NATIVE_FUNC(opengl_begin_polygon, [](const std::vector<Value>& args) -> Value {
    gl_render_queue.push_back([=]() {
        glBegin(GL_POLYGON);
    });
    return 1.0;
});

// 新增功能 8: 添加多边形顶点
REGISTER_NATIVE_FUNC(opengl_add_vertex, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    float x = static_cast<float>(valueToDouble(args[0]));
    float y = static_cast<float>(valueToDouble(args[1]));
    gl_render_queue.push_back([=]() {
        glVertex2f(x, y);
    });
    return 1.0;
});

// 新增功能 9: 结束绘制多边形
REGISTER_NATIVE_FUNC(opengl_end_polygon, [](const std::vector<Value>& args) -> Value {
    gl_render_queue.push_back([=]() {
        glEnd();
    });
    return 1.0;
});

// 新增功能 10: 平移画布
REGISTER_NATIVE_FUNC(opengl_translate, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    float tx = static_cast<float>(valueToDouble(args[0]));
    float ty = static_cast<float>(valueToDouble(args[1]));
    gl_render_queue.push_back([=]() {
        glTranslatef(tx, ty, 0.0f);
    });
    return 1.0;
});

// 新增功能 11: 旋转画布 (度数)
REGISTER_NATIVE_FUNC(opengl_rotate, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1) return 0.0;
    float angle = static_cast<float>(valueToDouble(args[0]));
    gl_render_queue.push_back([=]() {
        glRotatef(angle, 0.0f, 0.0f, 1.0f); // 2D 主要是绕 Z 轴旋转
    });
    return 1.0;
});

// 新增功能 12: 缩放画布
REGISTER_NATIVE_FUNC(opengl_scale, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    float sx = static_cast<float>(valueToDouble(args[0]));
    float sy = static_cast<float>(valueToDouble(args[1]));
    gl_render_queue.push_back([=]() {
        glScalef(sx, sy, 1.0f);
    });
    return 1.0;
});

// 新增功能 13: 保存画布状态
REGISTER_NATIVE_FUNC(opengl_push_matrix, [](const std::vector<Value>& args) -> Value {
    gl_render_queue.push_back([=]() {
        glPushMatrix();
    });
    return 1.0;
});

// 新增功能 14: 恢复画布状态
REGISTER_NATIVE_FUNC(opengl_pop_matrix, [](const std::vector<Value>& args) -> Value {
    gl_render_queue.push_back([=]() {
        glPopMatrix();
    });
    return 1.0;
});

// 5. 设置窗口标题
REGISTER_NATIVE_FUNC(opengl_set_window_title, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1) return 0.0;
    if (std::holds_alternative<std::string>(args[0])) {
        gl_window_title = std::get<std::string>(args[0]);
    }
    return 1.0;
});

// 原生窗口抽象
#ifdef _WIN32
LRESULT CALLBACK os_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE: PostQuitMessage(0); return 0;
        default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
void os_show_window() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = os_WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ChineseCompilerGL";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow("ChineseCompilerGL", gl_window_title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
    HDC hDC = GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0, 24, 8, 0,0,0,0,0,0 };
    int pf = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pf, &pfd);
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    std::cout << "[OpenGL] 正在显示绘图窗口 (请关闭窗口以继续)..." << std::endl;

    MSG msg; bool running = true;
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg); DispatchMessage(&msg);
        }
        if (!running) break;
        glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(0.0f, 0.0f, 0.0f);
        for (const auto& func : gl_render_queue) func();
        SwapBuffers(hDC);
        Sleep(16);
    }
    wglMakeCurrent(NULL, NULL); wglDeleteContext(hRC); ReleaseDC(hWnd, hDC); DestroyWindow(hWnd);
}
#elif defined(__APPLE__)
void os_show_window() {
    int argc = 1; char* argv[1] = { (char*)"ChineseCompiler" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow(gl_window_title.c_str());
    glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    glutDisplayFunc([]() {
        glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(0.0f, 0.0f, 0.0f);
        for (const auto& func : gl_render_queue) func();
        glFlush();
    });
    std::cout << "[OpenGL] 正在显示绘图窗口 (请关闭窗口以继续)..." << std::endl;
    glutMainLoop();
}
#else
void os_show_window() {
    Display *display = XOpenDisplay(NULL);
    if (!display) { std::cerr << "[错误] 无法连接到 X Server" << std::endl; return; }
    Window root = DefaultRootWindow(display);
    GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(display, 0, att);
    if (!vi) return;
    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    XSetWindowAttributes swa; swa.colormap = cmap; swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
    Window win = XCreateWindow(display, root, 0, 0, 800, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, win);
    XStoreName(display, win, gl_window_title.c_str());
    GLXContext glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glc);

    glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    std::cout << "[OpenGL] 正在显示绘图窗口 (请按任意键关闭窗口以继续)..." << std::endl;

    XEvent xev; bool running = true;
    while (running) {
        while (XPending(display)) {
            XNextEvent(display, &xev);
            if (xev.type == KeyPress) running = false;
        }
        glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(0.0f, 0.0f, 0.0f);
        for (const auto& func : gl_render_queue) func();
        glXSwapBuffers(display, win);
        usleep(16000);
    }
    glXMakeCurrent(display, None, NULL); glXDestroyContext(display, glc); XDestroyWindow(display, win); XCloseDisplay(display);
}
#endif

// 6. 显示窗口 (主循环，会阻塞)
REGISTER_NATIVE_FUNC(opengl_show_window, [](const std::vector<Value>& args) -> Value {
    os_show_window();
    return 1.0;
});

#endif // CUSTOM_OPENGL_EXT_H
