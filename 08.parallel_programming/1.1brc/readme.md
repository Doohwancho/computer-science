
---\
Takeaway


1. thread scaling 과 overhead, 그리고 균형점 찾기 및 최적화
2. parallel 하게 뭔가 처리할때 fine grained 하게 task 나누고 배분하기
3. 그렇게 처리한 작업들의 aggregation 은 어떻게 할건지
4. 내가 사용하는 언어/런타임에서 사용하는 primitive datatype 의 스펙



---\
Concepts


- java
    1. create 1 billion rows text file :white_check_mark:
    2. baseline :white_check_mark:
    3. parallel :white_check_mark:
    4. memory allocation: memory mapping :white_check_mark:
    5. memory allocation: direct buffer :white_check_mark:
    6. chunking :white_check_mark:
- c
- c++
- go
- js
