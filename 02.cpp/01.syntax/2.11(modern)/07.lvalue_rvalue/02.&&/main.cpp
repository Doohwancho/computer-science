#include <iostream>

//Q. 이거 왜 만듬?
//A. 11버전에 std::move() 에서 쓰려고. 
//   ex. string같은거 s1 = s2 할 때, s2에 있는 값 다 복사해서 개느린데, 
//       s1한테 s2에 데이터를 가르키는 포인터만 만들어주고, s2에 포인터는 삭제하는게 std::move()

// C++98: 이거 하나만 있었음
void func(int& x) { // lvalue 참조 (보통의 참조)
    std::cout << "lvalue reference function called" << std::endl;
}

// C++11: 이거 추가됨
void func(int&& x) { // rvalue 참조 (&&)
    std::cout << "rvalue reference function called" << std::endl;
}

int main() {
    /** 
     * case1) 
     */
    int a = 10;
    
    func(a);        // lvalue 참조 함수 호출
    func(10);       // rvalue 참조 함수 호출 (10은 임시값)
    func(a + 5);    // rvalue 참조 함수 호출 (x+5는 임시 결과)


    /** 
     * case2) 
     */
    int x = 5;  // x는 lvalue
    int y = x;  // x는 lvalue
    int z = x + y;  // (x + y)는 rvalue

    // lvalue 참조 (하나의 &)
    int& rx = x;  // OK: lvalue를 lvalue 참조에 바인딩
    // int& ry = 10;  // 에러: rvalue를 lvalue 참조에 바인딩 못함

    // rvalue 참조 (두 개의 &&)
    // int&& rrx = x;  // 에러: lvalue를 rvalue 참조에 바인딩 못함
    int&& rry = 10;  // OK: rvalue를 rvalue 참조에 바인딩

    // std::move는 lvalue를 rvalue로 변환
    int&& rrz = std::move(x);  // OK: lvalue인 x를 rvalue로 변환해서 rvalue 참조에 바인딩

    std::cout << x << std::endl; // 5  Q. 여기서 std:move(x) 한다는건, x를 참조하던 포인터를 "없애고" 그 데이터(rvalue)를 넘긴다는거 아냐? 그러니까 x는 nullptr가 들어있어야 정상 아니야? 5가 아니라?
                                 //    A. ㄴㄴ x를 참조하던 포인터를 없애진 않음. 그냥 std::move()는 lvalue -> rvalue로 타입 캐스팅만 하는거지 값을 바꾼다거나 포인터를 지운다거나 등의 작업을 하지는 않음. 
    std::cout << rrz << std::endl; // 5

    x = 50; //Q. 그럼 x, rrz 포인터 둘 다 x의 원본 데이터값을 가르킨다는거니까, x를 수정하면 rrz도 바뀌겠네?

    std::cout << x << std::endl; // 50  
    std::cout << rrz << std::endl; // 50
    
    x = 100; //Q. 그럼 x, rrz 포인터 둘 다 x의 원본 데이터값을 가르킨다는거니까, rrz를 수정하면 x도 바뀌겠네?

    std::cout << x << std::endl; // 100  
    std::cout << rrz << std::endl; // 100

    //Q. 그럼 굳이 새로운 lvalue, rvalue 만들지 말고, 그냥 새로운 포인터 하나 만들어서 x를 가르키면 되잖아?
    //A. ㅇㅇ 맞음. int같은 primitive class에 std::move() 쓰는게 의미는 없음.
    //   rvalue의 std::move()가 의미있어지는 상황은 클래스 객체 다룰 때임. std::string, std::vector 같은 애들.
    
    /**
     * case3)
     */
    std::string s1 = "매우 매우 큰 문자열";
    std::string s2 = s1;               // 복사: 문자열 전체 복사함 (비쌈)
    std::string s3 = std::move(s1);    // 이동: 내부 포인터만 가져옴 (싸다!) (값을 복사를 안하고 포인터 만들어서 기존 데이터에 붙임)

    // 이 시점에 s1은 비어있거나 무효한 상태일 수 있음 (개념적으로만)
    std::cout << "s1: " << s1 << std::endl;  // 비어있거나 정의되지 않은 값
    std::cout << "s2: " << s2 << std::endl;  // "매우 매우 큰 문자열"
    std::cout << "s3: " << s3 << std::endl;  // "매우 매우 큰 문자열"



    //Q. 왜 굳이 옮겨담음? s1 -> s3로 std::move() 해도 값이 똑같은데?
    //A. 03.std::move 디렉토리에 예시 적어둠 

    return 0;
}
