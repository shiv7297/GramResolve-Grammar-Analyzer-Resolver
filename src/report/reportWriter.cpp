#include "reportWriter.h"
#include <iostream>

using namespace std;

// ======================================
// Constructor — Opens report file
// ======================================
ReportWriter::ReportWriter() {
    file.open("src/report/report.txt", ios::app);
    if (!file.is_open()) {
        cerr << "❌ Failed to open src/report/report.txt for writing.\n";
    }
}

// ======================================
// Destructor — Close safely
// ======================================
ReportWriter::~ReportWriter() {
    if (file.is_open()) {
        file.close();
    }
}

// ======================================
// Get Singleton Instance
// ======================================
ReportWriter& ReportWriter::get() {
    static ReportWriter instance;
    return instance;
}

// ======================================
// Append text to report file
// ======================================
ReportWriter& ReportWriter::operator<<(const string &text) {
    if (file.is_open()) {
        file << text;
        file.flush();              // flush after each write
    }
    return *this;
}

// ======================================
// Clear the report file (fresh output)
// ======================================
void ReportWriter::clear() {
    if (file.is_open()) {
        file.close();
    }

    file.open("src/report/report.txt", ios::trunc);
    if (!file.is_open()) {
        cerr << "❌ Failed to clear report file.\n";
    }
}
