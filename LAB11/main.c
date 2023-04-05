#include<stdio.h>
#include<stdlib.h>

#include"file.h"
#include"functions.h"
#include"menu.h"

#define NUM_TASKS_MENU 4
#define EXIT 5
#define CONTINUE 1

int main()
{
	Picture* picture = NULL;

	char* file_name = inputFileName();

    chekerForFileInFolder(&file_name);

	void (**menu)(Picture*,char*) = (void(**)(Picture*,char*))calloc(NUM_TASKS_MENU, sizeof(void(*)(Picture*,char*)));
	if (menu == NULL)
		return 1;

	menu[0] = &makePictureBlackAndWhiteMenu;
	menu[1] = &makePictureNegativeMenu;
	menu[2] = &makeMedianFiltrationMenu;
	menu[3] = &makeGammaCorrectionMenu;

	int choise = 0;
	int task_choise = 0;

    do {
        printf("  Menu:\n1.Make picture black and white.\n2.Negative.\n3.Median filtr.\n4.Gamma correction.\n5.Exit.\nYour choise:");
        menuCorrect(&task_choise);
        system("cls");

        if (task_choise == EXIT)
        {
            free(menu);
            return 0;
        }
        
        menu[task_choise - 1](picture,file_name);
        
        printf("Do you want to continue?(Yes-1/No-0):");
        chekerForContinue(&choise);

        system("cls");
    } while (choise == CONTINUE);

    free(menu);

	return 0;
}