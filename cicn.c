#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <fnmatch.h>
#include <netinet/in.h>
#include <gdk/gdk.h>

#include "macres.h"
#include "cicn.h"

static RGBColor rgb_8[256] = {
  { 0xff00, 0xff00, 0xff00 },
  { 0xff00, 0xfe00, 0xcb00 },
  { 0xff00, 0xfe00, 0x9a00 },
  { 0xff00, 0xfe00, 0x6600 },
  { 0xff00, 0xfe00, 0x3300 },
  { 0xfe00, 0xfe00, 0x0000 },
  { 0xff00, 0xcb00, 0xfe00 },
  { 0xff00, 0xcb00, 0xcb00 },
  { 0xff00, 0xcc00, 0x9a00 },
  { 0xff00, 0xcc00, 0x6600 },
  { 0xff00, 0xcc00, 0x3300 },
  { 0xfe00, 0xcb00, 0x0000 },
  { 0xff00, 0x9a00, 0xfe00 },
  { 0xff00, 0x9a00, 0xcc00 },
  { 0xff00, 0x9a00, 0x9a00 },
  { 0xff00, 0x9900, 0x6600 },
  { 0xff00, 0x9900, 0x3300 },
  { 0xfe00, 0x9800, 0x0000 },
  { 0xff00, 0x6600, 0xfe00 },
  { 0xff00, 0x6600, 0xcc00 },
  { 0xff00, 0x6600, 0x9900 },
  { 0xff00, 0x6600, 0x6600 },
  { 0xff00, 0x6600, 0x3300 },
  { 0xfe00, 0x6500, 0x0000 },
  { 0xff00, 0x3300, 0xfe00 },
  { 0xff00, 0x3300, 0xcc00 },
  { 0xff00, 0x3300, 0x9900 },
  { 0xff00, 0x3300, 0x6600 },
  { 0xff00, 0x3300, 0x3300 },
  { 0xfe00, 0x3200, 0x0000 },
  { 0xfe00, 0x0000, 0xfe00 },
  { 0xfe00, 0x0000, 0xcb00 },
  { 0xfe00, 0x0000, 0x9800 },
  { 0xfe00, 0x0000, 0x6500 },
  { 0xfe00, 0x0000, 0x3200 },
  { 0xfe00, 0x0000, 0x0000 },
  { 0xcb00, 0xff00, 0xff00 },
  { 0xcb00, 0xff00, 0xcb00 },
  { 0xcc00, 0xff00, 0x9a00 },
  { 0xcc00, 0xff00, 0x6600 },
  { 0xcc00, 0xff00, 0x3300 },
  { 0xcb00, 0xfe00, 0x0000 },
  { 0xcb00, 0xcb00, 0xff00 },
  { 0xcc00, 0xcc00, 0xcc00 },
  { 0xcc00, 0xcc00, 0x9900 },
  { 0xcc00, 0xcc00, 0x6600 },
  { 0xcb00, 0xcb00, 0x3200 },
  { 0xcd00, 0xcd00, 0x0000 },
  { 0xcc00, 0x9a00, 0xff00 },
  { 0xcc00, 0x9900, 0xcc00 },
  { 0xcc00, 0x9900, 0x9900 },
  { 0xcc00, 0x9900, 0x6600 },
  { 0xcb00, 0x9800, 0x3200 },
  { 0xcd00, 0x9a00, 0x0000 },
  { 0xcc00, 0x6600, 0xff00 },
  { 0xcc00, 0x6600, 0xcc00 },
  { 0xcc00, 0x6600, 0x9900 },
  { 0xcc00, 0x6600, 0x6600 },
  { 0xcb00, 0x6500, 0x3200 },
  { 0xcd00, 0x6600, 0x0000 },
  { 0xcc00, 0x3300, 0xff00 },
  { 0xcb00, 0x3200, 0xcb00 },
  { 0xcb00, 0x3200, 0x9800 },
  { 0xcb00, 0x3200, 0x6500 },
  { 0xcb00, 0x3200, 0x3200 },
  { 0xcd00, 0x3300, 0x0000 },
  { 0xcb00, 0x0000, 0xfe00 },
  { 0xcd00, 0x0000, 0xcd00 },
  { 0xcd00, 0x0000, 0x9a00 },
  { 0xcd00, 0x0000, 0x6600 },
  { 0xcd00, 0x0000, 0x3300 },
  { 0xcd00, 0x0000, 0x0000 },
  { 0x9a00, 0xff00, 0xff00 },
  { 0x9a00, 0xff00, 0xcc00 },
  { 0x9a00, 0xff00, 0x9a00 },
  { 0x9900, 0xff00, 0x6600 },
  { 0x9900, 0xff00, 0x3300 },
  { 0x9900, 0xfe00, 0x0000 },
  { 0x9a00, 0xcc00, 0xff00 },
  { 0x9900, 0xcc00, 0xcc00 },
  { 0x0000, 0x9800, 0x6500 },
  { 0x9900, 0xcc00, 0x6600 },
  { 0x9900, 0xcb00, 0x3200 },
  { 0x9a00, 0xcd00, 0x0000 },
  { 0x9a00, 0x9a00, 0xff00 },
  { 0x9900, 0x9900, 0xcc00 },
  { 0x9900, 0x9900, 0x9900 },
  { 0x9800, 0x9800, 0x6500 },
  { 0x9a00, 0x9a00, 0x3300 },
  { 0x9800, 0x9800, 0x0000 },
  { 0x9900, 0x6600, 0xff00 },
  { 0x9900, 0x6600, 0xcc00 },
  { 0x9800, 0x6500, 0x9800 },
  { 0x9800, 0x6500, 0x6500 },
  { 0x9a00, 0x6600, 0x3300 },
  { 0x9800, 0x6500, 0x0000 },
  { 0x9900, 0x3300, 0xff00 },
  { 0x9800, 0x3200, 0xcb00 },
  { 0x9a00, 0x3300, 0x9a00 },
  { 0x9a00, 0x3300, 0x6600 },
  { 0x9a00, 0x3300, 0x3300 },
  { 0x9800, 0x3200, 0x0000 },
  { 0x9800, 0x0000, 0xfe00 },
  { 0x9a00, 0x0000, 0xcd00 },
  { 0x9800, 0x0000, 0x9800 },
  { 0x9800, 0x0000, 0x6500 },
  { 0x9800, 0x0000, 0x3200 },
  { 0x9800, 0x0000, 0x0000 },
  { 0x6600, 0xff00, 0xff00 },
  { 0x6600, 0xff00, 0xcc00 },
  { 0x6600, 0xff00, 0x9900 },
  { 0x6600, 0xff00, 0x6600 },
  { 0x6600, 0xff00, 0x3300 },
  { 0x6600, 0xfe00, 0x0000 },
  { 0x6600, 0xcc00, 0xff00 },
  { 0x6600, 0xcc00, 0xcc00 },
  { 0x6600, 0xcc00, 0x9900 },
  { 0x6600, 0xcc00, 0x6600 },
  { 0x6600, 0xcb00, 0x3200 },
  { 0x6600, 0xcd00, 0x0000 },
  { 0x6600, 0x9900, 0xff00 },
  { 0x6600, 0x9900, 0xcc00 },
  { 0x6500, 0x9800, 0x9800 },
  { 0x6500, 0x9800, 0x6500 },
  { 0x6600, 0x9a00, 0x3300 },
  { 0x6500, 0x9800, 0x0000 },
  { 0x6600, 0x6600, 0xff00 },
  { 0x6600, 0x6600, 0xcc00 },
  { 0x6500, 0x6500, 0x9800 },
  { 0x6600, 0x6600, 0x6600 },
  { 0x6500, 0x6500, 0x3200 },
  { 0x6600, 0x6600, 0x0000 },
  { 0x6600, 0x3300, 0xff00 },
  { 0x6500, 0x3200, 0xcb00 },
  { 0x6600, 0x3300, 0x9a00 },
  { 0x6500, 0x3200, 0x6500 },
  { 0x6500, 0x3200, 0x3200 },
  { 0x6600, 0x3300, 0x0000 },
  { 0x6500, 0x0000, 0xfe00 },
  { 0x6600, 0x0000, 0xcd00 },
  { 0x6500, 0x0000, 0x9800 },
  { 0x6600, 0x0000, 0x6600 },
  { 0x6600, 0x0000, 0x3300 },
  { 0x6600, 0x0000, 0x0000 },
  { 0x3300, 0xff00, 0xff00 },
  { 0x3300, 0xff00, 0xcc00 },
  { 0x3300, 0xff00, 0x9900 },
  { 0x3300, 0xff00, 0x6600 },
  { 0x3300, 0xff00, 0x3300 },
  { 0x3300, 0xfe00, 0x0000 },
  { 0x3300, 0xcc00, 0xff00 },
  { 0x3200, 0xcb00, 0xcb00 },
  { 0x3200, 0xcb00, 0x9800 },
  { 0x3200, 0xcb00, 0x6500 },
  { 0x3300, 0xcb00, 0x3200 },
  { 0x3300, 0xcd00, 0x0000 },
  { 0x3300, 0x9900, 0xff00 },
  { 0x3200, 0x9900, 0xcb00 },
  { 0x3300, 0x9a00, 0x9a00 },
  { 0x3300, 0x9a00, 0x6600 },
  { 0x3300, 0x9a00, 0x3300 },
  { 0x3200, 0x9800, 0x0000 },
  { 0x3300, 0x6600, 0xff00 },
  { 0x3200, 0x6600, 0xcb00 },
  { 0x3300, 0x6600, 0x9a00 },
  { 0x3200, 0x6500, 0x6500 },
  { 0x3200, 0x6500, 0x3200 },
  { 0x3300, 0x6600, 0x0000 },
  { 0x3300, 0x3300, 0xff00 },
  { 0x3200, 0x3300, 0xcb00 },
  { 0x3300, 0x3300, 0x9a00 },
  { 0x3200, 0x3200, 0x6500 },
  { 0x3300, 0x3300, 0x3300 },
  { 0x3300, 0x3300, 0x0000 },
  { 0x3200, 0x0000, 0xfe00 },
  { 0x3300, 0x0000, 0xcd00 },
  { 0x3200, 0x0000, 0x9800 },
  { 0x3300, 0x0000, 0x6600 },
  { 0x3300, 0x0000, 0x3300 },
  { 0x3300, 0x0000, 0x0000 },
  { 0x0000, 0xfe00, 0xfe00 },
  { 0x0000, 0xfe00, 0xcb00 },
  { 0x0000, 0xfe00, 0x9800 },
  { 0x0000, 0xfe00, 0x6500 },
  { 0x0000, 0xfe00, 0x3200 },
  { 0x0000, 0xfe00, 0x0000 },
  { 0x0000, 0xcb00, 0xfe00 },
  { 0x0000, 0xcd00, 0xcd00 },
  { 0x0000, 0xcd00, 0x9a00 },
  { 0x0000, 0xcd00, 0x6600 },
  { 0x0000, 0xcd00, 0x3300 },
  { 0x0000, 0xcd00, 0x0000 },
  { 0x0000, 0x9800, 0xfe00 },
  { 0x0000, 0x9a00, 0xcd00 },
  { 0x0000, 0x9800, 0x9800 },
  { 0x0000, 0x9800, 0x6500 },
  { 0x0000, 0x9800, 0x3200 },
  { 0x0000, 0x9800, 0x0000 },
  { 0x0000, 0x6600, 0xfe00 },
  { 0x0000, 0x6600, 0xcd00 },
  { 0x0000, 0x6500, 0x9800 },
  { 0x0000, 0x6600, 0x6600 },
  { 0x0000, 0x6600, 0x3300 },
  { 0x0000, 0x6600, 0x0000 },
  { 0x0000, 0x3300, 0xfe00 },
  { 0x0000, 0x3300, 0xcd00 },
  { 0x0000, 0x3200, 0x9800 },
  { 0x0000, 0x3300, 0x6600 },
  { 0x0000, 0x3300, 0x3300 },
  { 0x0000, 0x3300, 0x0000 },
  { 0x0000, 0x0000, 0xfe00 },
  { 0x0000, 0x0000, 0xcd00 },
  { 0x0000, 0x0000, 0x9800 },
  { 0x0000, 0x0000, 0x6600 },
  { 0x0000, 0x0000, 0x3300 },
  { 0xef00, 0x0000, 0x0000 },
  { 0xdc00, 0x0000, 0x0000 },
  { 0xba00, 0x0000, 0x0000 },
  { 0xab00, 0x0000, 0x0000 },
  { 0x8900, 0x0000, 0x0000 },
  { 0x7700, 0x0000, 0x0000 },
  { 0x5500, 0x0000, 0x0000 },
  { 0x4400, 0x0000, 0x0000 },
  { 0x2200, 0x0000, 0x0000 },
  { 0x1100, 0x0000, 0x0000 },
  { 0x0000, 0xef00, 0x0000 },
  { 0x0000, 0xdc00, 0x0000 },
  { 0x0000, 0xba00, 0x0000 },
  { 0x0000, 0xab00, 0x0000 },
  { 0x0000, 0x8900, 0x0000 },
  { 0x0000, 0x7700, 0x0000 },
  { 0x0000, 0x5500, 0x0000 },
  { 0x0000, 0x4400, 0x0000 },
  { 0x0000, 0x2200, 0x0000 },
  { 0x0000, 0x1100, 0x0000 },
  { 0x0000, 0x0000, 0xef00 },
  { 0x0000, 0x0000, 0xdc00 },
  { 0x0000, 0x0000, 0xba00 },
  { 0x0000, 0x0000, 0xab00 },
  { 0x0000, 0x0000, 0x8900 },
  { 0x0000, 0x0000, 0x7700 },
  { 0x0000, 0x0000, 0x5500 },
  { 0x0000, 0x0000, 0x4400 },
  { 0x0000, 0x0000, 0x2200 },
  { 0x0000, 0x0000, 0x1100 },
  { 0xee00, 0xee00, 0xee00 },
  { 0xdd00, 0xdd00, 0xdd00 },
  { 0xbb00, 0xbb00, 0xbb00 },
  { 0xaa00, 0xaa00, 0xaa00 },
  { 0x8800, 0x8800, 0x8800 },
  { 0x7700, 0x7700, 0x7700 },
  { 0x5500, 0x5500, 0x5500 },
  { 0x4400, 0x4400, 0x4400 },
  { 0x2200, 0x2200, 0x2200 },
  { 0x1100, 0x1100, 0x1100 },
  { 0x0000, 0x0000, 0x0000 }
};

