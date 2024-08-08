#include <iostream>
#include <vector>
#include <unordered_map>

template<typename T>
class Heap
{
    std::vector<T> data;
    std::unordered_map<T, int> map;

public:
    void add(T item)
    {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void display()
    {
        for (const T& item : data)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    int size() const
    {
        return data.size();
    }

    bool isEmpty() const
    {
        return data.empty();
    }

    T remove()
    {
        swap(0, data.size() - 1);
        T removedValue = data.back();
        data.pop_back();
        downheapify(0);
        map.erase(removedValue);
        return removedValue;
    }

    T get() const
    {
        return data[0];
    }

    void updatePriority(T item)
    {
        int index = map[item];
        upheapify(index);
    }

private:
    void upheapify(int ci)
    {
        int pi = (ci - 1) / 2;
        if (isLarger(data[ci], data[pi]) > 0)
        {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void downheapify(int pi)
    {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int mini = pi;

        if (lci < data.size() && isLarger(data[lci], data[mini]) > 0)
        {
            mini = lci;
        }

        if (rci < data.size() && isLarger(data[rci], data[mini]) > 0)
        {
            mini = rci;
        }

        if (mini != pi)
        {
            swap(mini, pi);
            downheapify(mini);
        }
    }

    void swap(int i, int j)
    {
        T ith = data[i];
        T jth = data[j];
        
        data[i] = jth;
        data[j] = ith;

        map[ith] = j;
        map[jth] = i;
    }

    int isLarger(const T& t, const T& o) const
    {
        return t > o ? 1 : (t < o ? -1 : 0);
    }
};

// Example usage
int main()
{
    Heap<int> heap;
    heap.add(10);
    heap.add(20);
    heap.add(5);
    heap.add(15);

    heap.display(); // Output: 20 15 5 10

    std::cout << "Removed: " << heap.remove() << std::endl; // Output: 20
    heap.display(); // Output: 15 10 5

    return 0;
}
