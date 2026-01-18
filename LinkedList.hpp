#pragma once
/**
 * @file LinkedList.hpp
 * @class LinkedList
 * @brief 一个基于模板单向链表类
 *
 * @author neOzkyol
 * @date 2026-01-17
 * @version 1.0
 */
#include <iterator> ///< std::forward_iterator_tag
#include <cstddef> ///< std::ptrdiff_t
#include <stdexcept> ///< std::out_of_range std::logic_error
template<typename DataType> ///< 链表存储的数据类型
class LinkedList {
private:
        struct Node { ///< 链表节点
                DataType _data; ///< 每个节点的数据
                Node* _next; ///< 指向下一个节点的指针
                /**
                 * @name 单个节点构造函数
                 * @brief 对成员进行初始化
                 * @{
                 */
                 
                 /**
                  * @brief 无参构造函数
                  *
                  * _next成员初始化为 nullptr
                  *
                  */
                Node(): _next(nullptr){}
                /**
                 * @brief 有参构造函数
                 *
                 * _data成员初始化为传入的 data
                 * _next成员初始化为 nullptr
                 *
                 * @param data 初始数据
                 */
                Node(DataType data): _data(data), _next(nullptr){}
                /**
                 * @brief 有参构造函数
                 *
                 * _data成员初始化为传入的 data
                 * _next成员初始化为传入的 next
                 *
                 * @param data 初始数据
                 * @param next 此节点指向的下一个节点
                 */
                Node(DataType data, Node* next): _data(data), _next(next){}
                /** @} */
        };
        /**
         * @brief 链表头结点
         *
         * 这是一个数据节点，而非哨兵节点
         *
         */
        Node* _head;
        int _length; ///< 链表的长度
        /**
         * @brief 寻找最后一个节点
         *
         * 链表内部自用函数
         * 是异常安全的，不修改链表
         *
         * @return 链表最后一个节点的指针
         * @note 时间：O(n), 空间：O(1)
         */
        Node* lastNodePtr() const noexcept{
                if(_head == nullptr) {
                        return nullptr;
                }
                auto curr = _head;
                while(curr -> _next != nullptr) {
                        curr = curr -> _next;
                }
                return curr;
        }
        /**
         * @brief 在删除之前进行检查
         *
         * 使用empty()检查是链表是否为空
         * 如果链表为空，则抛出异常
         * 处理了链表长度为1的情况：删除这个唯一的节点
         *
         * @sa empty()
         * @return 是否已经处理了删除操作
         * @retval true 已经处理(空链表抛出异常或单节点已删除)
         * @retval false 需要继续执行常规删除逻辑
         */
        bool checkEmptyOrSingle() {
                if(empty()) {
                        throw std::logic_error("cannot pop from empty list");
                } else if(_length == 1) {
                        delete _head;
                        _head = nullptr;
                        _length = 0;
                        return true;
                }
                return false;
        }
public:
        /**
         * @name 链表构造函数
         * @brief 对链表进行初始化
         * @{
         */
         
         /**
          * @brief 无参构造函数
          *
          * _head初始化为 nullptr
          * _length初始化为 0
          *
          */
        LinkedList() noexcept: _head(nullptr), _length(0){}
        /**
         * @brief 拷贝构造函数
         *
         * _head 先 初始化为 nullptr
         * _length 先 初始化为 0
         *
         * @param other 拷贝给此链表的链表
         * @note 时间：O(n), 空间：O(1)
         */
        LinkedList(const LinkedList& other): _head(nullptr), _length(0) {
                auto curr = other._head;
                while(curr != nullptr) {
                        push_back(curr -> _data);
                        curr = curr -> _next;
                }
        }
        /**
         * @brief 赋值构造函数
         *
         * @param other 赋值给次链表的链表
         * @return 当前链表
         * @note 时间：O(n), 空间：O(1)
         */
        LinkedList& operator=(const LinkedList& other) {
                if(this == &other) {
                        return *this;
                }
                clean();
                auto curr = other._head;
                while(curr != nullptr) {
                        push_back(curr -> _data);
                        curr = curr -> _next;
                }
                return *this;
        }
        /**
         * @brief 移动构造函数
         *
         * _head初始化为传入对象的_head
         * _length初始化为传入对象的_length
         *
         * @param other 被接受的对象
         * @note 被用作此对象初始化的对象将被置空
         * @note 时间：O(1), 空间：O(1)
         */
        LinkedList(LinkedList&& other) noexcept: _head(other._head), _length(other._length) {
                other._head = nullptr;
                other._length = 0;
        }
        /**
         * @brief 移动赋值构造函数
         *
         * _head初始化为传入对象的_head
         * _length初始化为传入对象的_length
         *
         * @param other 被接受的对象
         * @return 自己
         * @note 被用作此对象初始化的对象将被置空
         * @note 时间：O(1), 空间：O(1)
         */
        LinkedList& operator=(LinkedList&& other) noexcept {
                if(this == &other) {
                        return *this;
                }
                clean();
                _head = other._head;
                _length = other._length;
                other._head = nullptr;
                other._length = 0;
                return *this;
        }
        /** @} */
        
