#ifndef BOUNDED_PRIORITY_QUEUE_H
#define BOUNDED_PRIORITY_QUEUE_H

#define UNBOUNDED -1

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

template <class T, class V>
class BoundedPriorityQueue: public priority_queue<T*, vector<T*>, V> {
    int limit;
public:
    int count;

    BoundedPriorityQueue(int l) {
        limit = l;
        count = 0;
    }

    void operator += (T* item) {
        if (count < limit) {
            this->push(item);
            count += 1;
        } else {
            T* head = this->top();
            if (this->comp(item, head)) {
                this->pop();
                this->push(item);
            }
        }
    }

    vector<T*> toVector() {
        vector<T*> vector;
        for (int i = 0 ; i < count; i++) {
            T* head = this->top();
            vector.push_back(head);
            this->pop();
        }
        std::reverse(vector.begin(), vector.end());
        return vector;
    }
};
#endif
