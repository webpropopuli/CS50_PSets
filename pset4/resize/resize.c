#include <cs50.h>
#include <stdio.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Wrong number of arguments");
        return 1;
    }

    int reps = *argv[1] - '0';
    char *infile = argv[2];
    char *outfile = argv[3];
    int inPadding;
    int outPadding;

    // open infile/outfile
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // infile headers
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // outfile headers
    BITMAPFILEHEADER bfNew = bf;
    BITMAPINFOHEADER biNew = bi;

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biNew.biHeight = -(abs(bi.biHeight * reps));
    biNew.biWidth = bi.biWidth * reps;
    outPadding = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biNew.biSizeImage = ((sizeof(RGBTRIPLE) * biNew.biWidth) + outPadding) * abs(biNew.biHeight);
    bfNew.bfSize = biNew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);

    for (int i = 0, biHeight = bi.biHeight; i > biHeight; i--)
    {
        // array for all the RGBTRIPLEs in a row
        RGBTRIPLE triples[biNew.biWidth - 1];
        int triplesIndex = 0;

        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // duplicate triples to triples array
            for (int k = 0; k < reps; k++)
            {
                triples[triplesIndex] = triple;
                triplesIndex++;
            }
        }

        for (int heightReps = 0; heightReps < reps; heightReps++)
        {
            // write array to outfile
            fwrite(&triples, sizeof(RGBTRIPLE), biNew.biWidth, outptr);

            // add outfile padding
            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, outptr);
            }
        }

        // add infile padding
        fseek(inptr, inPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    return 0;
}