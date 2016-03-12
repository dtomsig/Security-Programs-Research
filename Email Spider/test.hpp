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
    ** "numTests" represents the total number of expected tests.
    **
    ** "testFile" is the file produced by findEmails().
    **
    ** "correctFile" is the file containing correct results of findEmails().
    **
    ** "test1" -> "test5" are the test cases.
    **
    ** "testString" is next string that is pulled out of the testFile.
    **
    ** "correctString" is the next string that is pulled out of correctTests.
    */
    int testCounter = 1, numTests;
    std::fstream testFile, correctFile;
    std::string test1 = "gwis@gmail.com test2@yahoo.com",
                test2 = "test this thing",
                test3 = "capachone@gmail.com",
                test4 = "a@wwwwww.0000",
                test5 = "chapo.guzman@root.de",
                testString,
                correctString;   
                
    testFile.open("testing/testFindEmails.txt", std::ios::in | std::ios::out);
    correctFile.open("testing/correctFindEmails.txt");
  
    findEmails(test1, testFile);
    testFile << "#\n";
    findEmails(test2, testFile);
    testFile << "#\n";
    findEmails(test3, testFile);
    testFile << "#\n";
    findEmails(test4, testFile);
    testFile << "#\n";
    findEmails(test5, testFile);
    testFile << "#\n";
    numTests = 5;
    
    testFile.clear();
    testFile.seekg(0, std::ios::beg);
    
    std::cout << std::setw(18) << "EXPECTED" << std::setw(32) << "ACTUAL " 
              << std::setw(28) << " STATUS\n" << std::endl;
    
    do
    {
        testFile >> testString;
        correctFile >> correctString;
        
        std::cout << "Test " << testCounter << ":  " << std::endl;

        while(testString != "#" || correctString != "#")
        {                
            if(testString != correctString)
                std::cout << "          " << std::left << std::setw(33)
                          << correctString << std::setw(28) << testString
                          << "FAIL" << std::endl;
            else
                std::cout << "          " << std::left << std::setw(33) 
                          << correctString << std::setw(28) << testString
                          << "PASS" << std::endl;
                          
            if(testString != "#")
                testFile >> testString;
            
            if(correctString != "#")
                correctFile >> correctString;
        }
        
        testCounter++;
        std::cout << std::endl;
    }while(!(testCounter > numTests));
        
    testFile.close();
    correctFile.close();
    
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
    ** "testUrls" is the url deque used for testing.
    **
    ** "correctUrl" is the url deque containing test results of findUrls().
    **
    ** "correctTests" is the file containing correct results of findUrls(). 
    **
    ** "test1" -> "test7" are the test cases.
    **
    ** "testString" is a temporary string to hold correct tests from the file.
    */
    int testCounter = 1;
    std::deque<url> testUrls, correctUrls;
    std::fstream correctFile;
    std::string test1 = "http://www.google.com/test/test1", 
                test2 = "https://www.reddit.com/test", 
                test3 = "test3",
                test4 = "http://www.test4.com",
                test5 = "http://www.test5.com/one/",
                test6 = "www.example.com",
                test7 = "www.example.com/",
                test8 = "http://fonts.googleapis.com/css?family=Fenix",
                testString;
    
    
    findUrls(test1, testUrls);
    findUrls(test2, testUrls);
    findUrls(test3, testUrls);
    findUrls(test4, testUrls);
    findUrls(test5, testUrls);
    findUrls(test6, testUrls);
    findUrls(test7, testUrls);
    findUrls(test8, testUrls);
    
    correctFile.open("testing/correctFindUrls.txt");    
    
    std::cout << std::right << std::setw(18) << "EXPECTED" << std::setw(32)
              << "ACTUAL " << std::setw(28) << " STATUS\n" << std::endl;
    
    while(correctFile >> testString)
    {
        std::cout << "\nTest " << testCounter << ":  " << std::endl;
        if(testString == "#")
        {
            std::cout << std::right << std::setw(75) << "PASS" << std::endl;
            testCounter++;
            continue;
        }
        if(testString == testUrls.front().hostName)
        { 
            std::cout << "          " << std::left << std::setw(33) 
                      << testString << std::setw(28) 
                      << testUrls.front().hostName << "PASS" << std::endl;
            correctFile >> testString;

            if(testString == testUrls.front().subDirectory)
                std::cout << "          " << std::left << std::setw(33) 
                          << testString << std::setw(28) 
                          << testUrls.front().subDirectory << "PASS"
                          << std::endl;
            else
                std::cout << "           " << std::left << std::setw(33) 
                      << testString << std::setw(28) 
                      << testUrls.front().subDirectory << "FAIL" << std::endl;
        }
        else
        {
            std::cout << "          " << std::left << std::setw(33) 
                      << testString << std::setw(28) 
                      << testUrls.front().hostName << "FAIL" << std::endl;
            correctFile >> testString;
            std::cout << "          " << std::left << std::setw(33) 
                      << testString << std::setw(28) 
                      << testUrls.front().subDirectory << "FAIL" << std::endl;
        }
        testUrls.pop_front();
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
