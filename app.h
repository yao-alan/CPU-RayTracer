#ifndef APP_H
#define APP_H

#include "windowing.h"
#include "vmath.h"
#include "model.h"
#include "loader.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#define WIDTH 1920
#define HEIGHT 1080

class App
{
    public:
        // main functions to edit
        void onCreate();
        void onUpdate();

        void run() {
            rtw::RTWindow window = rtw::RTWindow(WIDTH, HEIGHT, NULL, -1);
            pixels = (rtw::RTPixel *)malloc(WIDTH*HEIGHT);

            onCreate();
            while(!window.shouldClose()) {
                window.pollEvents();
                onUpdate();
            }

            free(pixels);
        }
    private:
        rtw::RTPixel *pixels;
};

#endif