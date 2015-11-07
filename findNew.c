// FindNew
// Author: Rixuel Patrick
// ---
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void enterToExit();
void discardExtraChar(char *fgetsStr);
void removeReturnLine(char *c);


typedef struct arrName {
	char name[200];
} arrayName;

void showUsage()
{
	printf("\nHELP\n\n");
	printf("Purpose of this tool:\n");
	printf("- List all the contents of a directory.\n");
	printf("- Allow you to find new added files/folders of the same directory.\n");
	printf("For example, you compare your old music folder with your new one.\n");
	printf("It should lists all the new added music files from your new music folder.\n");
	printf("NOTE: Do not use this tool to check diff!\n");
	
	printf("\nTerminal Usage: \n");
	printf("- Check in current Dir:\t ./checkThis \n");
	printf("- Check in a Dir: \t ./checkThis folder \n");
	printf("- Check for new files: \t ./checkThis oldlist.txt newlist.txt \n");
}

void enterToExit()
{
	char c[1];
	printf("\nPress ENTER to exit.\n");	
	fgets(c, sizeof(c), stdin);
	discardExtraChar(c);
	exit(0);
}

void discardExtraChar(char *fgetsStr)
{
	if(!strchr(fgetsStr, '\n')){
		//newline does not exist
		while(fgetc(stdin)!='\n');	//discard until newline
	}
}

void removeReturnLine(char *c)
{
	int len = strlen(c);
	if (len > 0 && c[len-1] == '\n') c[len-1] = '\0';
}

void makeLower(char *input)
{
    while (*input != '\0')
    {
        *input = tolower((unsigned char)*input);
        input++;
    }
}

void bubbleSort(int n, arrayName *b)
{
    arrayName temp;

    for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(strcmp(b[i].name, b[j].name) < 0)
			{
				temp = b[i];
				b[i] = b[j];
				b[j] = temp;
			}
		}
	}
}

