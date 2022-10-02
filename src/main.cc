#include <iostream>

#include <chrono>
#include <thread>

#include "window/TransparentWindow.h"

void drawCallback(TransparentWindow& transparentWindow);

int main() {

    int x = 1000, y = 250;
    unsigned int width = 300, height = 300;
    
    TransparentWindow window("Transparent window", x, y, width, height);
    window.startDrawing(drawCallback);

    return 0;
}

void drawCallback(TransparentWindow& transparentWindow) {

    // OpenGL code
    glClearColor( 0.0, 0.0, 0.0, 1) ;
    glClear(GL_COLOR_BUFFER_BIT) ;

    // Draw triangle
    static float alpha = 0.1f;
    glRotatef(alpha, 0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.75f);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f); glVertex2f(-0.75f, -0.75f);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f); glVertex2f(0.75f, -0.75f);
    glEnd();

    // If repaint, sleep for better performance or fix FPS
    transparentWindow.repaint();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
}