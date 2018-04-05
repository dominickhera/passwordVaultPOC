#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "HashTableAPI.h"

#define character "X"
#define wall '=';

int main()
{

    int c;
    int maxX;
    int maxY;
    int y = 0;
    int x = 0;
    int prevX;
    int prevY;
    int passwordVaultSize = 0;
    char tempPass[256];
    char tempKey[256];
    char tempStr[256];
    char assetLocation[50];
    char fileName[256];
    char outputData[256];
    char masterPassword[256];
    char passwordCheck[256];
    // char * parse;
    FILE *fp;
    FILE *config;


    // y = 0;
    // x = 0;


    char * menuOptions[] = {"Add New Password", "Remove Password", "Retrieve a Password", "Update a Password", "Exit"}; 
    int tempMenuNumb[7];

    HTable * hashTable = createTable(5000, &hashData, &free, &printData);

    strcpy(assetLocation, "./assets/");
    strcpy(fileName, "");
    config = fopen("./assets/congfig.txt", "r");

    if(config == NULL)
    {
        config = fopen("./assets/congfig.txt", "w");
        printf("new user detected\n\nenter new master password: ");
        scanf("%s", masterPassword);
        fprintf(config, "%s\n", masterPassword);
        printf("enter name of your new password storage file: ");
        scanf("%s", tempKey);
        strcat(fileName, assetLocation);
        strcat(fileName, tempKey);
        fp = fopen(fileName, "w");
        fprintf(config, "%s", fileName);
        fclose(config);
    }
    else
    {
        int configCount = 0;
        char line[256];
        // char * parse;

        while(fgets(line, sizeof(line), config) != NULL)
        {
            if(configCount == 0)
            {
                strcpy(masterPassword, line);
                configCount++;
            }
            else if(configCount == 1)
            {
                strcpy(fileName, line);
                configCount++;
            }

        }

        fp = fopen(fileName, "r");

        while(fgets(line, sizeof(line), fp) != NULL)
        {
            char * parse;

            if(line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }

            parse = strtok(line, ",");
            strcpy(tempKey, parse);

            while((parse = strtok(NULL, ",")) != NULL)
            {

                strcpy(tempPass, parse);

            }

            insertData(hashTable, tempKey, tempPass);
            passwordVaultSize++;

        }
    }


    initscr();
    // noecho();
    getmaxyx(stdscr,maxY,maxX);

    makeMainMenu(maxX, maxY);

    passwordChecker(masterPassword, passwordCheck, maxX, maxY);

    for(int i = 1; i < 6; i++)
    {
        makeMainMenuOptions(maxX, maxY, i, menuOptions[i - 1]);

    }

    mvprintw((maxY/8),5,character);
    move((maxY/8),5);


    c = getch();


    while (c != 'q')
    {

        if(c == 'w') 
        {
            getPos(&y,&x);
            if(y > 1) 
            {
                if((y - (maxY/8)) > 1)
                {

                    printw(" "); 
                    mvprintw((y - (maxY/8)), (x),character);
                    move(y - (maxY/8) , x);
                }
            }

        }
        else if(c == 's')
        {
            getPos(&y,&x);
            if(y < (maxY - (maxY/8)))
            {
                if((y + (maxY/8)) < ((maxY/3) * 2))
                {
                    printw(" ");
                    mvprintw((y+(maxY/8)), x,character);
                    move((y + (maxY/8)), x);
                }
            }
        }
        else if(c == '\n')
        {

            getPos(&y, &x);

            prevY = y;
            prevX = x;
            for(int i = 1; i < 7; i++)
            {
                tempMenuNumb[i] = (maxY/8) * i;
            }

            for(int i = 0; i < maxX; i++)
            {
                mvaddch(((maxY/3)*2) + 3, i, ' ');
            }

            if(y == tempMenuNumb[1])
            {
                echo();
                clearTextLine((((maxY/3)*2) + 2),0);
                memset(tempPass, 0, 256);
                memset(tempKey, 0, 256);
                clearMainMenu(maxX, maxY);
                if((passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions)) == 1)
                {
                    echo();
                    mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5), "%s", "what program/site is this for? : ");
                    getstr(tempKey);
                    if(lookupData(hashTable, tempKey) == NULL)
                    {
                        echo();
                        clearTextLine(((maxY/6)*2) , (((maxX/6) * 2) - 5));
                        mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5), "%s", "enter new password into vault : ");
                        getstr(tempPass);
                        insertData(hashTable, tempKey, tempPass);
                        clearTextLine(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5));
                        mvprintw(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5), "%s", "new password successfully entered.");
                        passwordVaultSize++;
                    }
                    else
                    {
                        clearTextLine(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5));
                        mvprintw(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5), "You already have a password entered for %s, try removing it or updating it instead.", tempKey);
                    }
                }

                clearTextLine(((maxY/6)*2) , (((maxX/6) * 2) - 5));
                for(int i = 1; i < 6; i++)
                {
                    makeMainMenuOptions(maxX, maxY, i, menuOptions[i - 1]);

                }
            }
            else if(y == tempMenuNumb[2])
            {
                echo();
                clearMainMenu(maxX, maxY);
                // passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions);
                if((passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions)) == 1)
                {
                    memset(tempPass, 0, 256);
                    memset(tempKey, 0, 256);
                    clearTextLine((((maxY/3)*2) + 2),0);
                    mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5), "%s","what is the site/program you want to delete your info for? : ");
                    getstr(tempKey);
                    if(lookupData(hashTable, tempKey) != NULL)
                    {
                        clearTextLine(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5));
                        removeData(hashTable, tempKey);
                        mvprintw(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5), "password successfully entered deleted.");
                        passwordVaultSize--;

                    }
                    else
                    {
                        clearTextLine((((maxY/3)*2) + 2),0);
                        mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5), "%s","you don't seem to have any accounts for that site/program...");
                    }
                }

                for(int i = 1; i < 6; i++)
                {
                    makeMainMenuOptions(maxX, maxY, i, menuOptions[i - 1]);

                }
                makeMainMenu(maxX, maxY);
            }
            else if (y == tempMenuNumb[3])
            {
                echo();
                clearMainMenu(maxX, maxY);
                if((passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions)) == 1)
                {
                    // passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions);
                    memset(tempStr, 0, 256);
                    clearTextLine((((maxY/3)*2) + 2),0);
                    mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 6), "%s","what program/website is this password for? : ");
                    getstr(tempStr);

                    if(lookupData(hashTable, tempStr) != NULL)
                    {
                        clearTextLine((((maxY/3)*2) + 2),0);
                        clearTextLine(((maxY/6)*2) , (((maxX/6) * 2) - 6));
                        mvprintw((((maxY/6)*2) + 4) , (((maxX/6)*2)-3),"Found it! Your %s Password is %s", tempStr,(char*)lookupData(hashTable, tempStr));
                    }
                    else
                    {
                        clearTextLine((((maxY/3)*2) + 2),0);
                        mvprintw((((maxY/6)*2) + 5) , (((maxX/6) * 2) - 5),"\nSorry, but I couldn't find any password for your %s account...\n\n", tempStr);
                    }
                }
                for(int i = 1; i < 6; i++)
                {
                    makeMainMenuOptions(maxX, maxY, i, menuOptions[i - 1]);

                }
                makeMainMenu(maxX, maxY);
            }
            else if(y == tempMenuNumb[4])
            {
                echo();
                clearMainMenu(maxX, maxY);
                if((passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions)) == 1)
                {
                    // passwordCheckerMainMenu(masterPassword, passwordCheck, maxX, maxY, menuOptions);
                    memset(tempPass, 0, 256);
                    memset(tempKey, 0, 256);
                    clearTextLine((((maxY/3)*2) + 2),0);
                    if(passwordVaultSize > 0)
                    {
                        mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5),"what program/site account info do you want to to update? : ");
                        getstr(tempKey);

                        if(lookupData(hashTable, tempKey) != NULL)
                        {
                            clearTextLine((((maxY/3)*2) + 2),0);
                            mvprintw(((maxY/6)*2) , (((maxX/6) * 2) - 5),"enter updated password into vault: ");
                            getstr(tempPass);
                            removeData(hashTable, tempKey);
                            insertData(hashTable, tempKey, tempPass);
                            clearTextLine((((maxY/3)*2) + 2),0);
                            // mvprintw((((maxY/3)*2) + 2) , 0,"enter updated password into vault: ");

                        }
                        else
                        {
                            clearTextLine(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5));
                            mvprintw(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5),"\n\nYou haven't entered a password for %s yet, try adding one instead\n\n", tempKey);
                        }
                    }
                    else
                    {
                        clearTextLine(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5));
                        mvprintw(((maxY/6)*2) + 5 , (((maxX/6) * 2) - 5),"\n\nYou haven't entered any accounts to the vault, try doing that first..."); 
                    }
                }

                for(int i = 1; i < 6; i++)
                {
                    makeMainMenuOptions(maxX, maxY, i, menuOptions[i - 1]);

                }
                makeMainMenu(maxX, maxY);
            }
            else if(y == tempMenuNumb[5])
            {

                fp = fopen(fileName, "w");

                for(int i = 0; i < hashTable->size; i++)
                {

                    Node * temp = hashTable->table[i];

                    while(temp != NULL)
                    {
                        strcpy(outputData, (char *)temp->key);
                        strcat(outputData, ",");
                        strcat(outputData, (char *)temp->data);
                        fprintf(fp, "%s\n", outputData);
                        temp = temp->next;

                    }
                }

                fclose(fp);

                destroyTable(hashTable);
                endwin();
                break;
            }
            else
            {
                printf("butts");
            }

            noecho();
            mvprintw(prevY, prevX,character);
            move(prevY, prevX);

        }

        refresh();
        c = getch();
    }
    endwin();

    return 0;
}