static RGBColor rgb_4[16] = {
  { 0xffff, 0xffff, 0xffff },
  { 0xffff, 0xffff, 0x0000 },
  { 0xffff, 0xa0a0, 0x7a7a },
  { 0xffff, 0x0000, 0x0000 },
  { 0xffff, 0x1414, 0x9393 },
  { 0x8a8a, 0x2b2b, 0xe2e2 },
  { 0x0000, 0x0000, 0x8080 },
  { 0x6464, 0x9595, 0xeded },
  { 0x2222, 0x8b8b, 0x2222 },
  { 0x0000, 0x6464, 0x0000 },
  { 0x8b8b, 0x4545, 0x1313 },
  { 0xd2d2, 0xb4b4, 0x8c8c },
  { 0xd3d3, 0xd3d3, 0xd3d3 },
  { 0xbebe, 0xbebe, 0xbebe },
  { 0x6969, 0x6969, 0x6969 },
  { 0x0000, 0x0000, 0x0000 }
};

static RGBColor rgb_2[4] = {
  { 0xffff, 0xffff, 0xffff },
  { 0xffff, 0xffff, 0x0000 },
  { 0x0000, 0xffff, 0xffff },
  { 0x0000, 0x0000, 0x0000 }
};

static RGBColor rgb_1[2] = {
  { 0xffff, 0xffff, 0xffff },
  { 0x0000, 0x0000, 0x0000 }
};

