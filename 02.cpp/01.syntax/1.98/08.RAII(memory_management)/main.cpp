
#include <iostream>
#include <memory>
#include <vector>

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
    // 일반 포인터 사용 시 메모리 누수 가능성 있음 ㅇㅇ (까먹고 delete 안 하면 ㅅㄲ)
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
