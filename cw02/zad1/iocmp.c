#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "iocmp.h"

int sys = 0;

RecordFile newRecordFile (const char *name, int size, int count) {
   RecordFile file; 
   file.name = name;
   file.size = size;
   file.count = count;
   return file;
}

char outprint[1000000] = {0};

void lib_generate (RecordFile recordFile) {

    recordFile.file = fopen (recordFile.name, "w");
    if (recordFile.file == NULL)
	perror ("Error while opening the file.");

    int size = recordFile.count * (recordFile.size + 1);
    char out[recordFile.count][recordFile.size + 1];
    for (int i = 0; i < recordFile.count; i++) {
	for (int j = 0; j <= recordFile.size; j++) {
	    out[i][j] = rand() % 26 + 65;
	}
	out[i][recordFile.size] = '\n';
    }

    if (sys) {
	write (resultFile, out, size);
	close (randomFile);
	close (resultFile);
    } else {
	fwrite (out, 1, size, resultFile);
	fclose (randomFile);
	fclose (resultFile);
    }
}

void lib_print (RecordFile recordFile) {
    FILE *file = fopen (recordFile.name, "r");
    if (file) {
		fread (outprint, 1, 100000, file);
		printf ("%s", outprint);
    }
    fclose (file);
}

int lib_compare (RecordFile recordFile, int i, int j) {
    char iVal[1];
    char jVal[1];
    fseek (file, i * (recordFile.size + 1), 0);
    fread (iVal, sizeof(char), 1, file);
    fseek (file, j * (recordFile.size + 1), 0);
    fread (jVal, sizeof(char), 1, file);
    return jVal[0] - iVal[0];
}

void lib_swap (RecordFile recordFile, int i, int j) {
    char iRecord[recordFile.size];
    char jRecord[recordFile.size];
    fseek (file, i * (recordFile.size + 1), 0);
    fread (iRecord, sizeof(char), recordFile.size, file);
    fseek (file, j * (recordFile.size + 1), 0);
    fread (jRecord, sizeof(char), recordFile.size, file);
    fseek (file, i * (recordFile.size + 1), 0);
    fwrite (jRecord, recordFile.size, 1, file);
    fseek (file, j * (recordFile.size + 1), 0);
    fwrite (iRecord, recordFile.size, 1, file);
}

void lib_sort (RecordFile recordFile) {
    recordFile.file = fopen (recordFile.name, "r+");
    if (recordFile.file != NULL) {
	/// SORTING   ///////////////////////////////////////////
	for (int i = 1; i < recordFile.count; i++) {
	    int j = i - 1;
	    while (j >= 0 && compare (recordFile, j, j+1) < 0) {
		swap (recordFile, j, j+1);
		j -= 1;
	    }
	}
	////////////////////////////////////////////////////////
    }
    fclose (recordFile.file);
}

void lib_copy (RecordFile recordFile1, RecordFile recordFile2, int linesToCopy) {
    FILE *file1 = fopen (recordFile1.name, "r");
    FILE *file2 = fopen (recordFile2.name, "a");
    assert (file1 && file2);
    assert (recordFile1.size == recordFile2.size);

    unsigned long bufferSize = recordFile1.size + 1;
    char buffer[recordFile1.size + 1];
    
    for (int i = 0; i < linesToCopy; i++) {
	fread (buffer, bufferSize, 1, file1);
	fwrite (buffer, bufferSize + 1, 1, file2);
    }	    
    fclose (file1);
    fclose (file2);
}