        /**
         * @brief 析构函数
         *
         * 调用clean()函数删除链表
         *
         * @sa clean()
         * @note 时间：O(n), 空间：O(1)
         */
        ~LinkedList() noexcept {
                clean();
        }
        
        /**
         * @class Iterator
         * @brief LinkedList的内置迭代器
         *
         * 部分未做nullptr检查
         * 出于性能考虑
         *
         */
        class Iterator {
        private:
                Node* _curr;///< 迭代器当前指向的节点
                friend class LinkedList;///< 使LinkedList能访问Iterator私有成员
        public:
                /**
                 * @brief 对接STL
                 * @{
                 */
                using iterator_category = std::forward_iterator_tag; ///< 此为单向链表
                using value_type = DataType; ///< 数据类型为DataType
                using difference_type = std::ptrdiff_t; ///< 此为距离类型
                using pointer = DataType*; ///< 指针类型
                using reference = DataType&; ///< 解引用时返回DataType&
                /** @} */
                
                /**
                 * @brief 构造函数
                 *
                 * 初始化成员，默认初始化为 nullptr
                 * 异常安全
                 *
                 */
                explicit Iterator(Node* ptr = nullptr) noexcept: _curr(ptr){}
                /**
                 * @brief 迭代器解引用
                 *
                 * 不修改链表,异常安全
                 *
                 * @return 迭代器指向的数据的引用
                 */
                DataType& operator*() const noexcept {
                        return _curr -> _data;
                }
                /**
                 * @brief 迭代器箭头运算符
                 *
                 * 不修改链表,异常安全
                 *
                 * @return 迭代器指向的数据的指针
                 */
                DataType* operator->() const noexcept {
                        return &(_curr -> _data);
                }
                /**
                 * @brief 前置递增
                 *
                 * 异常安全
                 *
                 */
                Iterator& operator++() noexcept {
                        _curr = _curr -> _next;
                        return *this;
                }
                /**
                 * @brief 后置递增
                 *
                 * 异常安全
                 *
                 * @param int 无用，做重载区分
                 */
                Iterator operator++(int) noexcept {
                        auto temp = *this;
                        _curr = _curr -> _next;
                        return temp;
                }
                /**
                 * @brief ==比较重载
                 *
                 * 不修改链表,异常安全
                 *
                 */
                bool operator==(const Iterator& other) const noexcept {
                        return _curr == other._curr;
                }
                /**
                 * @brief !=比较重载
                 *
                 * 基于迭代器的==重载
                 * 不修改链表,异常安全
                 *
                 * @sa operator==
                 */
                bool operator!=(const Iterator& other) const noexcept {
                        return !(*this == other);
                }
        }; ///< class Iterator
        
        /**
         * @class ConstIterator
         * @brief LinkedList的内置Const迭代器
         *
         * 部分未做nullptr检查
         * 出于性能考虑
         * 对比Iterator,主要修改在operator*()和operator->()
         * 是Iterator的const版
         *
         * @see Iterator
         */
        class ConstIterator {
        private:
                const Node* _curr;///< 迭代器当前指向的节点
                friend class LinkedList;///< 使LinkedList能访问Iterator私有成员
        public:
                /**
                 * @brief 对接STL
                 * @{
                 */
                using iterator_category = std::forward_iterator_tag; ///< 此为单向链表
                using value_type = DataType; ///< 数据类型为DataType
                using difference_type = std::ptrdiff_t; ///< 此为距离类型
                using pointer = const DataType*; ///< 指针类型,const版
                using reference = const DataType&; ///< 解引用时返回const DataType&
                /** @} */
                
