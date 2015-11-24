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
    ** "testCounter" counts the test number for displaying the test number.
    ** 
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
    int testCounter = 1;
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
                std::cout << "Test " << testCounter << ": FAIL" << std::endl;
            else
                std::cout << "Test " << testCounter << ": PASS" << std::endl;
        }
        testCounter++;
    }
        
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
    ** "testCounter" counts the test number for displaying the test number. 
    **
    ** "testUrls" is the url queue used for testing.
    **
    ** "correctUrl" is the url queue containing test results of findUrls().
    **
    ** "correctTests" is the file containing correct results of findUrls(). 
    **
    ** "test1" is the first test case.
    **
    ** "test2" is the second test case.
    **
    ** "test3" is the third test case.
    **
    ** "testString" is a temporary string to hold correct tests from the file.
    */
    int testCounter = 1;
    std::queue<url> testUrls, correctUrls;
    std::fstream correctTests;
    std::string test1 = "http://www.google.com/test/test1", 
                test2 = "https://www.reddit.com/test", 
                test3 = "test3",
                testString;
    
    
    findUrls(test1, testUrls, 30);
    findUrls(test2, testUrls, 30);
    
    correctTests.open("testing/correctFindUrls.txt");
    
    while(correctTests >> testString)
    {
        if(testString == testUrls.front().hostName)
        { 
            correctTests >> testString;
            
            if(testString == testUrls.front().subDirectory)
                std::cout << "Test " << testCounter << ": PASS" << std::endl;
            else
                std::cout << "Test " << testCounter << ": FAIL" << std::endl;
        }
        else
        {
            correctTests >> testString;
            std::cout << "Test " << testCounter << ": FAIL" << std::endl;
        }
        testUrls.pop();
        testCounter++;
    }
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
    
    std::cout << "\n\nTesting findEmails():\n" << std::endl;
    testFindEmails();
    
    std::cout << "\n\nTesting findUrls():\n" << std::endl;
    testFindUrls();
}

#endif
