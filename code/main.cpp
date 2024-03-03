#include <cstring>
#include <iostream>

const size_t SMALL_STRING_SIZE = 15;

class MyString {
private:
    union {
        char* dynamicData; // 指向动态分配内存的指针
        char smallData[SMALL_STRING_SIZE + 1]; // 用于存储小字符串的固定大小数组
    };
    size_t length; // 字符串长度
    bool isSmall; // 标记字符串是否存储在小字符串数组中

public:
    // Constructors
    MyString() : length(0), isSmall(true) {
        smallData[0] = '\0'; // 确保小字符串数组以空字符结尾
    }
    
    MyString(const char* str) : length(strlen(str)), isSmall(true) {
        if (length > SMALL_STRING_SIZE) {
            dynamicData = new char[length + 1];
            strcpy(dynamicData, str);
            isSmall = false;
        } else {
            strcpy(smallData, str);
        }
    }
    
    // Destructor
    ~MyString() {
        if (!isSmall) {
            delete[] dynamicData;
        }
    }
    
    // Copy constructor
    MyString(const MyString& other) : length(other.length), isSmall(other.isSmall) {
        if (isSmall) {
            strcpy(smallData, other.smallData);
        } else {
            dynamicData = new char[length + 1];
            strcpy(dynamicData, other.dynamicData);
        }
    }
    
    // Move constructor
    MyString(MyString&& other) noexcept : length(other.length), isSmall(other.isSmall) {
        if (isSmall) {
            strcpy(smallData, other.smallData);
        } else {
            dynamicData = other.dynamicData;
            other.dynamicData = nullptr;
            other.length = 0;
            other.isSmall = true;
        }
    }
    
    // Assignment operator
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            if (!isSmall) {
                delete[] dynamicData;
            }
            length = other.length;
            isSmall = other.isSmall;
            if (isSmall) {
                strcpy(smallData, other.smallData);
            } else {
                dynamicData = new char[length + 1];
                strcpy(dynamicData, other.dynamicData);
            }
        }
        return *this;
    }
    
    // Move assignment operator
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            if (!isSmall) {
                delete[] dynamicData;
            }
            length = other.length;
            isSmall = other.isSmall;
            if (isSmall) {
                strcpy(smallData, other.smallData);
            } else {
                dynamicData = other.dynamicData;
                other.dynamicData = nullptr;
                other.length = 0;
                other.isSmall = true;
            }
        }
        return *this;
    }
    
    // Accessors
    size_t size() const {
        return length;
    }
    
    const char* c_str() const {
        return isSmall ? smallData : dynamicData;
    }
    
    // Append
    MyString& append(const MyString& other) {
        if (length + other.length <= SMALL_STRING_SIZE) {
            strcat(smallData, other.c_str());
        } else {
            char* newData = new char[length + other.length + 1];
            strcpy(newData, c_str());
            strcat(newData, other.c_str());
            if (!isSmall) {
                delete[] dynamicData;
            }
            dynamicData = newData;
            isSmall = false;
        }
        length += other.length;
        return *this;
    }
    
    // Concatenation operator
    MyString operator+(const MyString& other) const {
        MyString result = *this;
        result.append(other);
        return result;
    }
};

int main() {
    MyString str1("Hello");
    MyString str2(" World");
    MyString str3 = str1 + str2;
    std::cout << str3.c_str() << std::endl;
    return 0;
}
