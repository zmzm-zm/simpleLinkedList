#include <iostream>
#include <string>
#include "../LinkedList.hpp"
using std::cout;
using std::string;
template<typename T>
void printList(LinkedList<T> l) { // list
        for(auto it = l.cbegin(); it != l.cend(); ++it) {
                cout << *it << " ";
        }
        cout << "\n";
}

int main() {
        LinkedList<int> ml; // myList
        ml.push_back(1);
        ml.push_back(3);
        ml.insert(1, 2);
        ml.push_front(0);
        printList(ml);
        auto ml2(std::move(ml));
        ml2.erase(2);
        ml2.pop_back();
        printList(ml2);
        
        LinkedList<string> sl; // stringList
        for(int i = 0; i < 10; i++) {
                sl.push_back("n");
        }
        for(auto& s : sl) {
                s = string(sl.size(), 'b');
        }
        printList(sl);
        for(int i = 0; i < 10; i++) {
                sl[i] = string(sl.size(), 'c');
        }
        printList(sl);
        return 0;
}