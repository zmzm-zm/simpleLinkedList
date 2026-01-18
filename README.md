# C++ 模板单链表类

我是一名C++初学者，这是一个我学习过程中写的**单向模板链表**
含有大量Doxygen注释,并有Doxygen生成的html网页 
详见`./docs/html/index.html`
可供新手学习参考 也欢迎大佬们提出意见 

## 基本介绍
- 异常安全，会抛出异常
- 提供迭代器和常量迭代器
- 基于模板，可以储存任意类型
- 移动语义支持
- 范围for循环支持

## 使用
直接包含:
```cpp
#include "LinkedList.hpp"
```
使用示例:
```cpp
// basic_usage.cpp
...
#include <iostream>
#include <algorithm>
#include <string>
#include "../LinkedList.hpp"
using std::cout;
using std::string;
...
template<typename T>
void printList(LinkedList<T> l) { // list
        for(auto it = l.cbegin(); it != l.cend(); ++it) {
                cout << *it << " ";
        }
        cout << "\n";
}
...
int main() {
        ...
        LinkedList<int> ml; // myList
        ml.push_back(1);
        ml.push_back(3);
        ml.insert(1, 2);
        ml.push_front(0);
        printList(ml); // 0 1 2 3
        auto ml2(std::move(ml));
        ml2.erase(2);
        ml2.pop_back();
        printList(ml2); // 0 1
        
        LinkedList<string> sl; // stringList
        for(int i = 0; i < 10; i++) {
                sl.push_back("n");
        }
        for(auto& s : sl) {
                s = string(sl.size(), 'b');
        }
        printList(sl); // bbbbbbbbbb ...
        for(int i = 0; i < 10; i++) {
                sl[i] = string(sl.size(), 'c');
        }
        printList(sl); // cccccccccc ...
        ...
        return 0;
}
```
上方代码已经编译好了,位于./bin/ 
可执行文件格式：Linux 
**如果你想自己编译:** 
***c++: >=11*** 
```console
~/examples/ $ g++ basic_usage.cpp -o basic_usage
```
然后运行:
```console
~/examples/ $ ./basic_usage
```

***个人环境原因只能提供Linux，请见谅***
## 关于API
有两种查看选择
- 直接看源码(中文小白推荐)
        好处:
        1. 注释很清楚(我觉得大概是)
        2. 源码就在旁边
        3. 由Doxygen生成的文档是英文，并且翻译器效果不理想
- 看由Doxygen生成的文档(高手推荐)
**当然我还是推荐看源码**
## 最后
如果你发现问题或有想法
欢迎在**Issue**提出意见
----------
by: neOzkyol
date: 2026. 1. 18
许可证: MIT License (详见./LICENSE)
