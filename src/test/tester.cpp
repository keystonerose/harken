#include "tester.h"

#include <iomanip>

Tester::Record::Record(const std::string& bullet, const std::string& title)
    : m_bullet{bullet}, m_title{title} {
}

void Tester::Record::print(std::ostream& os) const {
    
    os << "[" << m_bullet << "] " << m_title;
    if (!m_message.empty()) {
        os << ": " << m_message;
    }
    os << std::endl;
}

Tester::FailureRecord::FailureRecord(const std::string& title, const std::string& expected, const std::string& actual)
    : Record{"!!", title} {
        
    std::ostringstream oss;
    oss << "expected " << expected << "; got " << actual;
    m_message = oss.str();
}

Tester::SuccessRecord::SuccessRecord(const std::string& title)
    : Record{"ok", title} {
}

Tester::Tester(const std::string& title)
    : m_title{title} {
}

void Tester::printReport(std::ostream& os) const {
    
    const auto testCount = m_records.size();
    if (!testCount) {
        return;
    }
    
    const auto osFlags = os.flags();

    const auto heading = "Testing " + m_title;
    os << heading << std::endl;
    for (auto i = 0; i < heading.size(); ++i) {
        os << '-';
    }
    os << std::endl;
    
    for (const auto& record : m_records) {
        record->print(os); 
    }
    
    const auto successCount = m_records.size() - m_failureCount;
    os << successCount << "/" << testCount << " " << (successCount == 1 ? "test" : "tests") << " passed";
    
    const auto passRate = static_cast<float>(successCount) / static_cast<float>(testCount) * 100.0f;
    os << std::fixed << std::setprecision(0);
    os << " (" << passRate << "%)";
    os << std::endl;
    
    os.flags(osFlags);
}

