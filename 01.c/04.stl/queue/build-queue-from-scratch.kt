Q. build queue from scratch


1. what is queue?
FIFO인 통로


2. what are features of queue?

1. enque()
2. deque()
3. getSize()
4. isEmpty()
5. clear()


3. 방법론


---
방법A: using 2 stacks


1. stack 2개 만듬
2. enqueue()시, stack a에 담음. O(1)
3. dequeue()가 좀 문젠데, stack a에 있는걸 모두 stack b에 담고, stack-b.pop() 해서 리턴하는데, 이 때, 다음 enqueue()하는 대상을 stack-a에서 stack-b로 바꿈. vice-versa
4. 아, getSize()도 신경써야 하니까, enqueue()시 int size+1, deque()시 int size-1 해줌.
5. clear()은 stack-a, stack-b 둘 다 초기화 해줌.
6. isEmpty()는 현재 쌓는 stack의 size() == 0 return


방법A의 장단점
장점: enqueue() 빠름.
단점: dequeue()가 O(N). 너무 느림. + requires more memory


교정사항

1. 처음 예상에는, stack-a랑 stack-b를 dequeue()할 떄마다, 통째로 옮기고, enqueue()하는 대상을 swap()해줘야 한다고 했는데, 그럴 필요 없음. 아래와 같이 구현하면 됨. stack2가 비어있을 때에만, stack1에서 부으면 됨. 근데 이 방법도 dequeue()가 expensive.


```kotlin
class QueueUsingStack<T> {
    private val stack1 = Stack<T>()
    private val stack2 = Stack<T>()

    fun enqueue(item: T) {
        stack1.push(item)
    }

    fun dequeue(): T? {
        if (stack2.isEmpty()) {
            while (stack1.isNotEmpty()) {
                stack2.push(stack1.pop())
            }
        }
        return if (stack2.isEmpty()) null else stack2.pop()
    }
}

```


---
방법B: using array


1. dynamic array 형식. fixed size 10을 처음에 만들고, 80% 차면 사이즈 2배씩 곱하는 식으로 함.
2. array를 max_number나 min_number로 초기화 시키고, enqueue/dequeue의 parameter validation 검사시, max_num/min_num이면 기각되게 함.
3. two pointer로, deque할 떄 필요한 앞부분 인덱스 가르키는 포인터와, enqueu시 필요한 뒷부분 인덱스 가르키는 포인터 생성. head는 0부터, tail을 -1부터 시작.
4. enque()시, validation check 이후, array[++tail] = value; 넣음.
5. deque()시, return array[head++]
6. clear()은 array를 delete 후 다시 만들던가, for문으로 돌면서 다시 max_value 넣던가. dynamic array이니까, delete후 다시 만드는게 더 빠를 듯.
7. isEmpty()는 return head > tail하면 될 듯?


장점: enque()시 O(1), deque()시 O(1) 좋은데?
단점1: max_value도 input range중에 포함되는거라면, 씹히니까 문제 있을 듯? 근데 input range에 max_value가 걸리지만 않는다면, 성능상 압도적인거같은데?


교정사항
단점2: enqueue시 사이즈 꽉차서 늘릴 때, expensive할 수 있다.
단점3: dequeue()시 head++ 할 때, 원래있던 메모리 공간 안쓰는데 계속 쓴다고 잡힘. -> 코드가 오래되면, 문제발생할 수 있다.


```kotlin
class QueueUsingIntArray {
    private var array = IntArray(0)
    private var head = 0
    private var tail = 0

    fun enqueue(item: Int) {
        if (tail == array.size) {
            val newArray = IntArray(maxOf(2 * tail, 1))
            System.arraycopy(array, 0, newArray, 0, tail)
            array = newArray
        }
        array[tail++] = item
    }

    fun dequeue(): Int? {
        return if (isEmpty()) null else array[head++]
    }

    fun isEmpty(): Boolean {
        return head == tail
    }
}

```

Q. 단점3 문제 어떻게 해결하지?

circular buffer(ring buffer) 방식으로 해결 가능.

```kotlin
class CircularQueue(private val size: Int) {
    private val array = IntArray(size)
    private var front = 0
    private var rear = -1
    private var count = 0

    fun enqueue(value: Int) {
        if (isFull()) {
            throw IllegalStateException("Queue is full")
        }
        rear = (rear + 1) % size
        array[rear] = value
        count++
    }

    fun dequeue(): Int {
        if (isEmpty()) {
            throw IllegalStateException("Queue is empty")
        }
        val value = array[front]
        front = (front + 1) % size
        count--
        return value
    }

    fun isFull() = count == size

    fun isEmpty() = count == 0
}

```

- front가 head, rear이 tail
- front나 rear이나 queue.size를 초과하면, 다시 0으로 돌아오게끔 강제한다.



---
방법C: using linkedList

1. next node를 가르키는 필드랑 value 필드 두개 가진 Node 클래스 생성
2. Queue class에 Node인데, head & tail, 그리고 int size 필드 생성
3. enqueue()에 파라미터를 value로 받는 Node만든 후, head.next = new Node(val) 하고, tail은 해당 노드를 가르키게 함
4. dequeue()시, 맨 첫 노드의 값을 return 후, head = head.next;함.
5. clear()는 head = new Node(); 하고, head.next = null; 하면 됨. 끊어진 노드들은 gc가 처리하겠지.
6. isEmpty()는 return head != null


장점1: enqueue()시 O(1), dequeue()시 O(1)
장점2: 위에 두 방법보다 less memory 사용

```kotlin
class QueueUsingLinkedList<T> {
    private val list = LinkedList<T>()

    fun enqueue(item: T) {
        list.addLast(item)
    }

    fun dequeue(): T? {
        return if (list.isEmpty()) null else list.removeFirst()
    }
}

```


