#include "MinimumOverlap.hpp"
#include <fstream>
#include <sstream>
#include <climits>
#include <algorithm>
#include <iostream>

MinimumOverlap::MinimumOverlap(const std::string &setFile) {
    std::ifstream file(setFile);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << setFile << '\n';
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> group;
        int student;
        while (iss >> student) {
            group.push_back(student);
            maxStudentID = std::max(maxStudentID, static_cast<size_t>(student));
        }
        sets.emplace_back(std::move(group));
    }
    file.close();
}

std::vector<int> MinimumOverlap::preprocessCover(const std::vector<unsigned int> &cover) const {
    std::vector<int> counts(maxStudentID + 1, 0);
    for (int student : cover) {
        counts[student]++;
    }
    return counts;
}

unsigned int MinimumOverlap::findMinimumOverlap(const std::vector<unsigned int> &cover, const bool print) const {
    std::vector<int> remainingCounts = preprocessCover(cover);
    std::vector<bool> usedSets(sets.size(), false);
    std::vector<std::vector<int>> currentSolution;
    unsigned int bestSolution = UINT_MAX;

    recursiveFind(remainingCounts, usedSets, 0, bestSolution, 0, currentSolution);

    if (print) {
        std::cout << "Groups used in best solution:\n";
        for (const auto &group : bestSolutionGroups) {
            for (int student : group) {
                std::cout << student << " ";
            }
            std::cout << "\n";
        }
    }

    return bestSolution == UINT_MAX ? -1 : bestSolution;
}

unsigned int MinimumOverlap::recursiveFind(
    std::vector<int> &remainingCounts,
    std::vector<bool> &usedSets,
    unsigned int currentCount,
    unsigned int &bestSolution,
    size_t startSetIndex,
    std::vector<std::vector<int>> &currentSolution) const {

    if (std::all_of(remainingCounts.begin(), remainingCounts.end(), [](int count) { return count == 0; })) {
        if (currentCount < bestSolution) {
            bestSolution = currentCount;
            bestSolutionGroups = currentSolution;
        }
        return bestSolution;
    }

    if (currentCount >= bestSolution) {
        return bestSolution;
    }

    for (size_t i = startSetIndex; i < sets.size(); ++i) {
        if (usedSets[i]) continue;

        bool hasOverlap = false;
        for (int student : sets[i]) {
            if (remainingCounts[student] > 0) {
                hasOverlap = true;
                break;
            }
        }
        if (!hasOverlap) continue;

        usedSets[i] = true;
        currentSolution.push_back(sets[i]);

        std::vector<int> newRemainingCounts = remainingCounts;
        for (int student : sets[i]) {
            if (newRemainingCounts[student] > 0) {
                newRemainingCounts[student]--;
            }
        }

        recursiveFind(newRemainingCounts, usedSets, currentCount + static_cast<unsigned int>(sets[i].size()), bestSolution, i + 1, currentSolution);

        usedSets[i] = false;
        currentSolution.pop_back();
    }

    return bestSolution;
}
