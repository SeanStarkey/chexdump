#include <stdio.h>
#include "hexdump.h"

#include <CUnit/Basic.h>

#define OUTBUFFERSIZE 1024

static FILE* outFile = NULL;
static char outBuffer[OUTBUFFERSIZE];
static char testBuffer[256];

int initOutFile()
{
    if (NULL == (outFile = fopen("build/out.txt", "w"))) {
        return -1;
    }
    else {
        return 0;
    }
}

int closeOutFile()
{
   if (0 != fclose(outFile)) {
      return -1;
   }
   else {
      outFile = NULL;
      return 0;
   }
}

int readOutFile()
{
    int bytesRead;
    int cnt;

    if (NULL == (outFile = fopen("build/out.txt", "r"))) {
        return -1;
    }

    memset(outBuffer, 0, OUTBUFFERSIZE);

    bytesRead = fread(outBuffer, 1, OUTBUFFERSIZE, outFile);

    if (0 != fclose(outFile)) {
        return -1;
    }
    else {
        outFile = NULL;
    }
    return bytesRead;
}

void test_ptrToFile_OneLine()
{
    int bytesRead;
    char l1[] = "00000000  00 01 02 03 04 05                                 ......          \n";

    initOutFile();
    ptrhexdumpToFile(outFile, testBuffer, 6);
    closeOutFile();
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_ptrToFile_TwoLines()
{
    int bytesRead;
    char l1[] = "00000000  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ................\n";
    char l2[] = "00000010  10 11 12 13                                       ....            \n";
    char compareBuffer[1024];

    initOutFile();
    ptrhexdumpToFile(outFile, testBuffer, 20);
    closeOutFile();
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}

void test_ptrToStr_OneLine()
{
    char l1[] = "00000000  00 01 02 03 04 05                                 ......          \n";
    char outputBuffer[1024];
    ptrhexdumpToStr(outputBuffer, 1024, testBuffer, 6);

    CU_ASSERT_STRING_EQUAL(l1, outputBuffer);
}

void test_ptrToStr_TwoLines()
{
    char l1[] = "00000000  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ................\n";
    char l2[] = "00000010  10 11 12 13                                       ....            \n";
    char compareBuffer[1024];
    char outputBuffer[1024];
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    ptrhexdumpToStr(outputBuffer, 1024, testBuffer, 20);

    CU_ASSERT_STRING_EQUAL(compareBuffer, outputBuffer);
}

void test_fileToFile_OneLine()
{
    int bytesRead;
    char l1[] = "00000000  31 32 33 34 35 36                                 123456          \n";
    FILE* dataFile;

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdumpToFile(outFile, dataFile, 0, SEEK_SET, 6);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_fileToFile_TwoLines()
{
    int bytesRead;
    char l1[] = "00000000  31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47   123456789ABCDEFG\n";
    char l2[] = "00000010  48 49 4A 4B 4C 4D 4E 4F 50                        HIJKLMNOP       \n";
    FILE* dataFile;
    char compareBuffer[1024];

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdumpToFile(outFile, dataFile, 0, SEEK_SET, 25);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}

void test_fileToFile_OneLineOffset()
{
    int bytesRead;
    char l1[] = "00000002        33 34 35 36 37 38 39 41                       3456789A      \n";
    FILE* dataFile;

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdumpToFile(outFile, dataFile, 2, SEEK_SET, 8);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_fileToFile_TwoLinesOffset()
{
    int bytesRead;
    char l1[] = "00000006                    37 38 39 41 42 43 44 45 46 47         789ABCDEFG\n";
    char l2[] = "00000010  48 49 4A 4B 4C 4D 4E 4F 50                        HIJKLMNOP       \n";
    FILE* dataFile;
    char compareBuffer[1024];

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdumpToFile(outFile, dataFile, 6, SEEK_SET, 25);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}

void test_fileToStr_OneLine()
{
    char l1[] = "00000000  31 32 33 34 35 36                                 123456          \n";
    FILE* dataFile;

    dataFile = fopen("test/test.txt", "r");
    char outputBuffer[1024];
    filehexdumpToStr(outputBuffer, 1024, dataFile, 0, SEEK_SET, 6);
    fclose(dataFile);

    CU_ASSERT_STRING_EQUAL(l1, outputBuffer);
}



int ptrToFile_SuiteInit(void)
{
    int cnt;

    for (cnt=0; cnt<255; cnt++)
    {
        testBuffer[cnt] = cnt;
    }
    return 0;
}

int ptrToFile_SuiteCleanup(void)
{
    return 0;
}

int ptrToStr_SuiteInit(void)
{
    int cnt;

    for (cnt=0; cnt<255; cnt++)
    {
        testBuffer[cnt] = cnt;
    }
    return 0;
}

int ptrToStr_SuiteCleanup(void)
{
    return 0;
}

int fileToFile_SuiteInit(void)
{
    return 0;
}

int fileToFile_SuiteCleanup(void)
{
    return 0;
}

int fileToStr_SuiteInit(void)
{
    return 0;
}

int fileToStr_SuiteCleanup(void)
{
    return 0;
}


int setup_PtrToFile_Suite()
{
   CU_pSuite ptrToFile_Suite;

   ptrToFile_Suite = CU_add_suite("PtrToFile Suite", ptrToFile_SuiteInit, NULL);
   if (NULL == ptrToFile_Suite) {
      return -1;
   }

   if ((NULL == CU_add_test(ptrToFile_Suite, "ptr to file - one line", test_ptrToFile_OneLine)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(ptrToFile_Suite, "ptr to file - two lines", test_ptrToFile_TwoLines)))
   {
      return -1;
   }

   return 0;
}

int setup_PtrToStr_Suite()
{
    CU_pSuite ptrToStr_Suite;

    ptrToStr_Suite = CU_add_suite("PtrToStr Suite", ptrToStr_SuiteInit, NULL);
    if (NULL == ptrToStr_Suite) {
      return -1;
    }

    if ((NULL == CU_add_test(ptrToStr_Suite, "ptr to str - one line", test_ptrToStr_OneLine)))
    {
        return -1;
    }
    if ((NULL == CU_add_test(ptrToStr_Suite, "ptr to str - two lines", test_ptrToStr_TwoLines)))
    {
        return -1;
    }

    return 0;
}

int setup_FileToFile_Suite()
{
   CU_pSuite fileToFile_Suite;

   fileToFile_Suite = CU_add_suite("FileToFile Suite", fileToFile_SuiteInit, fileToFile_SuiteCleanup);
   if (NULL == fileToFile_Suite) {
       return -1;
   }

   if ((NULL == CU_add_test(fileToFile_Suite, "file to file - one line", test_fileToFile_OneLine)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileToFile_Suite, "file to file - two lines", test_fileToFile_TwoLines)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileToFile_Suite, "file to file - one line with offset", test_fileToFile_OneLineOffset)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileToFile_Suite, "file to file - two lines with offset", test_fileToFile_TwoLinesOffset)))
   {
      return -1;
   }

   return 0;
}


int setup_FileToStr_Suite()
{
   CU_pSuite fileToStr_Suite;

   fileToStr_Suite = CU_add_suite("FileToStr Suite", fileToStr_SuiteInit, fileToStr_SuiteCleanup);
   if (NULL == fileToStr_Suite) {
       return -1;
   }
   if ((NULL == CU_add_test(fileToStr_Suite, "file to str - one line", test_fileToStr_OneLine)))
   {
      return -1;
   }

   return 0;
}


int main()
{
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   if (setup_PtrToFile_Suite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }
   if (setup_PtrToStr_Suite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }
   if (setup_FileToFile_Suite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }
   if (setup_FileToStr_Suite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }

   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

