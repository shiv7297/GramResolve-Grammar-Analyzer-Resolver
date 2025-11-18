#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

#include <fstream>
#include <string>

class ReportWriter {
private:
    std::ofstream file;

    // Private constructor (Singleton)
    ReportWriter();

public:
    // Destroy file safely
    ~ReportWriter();

    // Access global instance
    static ReportWriter& get();

    // Append text to report
    ReportWriter& operator<<(const std::string &text);

    // Clear the report (start fresh)
    void clear();
};

#endif