                /**
                 * @brief 构造函数
                 *
                 * 初始化成员，默认初始化为 nullptr
                 * 异常安全
                 *
                 */
                explicit ConstIterator(const Node* ptr = nullptr) noexcept: _curr(ptr){}
                /**
                 * @brief 迭代器解引用
                 *
                 * 不修改链表,异常安全
                 *
                 * @return 迭代器指向的数据的常量引用
                 */
                const DataType& operator*() const noexcept {
                        return _curr -> _data;
                }
                /**
                 * @brief 迭代器箭头运算符
                 *
                 * 不修改链表,异常安全
                 *
                 * @return 迭代器指向的数据的常量指针
                 */
                const DataType* operator->() const noexcept {
                        return &(_curr -> _data);
                }
                /**
                 * @brief 前置递增
                 *
                 * 异常安全
                 *
                 */
                ConstIterator& operator++() noexcept {
                        _curr = _curr -> _next;
                        return *this;
                }
                /**
                 * @brief 后置递增
                 *
                 * 异常安全
                 *
                 * @param int 无用，做重载区分
                 */
                ConstIterator operator++(int) noexcept {
                        auto temp = *this;
                        _curr = _curr -> _next;
                        return temp;
                }
                /**
                 * @brief ==比较重载
                 *
                 * 不修改链表,异常安全
                 *
                 */
                bool operator==(const ConstIterator& other) const noexcept {
                        return _curr == other._curr;
                }
                /**
                 * @brief !=比较重载
                 *
                 * 基于迭代器的==重载
                 * 不修改链表,异常安全
                 *
                 * @sa operator==
                 */
                bool operator!=(const ConstIterator& other) const noexcept {
                        return !(*this == other);
                }
        }; ///< class ConstIterator
        
        /**
         * @brief 获取首迭代器
         *
         * 用LinkedList的首节点初始化一个匿名Iterator对象并返回
         * 使用了Iterator的构造函数
         *
         * @sa Iterator()
         * @return 用LinkedList的首节点初始化的匿名Iterator对象
         */
        Iterator begin() {
                return Iterator(_head);
        }
        /**
         * @brief 获取尾迭代器的后一位置
         *
         * 用nullptr初始化一个匿名Iterator对象并返回
         * 但其实Iterator构造函数的默认值就是nullptr
         * 使用了Iterator的构造函数
         *
         * @sa Iterator()
         * @return 用nullptr初始化的匿名Iterator对象
         */
        Iterator end() {
                return Iterator(nullptr);
        }
        /**
         * @name ConstIterator的begin
         * @brief 分为两个版本
         *
         * 主要分为了 begin() - 与普通Iterator同名
         * 和  cbegin() - 显示强调Const版本
         *
         * @{
         */
         
         /**
         * @brief 获取首迭代器
         *
         * 用LinkedList的首节点初始化一个匿名ConstIterator对象并返回
         * 使用了ConstIterator的构造函数
         *
         * @sa ConstIterator()
         * @return 用LinkedList的首节点初始化的匿名ConstIterator对象
         */
        ConstIterator begin() const {
               return ConstIterator(_head);
        }
        
        /**
        * @brief 获取首迭代器 -强调const版
        *
        * 用LinkedList的首节点初始化一个匿名ConstIterator对象并返回
        * 使用了ConstIterator的构造函数
        *
        * @sa ConstIterator()
        * @return 用LinkedList的首节点初始化的匿名ConstIterator对象
        */
        ConstIterator cbegin() const {
               return ConstIterator(_head);
        }
        /** @} */
        
        /**
        * @name ConstIterator的end
        * @brief 分为两个版本
        *
        * 主要分为了 end() - 与普通Iterator同名
        * 和  cend() - 显示强调Const版本
        *
        * @{
        */
        
        /**
        * @brief 获取尾迭代器的后一位置
        *
        * 用nullptr初始化一个匿名ConstIterator对象并返回
        * 但其实ConstIterator构造函数的默认值就是nullptr
        * 使用了ConstIterator的构造函数
        *
        * @sa ConstIterator()
        * @return 用nullptr初始化的匿名ConstIterator对象
        */
        ConstIterator end() const {
               return ConstIterator(nullptr);
        }
        /**
        * @brief 获取尾迭代器的后一位置 -强调const版
        *
        * 用nullptr初始化一个匿名ConstIterator对象并返回
        * 但其实ConstIterator构造函数的默认值就是nullptr
        * 使用了ConstIterator的构造函数
        *
        * @sa ConstIterator()
        * @return 用nullptr初始化的匿名ConstIterator对象
        */
        ConstIterator cend() const {
               return ConstIterator(nullptr);
        }
        
