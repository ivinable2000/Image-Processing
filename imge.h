#ifndef IMGE_H
#define IMGE_H

typedef struct image {
  // Pointer to pixel grayscale.  Row-major order.
  // unsigned char on x86-64 is 0-255
  unsigned char *data;

  // Width and height of the image, which will be the same since images will be square
  int sx;
  int sy;                       
} Image;

/* Allocate blank image from heap.
 */
Image *newImage(int sx, int sy);

/* Deallocate image
 */
void freeImage(Image *im);

/* Read image from PGM file (binary version). Space for the image is malloc'ed.
 */
Image *readPGMimage(const char *filename);

/* Write image to PGM file (binary version. */
void writePGMimage(Image *im, const char *filename);

#endif  /* ifndef PGMIO_H */
