#ifndef BASE_CLASSIFIER_H
#define BASE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/ScoredBox.hpp"

#include "detector/ensemble/Leaf.hpp"
#include "detector/ensemble/PixelComparison.hpp"

using namespace std;

class BaseClassifier {
private:
    vector<PixelComparison*> pixelComparisons;
    int nrOfComparisons;
    vector<Leaf*> decisionTree;

    int generateBinaryCode(Frame* frame, Box* box);
    double getProbability(int binaryCode);
public:
    BaseClassifier(vector<PixelComparison*> comparisons);
    void init(Frame* frame, Box* box, bool label);
    double score(Frame* frame, Box* box);
    void update(Frame* frame, ScoredBox* box, bool label);
};

#endif