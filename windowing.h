#ifndef WINDOWING_H
#define WINDOWING_H

#include <cstdio>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <X11/Xlib.h>

const int32_t DEFAULT_WIDTH       = 800;
const int32_t DEFAULT_HEIGHT      = 600;
const char*   DEFAULT_WINDOW_NAME = "CPU_Ray_Tracer";
const int16_t DEFAULT_DISPLAY_NUM = 0;
const uint8_t IMAGE_DEPTH         = 24; // "True Color (8-bit)"
const uint8_t IMAGE_STORAGE_DEPTH = 32;

/* Design of RTWindow:
    1. Instantiate with RTWindow(width, height, title, display_num);
    2. Open a while(!shouldClose()) loop
    3. Add main logic into the loop, along with a pollEvents() call
    4. Recommended two-part drawing procedure:
        a. Write to window framebuffer by passing an array of RTPixels
        b. Refresh on-screen image
    5. Alternative: direct drawing with drawPixel(); not recommended if redrawing many pixels
*/

namespace rtw
{
    typedef struct RTPixel
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } RTPixel;

    class RTWindow 
    {
        public:
            // empty parameters: -1, -1, NULL, -1
            RTWindow(int16_t     windowWidth,    // width in px
                     int16_t     windowHeight,   // height in px
                     const char *windowName,     // window title 
                     int16_t     displayNum);    // which monitor to put window on (0 is main)
            ~RTWindow();

            // event functions
            bool shouldClose();
            void pollEvents();

            // drawing commands
            void writeToBuffer(RTPixel  *pixels, 
                               int16_t  x,
                               int16_t  y,
                               uint16_t rows,
                               uint16_t cols);
            void displayBuffer(int16_t x,
                               int16_t y,
                               uint16_t rows,
                               uint16_t cols);
            // avoid directly drawing with drawPixel when possible; XCB calls introduce a lot of overhead
            void drawPixel    (int16_t  x,
                               int16_t  y,
                               uint16_t red,
                               uint16_t green,
                               uint16_t blue);
        private:
            // XCB-specific members
            xcb_connection_t          *m_connection;
            const xcb_setup_t         *m_setup;          // setup info
            xcb_window_t               m_xid;            // window id
            xcb_gcontext_t             m_gcid;           // graphics context id
            xcb_pixmap_t               m_pxid;           // pixmap id
            xcb_format_t              *m_pxfmt;          // pixmap format
            xcb_colormap_t             m_colormap;

            // RTWindow-specific members
            bool                m_should_close = false;
            uint16_t            m_window_width;
            uint16_t            m_window_height;
            const char         *m_window_name;

            // ignore RTFramebuffer functions unless specifically modifying RTWindow behavior
            typedef struct RTFramebuffer 
            {
                public:
                    RTFramebuffer(uint16_t width, uint16_t height, xcb_format_t *pxfmt) {
                        m_fb_width = width;
                        m_fb_height = height;
                        m_pxfmt = pxfmt;
                        format_buffer(m_fb_width, m_fb_height);
                        printf("Created framebuffer.\n");
                    }
                    ~RTFramebuffer() {
                        free(m_image);
                        free(m_image_data);
                        printf("Destroyed framebuffer.\n");
                    }
                    void resize(uint16_t width, uint16_t height) {
                        free(m_image);
                        free(m_image_data);
                        format_buffer(width, height);
                    }
                    // modifying buffer
                    // for some reason this doesn't draw colors without the fflush(stdout)
                    void modify(RTPixel *pixels, int16_t x, int16_t y, uint16_t rows, uint16_t cols) {
                        int p = 0;
                        uint8_t k = (IMAGE_STORAGE_DEPTH >> 3);
                        for (int i = 0; i < rows; ++i) {
                            for (int j = 0; j < cols; ++j) {
                                uint32_t offset = k * (m_fb_width * (x+i) + (y+j));
                                m_image_data[offset+0] = pixels[p].red;
                                fflush(stdout);
                                m_image_data[offset+1] = pixels[p].green;
                                fflush(stdout);
                                m_image_data[offset+2] = pixels[p].blue;
                                fflush(stdout);
                                ++p;
                            }
                        }
                    }
                    xcb_image_t *getImage() { return m_image; }
                private:
                    uint8_t     *m_image_data;      // format: [width][height][4]
                    xcb_image_t *m_image;
                    xcb_format_t*m_pxfmt;
                    uint16_t     m_fb_width;
                    uint16_t     m_fb_height;

                    void format_buffer(uint16_t width, uint16_t height) {
                        m_image_data = (uint8_t *)malloc(width*height*(IMAGE_STORAGE_DEPTH >> 3));
                        m_image = xcb_image_create(width,
                                                   height,
                                                   XCB_IMAGE_FORMAT_Z_PIXMAP,
                                                   m_pxfmt->scanline_pad,
                                                   m_pxfmt->depth,
                                                   m_pxfmt->bits_per_pixel,
                                                   0,
                                                   XCB_IMAGE_ORDER_MSB_FIRST,
                                                   XCB_IMAGE_ORDER_LSB_FIRST,
                                                   m_image_data,
                                                   width*height*(IMAGE_STORAGE_DEPTH >> 3),
                                                   m_image_data);
                    }
            } RTFramebuffer;
            
            RTFramebuffer       *m_rtfb;

            // format setup function
            xcb_format_t *find_px_format(const xcb_setup_t *setup) {
                xcb_format_t *px_it = xcb_setup_pixmap_formats(setup);
                xcb_format_t *it_end = px_it + xcb_setup_pixmap_formats_length(setup);
                for ( ; px_it != it_end; ++px_it) {
                    printf("Available pixmap format: scanline_pad: %d, depth: %d, bpp: %d\n", 
                            px_it->scanline_pad, px_it->depth, px_it->bits_per_pixel);
                    if ((px_it->depth == IMAGE_DEPTH) && (px_it->bits_per_pixel == IMAGE_STORAGE_DEPTH))
                        return px_it;
                }
                printf("No suitable pixmap format found. Ending.\n");
                exit(-1);
            }
    };
};

