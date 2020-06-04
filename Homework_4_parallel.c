#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timing.h"

// global variables
char words[466544][25]; //words in dictonary
int lengths[466544]; //lengths of words in dictonary
int task = 0; //count for current word in the dictonary
pthread_mutex_t lock;
int *results;
int tasks[26] = {0};

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

//reads the words from the dictonary into the array words
void readWords(){
    FILE *fp;
    fp = fopen("words.txt", "r");
    char c;
    int i, j = 0;
    int is_lowercase = 1;
    for(i=0; i<466544; i++){
        fscanf(fp, "%s", words[i]);
        c = words[i][0];
        if(c == 'y'){
            continue;
        }
        if(isLowercase(c)){
            if(tasks[c%97] == 0){
                tasks[c%97] = i;
            }
        }
    }
    tasks[24] = tasks[25];
    tasks[25] = 466544;
}

int getTask(){
    int current_task;
    pthread_mutex_lock(&lock);
    current_task = task;
    task++;
    pthread_mutex_unlock(&lock);
    return current_task;
}

//finds all of the unique words in the dictonary
void *findUniqueWords(void *arg){
    int *result = arg;
    int i, j=0, k;
    int uniqueCount = 0;
    char *word; //curent word from dictonary
    char letters[25]; //current letters in word checked 
    char c;//curent charecter in word
    int isUnique = 1; //1 if word has all unique letters; 0 otherwise
    int is_lowercase = 1; //1 is all letters in the word are lowercase letters; 0 otherwise
    //loops through all words in the dictonary
    while((i=getTask())<26){
        for(k=tasks[i]; k<tasks[i+1]; k++){
            word = words[k];
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
                lengths[k] = j;
                uniqueCount++;
            }

            j = 0;
            isUnique = 1;
            is_lowercase = 1;
        }
    }

    *result = uniqueCount;
}

//counts uniquie words and writes the words that are 6 or longer to the file results.txt
void outPut(){
    int i;
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

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("%s: Usage: %s <numbers of threads>\n", argv[0], argv[0]);
        exit(1);
    }

    int threadCount = atoi(argv[1]);
    pthread_t threads[threadCount-1];
    results = (int *)malloc(threadCount * sizeof(int));
    int i;
    int sum = 0;

    if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    readWords();

    timing_start();
    for(i=0; i<threadCount-1; i++){
        pthread_create(&threads[i], NULL, findUniqueWords, &results[i]);
    }

    findUniqueWords(&results[i]);

    for(i=0; i<threadCount-1; i++){
        pthread_join(threads[i], NULL);
    }
    timing_stop();

    for(i=0; i<threadCount; i++){
        sum += results[i];
    }

    printf("total unique words: %d\n", sum);
    print_timing();
    outPut();
    return 0;
}