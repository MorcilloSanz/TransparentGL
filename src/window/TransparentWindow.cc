#include "TransparentWindow.h"

TransparentWindow::TransparentWindow(const std::string& _title, int _x, int _y, unsigned int _width, unsigned int _height, bool _undecorated) 
    : display(nullptr), xserver(nullptr), title(_title), x(_x), y(_y), width(_width), height(_height), undecorated(_undecorated), open(true), redraw(false) {
    initWindow();
    drawCallback = [&](TransparentWindow& transparentWindow) { /* Do nothing */ };
}

TransparentWindow::~TransparentWindow() {
    XDestroyWindow(display, win); 
    XCloseDisplay(display); 
}

void TransparentWindow::initDisplay() {
    display = XOpenDisplay(0) ;
    xserver = getenv("DISPLAY") ;

    if (display == 0) {
        printf("Could not establish a connection to X-server '%s'\n", xserver ) ;
        exit(1) ;
    }
}

void TransparentWindow::createOpenGLContext() {
    GLXContext glcontext = glXCreateContext( display, &visualinfo, 0, True ) ;

    if (!glcontext) {
        printf("X11 server '%s' does not support OpenGL\n", xserver ) ;
        exit(1) ;
    }
    glXMakeCurrent( display, win, glcontext ) ;
}

void TransparentWindow::createWindow() {
    // query Visual for "TrueColor" and 32 bits depth (RGBA)
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualinfo);

    // create window
    XSetWindowAttributes attr ;
    attr.colormap   = XCreateColormap( display, DefaultRootWindow(display), visualinfo.visual, AllocNone) ;
    attr.event_mask = ExposureMask | KeyPressMask ;
    attr.background_pixmap = None ;
    attr.border_pixel = 0 ;
    
    int undecoratedFlag = 0;
    if(undecorated) {
        attr.override_redirect = true;
        undecoratedFlag = CWOverrideRedirect;
    }

    win = XCreateWindow(display, DefaultRootWindow(display),
        x, y, width, height, // x,y,width,height : are possibly opverwriteen by window manager
        0,
        visualinfo.depth,
        InputOutput,
        visualinfo.visual,
        CWColormap|CWEventMask|CWBackPixmap|CWBorderPixel|undecoratedFlag,
        &attr
    ) ;

    gc = XCreateGC(display, win, 0, 0) ;

    // set title bar name of window
    XStoreName(display, win, title.c_str()) ;

    // say window manager which position we would prefer
    XSizeHints sizehints ;
    sizehints.flags = PPosition | PSize ;
    sizehints.x     = x ;  sizehints.y = y ;
    sizehints.width = width ; sizehints.height = height;
    XSetWMNormalHints(display, win, &sizehints) ;

    // Switch On >> If user pressed close key let window manager only send notification >>
    Atom wm_delete_window = XInternAtom( display, "WM_DELETE_WINDOW", 0) ;
    XSetWMProtocols(display, win, &wm_delete_window, 1) ;

    // Create OpenGLContext
    createOpenGLContext();

    // Allow clicking through the window
    XRectangle rect;
    XserverRegion region = XFixesCreateRegion(display, &rect, 1);
    XFixesSetWindowShapeRegion(display, win, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);

    // now let the window appear to the user
    XMapWindow( display, win) ;
}

void TransparentWindow::initWindow() {
    initDisplay();
    createWindow();
}

void TransparentWindow::startDrawing(const Callback& drawCallback) {
    this->drawCallback = drawCallback;

    while(open) {

        while( XPending(display) > 0 ) {
            XNextEvent( display, &event) ;

            switch(event.type) {
            case Expose:
                redraw = true;
            break;
            default:
                break ;
            }
        }

        if(redraw) {
            // Important: redraw = false; before drawCallback
            redraw = false;
            // Draw callback
            drawCallback(*this);
            // Swap buffers
            glXSwapBuffers( display, win) ;
            glXWaitGL() ;
        }
    }
}