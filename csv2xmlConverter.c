#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char temp[4096];  //temp variable for replace function

//concat functions to combine multiple char
char *concat(char *a,char *b){
    int lena = strlen(a);
    int lenb = strlen(b);
    char *con = malloc(lena+lenb+1);
    // copy & concat (including string termination)
    memcpy(con,a,lena);
    memcpy(con+lena,b,lenb+1);        
    return con;
}

//replace function to rescue extraordinary situations
char *replace(char *str, char *oldValue, char *newValue)
{
    
    char buffer[4096];
    char *ch;

    strcpy(temp, str);
    char *temp2 = temp;

    if (!(ch = strstr(temp, oldValue)))
    {
        return temp;
    }

    strncpy(buffer, temp, ch - temp);
    buffer[ch - temp] = '\0';

    sprintf(buffer + (ch - temp), "%s%s", newValue, ch + strlen(oldValue));
    sprintf(str, "%s", buffer);
    return ((ch = strstr(temp2, oldValue))) ? replace(str, oldValue, newValue) : str;
}

//reach index in array
char* getfield(char* line, int num ,char* delimeter)
{
     char* tok;
    for (tok = strtok(line, delimeter);
            tok && *tok;
            tok = strtok(NULL, delimeter))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    //Checking the arguement number
    //If command is equal only "h"
    if(argc == 2)
    {
        printf("Welcome to the CSV2XML converter.\n");
        printf("This program converts your csv file to the xml file.\n");
        printf("You have to enter command like : file_name.csv converted_file_name.xml -seperator x -opsys y\n");
        printf("If x is 1 , your seperator is ,\n");
        printf("If x is 2 , your seperator is tab\n");
        printf("If x is 3 , your seperator is ;\n");
        printf("If y is 1 , your endline is windows\n");
        printf("If y is 2 , your endline is linux\n");
        printf("If y is 3 , your endline is macos\n");
        printf("Now recompile the program and enter command correctly !!!\n");
    }

    //If command entered correctly, starts the program
    else
    {
    //reading file
    FILE* stream = fopen(argv[1], "r");

    //writing file
    FILE *writed;
    writed = fopen(argv[2],"w"); 

    if (!writed) {
        printf("Can't open file\n");
        return 0;
    }

    if (!stream) {
        printf("Can't open file\n");
        return 0;
    }


    int control = 0;        //boolean integer for first line
    char line[1024];
    int elementCounter = 0;     //counter for attributes
    char *elementHolder[150];      //string array for first line's attributes


    char* splitter;         //assigning seperator 
    //if else conditions for command line -seperator
    if (strcmp(argv[4],"1") == 0)
    {
       splitter = ",";
    }
    else if (strcmp(argv[4],"2") == 0)
    {
        splitter = "\t";
    }
    else if(strcmp(argv[4],"3") == 0)
    {
         splitter = ";";
    }
    int count = 1;
    char* xmlFile = strtok(argv[1],".");   //holding file's name
    printf("<%s>\n",xmlFile);
    fprintf(writed,"<%s>\n",xmlFile);

    //reading txt file
    while (fgets(line, 1024, stream))
    {
        //element counter part for deciding how many attribute
        if(control == 0)
        {
            char* replaced = replace(line,concat(splitter,splitter),concat(concat(splitter,"-"),splitter));
            replaced = replace(replaced,concat(" ",splitter),"-");
            char* tmp = strdup(replaced);
            

            char *field = strtok(replaced, splitter);
            
            
            while (field)
            {
                elementCounter++;
                field = strtok(NULL, splitter);
            }
            
            free(tmp);
        }

        //holding attributes name by using array 
        if (control == 0)
        {
            control++;
            
            char* replaced2 = replace(line,concat(splitter,splitter),concat(concat(splitter,"-"),splitter));
            replaced2 = replace(replaced2,concat(" ",splitter),"-");
            char* tmp2 = strdup(replaced2);
             char *field2 = strtok(replaced2, splitter);

            int arraySizer = 0;
            while (field2)
            {
                //MAC LINUX WINDOWS OPSYS selection part
                //it works for evert opsys whatever it takes
                elementHolder[arraySizer] = (char *) malloc(sizeof(char)*100);
                if(field2[strlen(field2)-1] == '\n')
                {
                    field2[strlen(field2)-1] = '\0';
                    if (field2[strlen(field2)-1] == '\r')
                    {
                        field2[strlen(field2)-1] = '\0';
                    }
                    
                }
                else if(field2[strlen(field2)-1] == '\r')
                {
                    field2[strlen(field2)-1] = '\0';
                }
               
                strcpy(elementHolder[arraySizer],field2);
                arraySizer++;
                field2= strtok(NULL, splitter);
            }
            fgets(line, 1024, stream);  //printing all lines except first line
        }
        
        
        //Main part for printing to xml file
        if(control == 1)
        {
            
            printf("<row id:%d>\n",count);
             fprintf(writed,"<row id:%d>\n",count);
            for (int i = 0; i <elementCounter; i++)
            {
                
                printf("<%s>",replace(elementHolder[i]," ","_"));
                fprintf(writed,"<%s>",replace(elementHolder[i]," ","_"));

                //rescuing from multiple seperator stuffs
                char* replaced = replace(line,concat(splitter,splitter),concat(concat(splitter,"-"),splitter));
                replaced = replace(replaced,concat(splitter," "),"-");  ///assigning "-" for empty attributes
                char* tmp = strdup(replaced);

                char * last = getfield(tmp ,i+1,splitter);

                if (last[strlen(last)-1] == '\n')
                {
                    last[strlen(last)-1] = '\0';
                    
                    if(last[strlen(last)-1] == '\r')
                    {
                     last[strlen(last)-1] = '\0';
                    }
                }

                else if(last[strlen(last)-1] == '\r')
                {
                    last[strlen(last)-1] = '\0';
                }

                
                printf("%s", last);
                fprintf(writed,"%s", last);
                printf("</%s>\n",replace(elementHolder[i]," ","_"));
                fprintf(writed,"</%s>\n",replace(elementHolder[i]," ","_"));
                tmp= strtok(NULL, splitter);
                
            }

             printf("</row>\n");
            fprintf(writed,"</row>\n");
             count++;  //increasing counter for row id
        
      
        
        }
        
        
    }
     printf("</%s>",xmlFile);
     fprintf(writed,"</%s>",xmlFile);
     printf("\n");
     printf("CONVERTING DONE !!!\n");
    }
}