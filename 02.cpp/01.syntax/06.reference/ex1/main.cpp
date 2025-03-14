#include <iostream>

//Q. pointer랑 reference랑 내부적으로 뭔 차이임?
//A. pointer는 변수의 메모리 주소를 저장하는 별도의 변수임(8 bytes) 공간 차지
//   reference는 컴파일 시간에 처리되는 별칭에 가까움 (추가 메모리 거의 안씀)

//   pointer는 주소값 담는 변수 개념이라 다른 변수의 주솟값을 담을 수 있지만,
//   reference는 한번 초기화 하면 평생 같은 객체만 참조함 

//   reference는 일종의 '안전한 포인터'
//   무조건 초기화 되기 때문에 유효성 검사가 필요없고, 다른 주솟값이 할당되는걸 걱정할 필요도 없음 

void example() {
    //step1) 변수 선언 
    int num = 42;
    

    //step2) 포인터와 레퍼런스 
    // pointer
    int* ptr = &num; // 포인터: 주소를 저장
    // reference
    int& ref = num;  // 레퍼런스: 별명(alias)

    std::cout << "원본 값: " << num << std::endl;        //42
    std::cout << "포인터로 접근: " << *ptr << std::endl;   //42 
    std::cout << "레퍼런스로 접근: " << ref << std::endl;  //42

    
    //step3) 값을 변경해보자 
    *ptr = 100; //포인터로 값 변경(역참조 필요)
    std::cout << "포인터로 값 변경 후: " << num << std::endl;   //100 
    
    ref = 200;  //레퍼런스로 값 변경(역참조 필요 없음)
    std::cout << "레퍼런스로 값 변경 후 : " << num << std::endl;  //200 


    //step4) 재할당 
    int num2 = 500;
    ptr = &num2; //포인터가 다른 변수를 가르키면?

    std::cout << "포인터로 접근: " << *ptr << std::endl;    //500
    std::cout << "레퍼런스로 접근: " << num << std::endl;   //200 (영향 없음)

    // ref = num2;  // 컴파일 에러! 레퍼런스는 한번 초기화하면 변경 불가

    //step5) NULL/nullptr 가능성 
    ptr = nullptr; //포인터는 nullptr 가능
    // *ptr = 50; //런타임 에러! 널 포인터 역참조 

    //int& nullRef;  //컴파일 에러! 레퍼런스는 반드시 초기화 필요!
    //int& badRef = nullptr; //컴파일 에러! 레퍼런스는 nullptr 불가능!
}

//step6) 함수 파라미터 비교 
void modifyByPointer(int* ptr) {
    if(ptr) {
        *ptr *= 2;
    }
}

void modifyByReference(int& ref) {
    //null 체크 필요 없음. (레퍼런스는 항상 유효한 객체 참조함)
    ref *= 2;
}

int main() {
    int value = 10;

    modifyByPointer(&value);
    std::cout << "포인터로 수정 후: " << value << std::endl; //20

    modifyByReference(value); 
    std::cout << "레퍼런스로 수정 후: " << value << std::endl; //40
    
    return 0;
}
