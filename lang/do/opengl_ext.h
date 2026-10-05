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

// 6. 显示窗口 (主循环，会阻塞)
REGISTER_NATIVE_FUNC(opengl_show_window, [](const std::vector<Value>& args) -> Value {
    int argc = 1;
    char* argv[1] = { (char*)"ChineseCompiler" };
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow(gl_window_title.c_str());
    
    glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f); // 使用设置的背景色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);   // 左下角(0,0)，右上角(800,600)
    glMatrixMode(GL_MODELVIEW);           // 切换回模型视图用于变换
    glLoadIdentity();
    
    glutDisplayFunc([]() {
        glClearColor(gl_bg_r, gl_bg_g, gl_bg_b, 1.0f); // 动态更新背景色
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity(); // 每次绘制重置变换矩阵
        
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
