#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    //check that the input is valid
    if (argc!=4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }
    
    //declare the resize factor
    float f=atof(argv[1]);
    
    //check that the resize factor is valid
    if (f<=0 || f>100)
    {
        fprintf(stderr, "The resize factor should be between 0 and 100, inclusive of 100\n");
        return 1;
    }
    
    //declare infile and outfile
    char *infile=argv[2];
    char *outfile=argv[3];
    
    //open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }
    
    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
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
    
    //store previous header's fields
    int old_biWidth = bi.biWidth;
    int old_biHeight = abs(bi.biHeight);
    
    //resize the width and height for the new image
    bi.biWidth *= f;
    bi.biHeight *= f;
    
    //determine old and new padding
    int old_padding = (4 - (old_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //determine the new size of the image
    bi.biSizeImage=abs(bi.biHeight)*(new_padding + bi.biWidth * sizeof(RGBTRIPLE));
    
    //resize the file size 
    bf.bfSize =sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    //specifying other required variables
    int low_f=floor(f);
    int new_vertical_pixels=floor(((f-1)*old_biHeight));
    int new_horizontal_pixels=floor(((f-1)*old_biWidth));
    
    //determine if it will be necessary to skip any pixels
    int steps;
    if (f>1)
    {
        steps=1;
    }
    else 
    {
        steps= 1/f;
    }
    //int skip = steps - 1;
    
    // iterate over infile's scanlines
    for (int i = 0; i < old_biHeight; i+=steps)
    {
        //(re)set vertical counter to zero
        int ver_count=0;
        
        for (int n=0; n<f; n++)
        {
            if (i < new_vertical_pixels || ver_count<low_f || low_f == f || low_f==0)
            {
                // iterate over pixels in scanline
                for (int j = 0; j < old_biWidth; j+=steps)
                {
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE),1, inptr);
                    
                    //skip over pixels, if needed
                    // fseek(inptr, skip, SEEK_CUR);
                    
                    //(re)set horizontal counter to zero
				    int hor_count=0;
				    
                    for (int m=0;m<f;m++)
                    {
                        //multiple cases to accomodate for when f is an integer and non-integer
                        if (j < new_horizontal_pixels || hor_count < low_f || low_f == f || low_f==0)
                        {
                            // write RGB triple to outfile
                            fwrite(&triple, sizeof(RGBTRIPLE),1, outptr);
                        }
                        hor_count++;
                    }
                }
                
                // skip over padding, if any
                fseek(inptr, old_padding, SEEK_CUR);
                
                // then add it back (to demonstrate how)
                for (int k = 0; k < new_padding; k++)
                {
                    fputc(0x00, outptr);
                }
                
                //reset file position of the stream
                fseek(inptr, -(long int)( old_biWidth * sizeof(RGBTRIPLE) + old_padding), SEEK_CUR);
            }
            ver_count++;
        }
        //move file position to next row
        fseek(inptr, old_biWidth * sizeof(RGBTRIPLE) + old_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
