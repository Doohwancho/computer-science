#include <iostream>
#include <memory>
#include <vector>

//Q. what is RAII?
//A. "resources are released automatically when they are no longer needed."

//Q. what is shared pointer?
//A. 여러놈들이 같은 객체를 가르킬 경우 씀. 얘네들 다 하나씩 수동으로 포인터의 메모리 해제 안해줘서 됨. 편함 

//A. 일반 포인터 사용시 메모리 누수 가능성 있음(까먹고 delete 안하면)
//   공유 포인터는 여러 포인터들이 같은 객체를 가르치게 해주고, 그 객체 참조하는 포인터가 없어졌을 때, 자동으로 메모리 해제 해줌 
//ex. shared_ptr<int> ptr1 = make_shared<int>(42); 이렇게 하면 ptr1이 42라는 값을 가진 int 객체를 가리키게 돼. 
//    그리고 shared_ptr<int> ptr2 = ptr1; 이렇게 하면 ptr2도 같은 객체를 가리키게 되고, 참조 카운트가 1 증가해. 이제 참조 카운트는 2가 되는 거야.


class Resource {
private:
    std::string name;
    
public:
    Resource(const std::string& name) : name(name) {
        std::cout << "리소스 생성됨: " << name << std::endl;
    }
    
    void use() {
        std::cout << "리소스 사용 중: " << name << std::endl;
    }
    
    ~Resource() {
        std::cout << "리소스 소멸됨: " << name << std::endl;
    }
};

void processResource(std::shared_ptr<Resource> resource) {
    std::cout << "함수 내부 사용 카운트: " << resource.use_count() << std::endl;
    resource->use();
    // 함수 끝나면 참조 카운트 감소하지만 0 아니면 객체 안 삭제됨 ㅇㅇ
}

int main() {
    // 일반 포인터 사용 시 메모리 누수 가능성 있음 ㅇㅇ (까먹고 delete 안 하면...)
    // Resource* rawResource = new Resource("Raw 리소스");
    
    // 공유 포인터: 참조 카운트 기반 자동 메모리 관리 ㅇㅇ
    {
        std::cout << "=== 블록 시작 ===" << std::endl;
        
        // 공유 포인터 생성, 참조 카운트 1 ㅇㅇ
        std::shared_ptr<Resource> resource1 = std::make_shared<Resource>("공유 리소스");
        
        {
            // 같은 리소스 가리키는 두 번째 포인터, 참조 카운트 2 ㅇㅇ
            std::shared_ptr<Resource> resource2 = resource1;
            std::cout << "내부 블록 카운트: " << resource1.use_count() << std::endl;
            resource2->use();
        } // resource2 소멸, 참조 카운트 1로 감소 ㅇㅇ
        
        std::cout << "내부 블록 종료 후 카운트: " << resource1.use_count() << std::endl;
        
        // 함수에 공유 포인터 전달 ㅇㅇ
        processResource(resource1);
        
        std::cout << "함수 호출 후 카운트: " << resource1.use_count() << std::endl;
        
        // vector에 공유 포인터 저장 ㅇㅇ
        std::vector<std::shared_ptr<Resource>> resources;
        resources.push_back(resource1);
        
        std::cout << "벡터 추가 후 카운트: " << resource1.use_count() << std::endl;
        
    } // 블록 종료되면 resource1도 소멸, 참조 카운트 0되면서 리소스 자동 삭제됨 ㅇㅇ
    
    std::cout << "=== 블록 종료 ===" << std::endl;
    
    return 0;
}