rtw::RTWindow::RTWindow(int16_t     windowWidth, 
                        int16_t     windowHeight, 
                        const char *windowName, 
                        int16_t     displayNum)
{
    // change to default parameters if necessary
    m_window_width = (windowWidth == -1) ? DEFAULT_WIDTH : windowWidth;
    m_window_height = (windowHeight == -1) ? DEFAULT_HEIGHT : windowHeight;
    m_window_name = (windowName == NULL) ? DEFAULT_WINDOW_NAME : windowName;
    if (displayNum == -1) displayNum = DEFAULT_DISPLAY_NUM;

    printf("Window width, height: (%d, %d)", m_window_width, m_window_height);

    m_connection = xcb_connect(
        NULL,                          // const char*, display name; can be NULL
        NULL                                 // int*, screen number of connection
    );
    if (xcb_connection_has_error(m_connection)) {
        printf("Error: Cannot open window.\n");
        exit(1);
    }

    // check basic info about connection
    m_setup = xcb_get_setup(m_connection);
    xcb_screen_iterator_t display_it = xcb_setup_roots_iterator(m_setup);

    // run through windows on each display; .rem is the number of remaining displays
    for (uint16_t i = 0; i < displayNum; ++i)
        xcb_screen_next(&display_it);

    // display preliminary info
    xcb_screen_t *display = display_it.data;
    printf("\n");
    printf("Max display resolution: (%d, %d)\n", display->width_in_pixels, display->height_in_pixels);

    // create the window
    m_xid = xcb_generate_id(m_connection);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = { display->black_pixel, XCB_EVENT_MASK_KEY_PRESS };
    xcb_create_window(m_connection,
                      XCB_COPY_FROM_PARENT, // window depth
                      m_xid,
                      display->root,        // parent window
                      0, 0,                 // x, y
                      m_window_width, m_window_height,
                      0,                    // border width
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
                      display->root_visual, // visual
                      mask, values);        // masks

    // display the window
    xcb_map_window(m_connection, m_xid);
    xcb_flush(m_connection);

    // allocate color map
    m_colormap = display->default_colormap;

    // generate pixmap
    m_pxid = xcb_generate_id(m_connection);
    xcb_create_pixmap(m_connection,
                      display->root_depth,
                      m_pxid,
                      m_xid,
                      m_window_width,
                      m_window_height);

    // set up graphics context
    m_gcid = xcb_generate_id(m_connection);
    uint32_t valuemask = (XCB_GC_FOREGROUND | XCB_GC_BACKGROUND);
    const uint32_t valuelist[] = { display->black_pixel, display->white_pixel };
    xcb_create_gc(m_connection,
                  m_gcid,
                  m_pxid,
                  valuemask,
                  valuelist);

    // generate framebuffer
    m_pxfmt = find_px_format(m_setup);
    m_rtfb = new RTFramebuffer(m_window_width, m_window_height, m_pxfmt);
}

rtw::RTWindow::~RTWindow()
{
    delete(m_rtfb);
    xcb_free_pixmap(m_connection, m_pxid);
    xcb_free_colormap(m_connection, m_colormap);
    xcb_disconnect(m_connection);
    printf("Destroyed window.\n");
}

bool 
rtw::RTWindow::shouldClose() 
{
    return m_should_close;
}

void 
rtw::RTWindow::pollEvents()
{
    xcb_generic_event_t *event = xcb_poll_for_event(m_connection);
    if (event == NULL) return;
    switch (event->response_type & ~0x80) {
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
            printf("Key pressed: %u\n", kp->detail);
            if (kp->detail == 9) {
                printf("Closing window.\n");
                m_should_close = true;
            }
            break;
        }
    }
    //xcb_flush(m_connection);
    free(event);
}

void
rtw::RTWindow::writeToBuffer(RTPixel *pixels,
                             int16_t  x,
                             int16_t  y,
                             uint16_t width,
                             uint16_t height)
{
    m_rtfb->modify(pixels, x, y, height, width);
}

void
rtw::RTWindow::displayBuffer(int16_t x,
                             int16_t y,
                             uint16_t width,
                             uint16_t height)
{
    xcb_image_put(m_connection, m_pxid, m_gcid, m_rtfb->getImage(), 0, 0, 0);
    xcb_copy_area(m_connection, m_pxid, m_xid, m_gcid, x, y, x, y, width, height);
    xcb_flush(m_connection);    // image doesn't display unless this is written in
}

void
rtw::RTWindow::drawPixel(int16_t x, 
                         int16_t y, 
                         uint16_t red, 
                         uint16_t green, 
                         uint16_t blue)
{
    // create correct struct setup
    const xcb_point_t points[] = { {x, y} };
    // allocate entry in color map (or retrieve existing entry)
    xcb_alloc_color_reply_t *reply = xcb_alloc_color_reply(m_connection,
                                                           xcb_alloc_color(m_connection,
                                                                           m_colormap,
                                                                           red << 8,
                                                                           green << 8,
                                                                           blue << 8),
                                                           NULL);
    // change graphics context color
    const uint32_t vlist = XCB_GC_FOREGROUND;
    const uint32_t vmask[] = { reply->pixel };
    xcb_change_gc(m_connection, m_gcid, vlist, vmask);
    // draw point
    xcb_poly_point(m_connection, XCB_COORD_MODE_ORIGIN, m_xid, m_gcid, 1, points);
}

#endif