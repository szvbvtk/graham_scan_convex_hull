#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum mode { BOTTOM_UP, TOP_DOWN };
template <typename T>
struct BinaryHeap {
    int size;
    T* heap_array;
    BinaryHeap(T* array, int n, int (*func)(T, T), bool mode = 0) {
        heap_array = array;

        if (mode == TOP_DOWN) {
            size = 0;
            for (int i = 0; i < n; i++) {
                size++;
                heapUp(size, func);
            }
        }
        else if (mode == BOTTOM_UP) {
            size = n;
            for (int i = size / 2; i > -1; i--) {
                heapDown(i, func);
            }
        }

        sort(size, func);

    }

    void sort(int& size, int (*func)(T, T)) {
        for (int i = size - 1; i > 0; i--) {
            swap(heap_array[i], heap_array[0]);
            size--;
            heapDown(0, func);
        }
    }

    T get(int index) {

        if (index < size) {
            return heap_array[index];
        }
    }

    int parent(int i) {
        i++;
        return (i >> 1) - 1;
    }
    int left_child(int i) {
        i++;
        return (i << 1) - 1;
    }
    int right_child(int i) {
        i++;
        return (i << 1);
    }

    void heapUp(int el_index, int (*func)(T, T)) {
        if (func(heap_array[el_index], heap_array[parent(el_index)]) == 1 && el_index > 0) {
            T tmp = heap_array[parent(el_index)];
            heap_array[parent(el_index)] = heap_array[el_index];
            heap_array[el_index] = tmp;
            heapUp(parent(el_index), func);
        }
    }

    string str(string(*func)(T), int k = -1) {
        return heap_array->str(func, k);
    }

    void heapDown(int el_index, int (*func)(T, T)) {

        int l_child_index = left_child(el_index);
        int r_child_index = right_child(el_index);

        int largest;
        if (l_child_index < size && func(heap_array[l_child_index], heap_array[el_index]) == 1)
            largest = l_child_index;
        else
            largest = el_index;

        if (r_child_index < size && func(heap_array[r_child_index], heap_array[largest]) == 1)
            largest = r_child_index;

        if (el_index != largest) {
            swap(heap_array[el_index], heap_array[largest]);
            heapDown(largest, func);
        }
    }
};

struct Point {
    double x;
    double y;
};

int cross_product_cmp(Point p1, Point p2) {
    double cross_product = p1.y * p2.x - p2.y * p1.x;

    if (cross_product > 0)
        return -1;

    return 1;
}

template <typename T>
struct DynamicArray {

    int size{};
    int capacity{ 1 };
    T* array = new T[capacity];

    void swap(int i1, int i2) {
        T tmp = array[i1];
        array[i1] = array[i2];
        array[i2] = tmp;
    }

    void add(T data) {
        if (size == capacity) {

            capacity *= 2;

            T* tmp_array = new T[capacity];

            for (int i = 0; i < size; i++)
                tmp_array[i] = array[i];

            delete[] array;
            array = tmp_array;

        }

        array[size] = data;
        size++;

    }

    void del() {
        if (size > 0)
            size--;
    }

    T get(int index) {

        if (index < size) {
            return array[index];
        }
        else {
            throw out_of_range("Błędny zakres");
        }

    }

    int getSize() {
        return size;
    }

    void change(int index, T newData) {

        if (index < size) {
            array[index] = newData;
        }
        else {
            throw out_of_range("Błędny zakres");
        }

    }

    void clear() {
        delete[] array;
        size = 0;
        capacity = 1;
        array = new T[capacity];
    }

    string str(string(*func)(T so)) {

        string s = "Actual size : " + to_string(size) + "\nCapacity : " + to_string(capacity) + "\n";

        for (int i = 0; i < size; i++) {
            s += func(array[i]);
        }

        s += '\n';

        return s;

    }

    ~DynamicArray() {
        delete[] array;
    }
};

DynamicArray<Point>* pointsFromFile(string filename) {
    ifstream plik(filename);
    DynamicArray<Point>* points = new DynamicArray<Point>();
    int number_of_points;
    double x;
    double y;

    plik >> number_of_points;
    //cout.precision(11);
    //cout << number_of_points << '\n';
    for (int i = 0; i < number_of_points; i++) {
        plik >> x >> y;
        //cout << x << ' ' << y << '\n';
        Point tmp{ x, y };
        points->add(tmp);
    }
    //cout << points->size;

    plik.close();
    return points;
}

DynamicArray<int> graham_scan(DynamicArray<Point>* points) {
    // stworzenie kopii punktów
    DynamicArray<Point>* points_copy = new DynamicArray<Point>();
    DynamicArray<int>* convex_hull = new DynamicArray<int>();

    for (int i = 0; i < points->getSize(); i++) {
        points_copy->add(points->get(i));
    }

    double min_x = points_copy->get(0).x;
    double min_y = points_copy->get(0).y;
    int first_point_index = 0;

    for (int i = 1; i < points_copy->getSize(); i++) {
        Point tmp = points_copy->get(i);

        if (tmp.y < min_y) {
            first_point_index = i;
            min_y = tmp.y;
        }
        else if (tmp.y == min_y && tmp.x < min_x) {
            first_point_index = i;
            min_x = tmp.x;
        }
    }
    points_copy->swap(0, first_point_index);
    convex_hull->add(first_point_index);

    //BinaryHeap<Point>(points_copy->array + 1, points_copy->getSize() - 1, cross_product_cmp, 0); // nie wiem czy to zadziała, musi byc sortowanie od drugiego elementu
    // dalej nie wiem co robić

    delete points_copy;
    return *convex_hull;
}

int main()
{
    DynamicArray<Point>* punkty = pointsFromFile("../excercises/points2.txt");

    DynamicArray<int> otoczka = graham_scan(punkty);

    for (int i = 0; i < otoczka.size; i++) {
        cout << otoczka.get(i) << '\n';
    }
}

