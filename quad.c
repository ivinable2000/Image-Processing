#include <stdlib.h>
#include <math.h>                   /* for lrint */
#include "quad.h"
#include "pgmio.h"

/* Reconstruct image from quadtree.  Write into the Image record that im points
 * to.  Assume that the square region described by the quadtree fits into the
 * Image.
 *
 * Recall that each Quad node already contains position information; respect it,
 * and you will benefit from it too (a secret for successful recursions).
 */

void rsaveQuad(Image *im, Quad *q, int w)
{
    if(q->width == 1)
    {
        im->data[(q->ty * w) + q->tx] = q->grayscale;
    }
    
    int counter = 0;
    while(counter < 4)
    {
        if(q->children[counter] != NULL)
        {
            rsaveQuad(im, q->children[counter], w);
            counter++;
        }
        else
        {       
            for(int i = 0; i < q->width; i++)
            {
                int x = (q->tx + i);
                for(int j = 0; j < q->width; j++)
                {
                    int y = (q->ty + j);
                    im->data[(y * w) + x] = q->grayscale;
                }
            }
            counter = 4;
        }
    }
        
    
}

void saveQuad(Image *im, Quad *q)
{
    //The helper fuction will remember the size of the entire matrix, since im->sx may be empty or junk/
    rsaveQuad(im, q, q->width);
}

int averageGrayscale(Image *im, int tx, int ty, int w)
{
    if(w == 1)
    {
        return im->data[(ty * im->sx) + tx];
    }

    int sum = 0;
    for (int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            sum = sum + averageGrayscale(im,(tx + (w/2) * i), (ty + (w/2) * j), w/2);
        }
    }
    
    return lrint(sum/4);

}

int maxGrayscaleDifference(Image *im, int threshold, int tx, int ty, int w)
{
    int max = im->data[(ty * im->sx) + tx];
    int min = max;
    for(int i = 0; i < w; i++)
    {
        int x = (tx + i);
        for(int j = 0; j < w; j++)
        {
            int y = (ty + j);
            if (im->data[(y * im->sx) + x] > max)
            {
                max = im->data[(y * im->sx) + x];
            }
            if (im->data[(y * im->sx) + x] < min)
            {
                min = im->data[(y * im->sx) + x];
            }
        }
    }
    return (max-min <= threshold) ? 0 : 1;    
}

/* Build quadtree from image.  Splitting stops when maxDepth hits 0 or maximum
 * difference in grayscale is <= threshold.  Do this to only the square region
 * at top-left corner (tx, ty) of width w.  Use heap allocation for all Quad
 * nodes you produce.
 */
Quad *formQuadtree(Image *im, int maxDepth, int threshold, int tx, int ty, int w)
{
    if (maxDepth == 0)
    {
        return NULL;
    }
    
    Quad *q;
    q = (Quad *)calloc(1, sizeof(Quad));
    if (q != NULL)
    {
        q->tx = tx;
        q->ty = ty;
        q->width = w;
    }

    if(w == 1)
    {
        q->grayscale = im->data[(ty * im->sx) + tx];
        return q;
    }
    int counter = 0;   
    for (int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            if(maxGrayscaleDifference(im, threshold, tx, ty, w) == 1){
                q->children[counter] = formQuadtree(im, maxDepth -1, threshold, (tx + (w/2) * j), (ty + (w/2) * i), w/2);
                counter++;
            }
            else
            {
                q->children[counter] = (Quad *)calloc(1, sizeof(Quad));
                if (q->children[counter] != NULL)
                {
                    q->children[counter]->tx = (tx + (w/2) * i);
                    q->children[counter]->ty = (ty + (w/2) * j);
                    q->children[counter]->width = w/2;
                    q->children[counter]->grayscale = averageGrayscale(im, (tx + (w/2) * i), (ty + (w/2) * j), w/2);
                    q->children[counter]->children[0]=NULL;
                    q->children[counter]->children[1]=NULL;
                    q->children[counter]->children[2]=NULL;
                    q->children[counter]->children[3]=NULL;
                }
                counter++;
            }
            
        }
    }
    return q;   
}

/* Deallocate all Quad nodes of the given quadtree. Assume that they all came
 * from the heap.
 */
void freeQuadtree(Quad *q)
{
    for(int i = 0; i < 4; i++)
    {
        if (q->children[i] != NULL)
        {
            freeQuadtree(q->children[i]);
        }
    }
    free(q);    
}
