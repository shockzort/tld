#ifndef ENSEMBLE_CLASSIFICATION_DETAILS_H
#define ENSEMBLE_CLASSIFICATION_DETAILS_H

#include "detector/common/ClassificationDetails.hpp"
#include <vector>

class EnsembleClassificationDetails: public ClassificationDetails {
public:
    vector<int> binaryCodes;

    EnsembleClassificationDetails(int classifierSize) {
        binaryCodes.resize(classifierSize);
    }

    void add(int id, int binaryCode, double bcScore) {
        binaryCodes[id] = binaryCode;
        this->score += bcScore;
    }

    int getBinaryCode(int bcId) {
        return binaryCodes[bcId];
    }
};
#endif
