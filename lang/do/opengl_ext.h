#ifndef CUSTOM_OPENGL_EXT_H
#define CUSTOM_OPENGL_EXT_H

#include "../../source/interpreter/native_registry.h"
#include <vector>
#include <functional>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// 绘制队列：存储所有的 OpenGL 绘制闭包
static std::vector<std::function<void()>> gl_render_queue;

// 全局颜色状态
static float gl_current_r = 0.0f;
static float gl_current_g = 0.0f;
static float gl_current_b = 0.0f;

// 窗口标题
static std::string gl_window_title = "中文编程 - OpenGL绘图窗口";

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

// 5. 设置窗口标题
REGISTER_NATIVE_FUNC(opengl_set_window_title, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1) return 0.0;
    if (std::holds_alternative<std::string>(args[0])) {
        gl_window_title = std::get<std::string>(args[0]);
    }
    return 1.0;
});

// 6. 显示窗口 (主循环，会阻塞)
REGISTER_NATIVE_FUNC(opengl_show_window, [](const std::vector<Value>& args) -> Value {
    int argc = 1;
    char* argv[1] = { (char*)"ChineseCompiler" };
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow(gl_window_title.c_str());
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色背景
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);   // 左下角(0,0)，右上角(800,600)
    
    glutDisplayFunc([]() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 默认绘制颜色为黑色
        glColor3f(0.0f, 0.0f, 0.0f);
        
        for (const auto& func : gl_render_queue) {
            func();
        }
        
        glFlush();
    });
    
    std::cout << "[OpenGL] 正在显示绘图窗口 (请关闭窗口以继续)..." << std::endl;
    glutMainLoop(); // 阻塞执行，直到用户关闭窗口 (视不同系统行为可能不同)
    return 1.0;
});

#endif // CUSTOM_OPENGL_EXT_H
