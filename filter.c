#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    const double RGB = 3;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double average = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen)/RGB;
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
            temp[i][j].rgbtBlue = image[i][j].rgbtBlue;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][width-j-1].rgbtRed;
            image[i][j].rgbtBlue = temp[i][width-j-1].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][width-j-1].rgbtGreen;
        }
    }
    return;

}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    double RGB = 0;
    double averageR = 0;
    double averageG = 0;
    double averageB = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtBlue =  image[i][j].rgbtBlue;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if ( i+k < 0 || j+l < 0 || i+k == height || j+l == width)
                    {
                        continue;
                    }
                    else
                    {
                        averageR +=temp[i+k][j+l].rgbtRed;
                        averageG +=temp[i+k][j+l].rgbtGreen;
                        averageB +=temp[i+k][j+l].rgbtBlue;

                        RGB++;
                    }
                }
            }
            image[i][j].rgbtRed = round(averageR/RGB);
            image[i][j].rgbtGreen = round(averageG/RGB);
            image[i][j].rgbtBlue = round(averageB/RGB);

            averageR = 0;
            averageG = 0;
            averageB = 0;
            RGB = 0;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    double dxRed = 0;
    double dxGreen = 0;
    double dxBlue = 0;
    double dyRed = 0;
    double dyGreen= 0;
    double dyBlue = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtBlue =  image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if ( i+k < 0 || j+l < 0 || i+k == height || j+l == width)
                    {
                        continue;
                    }
                    else
                    {
                        if(k == 0)
                        {
                            dxRed += (temp[i+k][j+l].rgbtRed * 2*l);
                            dxGreen += (temp[i+k][j+l].rgbtGreen * 2*l);
                            dxBlue += (temp[i+k][j+l].rgbtBlue * 2*l);
                        }
                        else
                        {
                            dxRed += (temp[i+k][j+l].rgbtRed * l);
                            dxGreen += (temp[i+k][j+l].rgbtGreen * l);
                            dxBlue += (temp[i+k][j+l].rgbtBlue * l);
                        }
                        if(l == 0)
                        {
                            dyRed +=(temp[i+k][j+l].rgbtRed * 2*k);
                            dyGreen +=(temp[i+k][j+l].rgbtGreen * 2*k);
                            dyBlue +=(temp[i+k][j+l].rgbtBlue * 2*k);
                        }
                        else
                        {
                            dyRed +=(temp[i+k][j+l].rgbtRed * k);
                            dyGreen +=(temp[i+k][j+l].rgbtGreen * k);
                            dyBlue +=(temp[i+k][j+l].rgbtBlue * k);
                        }
                    }
                }
            }
            if (round(sqrt(dxRed*dxRed + dyRed*dyRed)) >= 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(sqrt(dxRed*dxRed + dyRed*dyRed));
            }
            if (round(sqrt(dxGreen*dxGreen + dyGreen*dyGreen)) >= 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(sqrt(dxGreen*dxGreen + dyGreen*dyGreen));
            }
            if (round(sqrt(dxBlue*dxBlue + dyBlue*dyBlue)) >= 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(sqrt(dxBlue*dxBlue + dyBlue*dyBlue));
            }
            dxRed = 0;
            dxGreen = 0;
            dxBlue = 0;
            dyRed = 0;
            dyGreen= 0;
            dyBlue = 0;
        }
    }
    return;
}
