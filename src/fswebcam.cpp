#include "fastcam/config.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <gd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fastcam/fswebcam.h"
#include "fastcam/log.h"
#include "fastcam/src.h"
#include "fastcam/dec.h"
#include "fastcam/parse.h"
#include "fastcam/dec_jpeg.h"

gdImage* fswc_gdImageDuplicate(gdImage* src)
{
    gdImage *dst;
    
    dst = gdImageCreateTrueColor(gdImageSX(src), gdImageSY(src));
    if(!dst) return(NULL);
    
    gdImageCopy(dst, src, 0, 0, 0, 0, gdImageSX(src), gdImageSY(src));
    
    return(dst);
}
int fswc_output(char *name, gdImage *image)
{
    char filename[FILENAME_MAX];
    gdImage *im;
    FILE *f;
    
    if(!name) return(-1);   
    
    /* Create a temporary image buffer. */
    im = fswc_gdImageDuplicate(image);
    if(!im)
    {
        //ERROR("Out of memory.");
        return(-1);
    }
    
    f = fopen(name, "wb");
    
    if(!f)
    {
        //ERROR("Error opening file for output: %s", name);
        //ERROR("fopen: %s", strerror(errno));
        gdImageDestroy(im);
        return(-1);
    }    

    //MSG("Writing JPEG image to '%s'.", name);
    gdImageJpeg(im, f, -1);

    if(f != stdout) fclose(f);
    
    gdImageDestroy(im);
    
    return(0);
}

int fswc_grab(char *name)
{
    uint32_t frame;
    avgbmp_t *abitmap, *pbitmap, *gbitmap;
    gdImage *image, *original;
    src_t src;
    
    /* Set source options... */
    memset(&src, 0, sizeof(src));
    src.input      = NULL; //config->input;
    src.tuner      = 0; //config->tuner;
    src.frequency  = 0; //config->frequency;
    src.delay      = 0; //config->delay;
    src.timeout    = 10; //config->timeout; 
    src.use_read   = 0; //config->use_read;
    src.list       = 0; //config->list;
    src.palette    = SRC_PAL_ANY; //config->palette;
    src.width      = 1920; //config->width;
    src.height     = 1080; //config->height;
    src.fps        = 1200; //config->fps;

    if(src_open(&src, strdup("/dev/video0")) == -1) return(-1);
        
    /* Allocate memory for the average bitmap buffer. */
    gbitmap = (avgbmp_t *)calloc(src.fps * 2 * src.height * 3, sizeof(avgbmp_t));
    abitmap = (avgbmp_t *)calloc(2 * src.height * 3, sizeof(avgbmp_t));
    FILE *f;
    uint32_t x, y, hlength;
    uint8_t *himg = NULL;
    gdImage *im, *im3;
    int i;
    /* Grab the requested number of frames. */
    //char str[13] = "frame____.jpg";

    im3 = gdImageCreateTrueColor(src.fps * 2, src.height);
    for(frame = 0; frame < src.fps; frame++)
    {
        src_grab(&src);        
    
        //i = copy_jpeg_dht(src.img, src.length, &himg, &hlength);    
        i = 1; himg = (uint8_t *)src.img; hlength = src.length;
        im = gdImageCreateFromJpegPtr(hlength, himg);       
        
        gdImageCopy(im3, im, frame*2, 0, src.width / 2, 0, 2, src.height);
        gdImageDestroy(im);
        //MSG("No. of Frames= %d", frame);
    }

    fswc_output(name, im3); 
    gdImageDestroy(im3);
    /* We are now finished with the capture card. */
    src_close(&src);
    
    return(0);
}

int main(int argc, char *argv[])
{
 
    int r = 0;
    
 
    r = fswc_grab(argv[1]);

    
    return(r);
}
