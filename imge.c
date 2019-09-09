#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imge.h"

/* Allocate blank image from heap.
 */
Image *newImage(int sx, int sy)
{
  Image *im;
  im = (Image *)calloc(1, sizeof(Image));
  if (im != NULL) {
    im->sx = sx;
    im->sy = sy;
    im->data = (unsigned char *)calloc(sx * sy, sizeof(int));
    if (im->data != NULL){
    return (im);      
    } 
  }
  perror("Error: Unable to create new image\n");
  return (NULL);
}

/* Deallocate image
 */
void freeImage(Image *im)
{
  free(im->data);
  free(im);
}

/* Read image from PGM file (binary version). Space for the image is malloc'ed.
 */
Image *readPGMimage(const char *filename)
{
  FILE *f;
  Image *im;
  char line[1024], *tmp;
  int sizx, sizy, i;

  im = (Image *)calloc(1, sizeof(Image));
  if (im != NULL) {
    im->data = NULL;
    f = fopen(filename, "rb");
    if (f == NULL) {
      printf("Error: Unable to open file %s for reading, please check name and path\n", filename);
      free(im);
      return (NULL);
    }
    tmp = fgets(&line[0], 1000, f);
    if (strcmp(&line[0], "P5\n") != 0) {
      printf("Error: Wrong file format, not a .pgm file or header end-of-line characters missing\n");
      free(im);
      fclose(f);
      return (NULL);
    }
    // Skip over comments
    tmp = fgets(&line[0], 512, f);
    while (line[0] == '#'){
      tmp = fgets(&line[0], 512, f);
    } 
    sscanf(&line[0], "%d %d\n", &sizx, &sizy);  // Read file size
    im->sx = sizx;
    im->sy = sizy;

    tmp = fgets(&line[0], 9, f);  // Read the remaining header line
    im->data = (unsigned char *)calloc(sizx * sizy, sizeof(unsigned char));
    if (tmp == NULL) {
      printf("Error: Out of memory allocating space for image\n");
      free(im);
      fclose(f);
      return (NULL);
    }

    // Read the data
    i = fread(im->data, sizeof(unsigned char), sizx * sizy, f);
    fclose(f);
    return (im);
  }

  printf("Error: Unable to allocate memory for image\n");
  return (NULL);
}

/* Write image to PGM file (binary version.) */
void writePGMimage(Image *im, const char *filename)
{
  FILE *f;

  if (im != NULL) {
    if (im->data != NULL) {
      f = fopen(filename, "wb");
      if (f == NULL) {
        printf("Error: Unable to open %s for output!\n",filename);
        return;
      }
      fprintf(f, "P5\n");
      fprintf(f, "%d %d\n", im->sx, im->sy);
      fprintf(f, "255\n");
      fwrite(im->data, sizeof(unsigned char), im->sx * im->sy, f);
      fclose(f);
      return;
    }
    printf("Error: Specified image is empty. Nothing output\n");
  }
  printf("Error: image has no memory allocated\n");
}
