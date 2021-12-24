#include "app.h"

#define WIDTH 1920
#define HEIGHT 1080

int main() {
   App().run();
   //  rtw::RTWindow window = rtw::RTWindow(WIDTH, HEIGHT, NULL, -1);
   //  // for (int j = 0; j < 600; ++j) {
   //  //     for (int i = 0; i < 800; ++i) {
   //  //         window.drawPixel(i, j, rand()%256, rand()%256, rand()%256);
   //  //     }
   //  // }
   //  rtw::RTPixel pixels[WIDTH*HEIGHT];
   //  for (int rows = 0; rows < HEIGHT; ++rows) {
   //     for (int cols = 0; cols < WIDTH; ++cols) {
   //        pixels[WIDTH*rows+cols] = {255, 255, 255};
   //     }
   //  }
   //  printf("%d, %d, %d\n", pixels[0].red, pixels[0].green, pixels[0].blue);
   //  window.writeToBuffer(pixels, 0, 0, WIDTH, HEIGHT);
   //  window.displayBuffer(0, 0, WIDTH, HEIGHT);
   //  while (!window.shouldClose()) {
   //    for (int i = 0; i < 10000; ++i) {
   //       pixels[WIDTH*(rand()%HEIGHT) + rand()%WIDTH] = {(uint8_t)(rand()%256), (uint8_t)(rand()%256), (uint8_t)(rand()%256) };
   //    }
   //    window.writeToBuffer(pixels, 0, 0, WIDTH, HEIGHT);
   //    window.displayBuffer(0, 0, WIDTH, HEIGHT);
   //    window.pollEvents();
   //    usleep(10000);
   //  }
   // Model *teapot = load::createObj("./assets/utah_teapot.obj");
   // std::cout << (teapot->getVertex(0)).getX() << std::endl;
}