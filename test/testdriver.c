#include <stdio.h>
#include "hexdump.h"

#include <CUnit/Basic.h>

static FILE* outFile = NULL;
static char outBuffer[1024];
static char testBuffer[256];

int initOutFile()
{
    if (NULL == (outFile = fopen("build/out.txt", "w+"))) {
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
    if (NULL == (outFile = fopen("build/out.txt", "r"))) {
        return -1;
    }

    bytesRead = fread(outBuffer, 1, 1024, outFile);

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

void test_file()
{
    /* FILE* out = fopen("out.txt", "w"); */
    /* FILE* out = stdout; */

    /* ptrhexdump(stdout, test, 208); */

    /* FILE* fp = fopen("test.txt", "r"); */
    /* ptrhexdump(temp_file, testBuffer, 0, SEEK_SET, 6); */
    /* fprintf(out, "------------------------------------------------\n"); */
    /* filehexdump(out, fp, 0, SEEK_SET, 25); */
    /* fprintf(out, "------------------------------------------------\n"); */
    /* filehexdump(out, fp, 6, SEEK_SET, 25); */
    /* fprintf(out, "------------------------------------------------\n"); */
    /* filehexdump(out, fp, 1, SEEK_SET, 8); */
    /* fclose(fp); */
    /* fclose(out); */
}

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int ptrSuiteInit(void)
{
    int cnt;

    for (cnt=0; cnt<255; cnt++)
    {
        testBuffer[cnt] = cnt;
    }
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
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

   if ((NULL == CU_add_test(fileSuite, "test of file - one line", test_file)))
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

