#pragma once

#include <iostream>
#include <functional>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#include <GL/gl.h>
#include <GL/glx.h>

class TransparentWindow;

typedef std::function<void(TransparentWindow& transparentWindow)> Callback;

class TransparentWindow {
private:
    Display* display;
    char * xserver;
    XVisualInfo visualinfo;
    Window win;
    GC gc;
    XEvent event;
    
    std::string title;
    int x, y;
    unsigned int width, height;
    bool undecorated, open;

    bool redraw;
    Callback drawCallback;
public:
    TransparentWindow(const std::string& _title, int _x, int _y, unsigned int _width, unsigned int _height, bool _undecorated = true);
    TransparentWindow() = default;
    ~TransparentWindow();
private:
    void initDisplay();
    void createOpenGLContext();
    void createWindow();
    void initWindow();
public:
    void setDrawCallback(const Callback& drawCallback);
    inline void repaint() { redraw = true; }

    inline void drawText(const std::string& text, int x, int y) {
        XDrawString(display, win, gc, x, y, text.c_str(), text.size()) ;
    }
public:
    inline Display* getDisplay() { return display; }
    inline Window& getWindow() { return win; }
    inline XEvent& getEvent() { return event; }

    inline int getX() const { return x; }
    inline int getY() const { return y; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    inline bool isOpen() const { return open; }
    inline bool isUndecorated() const { return undecorated; }
};