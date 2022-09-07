#include <cstdlib>
#include <iostream>
#include <string_view>


#ifndef LAB4_LN_H
#define LAB4_LN_H
class LN
{
public:
    explicit LN(char* buf, int size, int sign1);
    LN(char*, int);
    LN(char*);
    LN(LN&&);
    LN(const LN&);
    LN(long long int = 0);
    ~LN();
    explicit LN(const std::string_view& str);
    void operator=(const LN& b);
    LN& operator=(LN&&);
    LN operator+ (const LN& b) const;
    LN operator- (const LN& b) const;
    LN operator* (const LN& b) const;
    LN operator/ (const LN& b) const;
    LN operator% (const LN& b) const;
    LN operator~ () const;
    void operator+= (const LN& b);
    void operator-= (const LN& b);
    void operator*= (const LN& b);
    void operator/= (const LN& b);
    void operator%= (const LN& b);
    void operator- ();
    bool operator< (const LN& b) const;
    bool operator<= (const LN& b) const;
    bool operator> (const LN& b) const;
    bool operator>= (const LN& b) const;
    bool operator== (const LN& b) const;
    bool operator!= (const LN& b) const;
    explicit operator long long int() const;
    explicit operator bool() const;
    void print(FILE*) const;
    void printInfo() const;
private:
    char* num_;
    bool isNaN_;
    int sign_;
    int size_;
    int min(int a, int b) const;
    int max(int a, int b) const;
    int module(int a) const;
    void deleteNulls();
    int compare(const LN& b) const;
    LN add(const LN& b) const;
    LN subtract(const LN& b1) const;
    LN simpleMult(char num1, int dopNulls) const;
    void reverse(char* a, int size) const;
    int skipNulls(const LN& a, int left) const;
    LN divide(const LN& b, LN& mod)const;
    LN detach(int);
    size_t countNulls() const;
    void insertBack(const LN& b);
    LN createNaN() const;
};
LN operator ""_ln(const char*);
#endif
