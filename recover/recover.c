#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

int main (int argc, char *argv[])
{
    //checking that the command line arguments are valid
    if (argc!=2)
    {
        fprintf(stderr, "Please input forensic image name only\n");
        return 1;
    }
    
    //open the raw file
    char *raw=argv[1];
    FILE *inptr = fopen(raw, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", raw);
        return 2;
    }
    
    //specifying varibles
    uint8_t buffer[512];
    int number=0;
    FILE *img=NULL;
    
    //specifying the loop to read the entire file until its end
    while (fread(buffer,512,1, inptr)!=0)
    {
        //checking to see if the file is a jpg
        if ((buffer[0]== 0xff && buffer[1]==0xd8 && buffer[2]==0xff) && (buffer[3] & 0xf0) == 0xe0)
        {
            //closing any existing jpgs, only if one has already been found
            if (img!=NULL)
            {
                fclose(img);
            }
            
            //creating a new jpg
            char filename[8];
            sprintf(filename, "%03i.jpg", number);
            img=fopen(filename, "w");
            number++;
        }
        
        //writing pixels into the new jpg
        if (img != NULL)
        {
            fwrite(buffer, 512, 1, img);
        }
    }
}

