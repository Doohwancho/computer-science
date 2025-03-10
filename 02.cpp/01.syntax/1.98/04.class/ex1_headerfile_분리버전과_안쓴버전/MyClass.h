#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass {
private:
    int myPrivateMember;

public:
    MyClass(int value);
    void setPrivateMember(int value);
    int getPrivateMember();
};
#endif
