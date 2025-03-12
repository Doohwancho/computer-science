#include <map>


class TrieNode {
public:
    TrieNode() = default;

    explicit TrieNode(std::map<char, std::shared_ptr<const TrieNode>> children) : children_(std::move(children)) {}
    //Q. 왜 explicit 썼음?

    virtual ~TrieNode() = default;

    virtual auto Clone() const -> std::unique_ptr<TrieNode> { return std::make_unique<TrieNode>(children_); }

    std::map<char, std::shared_ptr<const TrieNode>> children_;

    bool is_value_node_{false};
};

void processMap(std::map<char, std::shared_ptr<const TrieNode>> m) {
    // 맵 처리 로직
}

int main() {
    std::map<char, std::shared_ptr<const TrieNode>> myMap;
    
    // explicit 없으면 이런 코드가 컴파일됨 (의도 아닌데도)
    TrieNode node;  // 빈 TrieNode 생성
    // processMap(node);  // 컴파일러가 'node'를 'std::map'으로 변환하려고 시도
    
    // explicit 있으면 이런 개삽질 코드 컴파일 안됨
    // 명시적으로 변환해야 함
    processMap(std::map<char, std::shared_ptr<const TrieNode>>{});

    return 0;
}
