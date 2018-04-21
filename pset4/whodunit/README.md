# Questions

## What's `stdint.h`?

A header file allowind fixed-width ariable types.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

They make it clear the exact-size of vars and remove hardware-speciifc variance.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

8, 32, 32 16

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

0x4d, 0442

## What's the difference between `bfSize` and `biSize`?

bfSize is size of the bitmap file; biSize is the size of the info header structure (i.e. "size of me)")

## What does it mean if `biHeight` is negative?

Looking at the image will make you younger. ("Helloooo, CS50!") OK...positive values
indicate the image starts in the lower left and builds upwards. Negative values
start from the top line and move downwards instead.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

Either can't open the input file, or create the output file.

## Why is the third argument to `fread` always `1` in our code?

We are only reading a single element of "size" as specified by the 2nd parameter.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

Umm...this much -->
        (4 - (3 * sizeof(RGBTRIPLE)) % 4) % 4;
        (4 - (3 * 24) % 4) % 4);
        (4 - (72) % 4) % 4);
        (4 - 0 % 4);
        so...'0'.
    Man, I coulda just printf'd that into submission. Grrrr....

## What does `fseek` do?

Moves the file pointer (the current "read point") to some offset from a given point.

## What is `SEEK_CUR`?

The current file pointer position.
