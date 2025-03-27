#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <climits>
#include "MinimumOverlap.hpp"

/**
 * Passed the command-line arguments and verifies they are what
 * is expected.
 */
bool validArguments(int argc, char * const argv[], bool &doPrint);

/**
 * Parses a string of integers into a vector of integers.
 */
void createCover(const std::string& str, std::vector<unsigned int> &cover);

/**
 * If isStart is true, it records the current time and returns 0.
 * Otherwise, it returns the different from teh current time and the last
 * time it was called when startTime was false.
 */
inline double elapsedSeconds(bool isStart);

/**
 * Formats a a vector of integers to be printed.
 */
std::string coverToString(const std::vector<unsigned int> &cover);

int main(int argc, char *argv[]) {
    std::vector<double> timings;
    bool doPrint;

    if (!validArguments(argc, argv, doPrint)) {
        return 1;
    }

    // Get input file name with extension
    const std::string FILENAME_IN {argv[1]};
    const auto EXT_LOC = FILENAME_IN.rfind(".");
    const std::string FILENAME_OUT {
        (EXT_LOC == std::string::npos
            ? FILENAME_IN
            : FILENAME_IN.substr(0, EXT_LOC))
            + "-timings.dat"
    };

    std::cout << "Arguments were validated, calling constructor...\n";

    // Now we will time the preprocessor/constructor phase
    elapsedSeconds(true);
    MinimumOverlap minOverlap(FILENAME_IN);
    //end = clock();
    std::cout << std::setprecision(6) << std::fixed;
    std::cout << FILENAME_IN << " took "
        << elapsedSeconds(false)
        << " seconds preprocessing.\n";

    // Now run test for each cover passed int
    std::vector<unsigned int> cover;
    std::cout << std::left << std::setw(48) << "Cover Tested"
        << std::right << std::setw(17) << "Minimum # of Els"
        << std::setw(12) << "Time (sec)" << '\n';
    for (int i = 3; i < argc; i++) {

        createCover(argv[i], cover);

        std::cout << std::left << std::setw(60) << coverToString(cover) << std::right;

        elapsedSeconds(true);
        unsigned int count = minOverlap.findMinimumOverlap(cover, doPrint);
        double time = elapsedSeconds(false);

        std::cout << std::setw(5);

        if(count == UINT_MAX)
            std::cout << "DNE"; // Does not exist
        else
            std::cout << count;

        std::cout << std::setw(12) << time << '\n';
        timings.push_back(time);
    }

    // If timing data was record, display median and output to file.
    if (!timings.empty()) {
        std::sort(timings.begin(), timings.end());

        std::cout << "Median time to find a cover: "
            << timings[timings.size()/2] << " seconds.\n\n";

        std::ofstream timingOut(FILENAME_OUT);
        if (timingOut) {
            timingOut << std::setprecision(8);
            for (std::size_t i = 0; i < timings.size(); i++) {
                timingOut << i << '\t' << timings[i] << '\n';
            }
            timingOut.close();
            std::cout << "Timing information written to '" << FILENAME_OUT
                << "'\n";
        } else {
            std::cout << "Unable to open '" << FILENAME_OUT
                << "' for writing. No timing information written.\n";
        }
    }

    return 0;
}

bool validArguments(int argc, char * const argv[], bool &doPrint) {

    // Check number of arguments
    if (argc < 4) {
        std::cout << "Error, not enough parameters.  Usage:\n";
        std::cout << argv[0] << " <set file> <yes/no> <list of phrases>\n";
        return false;
    }

    // Check the yes/no argument
    if (strcmp(argv[2], "yes") == 0) {
        doPrint = true;
    } else if (strcmp(argv[2], "no") == 0) {
        doPrint = false;
    } else {
        std::cout << "error in 2nd parameter which must be yes or no, got "
            << argv[2] << '\n';
        return false;
    }

    // Ensure the set file can be opened.
    std::ifstream inTest;
    inTest.open(argv[1]);
    if (!inTest.is_open()) {
        std::cout << "File " << argv[1] << " could not be opened\n";
        return false;
    }
    inTest.close();

    return true;
}

inline double elapsedSeconds(bool isStart) {
    using namespace std::chrono;
    static time_point<high_resolution_clock> start;
    double seconds = 0;

    if (isStart) {
        start = high_resolution_clock::now();
    }
    else {
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start).count();
        seconds = static_cast<double>(duration) * 1e-6; // convert from microseconds to seconds
    }

    return seconds;
}

void createCover(const std::string& str, std::vector<unsigned int> &cover) {
    const auto LEN = std::count(str.begin(), str.end(), ' ') + 1; // count spaces
    std::istringstream coverIn(str);

    // resize the vector
    cover.resize(LEN);

    // now grab each number and place into the vector
    for (auto &el : cover) {
        coverIn >> el;
    }
}

std::string coverToString(const std::vector<unsigned int> &cover) {
    std::ostringstream out;
    out << "{";
    for (const auto& num : cover) {
        out << num << ' ';
    }
    std::string output = out.str();
    output.back() = '}';
    return output;
}
