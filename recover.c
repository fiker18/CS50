#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s FILE\n", argv[0]);
        return 1;
    }

    FILE *card = fopen(argv[1], "rb");

    if (card == NULL)
    {
        printf("File cannot be opened!");
        fclose(card);
        return 1;
    }

    uint8_t Buffer[512];
     int count = 0;
    char photo[8];

    while(fread(&Buffer, 1, 512, card) == 512)
    {
        if(Buffer[0] == 0xff && Buffer[1] == 0xd8 && Buffer[2] == 0xff && (Buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(photo, "%03i.jpg", count);

            FILE *image = fopen(photo, "wb");

            if (image == NULL)
            {
                printf("Not enough space in memory!");
                fclose(image);
                return 1;
            }

            fwrite(&Buffer, 1, 512, image);
            fclose(image);
            count++;
        }
        else if (count != 0)
        {
            FILE *images = fopen(photo, "ab");

            if (images == NULL)
            {
                printf("File cannot be opened!");
                fclose(images);
                return 1;
            }

            fwrite(&Buffer, 1, 512, images);
            fclose(images);
        }
    }
    fclose(card);
}
