#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timing.h"

// global variables
char words[466544][25]; //words in dictonary
int lengths[466544]; //lengths of words in dictonary
int count = 0;

//reads the words from the dictonary into the array words
void readWords(){
    FILE *fp;
    fp = fopen("words.txt", "r");
    char *word = (char *)malloc(25 * sizeof(char));
    int i;
    for(i=0; i<466544; i++){
        fscanf(fp, "%s", words[i]);
    }
}

//checks if letter already exsits in the word
int letterExsits(char *letters, char letter){
    int i=0;
    char c = letters[i];
    while(c != '\0'){
        if(c == letter){
            return 1;
        }
        c = letters[++i];
    }
    return 0;
}

//checks if the letters in the word are lowercase
int isLowercase(char c){
    if(c < 97 || c > 122){
        return 0;
    }
    else{
        return 1;
    }
}

//finds all of the unique words in the dictonary
void findUniqueWords(){
    int i, j=0;
    char *word; //curent word from dictonary
    char letters[25]; //current letters in word checked 
    char c;//curent charecter in word
    int isUnique = 1; //1 if word has all unique letters; 0 otherwise
    int is_lowercase = 1; //1 is all ;etters in the word are lowercase letters; 0 otherwise
    //loops through all words in the dictonary
    for(i=0; i<466544; i++){
        word = words[i];
        c = word[j];
        //checks if cuurent char is lowercase; if not goes to next word in dictonary
        if(!isLowercase(c)){
            continue;
        }
        letters[j] = c;
        letters[++j] = '\0';
        c = word[j];
        //loops until end of word
        while(c != '\0'){
            //checks if current char is a lowercase letter; if not sets is_lowercase to 0 (false) and exits loop
            if(!isLowercase(c)){
                is_lowercase = 0;
                break;
            }
            //checks if the letter is unigue in the word; if isUnique to 0 and exits loop
            else if(letterExsits(letters, c)){
                isUnique = 0;
                break;
            }
            //if lowercase and unique adds charecter to the list of charecters checked
            else{
                letters[j] = c;
                letters[++j] = '\0';
                c = word[j];
            }
        }

        //checks if all letters are unique and lowercase; if so sets length of word
        if(isUnique && is_lowercase){
            lengths[i] = j;
            count++;
        }
        j = 0;
        isUnique = 1;
        is_lowercase = 1;
    }
}

//counts uniquie words and writes the words that are 6 or longer to the file results.txt
void countUniqueWords(){
    int i, count=0;
    FILE *fp;
    fp = fopen("results.txt", "w");
    //loops through the lengths 
    for(i=0; i<466544; i++){
        //writes to results.txt if length is greater than 5
        if(lengths[i] >= 6){
            fprintf(fp, "%s%c", words[i], '\n');
        }
    }
}

int main(){
    readWords();
    timing_start();
    findUniqueWords();
    timing_stop();
    printf("total unique words: %d\n", count);
    print_timing();
    countUniqueWords();
    return 0;
}