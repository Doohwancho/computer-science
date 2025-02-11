struct Person {
    char name [50];
    int age;
    Person prev; //안됨! 무조건 struct 붙여야 됨!
    struct Person next;
};

typedef struct Person2 {
    char name[50];
    int age;
    struct Person2 prev; //둘 다 됨
    Person2 next; //둘 다 됨
} Person2;

int main() {
    // Person p1; //WRONG! - struct 안붙이면 안됨.
    struct Person p1;
    Person2 p2;
    return 0;
}
