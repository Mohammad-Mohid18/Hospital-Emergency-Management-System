#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    string symptoms;
    int priority; // 1 = Critical, 2 = Urgent, 3 = Standard
};

class MinHeap {
private:
    vector<Patient> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].priority < heap[parent].priority) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else break;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left].priority < heap[smallest].priority)
                smallest = left;
            if (right < size && heap[right].priority < heap[smallest].priority)
                smallest = right;

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else break;
        }
    }

public:
    void insert(Patient p) {
        heap.push_back(p);
        heapifyUp(heap.size() - 1);
    }

    Patient extractMin() {
        if (heap.empty())
            return {-1, "None", 0, "", 3};

        Patient minPatient = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return minPatient;
    }

    Patient peek() const {
        if (!heap.empty()) return heap[0];
        return {-1, "None", 0, "", 3};
    }

    const vector<Patient>& getPatients() const {
        return heap;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

#endif
