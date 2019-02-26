#include "strfilter.h"
char* strfilter(char *string, char *letters){
    int size = strlen(string)+1;
    char * filtered_string = (char*) malloc(sizeof(char)*size);
    int num = 0;
    while(*string!='\0'){
        char temp = *string;
        char* pos = strchr(letters, temp);
        if(pos){
            filtered_string[num] = temp;
            num++;
        }
        string++;
    }
    filtered_string[num] = '\0';
    return filtered_string;
}

int main()
{
    printf("Hello World\n");

    char test_string[] = "abebe";
    char test_letters[] = "aeiou";

    char *result;
    result = strfilter(test_string, test_letters);
    printf("%s\n", result);

    return 0;
}
