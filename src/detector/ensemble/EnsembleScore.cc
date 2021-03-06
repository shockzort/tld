#include "detector/ensemble/EnsembleScore.hpp"

EnsembleScore::EnsembleScore(int nrOfModels, int nrOfBaseClassifiers) {
    isMerged = false;
    scores.resize(nrOfModels);
    binaryCodes = new CodeVector(nrOfBaseClassifiers);
}

EnsembleScore::EnsembleScore(vector<double> scores) {
    isMerged = true;
    this->scores = scores;
}

EnsembleScore::~EnsembleScore() {
    delete binaryCodes;
}

void EnsembleScore::setBinaryCode(int index, int code) {
    binaryCodes->set(index, code);
}

int EnsembleScore::getBinaryCode(int index) {
    return binaryCodes->get(index);
}

CodeVector* EnsembleScore::getCodeVector() {
    return binaryCodes;
}

Score* EnsembleScore::clone() {
    EnsembleScore* clone = new EnsembleScore(this->scores);
    CodeVector* codeVector = new CodeVector(this->binaryCodes->codes);
    clone->binaryCodes = codeVector;
    return clone;
}

Score* EnsembleScore::sum(Score* other) {
    EnsembleScore* otherEnsembleScore = (EnsembleScore*) other;

    int n = (int) scores.size();
    for (int i = 0; i < n; i++) {
        float thisScore = scores[i];
        float otherScore = otherEnsembleScore->scores[i];
        scores[i] = thisScore + otherScore;
    }

    return this;
}

Score* EnsembleScore::divide(int n) {
    int m = (int) scores.size();
    for (int i = 0; i < m; i++) {
        scores[i] = scores[i] / n;
    }
    return this;
}

double EnsembleScore::getProbability(int modelId) {
    return scores[modelId];
}

double EnsembleScore::getValue(int modelId) {
    return scores[modelId];
}
