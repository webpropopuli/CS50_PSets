#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#define DJM

bool IsHdr(unsigned char *data)
{
if( ( (data[0] & 0x00ff) == 0x00ff) &&
    ( (data[1]  & 0x00ff) == 0x00d8) &&
    ( (data[2]  & 0x00ff) == 0x00ff) &&
    ( (data[3]  & 0x00f0) == 0xe0) )
    return true;
else
    return false;
}

int main(int argc, char *argv[])
{
int counter = 0;                    // for naming output file. Increments with each fOut open
unsigned char data[512] = {0};      // Hold our image data here
int ret = 0;                        //return value, 0 is happy time

FILE *fRaw = fopen(argv[1], "r");
char outfile[] = "000.jpg"; // pre-size it here

    // ensure proper usage
if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

if (fRaw == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        ret = 2;
    }

// Read chunks until we find first one with a header
int cntRead = 0;
do
{
    cntRead = fread(data, 1, 512, fRaw);
    if (cntRead != 512)
    {
        if (fRaw)
        {
            fclose(fRaw);
            fRaw = NULL;
        }
        fprintf(stderr, "Header never found in file.\n");
        ret=5;
        break;
    }
    if (IsHdr(data))
        break;

} while (1);

FILE *fOut = NULL;      //Write found files here. May create > 1 file

// Now we're at first valid block
while(0 == ret)
{
    if (NULL == fOut)
    {
        sprintf(outfile, "%03d.jpg", counter);
        fOut = fopen(outfile, "w");
        counter++ ;
DJM printf("Opened %s\n", outfile);
        if (fOut == NULL)
        {
            fclose(fRaw);
            fRaw = NULL;
            fprintf(stderr, "Could not create %s.\n", outfile);
            ret= 3;
            break;
        }
    }
    fwrite(data, 1, 512, fOut);

    //Get more
    int BlockSize = fread(data, 1, 512, fRaw);
    if (512 != BlockSize || IsHdr(data))
    {
        //fwrite(data, 1, BlockSize, fOut);
        fclose(fOut);
        fOut = NULL;
        if (512 != BlockSize)
            break;  // must be EOF
    }
} // while


if(NULL != fRaw)
    fclose(fRaw);
if(NULL != fOut)
    fclose(fOut);

return ret;
}