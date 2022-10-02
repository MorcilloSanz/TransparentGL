/*
    MIT License

    Copyright (c) 2022 Alberto Morcillo Sanz

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    Inspired by: Joerg Seebohn
    https://gist.github.com/je-so/903479
*/

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

#define _DEPRECATED_

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
    void startDrawing(const Callback& drawCallback);

    inline void repaint() { redraw = true; }

    inline void moveWindow(int x, int y) {
        XMoveWindow(display, win, x, y);
    }

    inline void resizeWindow(unsigned int width, unsigned int height) {
        XResizeWindow(display, win, width, height);
    }

    _DEPRECATED_ inline void drawText(const std::string& text, int x, int y) {
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