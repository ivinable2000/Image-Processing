#ifndef QUAD_H
#define QUAD_H

#include "imge.h"

/* A node in a quadtree. */
typedef struct quad {
  int tx, ty;  // (x,y) coordinates of the top-left pixel in this quad.
  int width;   // How many pixels wide this quad is.

  // 4 pointers to the 4 children of this node, in this order: top left, top
  // right, bottom left, bottom right.  Either all 4 should be NULL (when this
  // is a leaf node) or all 4 should be non-NULL.
  struct quad *children[4];

  // The rounded average grayscale of this quad.  Round to nearest; if n.5,
  // round to even.  
  // This field is ignored and unfilled if there are children.
  unsigned char grayscale;
} Quad;

/* Reconstruct image from quadtree.  Write into the Image record that im points
 * to.  Assume that the square region described by the quadtree fits into the
 * Image.
 */
void saveQuad(Image *im, Quad *q, int width);

/* Build quadtree from image.  Splitting stops when maxDepth hits 0 or maximum
 * difference in grayscale is <= threshold.  Do this to only the square region
 * at top-left corner (tx, ty) of width w.  
 */
Quad *formQuadtree(Image *im, int maxDepth, int threshold, int tx, int ty, int w);

/* Deallocate all Quad nodes of the given quadtree. Assume that they all came
 * from the heap.
 */
void freeQuadtree(Quad *q);

#endif  /* ifndef QUAD_H */
