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

void test_ptrOneLine()
{
    int bytesRead;
    char l1[] = "00000000  00 01 02 03 04 05                                 ......          \n";

    initOutFile();
    ptrhexdump(outFile, testBuffer, 6);
    closeOutFile();
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_ptrTwoLines()
{
    int bytesRead;
    char l1[] = "00000000  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ................\n";
    char l2[] = "00000010  10 11 12 13                                       ....            \n";
    char compareBuffer[1024];

    initOutFile();
    ptrhexdump(outFile, testBuffer, 20);
    closeOutFile();
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}

void test_fileOneLine()
{
    int bytesRead;
    char l1[] = "00000000  31 32 33 34 35 36                                 123456          \n";
    FILE* dataFile;

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdump(outFile, dataFile, 0, SEEK_SET, 6);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_fileTwoLines()
{
    int bytesRead;
    char l1[] = "00000000  31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47   123456789ABCDEFG\n";
    char l2[] = "00000010  48 49 4A 4B 4C 4D 4E 4F 50                        HIJKLMNOP       \n";
    FILE* dataFile;
    char compareBuffer[1024];

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdump(outFile, dataFile, 0, SEEK_SET, 25);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}

void test_fileOneLineOffset()
{
    int bytesRead;
    char l1[] = "00000002        33 34 35 36 37 38 39 41                       3456789A      \n";
    FILE* dataFile;

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdump(outFile, dataFile, 2, SEEK_SET, 8);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();

    CU_ASSERT_EQUAL_FATAL(77, bytesRead);
    CU_ASSERT_STRING_EQUAL(l1, outBuffer);
}

void test_fileTwoLinesOffset()
{
    int bytesRead;
    char l1[] = "00000006                    37 38 39 41 42 43 44 45 46 47         789ABCDEFG\n";
    char l2[] = "00000010  48 49 4A 4B 4C 4D 4E 4F 50                        HIJKLMNOP       \n";
    FILE* dataFile;
    char compareBuffer[1024];

    dataFile = fopen("test/test.txt", "r");
    initOutFile();
    filehexdump(outFile, dataFile, 6, SEEK_SET, 25);
    closeOutFile();
    fclose(dataFile);
    bytesRead = readOutFile();
    strcpy(compareBuffer, l1);
    strcat(compareBuffer, l2);

    CU_ASSERT_EQUAL_FATAL(154, bytesRead);
    CU_ASSERT_STRING_EQUAL(compareBuffer, outBuffer);
}


int ptrSuiteInit(void)
{
    int cnt;

    for (cnt=0; cnt<255; cnt++)
    {
        testBuffer[cnt] = cnt;
    }
    return 0;
}

int fileSuiteInit(void)
{
    return 0;
}

int fileSuiteCleanup(void)
{
    return 0;
}

int setupPtrSuite()
{
   CU_pSuite ptrSuite;

   ptrSuite = CU_add_suite("Ptr Suite", ptrSuiteInit, NULL);
   if (NULL == ptrSuite) {
      return -1;
   }

   if ((NULL == CU_add_test(ptrSuite, "test of ptr - one line", test_ptrOneLine)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(ptrSuite, "test of ptr - two lines", test_ptrTwoLines)))
   {
      return -1;
   }

   return 0;
}

int setupFileSuite()
{
   CU_pSuite fileSuite;

   fileSuite = CU_add_suite("File Suite", fileSuiteInit, fileSuiteCleanup);
   if (NULL == fileSuite) {
       return -1;
   }

   if ((NULL == CU_add_test(fileSuite, "test of file - one line", test_fileOneLine)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileSuite, "test of file - two lines", test_fileTwoLines)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileSuite, "test of file - one line with offset", test_fileOneLineOffset)))
   {
      return -1;
   }
   if ((NULL == CU_add_test(fileSuite, "test of file - two lines with offset", test_fileTwoLinesOffset)))
   {
      return -1;
   }

   return 0;
}

int main()
{
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   if (setupPtrSuite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }
   if (setupFileSuite() != 0)
   {
       CU_cleanup_registry();
       return CU_get_error();
   }

   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

