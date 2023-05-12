#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"file.h"

#define CANT_OPEN_FILE 1
#define MEMORY_MISTAKE 1
#define FORMAT_ERROR 1

#define NUM_BYTES_IN_PIXEL_24 3

#define ONE_TIME 1

#define KB 1024
#define NULL_SYMBOL 1
#define FORMAT_SIZE 4

#define SIZE_OF_PALITR 256

#define MULTIPLE_4 4

#define PIXEL_8 8
#define PIXEL_24 24


char* inputFileName()
{
	char *file_name = (char*)calloc(KB, sizeof(char));
	if (file_name == NULL)
		exit(MEMORY_MISTAKE);

	printf("Input file name:");
	scanf_s("%s", file_name, KB);

	char format[] = ".bmp";

	strcat(file_name, format);

	return file_name;
}

void chekerForFileInFolder(char** file_name)
{
	FILE* file;
	errno_t err_file = fopen_s(&file, *file_name, "rb");

	if (err_file != 0)
	{
		printf("Cant open file");
		free(*file_name);
		exit(CANT_OPEN_FILE);
	}
	fclose(file);
}

int findRowPadding(const Picture* picture)
{
	int row_padding = (MULTIPLE_4 - (picture->info.Width % MULTIPLE_4)) % MULTIPLE_4;

	return row_padding;
}

void makeMasOfPixels(FILE* file, Picture** picture)
{
	fseek(file, (*picture)->file.DataOffset, SEEK_SET);

	int row_padding = findRowPadding(*picture);

	for (unsigned int i = 0; i < (*picture)->info.Height; i++)
		for (unsigned int j = 0; j < (*picture)->info.Width; j++)
		{
			fread(&(*picture)->data[i][j].blue, sizeof(unsigned char), ONE_TIME, file);
			fread(&(*picture)->data[i][j].green, sizeof(unsigned char), ONE_TIME, file);
			fread(&(*picture)->data[i][j].red, sizeof(unsigned char), ONE_TIME, file);

			if (row_padding != 0)
				fseek(file, row_padding * NUM_BYTES_IN_PIXEL_24, SEEK_CUR);
		}	
}

void ckekerForFormat(Picture ** picture)
{
	if ((*picture)->file.Signature[0]!='B' && (*picture)->file.Signature[1] != 'M')
	{
		free(*picture);
		exit(FORMAT_ERROR);
	}
}

char** giveMemoryForPixels(Picture* picture, RGBQUAD** palitr)
{
	char **mas = (unsigned char**)calloc(picture->info.Height, sizeof(unsigned char*));
	if (mas == NULL)
	{
		free(picture);
		free(palitr);
		exit(MEMORY_MISTAKE);
	}

	for (int i = 0; i < (int)picture->info.Height; i++)
	{
		mas[i] = (unsigned char*)calloc(picture->info.Width, sizeof(unsigned char));

		if (mas[i] == NULL)
		{
			free(picture);
			free(palitr);
			exit(MEMORY_MISTAKE);
		}
	}

	return mas;
}

