#include "detector/common/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* _box) {
    box = _box;
}

void ScoredBox::withScore(string scorerKey, ClassificationDetails* detail) {}
