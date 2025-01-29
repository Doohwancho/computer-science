1. goal

build stack from scratch, using 1. array 2. linked list 3. queue


2. features

1. insert()
2. pop()
3. size()
4. isEmpty()


3. build stack using array

1. 생성자에 int size를 받아, new array[size]; 해서 array 생성
2. 'tail' 이라는 이름의 int index를 -1로 생성해서, insert() 시+1,
3. pop()시, tail이 < 0 인지 validation check한 후, 아니면 return arr[tail--];
4. size()는 return tail+1;
5. isEmpty()는 return tail == -1;


time complexity: O(1) (both insert(), pop())
space complexity: O(N)



4. build stack using linkedlist

1. class Node에 Node prev, Integer value 필드 가진 클래스 만들고,
2. class Stack에 Node node, int size 필드 생성
3. insert()시 Node newNode = new Node(val) 새로 만들고, newNode.prev = node, node = newNode; 하면 될 듯? size++; 하고.
4. pop()시, if(node == null) throw Message(); 로 validation check 이후, node.val 값 반환하기 전, node = node.prev; 할 듯?
5. size()는 return size;
6. isEmpty()는 return node == null;

time complexity: O(1) for both insert(), pop()
space complexity: O(N)



5. build stack using queue

2개의 큐를 이용해서, 하노이의 탑 처럼,
insert()시엔 queue-1 에 새 값을 넣고, queue-2에 있는 값들을 .enque()로 queue-1에 넣고,
다시 insert()시에는 queue-2에 새 값을 넣고, queue-1에 있는 값들을 .enque()로 queue-2에 넣는 식으로 구현하면 될 듯.

pop()시에는 main queue.deque()하면 될 듯?


1. class Stack에 queue a,b 2개 만듬 + queue a,b중 어느게 main-queue인지 알게하기 위한 boolean flag 생성
2. insert()는, sub-queue에 insert하고, main-queue -> sub-queue로 모조리 옮김.
3. pop()은 main-queue.enque()
4. size()는 return main-queue.size()
5. isEmpty()는 return main-queue.size() == 0;

time complexity: pop()은 O(1)로 다른 풀이랑 같은데, insert()할 떄마다, queue를 통쨰로 옮겨야 하니까 O(N)
space complexity: enque()할 때마다, 모든 queue의 있는 값들을 다른 큐로 옮겨야 하기 때문에 O(N), 이긴 한데, 다른 풀이 대비, 메모리 소모량 2배

sample code in kotlin
```kotlin
class StackQueue {
    private val q1 = LinkedList<Int>()
    private val q2 = LinkedList<Int>()

    fun push(item: Int) {
        q2.addFirst(item)
        while (!q1.isEmpty()) {
            q2.addFirst(q1.removeFirst())
        }
        val temp = q1
        q1 = q2
        q2 = temp
    }

    fun pop(): Int? {
        if (!isEmpty()) {
            return q1.removeFirst()
        } else {
            return null
        }
    }

    fun isEmpty(): Boolean {
        return q1.isEmpty()
    }
}

```

이 코드엔 boolean flag안쓰고 main/sub queue 구분함.


