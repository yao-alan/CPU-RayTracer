#include "windowing.h"
#include "vmath.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#define WIDTH 1920
#define HEIGHT 1080

int main() {
    rtw::RTWindow window = rtw::RTWindow(WIDTH, HEIGHT, NULL, -1);
    // for (int j = 0; j < 600; ++j) {
    //     for (int i = 0; i < 800; ++i) {
    //         window.drawPixel(i, j, rand()%256, rand()%256, rand()%256);
    //     }
    // }
    rtw::RTPixel pixels[WIDTH*HEIGHT];
    for (int rows = 0; rows < HEIGHT; ++rows) {
       for (int cols = 0; cols < WIDTH; ++cols) {
          pixels[WIDTH*rows+cols] = {255, 255, 255};
       }
    }
    printf("%d, %d, %d\n", pixels[0].red, pixels[0].green, pixels[0].blue);
    window.writeToBuffer(pixels, 0, 0, WIDTH, HEIGHT);
    window.displayBuffer(0, 0, WIDTH, HEIGHT);
    while (!window.shouldClose()) {
      for (int i = 0; i < 10000; ++i) {
         pixels[WIDTH*(rand()%HEIGHT) + rand()%WIDTH] = {(uint8_t)(rand()%256), (uint8_t)(rand()%256), (uint8_t)(rand()%256) };
      }
      window.writeToBuffer(pixels, 0, 0, WIDTH, HEIGHT);
      window.displayBuffer(0, 0, WIDTH, HEIGHT);
      window.pollEvents();
      usleep(10000);
    }
}

// #include <string.h>

// #include <xcb/xcb.h>
// #include <xcb/xcb_image.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>

// static xcb_format_t *
// find_format (xcb_connection_t * c, uint8_t depth, uint8_t bpp)
// {
//  const xcb_setup_t *setup = xcb_get_setup(c);
//  xcb_format_t *fmt = xcb_setup_pixmap_formats(setup);
//  xcb_format_t *fmtend = fmt + xcb_setup_pixmap_formats_length(setup);
//  for(; fmt != fmtend; ++fmt)
//   if((fmt->depth == depth) && (fmt->bits_per_pixel == bpp)) {
//    printf("fmt %p has pad %d depth %d, bpp %d\n",
//       fmt,fmt->scanline_pad, depth,bpp);
//    return fmt;
//   }
//  return 0;
// }

// void
// fillimage(unsigned char *p, int width, int height)
// {
//  int i, j;
//  for(i=0; i < width; i++)
//  {
//   for(j=0; j < height; j++)
//   {
//    if((i < 256)&&(j < 256))
//    {
//     *p++=rand()%256; // blue
//     *p++=rand()%256; // green
//     *p++=rand()%256; // red
//    } else {
//     *p++=i%256; // blue
//     *p++=j%256; // green
//     if(i < 256)
//      *p++=i%256; // red
//     else if(j < 256)
//      *p++=j%256; // red
//     else
//      *p++=(256-j)%256; // red
//    }
//    p++; /* unused byte */
//   }
//  }
// }

// xcb_image_t *
// CreateTrueColorImage(xcb_connection_t *c,
//        int width,
//        int height)
// {
//  const xcb_setup_t *setup = xcb_get_setup(c);
//  unsigned char *image32=(unsigned char *)malloc(width*height*4);
//  xcb_format_t *fmt = find_format(c, 24, 32);
//  if (fmt == NULL)
//   return NULL;

//  fillimage(image32, width, height);

//  return xcb_image_create(width,
//     height,
//     XCB_IMAGE_FORMAT_Z_PIXMAP,
//     32,
//     24,
//     32,
//     0,
//     XCB_IMAGE_ORDER_MSB_FIRST,
//     XCB_IMAGE_ORDER_LSB_FIRST,
//     image32,
//     width*height*4,
//     image32);
// }

// int
// main (int argc, char **argv)
// {
//  xcb_connection_t *c;
//  xcb_screen_t *s;
//  xcb_window_t w;
//  xcb_pixmap_t pmap;
//  xcb_gcontext_t gc;
//  xcb_generic_event_t *e;
//  uint32_t mask;
//  uint32_t values[2];
//  int done=0;
//  xcb_image_t *image;
//  uint8_t *image32;
//  xcb_expose_event_t *ee;
//  char *title="Hello World!";

//  /* open connection with the server */
//  c = xcb_connect (NULL, NULL);

//  if (!c) {
//   printf ("Cannot open display\n");
//   exit (1);
//  }

//  s = xcb_setup_roots_iterator (xcb_get_setup (c)).data;

//  /* printf("root depth %d\n",s->root_depth); */

//  /* create image */
//  image = CreateTrueColorImage(c, 640, 480);
//  if (image == NULL) {
//   printf ("Cannot create iamge\n");
//   xcb_disconnect(c);
//   return 1;
//  }
//  image32 = image->data;
//  printf("%d %d %d %d\n", image32[0], image32[1], image32[2], image32[3]);

//  /* create window */
//  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
//  values[0] = s->black_pixel;
//  values[1] = XCB_EVENT_MASK_EXPOSURE |
//   XCB_EVENT_MASK_KEY_PRESS |
//   XCB_EVENT_MASK_BUTTON_PRESS;

//  w = xcb_generate_id (c);
//  xcb_create_window (c, XCB_COPY_FROM_PARENT, w, s->root,
//       10, 10, image->width, image->height, 1,
//       XCB_WINDOW_CLASS_INPUT_OUTPUT,
//       s->root_visual,
//       mask, values);

//  /* create backing pixmap */
//  pmap = xcb_generate_id(c);
//  xcb_create_pixmap(c, 24, pmap, w, image->width, image->height);

//  /* create pixmap plot gc */
//  mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
//  values[0] = s->black_pixel;
//  values[1] = 0xffffff;

//  gc = xcb_generate_id (c);
//  xcb_create_gc (c, gc, pmap, mask, values);

//  /* show the window */
//  xcb_map_window (c, w);

//   /* put the image into the pixmap */
//  xcb_image_put(c, pmap, gc, image, 0, 0, 0);
//  xcb_copy_area(c, pmap, w, gc, 0, 0, 0, 0, 640, 480);

//  /* event loop */
//  while (!done && (e = xcb_wait_for_event (c))) {
//   switch (e->response_type) {
//   case XCB_KEY_PRESS:
//    /* exit on keypress */
//    done = 1;
//    break;
//   }
//   xcb_flush(c);
//   free (e);
//  }

//  /* free pixmap */
//  xcb_free_pixmap(c, pmap);

//  /* close connection to server */
//  xcb_disconnect (c);

//  return 0;
// }