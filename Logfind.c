#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"


int findWordInBatch(int *argumentFoundChecklist, int index, int isOr, char *word, char *file, char *batchString){

	int wordsFound = 0;

	if(argumentFoundChecklist[index] == 0){

		char *stringPresent = strstr(batchString, word);

		if(stringPresent){

			if(isOr) printf("Word \"%s\" found in %s\n", word, file);
			argumentFoundChecklist[index] = 1;
			wordsFound += 1;
		}
	}
	return wordsFound;
}

void searchFiles(int batchSize, char *file, char *arguments[], int argumentCount, int longestWordIndex, int isOr){

	int argumentFoundChecklist[argumentCount];
	memset(argumentFoundChecklist, 0, sizeof argumentFoundChecklist);

	FILE *fp;
	fp = fopen (file, "r");

	if(fp == NULL) {
		printf("%s not found\n", file);
		return;
	}

	int wordsFound = 0;
	int isFirstIteration = 1;
	
	while(!feof((FILE*)fp)){

		//In case word overlaps batch boundary, start new batch a few places back
		if(isFirstIteration != 1){
			fseek(fp, -strlen(arguments[longestWordIndex]), SEEK_CUR );
		}

		isFirstIteration = 0;


		char buff[batchSize];
		char *batchString = fgets(buff, batchSize, (FILE*)fp);

		int startingIndex = 2;

		if(isOr){
			startingIndex = 3;
		} 

		for(int j = startingIndex; j < argumentCount; j++){

			wordsFound += findWordInBatch(&argumentFoundChecklist[0], j, isOr, arguments[j], file, batchString);
		
		}

		if(wordsFound == argumentCount - startingIndex){
			if(!isOr) printf("Words found in %s\n", file);
			fclose(fp);
			return;
		} 
	}

	printf("Not all words were found in %s\n", file);
	fclose(fp);
	return;

}

int getLongestArgumentIndex(char *arguments[], int argumentCount, int batchSize, int startingIndex){

	int maxArgumentLength = 0;

	int maxArgumentIndex = 0;

	for(int i = startingIndex; i < argumentCount; i++){

		if(strlen(arguments[i]) > maxArgumentLength) {
			maxArgumentLength = strlen(arguments[i]);
			maxArgumentIndex = i;
		}
	}

	if(maxArgumentLength > batchSize - 1){
		printf("Arguments must be under %d characters.", batchSize);
		return 0 ;
	}

	return maxArgumentIndex;


}

int main(int argc, char *argv[])
{
	char *filePaths[3];
	filePaths[0]="logfiles/file.txt";
	filePaths[1]="logfiles/file2.txt";
	filePaths[2]="logfiles/file3.txt";

	int filesPathsCount = sizeof(filePaths)/sizeof(filePaths[0]);

	int batchSize = atoi(argv[1]);

	if(batchSize == 0){
		printf("Invalid batch size for your first parameter");
		return 1;
	}

	int isOrFlag = 0;

	int startingIndexForWordParameters = 2;

	if(strcmp(argv[2],"-o") == 0){

		isOrFlag = 1;

		startingIndexForWordParameters = 3;

	} 

	int longestWordIndex = getLongestArgumentIndex(argv, argc, batchSize, startingIndexForWordParameters);

	if(longestWordIndex == 0){
		return 1;
	}

	for(int i = 0; i < filesPathsCount; i++){

		searchFiles(batchSize, filePaths[i], argv, argc, longestWordIndex, isOrFlag);

	}

	return(0);
}