int writeFile(char *str)
{
	char word[256];
	
	// Loop to prevent empty word.
	do{
		printf("\nName your file (.txt):\n");
		fgets(word, sizeof(word), stdin);
	}while(word[0] == '\n');
	discardExtraChar(word);
	removeReturnLine(word);
  
	// Concatenate word and ".txt". word.txt
	strcat(word,".txt");
	
	// File writting...
	FILE *f = fopen(word, "w");
	if (f == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	
	fprintf(f, "%s", str);
	printf("\nWritting in %s ...\n", word);
	printf("%s is in the same path as the program.\n", word);
	enterToExit();
	
	fclose(f);
	return 0;
}

static int goDir(char *dirname)
{
    DIR *dir = opendir(dirname);
    struct dirent *dit;
    struct stat st;
    char filePath[1024];
	char *str2 = (char *) malloc(sizeof(char) * 100);
	arrayName *str = (arrayName *) malloc(sizeof(arrayName) * 200);
	char answer[4];
	int c=0;
	
	// Do nothing if folder doesn't exist and show its usage.
	if (dir==NULL) 
	{
		printf("ERROR. Cannot read \"%s\"! \n", dirname);
		enterToExit();
		return 0;
	}

	// List files and directories.
    while ((dit = readdir(dir)) != NULL)
    {
		// Exclude directories ".." (parent) and "." (current).
        if ( (strcmp(dit->d_name, ".") == 0) || (strcmp(dit->d_name, "..") == 0) ) continue;

		// Get correctly file names and directory names.
        sprintf(filePath, "%s/%s", dirname, dit->d_name);
        if (lstat(filePath, &st) != 0) continue;
		
		// Extend Memory for each file/dir found.
		str2 = realloc(str2, 100*c);

        // Get names of files and dirs. S_ISDIR, S_ISREG, etc.
		strcat(str[c].name, dit->d_name);
		strcat(str[c].name, "\r\n");
		c++;
    }
	
	bubbleSort(c,str);
	
	printf("\nContents of the directory:\n\n");
	
	for(int i=0; i<c; i++){
		printf("%s", str[i].name);
		strcat(str2, str[i].name);
	}
	
	do{
		printf("\nDo you want to save to .txt file? (yes/no)\n");
		fgets(answer,sizeof(answer),stdin);
		makeLower(answer);
	}while(strcmp(answer,"\n")==0);
	discardExtraChar(answer);

	if(strcmp(answer,"yes")==0){
		writeFile(str2);
	}
	
	closedir(dir);
	free(str);
	free(str2);
    return 0;
}

void showNewFiles(FILE *fp1, FILE *fp2)
{
	char cbuff;
	int nlines = 0; //counter of new lines
	int chr = 0; //counter of chars (except new line char)
	int maxlen = 0;
	
	while( (cbuff = fgetc(fp1) - 0) != EOF )
	{
		if(cbuff == '\n')
		{
			if (chr > maxlen)
			{
				maxlen = chr + 1;
			}

			chr = 0;
			nlines++;
		}
		else
		{
			chr++;
		}
	}

	//printf( "lines: %d\nmax string len: %d\n\n", nlines, maxlen );
	rewind(fp1);

	char *list[nlines];
	int buffsize = maxlen * sizeof(int);
	char buff[buffsize];
	int i = 0;
	
	while(fgets(buff, buffsize, fp1))
	{
		list[i] = malloc(strlen(buff) * sizeof(char));
		strcpy(list[i], buff);
		i++;
	}
	
	/*
	printf("LIST:\n");
	for(int c=0; c < nlines; c++)
	{
		printf( "%s", list[c] );
	}
	*/
	
	////////////////////////////////////////////////////////
	
	char cbuff2;
	int nlines2 = 0; //counter of new lines
	int chr2 = 0; //counter of chars (except new line char)
	int maxlen2 = 0;
	
	while( (cbuff2 = fgetc(fp2) - 0) != EOF )
	{
		if(cbuff2 == '\n')
		{
			if (chr2 > maxlen2)
			{
				maxlen2 = chr2 + 1;
			}

			chr2 = 0;
			nlines2++;
		}
		else
		{
			chr2++;
		}
	}

	//printf("\nlines2: %d\nmax string len2: %d\n\n", nlines2, maxlen2 );
	rewind(fp2);

	char *list2[nlines2];
	int buffsize2 = maxlen2 * sizeof(int);
	char buff2[buffsize2];
	int j = 0;
	
	while(fgets(buff2, buffsize2, fp2))
	{
		list2[j] = malloc(strlen(buff2) * sizeof(char));
		strcpy(list2[j], buff2);
		j++;
	}
	
	/*
	printf("LIST2:\n");
	for(int c=0; c < nlines2; c++)
	{
		//bubbleSort(list2[c]);
		printf( "%s", list2[c] );
	}
	*/
	
	printf("\nSame contents from both files won't be displayed. \n");
	printf("Different contents will be listed below... \n\n");
	
	int a,b,ctemp=0,csame=0;
	if (nlines<=nlines2)
	{		
		// When 1st file is smaller than 2nd file or equal.
		// Each array case of the small list compare with each case of the big list.
		for(a=0; a<nlines; a++)
		{
			for(b=0; b<nlines2; b++)
			{
				if (strcmp(list[a], list2[b])==0)
				{
					list2[b]="";
					csame++;
				}
			}
			
			ctemp=csame;
			if(ctemp==0)
			{
				printf("%s", list[a]);
			}
			csame=0;
		}
		
		// Showing new contents of 2nd file.
		for(int i=0; i<nlines2; i++){
			printf("%s", list2[i]);
		}
	}
	else
	{
		// When 1st file is bigger than 2nd file.		
		// Each array case of the small list compare with each case of the big list.
		for(b=0; b<nlines2; b++)
		{
			for(a=0; a<nlines; a++)
			{
				if (strcmp(list[a], list2[b])==0)
				{
					list[a]="";
					csame++;
				}
			}
			ctemp=csame;
			if(ctemp==0)
			{
				printf("%s", list2[b]);
			}
			csame=0;
		}
		
		// Showing new contents of 1st file.
		for(int i=0; i<nlines; i++){
			printf("%s", list[i]);
		}
	}
	
	// Free Memories
	while(fgets(buff, buffsize, fp1))
	{
		free(list[i]);
	}
	
	while(fgets(buff2, buffsize2, fp2))
	{
		free(list2[i]);
	}
	
	enterToExit();
}



int compare(char *fname1, char *fname2)
{
	FILE *fp1 = fopen(fname1, "r"); 
	FILE *fp2 = fopen(fname2, "r");

	if (fp1 == NULL) 
	{
		printf("ERROR. Cannot read \"%s\"! \n", fname1);
		enterToExit();
		exit(1);
	} 
	if (fp2 == NULL)
	{
		printf("ERROR. Cannot read \"%s\"! \n", fname2);
		enterToExit();
		exit(1);
	} 	
	
	showNewFiles(fp1, fp2);
	
	fclose(fp1);
	fclose(fp2);
	return 0;
}


void exeProg()
{
	// Executable
	char choice[10];
	
	printf("+----------------------------+\n");
	printf("| FindNew by Rixuel Patrick  |\n");
	printf("+----------------------------+\n\n");
	printf("Menu:\n");
	printf(" 1 - List contents of current Directory. \n");
	printf(" 2 - List contents of selected Directory. \n");
	printf(" 3 - Compare two .txt files. \n");
	printf(" 4 - Help. \n");
	printf(" 0 - Exit program. \n");
	
	do{
		printf("\nYour choice: ");
		fgets(choice, sizeof(choice), stdin);
	}while(choice[0] == '\n');
	discardExtraChar(choice);
	removeReturnLine(choice);
	
	if (strcmp(choice,"1")==0)
	{
		goDir(".");  
	}
	else if (strcmp(choice,"2")==0)
	{
		char nameTheDir[256];
		
		do{
			printf("Enter the name of the directory: \n");
			fgets(nameTheDir, sizeof(nameTheDir), stdin);
		}while(nameTheDir[0] == '\n');
		discardExtraChar(nameTheDir);
		removeReturnLine(nameTheDir);
		
		goDir(nameTheDir);
	}
	else if (strcmp(choice,"3")==0)
	{
		char nameFile1[256];
		do{
			printf("Enter the name of the 1st file (name1.txt): \n");
			fgets(nameFile1, sizeof(nameFile1), stdin);
		}while(nameFile1[0] == '\n');
		discardExtraChar(nameFile1);
		removeReturnLine(nameFile1);
		
		char nameFile2[256];
		do{
			printf("Enter the name of the 2nd file (name2.txt): \n");
			fgets(nameFile2, sizeof(nameFile2), stdin);
		}while(nameFile2[0] == '\n');
		discardExtraChar(nameFile2);
		removeReturnLine(nameFile2);
		
		compare(nameFile1, nameFile2);
	}
	else if (strcmp(choice,"4")==0)
	{
		showUsage();
		enterToExit();
	}
	else
	{
		exit(0);
	}
}


int main(int argc, char *argv[])
{
	if (argc == 1)
    {
        exeProg();
    }
    else if (argc == 2)
    {
        goDir(argv[1]);
    }
	else if (argc == 3)
    {
		compare(argv[1], argv[2]);
    }
	else
	{
		showUsage();
	}
	
    return 0;
}


