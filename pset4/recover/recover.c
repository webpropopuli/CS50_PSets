#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>


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

#if 0
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
// and data[] is full
while(0 == ret)
{
    // if we're not currently writing a file, create it here
    if (NULL == fOut)
    {
        sprintf(outfile, "%03d.jpg", counter);
        fOut = fopen(outfile, "w");
        counter++ ;

        if (fOut == NULL)
        {                   // error
            fclose(fRaw);
            fRaw = NULL;
            fprintf(stderr, "Could not create %s.\n", outfile);
            ret= 3;
            break;
        }
    }
    fwrite(data, 1, 512, fOut);

    //Get more
    if (512 != fread(data, 1, 512, fRaw) )
       break;  // must be EOF

    if (IsHdr(data))
    {
        //fwrite(data, 1, BlockSize, fOut);
        fclose(fOut);
        fOut = NULL;

    }
} // end while


if(NULL != fRaw)
    fclose(fRaw);
if(NULL != fOut)
    fclose(fOut);

return ret;
}
#endif
#if 1
// ERR CODES        //! Why not just use these variables as ints
#define ERR_BAD_PARAMS          0xFFF0001
#define ERR_FAIL_OPEN_INPUT     0xFFF0002
#define ERR_HDR_NEVER_FOUND     0xFFF0005
#define ERR_CREATING_OUTFILE    0xFFF0003
#define ERR_WRITING_OUTFILE     0xFFF0004
//! Is there a better place for these in a larger program?

#define NOTES   //! what am I?

int main(int argc, char *argv[])
{
    int counter = 0;               // for naming output file. Increments with each fOut open
    unsigned char data[512] = {0}; // Hold our image data here
    int err_code = 0;              // err value, 0 is happy time

    char outfile[] = "000.jpg"; //! pre-size it here

    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return ERR_BAD_PARAMS;
    }

    FILE *fRaw = fopen(argv[1], "r");
    if (fRaw == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return ERR_FAIL_OPEN_INPUT;
    }

    // Read chunks until we get to the first header block
    int cntRead = 0;
    do
    {
        //! ERROR when can't read 512 bytes
        cntRead = fread(data, 1, 512, fRaw);
        //printf("cntRead=%d\n", cntRead);
        if (cntRead != 512)
        {
            if (fRaw)
            {
                fclose(fRaw);
                fRaw = NULL;
            }
            fprintf(stderr, "Header never found in file.\n");
            err_code = ERR_HDR_NEVER_FOUND;
            break;
        }
        if (IsHdr(data))
            break;

    } while (1);

    FILE *fOut = NULL; //Write found files here. May create > 1 file

    // Now we're at first valid block and data[] is full
    while (0 == err_code)
    {
        // if we're not currently writing a file, create it here
        if (NULL == fOut)
        {
            sprintf(outfile, "%03d.jpg", counter);
            fOut = fopen(outfile, "w");
            counter++;

            if (fOut == NULL)
            { // error
                fclose(fRaw);
                fRaw = NULL;
                fprintf(stderr, "Could not create %s.\n", outfile);
                err_code = ERR_CREATING_OUTFILE;
                break;
            }
        }
        if( 512 != fwrite(data, 1, 512, fOut))
            err_code = ERR_WRITING_OUTFILE;

        //Get more
        if (512 != fread(data, 1, 512, fRaw)) //! why is 512 second?
            break;                                // must be EOF
        if (IsHdr(data))
        {
            fclose(fOut);
            fOut = NULL;
        }
    } // end while

    if (NULL != fRaw)
        fclose(fRaw);
    if (NULL != fOut)
        fclose(fOut);

    return err_code ? err_code : 0;  //! What is this? Why not just "return (err)"?
}
#endif
