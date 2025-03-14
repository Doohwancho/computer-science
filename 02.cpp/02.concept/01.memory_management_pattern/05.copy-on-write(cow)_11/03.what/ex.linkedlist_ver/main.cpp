#include <iostream>
#include <memory>

/*
---
핵심 

1. 리스트 공유: 처음에 list2 = list1 하면 모든 노드를 공유함 (얕은 복사)
2. 변경 감지: 리스트를 수정할 때 shared_ptr의 참조 카운트 확인
    ```cpp
    if (head_.use_count() > 1) {
        // 다른 객체가 공유 중 -> 복제 필요
    }
    ```
3. 최소 복제: set() 메서드가 진짜 COW의 핵심인데, 참조 카운트 확인한게 1 이상이면, 
    1. 딱 고 부분만 "수정하는 노드와 그 앞부분만 복제"하고, 
    2. 수정 노드 이후는 원본 리스트와 공유함
    3. head를 수정함. (그러니까 복사한 COWLinkedList에 바뀐 부분은 head + 수정한 node부분. 나머지 node들은 똑같음.) 
    4. 그러면 처음에 얕은 복사했을 땐 원본 데이터의 shared_count가 2였는데, 이젠 새로운 head 붙였으니까 COWLinkedList가 2개 생기면서 shared_count가 각자 1씩 됨 

이렇게 하면 리스트에서 일부 노드만 변경되고 나머지는 공유되는 구조가 만들어짐. 깊은 복사보다 효율적이면서도 얕은 복사의 문제 없이 사용 가능함.

대규모 데이터 다룰 때 쓰는 패턴 
*/


// 연결 리스트 노드
template <typename T>
class Node {
public:
    T data;
    std::shared_ptr<Node<T>> next;
    
    Node(const T& value) : data(value), next(nullptr) {}
};

// Copy-on-Write 연결 리스트
template <typename T>
class CowLinkedList {
private:
    std::shared_ptr<Node<T>> head_;
    
    // 특정 위치까지 노드 복제 (COW의 핵심)
    std::shared_ptr<Node<T>> copy_nodes_until(std::shared_ptr<Node<T>> original, 
                                             std::shared_ptr<Node<T>> until) {
        if (!original || original == until) {
            return nullptr;
        }
        
        // 현재 노드 복제
        auto new_node = std::make_shared<Node<T>>(original->data);
        
        // 다음 노드 재귀적으로 복제
        new_node->next = copy_nodes_until(original->next, until);
        
        return new_node;
    }
    
public:
    CowLinkedList() : head_(nullptr) {}
    
    // 얕은 복사 생성자 - 리스트 구조 공유
    CowLinkedList(const CowLinkedList& other) : head_(other.head_) {
        // 포인터만 복사 (얕은 복사)
        std::cout << "얕은 복사 수행: 그냥 포인터만 복사함" << std::endl;
    }
    
    // 리스트 끝에 노드 추가
    void append(const T& value) {
        if (!head_) {
            head_ = std::make_shared<Node<T>>(value);
            return;
        }
        
        // 기존 리스트 공유하는지 확인
        if (head_.use_count() > 1) {
            // 다른 객체가 이 리스트 참조 중이면 전체 복제 (COW)
            std::cout << "COW 발생: 전체 리스트 복제 중..." << std::endl;
            auto new_head = std::make_shared<Node<T>>(head_->data);
            
            auto current = head_->next;
            auto new_current = new_head;
            
            while (current) {
                new_current->next = std::make_shared<Node<T>>(current->data);
                current = current->next;
                new_current = new_current->next;
            }
            
            // 새 노드를 새로 복제한 리스트 끝에 추가
            new_current->next = std::make_shared<Node<T>>(value);
            head_ = new_head;  // 새 헤드로 교체
        } else {
            // 이 리스트 혼자 쓰면 그냥 끝에 추가
            std::cout << "일반 추가: 리스트 끝에 노드 추가" << std::endl;
            auto current = head_;
            while (current->next) {
                current = current->next;
            }
            current->next = std::make_shared<Node<T>>(value);
        }
    }
    
