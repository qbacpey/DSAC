#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "BinTree.h"

void read_command(BinTree *tree, const char *filename);

int main()
{
    BinTree *tree = load_tree("definition.txt");
    read_command(tree, "queries.txt");
}

void read_command(BinTree *tree, const char *filename)
{
    FILE *definition = fopen(filename, "r");
    char *flag = (char *)malloc(STRING_SIZE * sizeof(char));
    char *parent_name = (char *)malloc(STRING_SIZE * sizeof(char));
    char *child_name = (char *)malloc(STRING_SIZE * sizeof(char));

    while (fscanf(definition, "%s ", flag) != EOF)
    {
        if (!strcmp(flag, "whatis"))
        {
            fscanf(definition, "%s", parent_name);
            printf("\nWhat is %s:\n", parent_name);
            print_children(tree, parent_name);
        }
        else if (!strcmp(flag, "howmany"))
        {
            fscanf(definition, "%s %s", child_name, parent_name);
            printf("\nHow many %s in %s:\n", child_name, parent_name);
            BinNode *temp = find_child(tree, parent_name, child_name);
            if(temp){
                print_data(temp->data);
            }else{
                printf("%s don't have any %s\n",parent_name,child_name);
            }
        }
        else
        {
            printf("No such command!\n");
            return;
        }
    }
}