#include<stdio.h>
#include<stdlib.h>

#include"file.h"
#include"functions.h"
#include"math.h"

#define MEMORY_MISTAKE 1

#define SIZE_OF_PALITR 256

#define MAX_ID 255
#define MIN_ID 0

#define PIXEL_8 8
#define PIXEL_24 24

void makePictureNegative(const Picture* picture, RGBQUAD** palitr)
{
	switch (picture->info.BitCount)
	{
	case PIXEL_8:

		for (int i = 0; i < SIZE_OF_PALITR; i++)
		{
			(*palitr)[i].rgbBlue = MAX_ID - (*palitr)[i].rgbBlue;
			(*palitr)[i].rgbGreen = MAX_ID - (*palitr)[i].rgbGreen;
			(*palitr)[i].rgbRed = MAX_ID - (*palitr)[i].rgbRed;
		}

		break;

	case PIXEL_24:

		for (unsigned int i = 0; i < picture->info.Height; i++)
			for (unsigned int j = 0; j < picture->info.Width; j++)
			{
				picture->data[i][j].blue = MAX_ID - picture->data[i][j].blue;
				picture->data[i][j].green = MAX_ID - picture->data[i][j].green;
				picture->data[i][j].red = MAX_ID - picture->data[i][j].red;
			}

		break;
	default:
		break;
	}
}

void makePictureWhiteAndBlack(const Picture* picture, RGBQUAD** palitr)
{
	switch (picture->info.BitCount)
	{
	case PIXEL_8:

		for (int i = 0; i < SIZE_OF_PALITR; i++)
		{
			int b = (int)(*palitr)[i].rgbBlue;
			int g = (int)(*palitr)[i].rgbGreen;
			int r = (int)(*palitr)[i].rgbRed;

			if ((b + g + r) / 3 > MAX_ID / 2)
			{
				(*palitr)[i].rgbRed = (unsigned char)MAX_ID;
				(*palitr)[i].rgbGreen = (unsigned char)MAX_ID;
				(*palitr)[i].rgbBlue = (unsigned char)MAX_ID;
			}
			else
			{
				(*palitr)[i].rgbRed = (unsigned char)MIN_ID;
				(*palitr)[i].rgbGreen = (unsigned char)MIN_ID;
				(*palitr)[i].rgbBlue = (unsigned char)MIN_ID;
			}
		}

		break;

	case PIXEL_24:

		for (unsigned int i = 0; i < picture->info.Height; i++)
			for (unsigned int j = 0; j < picture->info.Width; j++)
			{
				int b = (int)picture->data[i][j].blue;
				int g = (int)picture->data[i][j].green;
				int r = (int)picture->data[i][j].red;
			
				picture->data[i][j].blue = 0.2989 * r + 0.5870 * g + 0.1140 * b;
				picture->data[i][j].green = 0.2989 * r + 0.5870 * g + 0.1140 * b;
				picture->data[i][j].red = 0.2989 * r + 0.5870 * g + 0.1140 * b;
				
			}
		break;
	default:
		break;
	}
}



