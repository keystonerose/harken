#ifndef TESTER_H
#define TESTER_H

#include <memory>
#include <sstream>
#include <string>
#include <vector>

/**
 * Provides functionality for the comparing actual behaviour of program units with their expected
 * behaviour, and keeping track of the results of these tests for display.
 */

class Tester {
public:
    
    /**
     * Constructs the Tester with a title string to be displayed at the top of the test report.
     */
    
    explicit Tester(const std::string& title);
    
    /**
     * Uses <tt>T::operator==()</tt> to compare the values of @p expected and @p actual. A row with
     * title @p title is added to the Tester's report to indicate the outcome of this comparison; 
     * this will be either a SuccessRecord or a FailureRecord (with information about the expected
     * and actual values in the latter case).
     */
    
    template<typename ExpectedType, typename ActualType>
    void assertEqual(const std::string& title, const ExpectedType& expected, const ActualType& actual) {
        
        if (expected == actual) {
            m_records.push_back(std::make_unique<SuccessRecord>(title));
        }
        else {
            
            std::ostringstream expectedStream;
            expectedStream << expected;
            
            std::ostringstream actualStream;
            actualStream << actual;
            
            m_records.push_back(std::make_unique<FailureRecord>(title, expectedStream.str(), actualStream.str()));
            ++m_failureCount;
        }
    }
    
    /**
     * Prints a formatted test report to the output stream @p os, describing the outcome of all 
     * assertEqual() tests and providing information about any errors that occurred. 
     */
    
    void printReport(std::ostream& os) const;
    
private:
    
    /**
     * Polymorphic base class describing the outcome of a single Tester assertion. Contains
     * descriptive information about how the test record should be displayed, including a bullet 
     * string to mark the record in the output and a short title to indicate what was being tested.
     * Also provides an @c m_message member that can be written to by derived classes to carry
     * additional information about the test record when it is displayed.
     */
    
    class Record {
    public:
        
        /**
         * Constructs the test record with some basic display strings.
         * @param bullet A short indicator of the type (e.g. success, failure) of the test record.
         * @param title A short heading to indicate what was being tested.
         */
        
        Record(const std::string& bullet, const std::string& title);
        
        virtual ~Record() = default;
        
        /**
         * Prints a single line to @p os indicating the pass/fail status, title and (if applicable)
         * message for the test that this Record describes.
         */
        
        void print(std::ostream& os) const;
        
    protected:
        
        std::string m_bullet;
        std::string m_message;
        std::string m_title;
    };
    
    class FailureRecord : public Record {
    public:
        FailureRecord(const std::string& title, const std::string& expected, const std::string& actual);
    };
    
    class SuccessRecord : public Record {
    public:
        SuccessRecord(const std::string& title);
    };
    
    int m_failureCount = 0;
    std::string m_title;
    std::vector<std::unique_ptr<Record>> m_records;
};

#endif
