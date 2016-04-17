#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int id, vector<PixelComparison*> comparisons) {
    this->id = id;
    pixelComparisons = comparisons;
    nrOfComparisons = (int) comparisons.size();
    decisionTree.resize(pow(2, nrOfComparisons));
    for (int i = 0; i < pow(2, nrOfComparisons); i++) {
        decisionTree[i] = new Leaf();
    }
}

double BaseClassifier::score(Frame* frame, Box* box) {
    int binaryCode = generateBinaryCode(frame, box);
    double score = getProbability(binaryCode);
    return score;
}

int BaseClassifier::generateBinaryCode(Frame* frame, Box* box) {
    int binaryCode = 0;
    for (int i = 0 ; i < nrOfComparisons; i++) {
        binaryCode <<= 1;
        PixelComparison* comparison = pixelComparisons[i];
        bool bit = comparison->compare(frame, box);
        if (bit) {
            binaryCode |= 1;
        }
    }
    return binaryCode;
}

double BaseClassifier::getProbability(int binaryCode) {
    Leaf* leaf = decisionTree[binaryCode];
    return leaf->probability;
}

void BaseClassifier::init(Frame* frame, Box* box, bool label) {
    int binaryCode = generateBinaryCode(frame, box);
    Leaf* leaf = decisionTree[binaryCode];
    if (label) {
        leaf->incrementPositive();
    } else {
        leaf->incrementNegative();
    }
}

void BaseClassifier::update(Frame* frame, ScoredBox* box, bool label) {
    EnsembleClassificationDetails* detail = (EnsembleClassificationDetails*) box->getDetail("ensemble");
    int binaryCode = detail->getBinaryCode(this->id);
    double score = getProbability(binaryCode);
    if (label == true && score < 0.8) {
        Leaf* leaf = decisionTree[binaryCode];
        leaf->incrementPositive();
    }

    if (label == false && score > 0.5) {
        Leaf* leaf = decisionTree[binaryCode];
        leaf->incrementNegative();
    }
}

void  BaseClassifier::dumpDecisionTree() {
    for (int i = 0; i < 8192; i++) {
        Leaf* leaf = decisionTree[i];
        if (leaf->nrOfPositives > 0 || leaf->nrOfNegatives > 0) {
            println("%s", leaf->toString().c_str());
        }
    }
}