void
palette_get_rgb_8 (unsigned int *palette)
{
  unsigned int i;

  for (i = 0; i < 256; i++) {
    palette[i] = ((rgb_8[i].blue >> 8) << 16)
                 | ((rgb_8[i].green >> 8) << 8)
                 | ((rgb_8[i].red >> 8) << 0)
                 | 0xff000000;
  }
}

static unsigned int cmap_8[256], cmap_4[16], cmap_2[4], cmap_1[2];
#if WHITE_CLEAR
  static unsigned int white = 0;
#endif

static unsigned int
rgb_to_pixel (GdkColormap *colormap, RGBColor *rgb)
{
  GdkColor col;

  col.pixel = 0;
  /* col.red = rgb->red;
     col.green = rgb->green;
     col.blue = rgb->blue; */
  col.red = (rgb->red & 0xff) | ((rgb->red & 0xff) << 8);
  col.green = (rgb->green & 0xff) | ((rgb->green & 0xff) << 8);
  col.blue = (rgb->blue & 0xff) | ((rgb->blue & 0xff) << 8);

  if (!gdk_colormap_alloc_color(colormap, &col, 0, 1))
    fprintf(stderr, "rgb_to_pixel: can't allocate %u/%u/%u\n",
            rgb->red, rgb->green, rgb->blue);

  return col.pixel;
}

