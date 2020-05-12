#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *stringDuplicator(char *str, int number_of_times) {
    assert(str);
    int input_length = strlen(str);
    char *duplicated_string = malloc(input_length * number_of_times + 1);
    if(duplicated_string == NULL) {
        return NULL;
    }
    char *duplicated_string_head = duplicated_string;

    if(number_of_times > 0) {
        for (int i = 0; i < number_of_times; i++) {
            strcpy(duplicated_string, str);
            duplicated_string = duplicated_string + input_length;
        }
    }

    *duplicated_string = '\0'; //terminate the new string at the end of it.
    return duplicated_string_head;
}