Picture* loadPicture(const char* file_name, RGBQUAD** palitr)
{
	FILE* file;
	errno_t err_file = fopen_s(&file, file_name, "rb");

	if (err_file != 0)
	{
		printf("Cant open file");
		exit(CANT_OPEN_FILE);
	}

	Picture* picture = (Picture*)calloc(1,sizeof(Picture));
	if (picture == NULL)	
		exit(MEMORY_MISTAKE);
	
	fread(&picture->file, sizeof(File_Header), ONE_TIME, file);
	ckekerForFormat(&picture);
	fread(&picture->info, sizeof(Info_Header), ONE_TIME, file);

	switch (picture->info.BitCount)
	{
		case PIXEL_8:
			*palitr = (RGBQUAD*)calloc(256, sizeof(RGBQUAD));
			if (*palitr == NULL)
			{
				free(picture);
				exit(MEMORY_MISTAKE);
			}

			for (int i = 0; i < SIZE_OF_PALITR; i++)
			{
				fread(&(*palitr)[i].rgbBlue, sizeof(unsigned char), 1, file);
				fread(&(*palitr)[i].rgbGreen, sizeof(unsigned char), 1, file);
				fread(&(*palitr)[i].rgbRed, sizeof(unsigned char), 1, file);
				fread(&(*palitr)[i].rgbReserved, sizeof(unsigned char), 1, file);
			}
		
			fseek(file, picture->file.DataOffset, SEEK_SET);

			picture->pixel_8 = giveMemoryForPixels(picture, palitr);

			int row_padding = findRowPadding(picture);

			for (unsigned int i = 0; i < picture->info.Height; i++)
			{
				for (unsigned int j = 0; j < picture->info.Width; j++)
				{
					fread(&picture->pixel_8[i][j], sizeof(unsigned char), 1, file);
				}

				if (row_padding != 0)
					fseek(file, row_padding, SEEK_CUR);
			}

			break;

		case PIXEL_24:
			picture->data = (pixel_24**)calloc(picture->info.Height, sizeof(pixel_24*));
			if (picture->data == NULL)
			{
				free(picture);
				free(palitr);
				exit(MEMORY_MISTAKE);
			}

			for (int i = 0; i < (int)picture->info.Height; i++)
			{
				picture->data[i] = (pixel_24*)calloc(picture->info.Width, sizeof(pixel_24));

				if (picture->data[i] == NULL)
				{
					free(picture);
					free(palitr);
					exit(MEMORY_MISTAKE);
				}
			}

			makeMasOfPixels(file, &picture);

			break;
	
		default:
			break;
	}
				
	fclose(file);
	return picture;
}

void makeFileResult(const Picture* picture,const char *name, RGBQUAD* palitr)
{
	FILE* file;
	errno_t err_file = fopen_s(&file, name, "wb");

	if (err_file != 0)
	{
		printf("Cant open file");
		exit(CANT_OPEN_FILE);
	}

	fwrite(&picture->file, sizeof(File_Header), ONE_TIME, file);
	fwrite(&picture->info, sizeof(Info_Header), ONE_TIME, file);

	int row_padding = findRowPadding(picture);

	switch (picture->info.BitCount)
	{

	case PIXEL_8:

		for (int i = 0; i < SIZE_OF_PALITR; i++)
		{
			fwrite(&palitr[i].rgbBlue, sizeof(unsigned char), 1, file);
			fwrite(&palitr[i].rgbGreen, sizeof(unsigned char), 1, file);
			fwrite(&palitr[i].rgbRed, sizeof(unsigned char), 1, file);
			fwrite(&palitr[i].rgbReserved, sizeof(unsigned char), 1, file);
		}

		free(palitr);

		fseek(file, picture->file.DataOffset, SEEK_SET);

		for (unsigned int i = 0; i < picture->info.Height; i++)
		{
			for (unsigned int j = 0; j < picture->info.Width; j++)
			{
				fwrite(&picture->pixel_8[i][j], sizeof(unsigned char), 1, file);
			}

			unsigned char null_pixel = '\0';
			if (row_padding != 0)
				fwrite(&null_pixel, sizeof(unsigned char), row_padding, file);
		}

		break;

	case PIXEL_24:

		fseek(file, picture->file.DataOffset, SEEK_SET);

		for (unsigned int i = 0; i < picture->info.Height; i++)
		{
			for (unsigned int j = 0; j < picture->info.Width; j++)
			{
				fwrite(&picture->data[i][j].blue, sizeof(unsigned char), ONE_TIME, file);
				fwrite(&picture->data[i][j].green, sizeof(unsigned char), ONE_TIME, file);
				fwrite(&picture->data[i][j].red, sizeof(unsigned char), ONE_TIME, file);
			}

			unsigned char null_pixel[] = "\0\0\0";
			if (row_padding != 0)
				fwrite(&null_pixel, sizeof(unsigned char), row_padding, file);
		}
		break;
	default:
		break;
	}

	fclose(file);
}

void freePicture(Picture ** picture)
{
	switch ((*picture)->info.BitCount)
	{
	case PIXEL_8:
		for (unsigned int i = 0; i < (*picture)->info.Height; i++)
			free((*picture)->pixel_8[i]);
		break;
	case PIXEL_24:
		for (unsigned int i = 0; i < (*picture)->info.Height; i++)
			free((*picture)->data[i]);
		break;
	default:
		break;
	}

	free(*picture);
}