    // 핵심!!!!!!!!!!!!!!!!!!!!
    // 특정 위치의 노드 값 변경 (COW 핵심 로직)
    void set(int index, const T& value) {
        if (!head_ || index < 0) {
            return;
        }
        
        // step1) 기존 리스트 공유하는지 확인.
        if (head_.use_count() > 1) { //현재 linkedlist에 head를 다른 포인터가 참조중임? 확인하는 것. 참조중이면 복사해야 하거든. 
            std::cout << "COW 발생: 부분 리스트 복제 중..." << std::endl;
            
            // step2) 복사할 타겟 노드 찾기
            auto original = head_;
            auto target_node = head_;
            int current_idx = 0;
            
            // index만큼 이동 
            while (target_node && current_idx < index) {
                target_node = target_node->next;
                current_idx++;
            }
            
            if (!target_node) {
                return;  // 인덱스 범위 초과
            }
            
            // step3) 타겟 노드까지 새로 복제 - 진짜 COW의 시작 
            auto new_head = std::make_shared<Node<T>>(head_->data);
            auto new_current = new_head;
            auto current = head_->next;
            current_idx = 1;
            
            while (current && current_idx <= index) {
                if (current_idx == index) {
                    // 타겟 노드에 새 값 설정
                    new_current->next = std::make_shared<Node<T>>(value); //오해) std::make_shared는 메모리 관리용이지 COW 패턴 자체의 필수 요소는 아님
                } else {
                    // 그냥 복제
                    new_current->next = std::make_shared<Node<T>>(current->data);
                }
                
                // 타겟 이후 노드는 원본 그대로 공유 (COW의 핵심!)
                if (current_idx == index) {
                    new_current->next->next = current->next;
                    break;
                }
                
                current = current->next;
                new_current = new_current->next;
                current_idx++;
            }
            
            head_ = new_head;  // 새 헤드로 교체. 복제 끝나면 이 객체의 헤드를 새 리스트로 바꿔치기함. 이제 이 객체는 원본 리스트랑 연결 끊어짐.
        } else {
            // 이 리스트 혼자 쓰면 그냥 값 바꿈
            std::cout << "일반 수정: 리스트 값 직접 변경" << std::endl;
            auto current = head_;
            int current_idx = 0;
            
            while (current && current_idx < index) {
                current = current->next;
                current_idx++;
            }
            
            if (current) {
                current->data = value;
            }
        }
    }
    
    // 리스트 출력
    void print() const {
        auto current = head_;
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }
    
    // 디버깅용: 헤드 노드의 참조 카운트 확인
    int get_head_ref_count() const {
        return head_ ? head_.use_count() : 0;
    }
};

int main() {
    // 원본 리스트 생성
    CowLinkedList<int> list1;
    list1.append(10);
    list1.append(20);
    list1.append(30);
    list1.append(40);
    
    std::cout << "List1: ";
    list1.print();
    
    // 얕은 복사로 두 번째 리스트 생성
    CowLinkedList<int> list2 = list1;
    std::cout << "List2: ";
    list2.print();
    
    std::cout << "Head 참조 카운트: " << list1.get_head_ref_count() << std::endl;
    
    // COW 발생: list2 수정
    std::cout << "\n===== list2의 인덱스 1 값을 25로 변경 =====\n";
    list2.set(1, 25);
    
    std::cout << "List1: ";
    list1.print();
    std::cout << "List2: ";
    list2.print();
    
    std::cout << "각 리스트의 Head 참조 카운트: " << list1.get_head_ref_count() 
              << ", " << list2.get_head_ref_count() << std::endl;
    
    // list1과 list2 분리 후 list1 수정
    std::cout << "\n===== list1의 끝에 50 추가 =====\n";
    list1.append(50);
    
    std::cout << "List1: ";
    list1.print();
    std::cout << "List2: ";
    list2.print();
    
    return 0;
}