        /** @} */
        
        /**
         * @name 下标访问
         * @brief 提供[]的下标访问
         *
         * 重载了const和非const两个版本
         * 在所索引超出范围时抛出异常
         *
         * @param index 目标索引
         * @return 所存储的数据类型
         * @note 时间：O(n), 空间：O(1)
         * @{
         */
         
         /**
          * @brief 非const版本
          */
        DataType& operator[](const int& index) {
                if(index < 0 || index >= _length) {
                   throw std::out_of_range("index out of range");
                }
                auto curr = _head;
                for(int i = 0; i < index; i++) {
                        curr = curr -> _next;
                }
                return curr -> _data;
        }
        /**
          * @brief const版本
          */
        const DataType& operator[](const int& index) const {
                if(index < 0 || index >= _length) {
                   throw std::out_of_range("index out of range");
                }
                auto curr = _head;
                for(int i = 0; i < index; i++) {
                        curr = curr -> _next;
                }
                return curr -> _data;
        }
        /** @} */
        
        /**
         * @brief 获取链表长度
         *
         * 不修改链表
         * 异常安全
         *
         * @return _length成员
         */
        int size() const noexcept {
                return _length;
        }
        /**
         * @brief 获取链表长度
         *
         * 不修改链表
         * 异常安全
         *
         * @return 链表是否为空
         * @retval true 链表为空
         * @retval false 反之
         */
        bool empty() const noexcept {
                return _head == nullptr;
        }
        /**
         * @brief 清空链表
         *
         * 异常安全
         *
         * @note 时间：O(n), 空间：O(1)
         */
        void clean() noexcept {
                while(_head != nullptr) {
                        auto temp = _head;
                        _head = _head -> _next;
                        delete temp;
                }
                _head = nullptr;
                _length = 0;
        }
        /**
         * @name 寻找是否有某个值
         * @brief 分为三个版本
         * @{
         */
        
        /**
         * @brief 返回int版本
         *
         * 不修改链表
         * 使用empty()检查链表是否为空
         *
         * @sa empty()
         * @param data 要寻找的数据
         * @return int 目标值的索引
         * @retval >=0 找到的索引
         * @retval -1 未找到
         * @note 时间：O(n), 空间：O(1)
         */
        int find_index(const DataType& data) const {
                if(empty()) {
                        return -1;
                }
                auto curr = _head;
                int index = 0;
                while(curr -> _data != data) {
                        if(curr -> _next == nullptr) {
                                return -1;
                        }
                        curr = curr -> _next;
                        index++;
                }
                return index;
        }
        /**
         * @brief 返回Iterator版本
         *
         * 不修改链表
         * 使用Iterator的begin() end()来确定范围
         *
         * @sa begin() end()
         * @param data 要寻找的数据
         * @return Iterator 目标值的迭代器
         * @retval Iterator 找到的迭代器
         * @retval end() 未找到
         * @note 时间：O(n), 空间：O(1)
         */
        Iterator find(const DataType& data) {
                for(auto it = begin(); it != end(); ++it) {
                        if (*it == data) {
                                return it;
                        }
                }
                return end();
        }
        /**
         * @brief 返回ConstIterator版本
         *
         * 不修改链表
         * 使用ConstIterator的cbegin() cend()来确定范围
         *
         * @sa cbegin() cend()
         * @param data 要寻找的数据
         * @return ConstIterator 目标值的迭代器
         * @retval ConstIterator 找到的迭代器
         * @retval cend() 未找到
         * @note 时间：O(n), 空间：O(1)
         */
        ConstIterator find(const DataType& data) const {
                for(auto it = cbegin(); it != cend(); ++it) {
                        if (*it == data) {
                                return it;
                        }
                }
                return cend();
        }
        /** @} */
        
        /**
         * @name 获取头元素
         * @brief返回头指针的数据
         *
         * 使用empty()检查链表是否为空
         * 重载const和非const版本
         * 链表为空会抛出异常
         *
         * @sa empty()
         * @return 头指针数据
         * @{
         */
         
         /**
          * @brief 非const版
          */
        DataType& front() {
                if(empty()) {
                        throw std::logic_error("LinkedList is empty");
                }
                return _head -> _data;
        }
        /**
         * @brief const版
         */
        const DataType& front() const {
                if(empty()) {
                        throw std::logic_error("LinkedList is empty");
                }
                return _head -> _data;
        }
        /** @} */
        
