#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
char *username;
char *names[50];
int i = 0;
char delim[2] = { ' ', '*'};

username = get_string();


//strtok breaks str to parts in names[]
i=0;

names[0] = strtok(username, delim);  
while (names[i] != NULL) {
    i++;
    names[i] = strtok(NULL, delim); 
    }
    
    //Display all in names[]
    for(int x = 0; x < i; x++)
        printf("%s ", names[x]);
    printf("\n");

    // Now the initials
    for(int x = 0; x < i; x++)
        printf("%c", toupper(names[x][0]));
    printf("\n");

}