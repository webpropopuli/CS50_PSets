#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "bmp.h"

#define DJM
/*
 * PSET4 RESIZE
 *  I have a few redundant variables in here, but they make the code much more readable imho
 */
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: RESIZE fMult infile outfile\n");
        return 1;
    }

    int Multiplier = (int) (atof(argv[1]) * 100)/100;
DJM    printf("Multiplier is = %d\t", Multiplier);


    if (Multiplier < 0)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER and BITMAPINFOHEADER
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write NEW outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    // These will be overwritten later with corrected values but we can't insert later
    fwrite(&bf, 1, sizeof(BITMAPFILEHEADER),  outptr);
    fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), outptr);

    const long InWidth = bi.biWidth;
    long OutWidth = bi.biWidth * Multiplier;
    signed long OutHeight = (bi.biHeight * Multiplier );
    signed long InHeight = abs(bi.biHeight);

    const int RGBSIZE = sizeof(RGBTRIPLE);      // save a few strokes

    // determine padding for scanlines
    int InPad = (4 - (bi.biWidth * RGBSIZE) % 4) % 4;
    int OutPad =(4 - (OutWidth * RGBSIZE) % 4) % 4;

    const int LINESIZE = RGBSIZE * OutWidth + OutPad;
    int OutSizeImage = LINESIZE * labs(OutHeight);
    int OutSizeFile = OutSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    RGBTRIPLE triple;               // temporary storage

    BYTE *pBuf = NULL;

    // for each INPUT line
    for (int i = 0; i < InHeight; i++)
    {
    //DJM 2do move alloc out of this loop and add padding later
    BYTE *buf = (BYTE *)calloc(LINESIZE, sizeof(BYTE));     // running calloc here means I don't need to manually add padding
    if(NULL == buf)
        {
        fprintf(stderr, "calloc failure. End days are near...\n");
        return 5;
        }

    pBuf = buf;    //reset to head

    for(int x = 0; x < InWidth; x++)
        {
            fread(&triple, RGBSIZE, 1, inptr);          // read a pixel

            for(int z = 0; z < Multiplier; z++)         // put in buf for each Multiple
            {
                memcpy(pBuf, &triple, RGBSIZE);
                pBuf += RGBSIZE;
            }
        }

    // Add padding? but calloc already set 0
    // if I lose the calloc() then I need to append 0x0 * OutPad

    // Now write the line(s)
    for(int y = 0; y < Multiplier; y++)
        fwrite(buf, 1, LINESIZE, outptr);

    // Skip input padding
    fseek(inptr, InPad, SEEK_CUR);

    free(buf);

    }  // INPUT loop

/* Wrap it up....*/

    fclose(inptr);

    /*
     * Update BITMAPFILEHEADER and BITMAPINFOHEADER with new info and overwrite
     */
    fflush(outptr);     //never hurts

    bi.biSizeImage = OutSizeImage;
    bf.bfSize = OutSizeFile;
    bi.biWidth = OutWidth;
    bi.biHeight = OutHeight;

    fseek(outptr, 0, SEEK_SET);     //rewind

    fwrite(&bf, 1, sizeof(BITMAPFILEHEADER), outptr);
    fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), outptr);

    fclose(outptr);

    // success
    return 0;
}