GdkImage *
cicn_to_gdkimage (GdkColormap *colormap, GdkVisual *visual,
                  void *cicn_rsrc, unsigned int len, GdkImage **maskimp)
{
  GdkImage *gim;
  GdkImage *mim;
  PixMap *pm = (PixMap *)((unsigned char *)cicn_rsrc);
  BitMap *mbm = (BitMap *)((unsigned char *)cicn_rsrc + 50);
  BitMap *bm = (BitMap *)((unsigned char *)cicn_rsrc + 64);
  unsigned int height = ntohs(pm->bounds.bottom) - ntohs(pm->bounds.top);
  unsigned int width = ntohs(pm->bounds.right) - ntohs(pm->bounds.left);
  unsigned int bpp = ntohs(pm->pixelSize);
  ColorTable *ct = (ColorTable *)
                   ((unsigned char *)cicn_rsrc
                    + 50 + 14 + 14 + 4
                    + (ntohs(mbm->rowBytes) * (ntohs(mbm->bounds.bottom) - ntohs(mbm->bounds.top)))
                    + (ntohs(bm->rowBytes) * (ntohs(bm->bounds.bottom) - ntohs(bm->bounds.top))));
  unsigned int i, x, y, rowBytes, ctsize;
  unsigned int idlen;
  unsigned char *base_id, *id;
  unsigned char maski, maskp, *maskfb;
  unsigned int maskfb_size;

  #if WHITE_CLEAR
  if (!white) {
    white = rgb_to_pixel(colormap, &rgb_8[0]);
  }
  #endif

  rowBytes = ntohs(pm->rowBytes) & 0x7fff;
  idlen = height * rowBytes;
  switch (bpp) {
    case 8:
      for (i = 0; i < 256; i++) {
        cmap_8[i] = 257;
      }
      break;
    case 4:
      for (i = 0; i < 16; i++) {
        cmap_4[i] = 17;
      }
      break;
    case 2:
      for (i = 0; i < 4; i++) {
        cmap_2[i] = 5;
      }
      break;
    case 1:
      for (i = 0; i < 2; i++) {
        cmap_1[i] = 2;
      }
      break;
    default:
      return 0;
  }
  base_id = (((unsigned char *)cicn_rsrc + len) - idlen);

  if (mbm->bounds.right == 0 || mbm->bounds.bottom == 0) {
    maskfb = 0;
  } else {
    unsigned char *md = ((unsigned char *)cicn_rsrc) + 82;
    unsigned int mrb = ntohs(mbm->rowBytes);
    unsigned int w3 = (width >> 3) + ((width % 8) ? 1 : 0);

    maskfb_size = ((((w3<<3) * height) / 8) + 1);
    maskfb = malloc(maskfb_size);
    if (!maskfb) {
      return 0;
    }
    for (y = 0; y < height; y++) {
      id = md + mrb * y;
      for (x = 0; x < w3; x++) {
        maskfb[y * w3 + x] = ~(*id);
        id++;
      }
    }
  }

  gim = gdk_image_new(GDK_IMAGE_FASTEST, visual, width, height);
  if (!gim) {
    return 0;
  }

  ctsize = ntohs(ct->ctSize) + 1;
  for (i = 0; i < ctsize; i++) {
    unsigned int v = ntohs(ct->ctTable[i].value);
    if (bpp == 8) {
      cmap_8[v&0xff] = rgb_to_pixel(colormap, &ct->ctTable[i].rgb);
    } else if (bpp == 4) {
      cmap_4[v&0xf] = rgb_to_pixel(colormap, &ct->ctTable[i].rgb);
    } else if (bpp == 2) {
      cmap_2[v&0x3] = rgb_to_pixel(colormap, &ct->ctTable[i].rgb);
    } else if (bpp == 1) {
      cmap_1[v&0x1] = rgb_to_pixel(colormap, &ct->ctTable[i].rgb);
    }
  }

  if (bpp == 1) {
    for (y = 0; y < height; y++) {
      id = base_id + rowBytes * y;
      for (x = 0; x < width; x++) {
        unsigned int i, j, k, l, m, n, o, p;
        i = *id >> 7;
        j = (*id & 0x40) >> 6;
        k = (*id & 0x20) >> 5;
        l = (*id & 0x10) >> 4;
        m = (*id & 0x8) >> 3;
        n = (*id & 0x4) >> 2;
        o = (*id & 0x2) >> 1;
        p = *id & 0x1;
        if (cmap_1[i] == 2) {
          cmap_1[i] = rgb_to_pixel(colormap, &rgb_1[i]);
        }
        if (cmap_1[j] == 2) {
          cmap_1[j] = rgb_to_pixel(colormap, &rgb_1[j]);
        }
        if (cmap_1[k] == 2) {
          cmap_1[k] = rgb_to_pixel(colormap, &rgb_1[k]);
        }
        if (cmap_1[l] == 2) {
          cmap_1[l] = rgb_to_pixel(colormap, &rgb_1[l]);
        }
        if (cmap_1[m] == 2) {
          cmap_1[m] = rgb_to_pixel(colormap, &rgb_1[m]);
        }
        if (cmap_1[n] == 2) {
          cmap_1[n] = rgb_to_pixel(colormap, &rgb_1[n]);
        }
        if (cmap_1[o] == 2) {
          cmap_1[o] = rgb_to_pixel(colormap, &rgb_1[o]);
        }
        if (cmap_1[p] == 2) {
          cmap_1[p] = rgb_to_pixel(colormap, &rgb_1[p]);
        }
        gdk_image_put_pixel(gim, x++, y, cmap_1[i]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[j]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[k]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[l]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[m]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[n]);
        gdk_image_put_pixel(gim, x++, y, cmap_1[o]);
        gdk_image_put_pixel(gim, x, y, cmap_1[p]);
        #if WHITE_CLEAR
        if (maskfb) {
          maskp = ((cmap_1[i]!=white)?0:1<<7)
                  | ((cmap_1[j]!=white)?0:1<<6)
                  | ((cmap_1[k]!=white)?0:1<<5)
                  | ((cmap_1[l]!=white)?0:1<<4)
                  | ((cmap_1[m]!=white)?0:1<<3)
                  | ((cmap_1[n]!=white)?0:1<<2)
                  | ((cmap_1[o]!=white)?0:1<<1)
                  | ((cmap_1[p]!=white)?0:1);
          maskfb[y * (width>>3) + (x>>3)] |= maskp;
        }
        #endif
        id++;
      }
    }
  } else if (bpp == 2) {
    for (y = 0; y < height; y++) {
      id = base_id + rowBytes * y;
      maski = 7;
      maskp = 0;
      for (x = 0; x < width; x++) {
        unsigned int i = *id >> 6;
        unsigned int j = (*id & 0x30) >> 4;
        unsigned int k = (*id & 0xc) >> 2;
        unsigned int l = *id & 0x3;
        if (cmap_2[i] == 5) {
          cmap_2[i] = rgb_to_pixel(colormap, &rgb_2[i]);
        }
        if (cmap_2[j] == 5) {
          cmap_2[j] = rgb_to_pixel(colormap, &rgb_2[j]);
        }
        if (cmap_2[k] == 5) {
          cmap_2[k] = rgb_to_pixel(colormap, &rgb_2[k]);
        }
        if (cmap_2[l] == 5) {
          cmap_2[l] = rgb_to_pixel(colormap, &rgb_2[l]);
        }
        gdk_image_put_pixel(gim, x++, y, cmap_2[i]);
        gdk_image_put_pixel(gim, x++, y, cmap_2[j]);
        gdk_image_put_pixel(gim, x++, y, cmap_2[k]);
        gdk_image_put_pixel(gim, x, y, cmap_2[l]);
        #if WHITE_CLEAR
        if (maskfb) {
          maskp |= ((cmap_2[i]!=white)?0:1<<maski)
                   | ((cmap_2[j]!=white)?0:1<<(maski-1))
                   | ((cmap_2[k]!=white)?0:1<<(maski-2))
                   | ((cmap_2[l]!=white)?0:1<<(maski-3));
          if (maski == 3) {
            maskfb[y * (width>>3) + (x>>3)] |= maskp;
            maski = 7;
            maskp = 0;
          } else {
            maski = 3;
          }
        }
        #endif
        id++;
      }
    }
  } else if (bpp == 4) {
    for (y = 0; y < height; y++) {
      id = base_id + rowBytes * y;
      maski = 7;
      maskp = 0;
      for (x = 0; x < width; x++) {
        unsigned int i = *id >> 4;
        unsigned int j = *id & 0xf;
        if (cmap_4[i] == 17) {
          cmap_4[i] = rgb_to_pixel(colormap, &rgb_4[i]);
        }
        if (cmap_4[j] == 17) {
          cmap_4[j] = rgb_to_pixel(colormap, &rgb_4[j]);
        }
        gdk_image_put_pixel(gim, x++, y, cmap_4[i]);
        gdk_image_put_pixel(gim, x, y, cmap_4[j]);
        #if WHITE_CLEAR
        if (maskfb) {
          maskp |= ((cmap_4[i]!=white)?0:1<<maski)
                   | ((cmap_4[j]!=white)?0:1<<(maski-1));
          if (maski == 1) {
            maskfb[y * (width>>3) + (x>>3)] |= maskp;
            maski = 7;
            maskp = 0;
          } else {
            maski -= 2;
          }
        }
        #endif
        id++;
      }
    }
  } else if (bpp == 8) {
    for (y = 0; y < height; y++) {
      id = base_id + rowBytes * y;
      maski = 7;
      maskp = 0;
      for (x = 0; x < width; x++) {
        unsigned int i = *id++;
        if (cmap_8[i] == 257) {
          cmap_8[i] = rgb_to_pixel(colormap, &rgb_8[i]);
        }
        gdk_image_put_pixel(gim, x, y, cmap_8[i]);
        #if WHITE_CLEAR
        if (maskfb) {
          maskp |= ((cmap_8[i]!=white)?0:1<<maski);
          if (!maski) {
            maskfb[y * (width>>3) + (x>>3)] |= maskp;
            maski = 7;
            maskp = 0;
          } else {
            maski--;
          }
        }
        #endif
      }
    }
  }

  if (maskimp) {
    if (!maskfb) {
      mim = 0;
    } else {
      mim = gdk_image_new_bitmap(visual, maskfb, width, height);
    }
    *maskimp = mim;
  }

  return gim;
}