void makeMedianFiltration(Picture* picture, int x_time, RGBQUAD* palitr)
{
	pixel_24** temp_mas = NULL;

	unsigned char** temp_mas_palitr = NULL;

	switch (picture->info.BitCount)
	{
	case PIXEL_8:

		temp_mas_palitr = (unsigned char**)calloc(picture->info.Height - 2, sizeof(unsigned char*));
		if (temp_mas_palitr == NULL)
		{
			freePicture(&picture);
			free(palitr);
			exit(MEMORY_MISTAKE);
		}

		for (int i = 0; i <(int) picture->info.Height - 2; i++)
		{
			*(temp_mas_palitr + i) = (unsigned char*)calloc(picture->info.Width - 2, sizeof(unsigned char));

			if (*(temp_mas_palitr + i) == NULL)
			{
				free(temp_mas_palitr);
				freePicture(&picture);
				free(palitr);
				exit(MEMORY_MISTAKE);
			}
		}

		for (int s = 0; s < x_time; s++)
		{
			for (unsigned int i = 1; i < picture->info.Height - 1; i++)
				for (unsigned int j = 1; j < picture->info.Width - 1; j++)
				{

					int sum_b = 0;
					int sum_g = 0;
					int sum_r = 0;

					for (int k = -1; k < 2; k++)
						for (int o = -1; o < 2; o++)
						{
							if (k != 0 || o != 0)
							{
								sum_b += (int)palitr[picture->pixel_8[i + k][j + o]].rgbBlue;
								sum_g += (int)palitr[picture->pixel_8[i + k][j + o]].rgbGreen;
								sum_r += (int)palitr[picture->pixel_8[i + k][j + o]].rgbRed;
							}
						}

					unsigned char new_b = (unsigned char)(sum_b / 8);
					unsigned char new_g = (unsigned char)(sum_g / 8);
					unsigned char new_r = (unsigned char)(sum_r / 8);

					unsigned char new_id = 0;

					int similarity = (findSubModule((int)palitr[0].rgbBlue, (int)new_b) + findSubModule((int)palitr[0].rgbGreen, (int)new_g) + findSubModule((int)palitr[0].rgbRed, (int)new_r));

					for (int k = 0; k < SIZE_OF_PALITR; k++)
					{
						int similarity_now = (findSubModule((int)palitr[k].rgbBlue, (int)new_b) + findSubModule((int)palitr[k].rgbGreen, (int)new_g) + findSubModule((int)palitr[k].rgbRed, (int)new_r));
						if (similarity_now < similarity)
						{
							new_id = (unsigned char)k;
							similarity = similarity_now;
						}
					}

					temp_mas_palitr[i-1][j-1] = new_id;
				}

			for (unsigned int i = 1; i < picture->info.Height - 1; i++)
				for (unsigned int j = 1; j < picture->info.Width - 1; j++)
				{
					picture->pixel_8[i][j] = temp_mas_palitr[i - 1][j - 1];
				}
		}
			
		free(temp_mas_palitr);
		break;

	case PIXEL_24:
	
		temp_mas = (pixel_24**)calloc((picture->info.Height) - 2, sizeof(pixel_24*));
		if (temp_mas == NULL)
		{
			freePicture(&picture);
			exit(MEMORY_MISTAKE);
		}

		for (int i = 0; i < (int)picture->info.Height - 2; i++)
		{
			temp_mas[i] = (pixel_24*)calloc((picture->info.Width) - 2, sizeof(pixel_24));
			if (temp_mas[i] == NULL)
			{
				freePicture(&picture);
				free(temp_mas);
				exit(MEMORY_MISTAKE);
			}
		}
		for (int s = 0; s < x_time; s++)
		{
			for (unsigned int i = 1; i < picture->info.Height - 1; i++)
				for (unsigned int j = 1; j < picture->info.Width - 1; j++)
				{
					int sum_b = 0;
					int sum_g = 0;
					int sum_r = 0;

					for (int k = -1; k < 2; k++)
						for (int o = -1; o < 2; o++)
						{
							if (k != 0 || o != 0)
							{
								sum_b += (int)picture->data[i + k][j + o].blue;
								sum_g += (int)picture->data[i + k][j + o].green;
								sum_r += (int)picture->data[i + k][j + o].red;
							}
						}

					temp_mas[i - 1][j - 1].blue = (unsigned char)(sum_b / 8);
					temp_mas[i - 1][j - 1].green = (unsigned char)(sum_g / 8);
					temp_mas[i - 1][j - 1].red = (unsigned char)(sum_r / 8);
				}

			for (unsigned int i = 1; i < picture->info.Height - 1; i++)
				for (unsigned int j = 1; j < picture->info.Width - 1; j++)
				{
					picture->data[i][j].blue = temp_mas[i - 1][j - 1].blue;
					picture->data[i][j].green = temp_mas[i - 1][j - 1].green;
					picture->data[i][j].red = temp_mas[i - 1][j - 1].red;
				}
		}
		for (int i = 0; i < (int)picture->info.Height - 2; i++)
		{
			free(temp_mas[i]);
		}
		free(temp_mas);
		break;
	default:
		break;
	}
}

void makeGammaCorrection(const Picture* picture, int gamma_value, RGBQUAD** palitr)
{
	double mas_values[SIZE_OF_PALITR]={'\0'};

	for (int i = 0; i < SIZE_OF_PALITR; i++)
	{
		double sqrt_number = sqrt((i / MAX_ID), gamma_value);
		mas_values[i] =(sqrt_number* MAX_ID);
	}

	switch (picture->info.BitCount)
	{
	case PIXEL_8:

		for (int i = 0; i < SIZE_OF_PALITR; i++)
		{
			(*palitr)[i].rgbBlue =(unsigned char) mas_values[(*palitr)[i].rgbBlue];
			(*palitr)[i].rgbGreen = (unsigned char)mas_values[(*palitr)[i].rgbGreen];
			(*palitr)[i].rgbRed = (unsigned char)mas_values[(*palitr)[i].rgbRed];
		}
		break;

	case PIXEL_24:

		for (unsigned int i = 0; i < picture->info.Height; i++)
			for (unsigned int j = 0; j < picture->info.Width; j++)
			{
				picture->data[i][j].blue = (unsigned char)mas_values[(int)picture->data[i][j].blue];
				picture->data[i][j].green = (unsigned char)mas_values[(int)picture->data[i][j].green];
				picture->data[i][j].red = (unsigned char)mas_values[(int)picture->data[i][j].red];
			}
		break;
	default:
		break;
	}
}

