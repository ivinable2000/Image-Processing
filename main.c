#include <stdio.h>
#include <stdlib.h>
#include "imge.h"
#include "quad.h"

/* Command line arguments:
 * input file, output file, threshold, max depth
 */
int main(int argc, char *argv[])
{
  if (argc < 5) {
    printf("Parameters: inputfile outputfile threshold maxdepth\n");
    return 1;
  }

  struct image *im = readPGMimage(argv[1]);

  Quad *q = formQuadtree(im, atoi(argv[4]), atoi(argv[3]), 0, 0, im->sx);
  saveQuad(im, q, q->width);
  writePGMimage(im, argv[2]);

  freeImage(im);
  freeQuadtree(q);

  return 0;
}
