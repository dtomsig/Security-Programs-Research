/*******************************************************************************
* FILE: test.h                                                                 *
*                                                                              *                                                                                     *
* DESCRIPTION: test.hpp is a header file for testing functions in spider.cpp.  *
* To run tests, simply include test.hpp and call runTests() from within the    *
* main. This is already done by calling ./emailSpider --test.                  *                                                                        *
*                                                                              *
* OUTPUT FILE: None                                                            *
*******************************************************************************/

#ifndef TEST_HPP
#define TEST_HPP

#include "spider.hpp"

/*******************************************************************************
* FUNCTION: testFindEmails()                                                   *
*                                                                              * 
* DESCRIPTION: testFindEmails() tests to see if findEmails() finds the correct *
* email addresses from a variety of test cases. The corect solutions have been *
* manually extracted from the test strings and placed in a text file. This     *
* file is compared to the one that is generated by findEmails() to test for    *
* accuracy. The result is printed.                                             *
*                                                                              *
* PARAMETERS: None                                                             *
*******************************************************************************/


void testFindEmails()
{
    /*
    ** "parityFlag" represents whether or not the file produced by 
    ** findEmails() matches a predetermined test file.
    **
    ** "testFile" is the file produced by findEmails().
    **
    ** "correctTests" is the file containing correct results of findEmails();
    **
    ** "test1" is the first test case.
    **
    ** "test2" is the second test case.
    **
    ** "test3" is the third test case.
    **
    ** "testString" is next string that is pulled out of the testFile.
    **
    ** "correctString" is the next string that is pulled out of correctTests.
    */
    
    int parityFlag = 1;
    std::fstream testFile, correctTests;
    std::string test1 = "gwis@gmail.com test2@yahoo.com",
                test2 = "test",
                test3 = "capachone@gmail.com",
                testString,
                correctString;   
                
    testFile.open("testing/testFindEmails.txt", std::ios::out);
    correctTests.open("testing/correctFindEmails.txt");
  
    findEmails(test1, testFile);
    findEmails(test2, testFile);
    findEmails(test3, testFile);
    
    testFile.close();
    testFile.open("testing/testFindEmails.txt");
    
    while(testFile >> testString)
    {
        if(correctTests >> correctString)
        {
            if(testString != correctString)
                parityFlag = 0;
        }
        else
        {
            parityFlag = 0;
            break;
        }
    }
    
    if(parityFlag)
        std::cout << "Test: PASSED\n" << std::endl;
    else
        std::cout << "Test: FAILED\n" << std::endl;
    
    
    testFile.close();
    correctTests.close();
}

/*******************************************************************************
* FUNCTION: testFindUrls()                                                     *
*                                                                              *
* DESCRIPTION: testFindUrls() tests to see if findUrls() finds the correct     *
* urls from a variety of test cases. The corect solutions have been            *
* manually extracted from the test strings and placed in a text file. This     *
* file is compared to the one that is generated by findUrls() to test for      *
* accuracy. The result is printed.                                             *
*                                                                              *
* PARAMETERS: None                                                             *
*******************************************************************************/

void testFindUrls()
{
    /*
    ** "parityFlag" represents whether or not the file produced by 
    ** findEmails() matches a predetermined test file.
    **
    ** "testUrls" is the url queue used for testing.
    **
    ** "correctUrl" is the url queue containing correct results of findUrls();
    **
    ** "test1" is the first test case.
    **
    ** "test2" is the second test case.
    **
    ** "test3" is the third test case.
    */
    
    int parityFlag = 1;
    std::queue<url> testUrls, correctUrls;
    std::string test1 = "test", 
                test2 = "test2", 
                test3 = "test3";
            
    findUrls(test1, testUrls, 30);
    findUrls(test2, testUrls, 30);
    findUrls(test3, testUrls, 30);
    
}

/*******************************************************************************
* FUNCTION: testObtainGetResponse()                                            *
*                                                                              * 
* DESCRIPTION: testObtainGetResponse() tests to see if obtainGetResponse()     *
* finds the correct urls from a variety of test cases. The corect solutions    *
* have been manually extracted from the test strings and placed in a text      *
* file. This file is compared to the one that is generated by findUrls()       *
* to test for accuracy. The result is printed.                                 *
*                                                                              *
* PARAMETERS: None                                                             *
*******************************************************************************/

void testObtainGetResponse()
{
    std::string testSite1 = "", 
                testSite2 = "";
}

void runTests()
{
    std::cout << "\n\nBeginning testing of emailSpider functions." << std::endl;
    
    std::cout << "\n\nTesting findEmails():" << std::endl;
    testFindEmails();
    
    std::cout << "\n\nTesting findUrls():" << std::endl;
}

#endif
