#include<stdio.h>
#include<stdlib.h>

#include"file.h"
#include"menu.h"
#include"functions.h"

#define MAX_GAMMA_CORRECTION 22
#define MIN_GAMMA_CORRECTION 1

#define LAST_TASK 5
#define FiRST_TASK 1

#define CONTINUE 1
#define EXIT 0

#define MAX_PX 10
#define MIN_PX 1

#define MEMORY_MISTAKE 1

void menuCorrect(int* task)
{
	while ((scanf_s("%d", task)) != 1 || *task < FiRST_TASK || *task>LAST_TASK || getchar() != '\n')
	{
		printf("Make right choise(1-6):");
		rewind(stdin);
	}
}

void chekerForContinue(int* num)
{
	while ((scanf_s("%d", num)) != 1 || getchar() != '\n' || *num < EXIT || *num > CONTINUE)
	{
		printf("Intput correct number:");
		rewind(stdin);
	}
}

void makePictureBlackAndWhiteMenu(Picture* picture,char * file_name)
{
	RGBQUAD* palitr = NULL;

	picture = loadPicture(file_name, &palitr);

	makePictureWhiteAndBlack(picture,&palitr);

	makeFileResult(picture, "WhileAndBlack.bmp",palitr);

	freePicture(&picture);
}

void makePictureNegativeMenu(Picture* picture, char* file_name)
{
	RGBQUAD* palitr = NULL;

	picture = loadPicture(file_name,&palitr);

	makePictureNegative(picture,&palitr);

	makeFileResult(picture, "Negative.bmp",palitr);

	freePicture(&picture);
}

void makeMedianFiltrationMenu(Picture* picture, char* file_name)
{
	RGBQUAD* palitr = NULL;

	picture = loadPicture(file_name, &palitr);

	printf("Px(1-10):");

	int px;
	while (scanf_s("%d", &px) != 1 || px > MAX_PX || px < MIN_PX || getchar() != '\n')
	{
		printf("Input correct px:");
		rewind(stdin);
	}

	makeMedianFiltration(picture,px,palitr);

	makeFileResult(picture, "MadianFiltration.bmp",palitr);

	freePicture(&picture);
}

void makeGammaCorrectionMenu(Picture* picture, char* file_name)
{
	RGBQUAD* palitr = NULL;

	picture = loadPicture(file_name, &palitr);

	int gamma_value;

	printf("Input gamma_value:");
	while (scanf_s("%d", &gamma_value) != 1 || gamma_value > MAX_GAMMA_CORRECTION || gamma_value < MIN_GAMMA_CORRECTION || getchar() != '\n')
	{
		printf("Input correct gamma_value:");
		rewind(stdin);
	}

	makeGammaCorrection(picture,gamma_value,&palitr);

	makeFileResult(picture, "Gamma correction.bmp",palitr);

	freePicture(&picture);
}

