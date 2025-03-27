#ifndef MINIMUMOVERLAP_HPP
#define MINIMUMOVERLAP_HPP

#include <vector>
#include <string>

class MinimumOverlap {
public:
    /**
    * Passes in a string pointing to the set/group file.
    * Make sure you store all the sets/groups!
    */
    MinimumOverlap(const std::string &setFile);

    /**
    * Returns the minimum number of overlaps possible given a cover.
    * If print is true, it prints the set(s).
    */
    unsigned int findMinimumOverlap(const std::vector<unsigned int> &cover, const bool print) const;

private:
    // All sets/groups loaded from the file
    std::vector<std::vector<int>> sets;

    // Maximum student ID encountered
    size_t maxStudentID = 0;

    // Groups used in the best solution
    mutable std::vector<std::vector<int>> bestSolutionGroups;

    std::vector<int> preprocessCover(const std::vector<unsigned int> &cover) const;

    unsigned int recursiveFind(
        std::vector<int> &remainingCounts,
        std::vector<bool> &usedSets,
        unsigned int currentCount,
        unsigned int &bestSolution,
        size_t startSetIndex,
        std::vector<std::vector<int>> &currentSolution) const;
};

#endif // MINIMUMOVERLAP_HPP