        /**
         * @name 获取尾元素
         * @brief返回尾指针的数据
         *
         * 使用empty()检查链表是否为空
         * 使用lastNodePtr()获取尾指针
         * 重载const和非const版本
         * 链表为空会抛出异常
         *
         * @sa empty() lastNodePtr()
         * @return 尾指针数据
         * @{
         */
         
         /**
          * @brief 非const版
          */
        DataType& back() {
                if(empty()) {
                        throw std::logic_error("LinkedList is empty");
                }
                return lastNodePtr() -> _data;
        }
        /**
         * @brief const版
         */
        const DataType& back() const {
                if(empty()) {
                        throw std::logic_error("LinkedList is empty");
                }
                return lastNodePtr() -> _data;
        }
        /** @} */
        
        /**
         * @brief 头插入
         *
         * 将新元素作为头元素
         *
         * @param data 要插入的数据
         * @note 时间：O(1), 空间：O(1)
         */
        void push_front(DataType data) {
                Node* newNode = new Node(data);
                newNode -> _next = _head;
                _head = newNode;
                _length++;
        }
        /**
         * @brief 尾插入
         *
         * 将新元素作为尾元素
         * 使用lastNodePtr()获取尾指针
         *
         * @sa lastNodePtr()
         * @param data 要插入的数据
         * @note 时间：O(n), 空间：O(1)
         */
        void push_back(DataType data) {
                Node* newNode = new Node(data);
                Node* last = lastNodePtr();
                last == nullptr? _head = newNode: last -> _next = newNode;
                _length++;
        }
        /**
         * @brief 指定位置插入
         *
         * 在列表中插入新元素
         * 插入在指定位置的后方
         * 当索引超出范围时抛出异常
         * 当插入位置在首尾时
         * 分别调用push_front() push_back()
         *
         * @sa push_front() push_back()
         * @param index 要插入的位置
         * @param data 要插入的数据
         * @note 时间：O(n), 空间：O(1)
         */
        void insert(int index, DataType data) {
                if(index < 0 || index > _length) {
                        throw std::out_of_range("index out of range");
                } else if(index == 0) {
                        push_front(data);
                        return;
                } else if (index == _length) {
                        push_back(data);
                        return;
                }
                Node* newNode = new Node(data);
                auto curr = _head;
                for(int i = 0; i < index - 1; i++) {
                        curr = curr -> _next;
                }
                newNode -> _next = curr -> _next;
                curr -> _next = newNode;
                _length++;
        }
        /**
         * @brief 头删除
         *
         * 删除头元素
         * 使用checkEmptyOrSingle()进行相关检查
         *
         * @sa checkEmptyOrSingle()
         * @note 时间：O(1), 空间：O(1)
         */
        void pop_front() {
                if(checkEmptyOrSingle()) {
                        return;
                }
                auto target = _head;
                _head = target -> _next;
                delete target;
                _length--;
        }
        /**
         * @brief 尾删除
         *
         * 删除尾元素
         * 使用checkEmptyOrSingle()进行相关检查
         *
         * @sa checkEmptyOrSingle()
         * @note 时间：O(n), 空间：O(1)
         */
        void pop_back() {
                if(checkEmptyOrSingle()) {
                        return;
                }
                auto curr = _head;
                while(curr -> _next -> _next != nullptr) {
                        curr = curr -> _next;
                }
                delete curr -> _next;
                curr -> _next = nullptr;
                _length--;
        }
        /**
         * @brief 删除指定元素
         *
         * 索引如果为首元素，则调用pop_front()进行删除
         * 索引超出范围时抛出异常
         * 链表为空时抛出异常
         *
         * @param index 要删除的元素的索引
         * @note 时间：O(n), 空间：O(1)
         */
        void erase(int index) {
                if(index < 0 || index >= _length) {
                        throw std::out_of_range("index out of range");
                } else if(empty()) {
                        throw std::logic_error("cannot erase from empty list");
                } else if(index == 0) {
                        pop_front();
                        return;
                }
                auto previous = _head;
                auto target = _head;
                for(int i = 0; i < index; i++) {
                        previous = target;
                        target = target -> _next;
                }
                previous -> _next = target -> _next;
                delete target;
                _length--;
        }
}; ///< class LinkedList