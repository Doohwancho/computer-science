# Overview

![alt text](../overview.png)

# how to compile and run?

```cpp
cd dir/
clang++ *.cpp -std=c++17 -o main 
./main
```


# prediction 1
책 읽기 전


1. 최소단위로 쪼개기
    1. 최소단위? -> 변수, 함수, 클래스. 근데 클래스도 함수로 치환 가능하고, 함수도 변수로 치환 가능하고, 변수는 포인터+값으로 치환 가능하지 않나? 그러니까 최소단위는 포인터+값이 아닐까? 
2. {} 영역별로 최소단위로 쪼갠거 구분
3. 트리에 서순에 맞게 넣기
    1. Q. 서순을 먼저 정렬한 다음, 트리에 정렬한거 순서대로 넣는건가? 아니면 트리에 넣을 때 정렬 알고리즘이 메겨진 insert하는건가?
4. 트리 만든걸 어셈블리어로 변환해서 cpu에 순차적으로 쏠 수 있게 이쁘게 정리. 이게 아마 compile
    1. 트리에 차곡차곡 넣은걸 bfs로 돌면서 어셈블리어로 바꾸겠지?


# prediction 2
책 대목차까지 읽은 후


1. 최소단위로 쪼개고 {} 단위로 묶기
    1. 최소단위? -> 변수, 함수, 클래스. 근데 클래스도 함수로 치환 가능하고, 함수도 변수로 치환 가능하고, 변수는 포인터+값으로 치환 가능하지 않나? 그러니까 최소단위는 포인터+값이 아닐까? 
    2. 어휘 분석
        1. 어휘(숫자, 문자열, 식별자, 연산자 등..) -> 토큰
    3. 구문 분석
        1. 여기에서 {} 단위로 구분하는 듯?
2. 트리에 서순에 맞게 넣기
    1. Q. 서순을 먼저 정렬한 다음, 트리에 정렬한거 순서대로 넣는건가? 아니면 트리에 넣을 때 정렬 알고리즘이 메겨진 insert하는건가?
    2. 인터프리터
        1. 해석기가 최소단위로 쪼개고 {}단위로 구분된 토큰들을 돌면서 트리에 넣는건가?
    3. 코드 생성
        1. 여긴 뭐하는 단계지? 트리에 넣은애들은 bfs로 돌면서 한줄로 뽑는 컴파일 단계인건가?
    4. 가상머신
        1. 트리 만들고 트리에 넣고, 빼서 어셈블리 한줄 촤라락 하는 작업도 메모리가 필요하니까, 그걸 할당해주는 부분인건가? 
4. 트리 만들걸 어셈블리어로 변환해서 cpu에 순차적으로 쏠 수 있게 이쁘게 정리. 이게 아마 compile
    1. 트리에 차곡차곡 넣은걸 bfs로 돌면서 어셈블리어로 바꾸겠지?



# Correction after reading part1. 어휘 분석기

1. 어휘 분석기(parser)
2. 구문 분석기(scanner)
	1. 인터프리터(interpretor)
	2. 제너레이터(generator, 소스코드 -> 목적코드) 
		1. 목적코드 runs on virtual machine


Q. 최소 단위 어떻게 쪼개지?\
Q. 최소 단위는 포인터+값 아닐까?\
Q. 구문 분석기에서, 트리에 넣을 때, 넣을 순서를 먼저 정렬한 후에 넣는건가 아니면 넣으면서 순서 정렬하는 insert tree algorithm 쓰는건가?\
Q. 인터프리터던 제너레이터던 트리를 돌 때, bfs로 도나 dfs로 도나?\
Q. 인터프리터는 제너레이터+실행 명령어인가? 본질적으로 제너레이터+알파인가?\
Q. 가상 머신이라는게, parser, scanner, generator가 돌 메모리 할당해준다는거 아닌가? 그러면 왜 굳이 가상이라는 단어를 붙였지? 그냥 메모리 할당해주는건데.




# Correction2 after going through codes in parser()

1. 구문 분석기(scanner)
2. 어휘 분석기(parser)
	1. 인터프리터(interpretor)
	2. 제너레이터(generator, 소스코드 -> 목적코드) 
		1. 목적코드 runs on virtual machine


## Q. 최소 단위 어떻게 쪼개지?

A. Kind라는 enum에 숫자,문자열,특수문자,null 등 타입을 정해놓고,
Map<Kind, string> 을 만든다.
kind:string은 Token이라는 최소단위가 된다.
scanner(param)에 입력받은 애를 돌면서, 타입에 맞게 Token을 만들어서 vector<Token>에 넣어준다. 

## Q. 최소 단위는 포인터+값 아닐까?

A. 어셈단 까지 가면 그렇겠지만,

Token.h에서 선언한 enum::Kind가 ( ; String 등 타입 선언해주고, 이걸 key삼고 value에 이름 매핑한게 
Token.cpp에서 Token{Kind,string} 인 map임.

이게 최소단위가 되어, Scanner.cpp에서 input code in string을 스캔시, 각 단어마다 parse해서 저 Token단위로 쪼개서 list에 넣음.)
 

## Q. 구문 분석기에서, 트리에 넣을 때, 넣을 순서를 먼저 정렬한 후에 넣는건가 아니면 넣으면서 순서 정렬하는 insert tree algorithm 쓰는건가?

## Q. 인터프리터던 제너레이터던 트리를 돌 때, bfs로 도나 dfs로 도나? 


## Q. 인터프리터는 제너레이터+실행 명령어인가? 본질적으로 제너레이터+알파인가?

## Q. 가상 머신이라는게, parser, scanner, generator가 돌 메모리 할당해준다는거 아닌가? 그러면 왜 굳이 가상이라는 단어를 붙였지? 그냥 메모리 할당해주는건데.




# input && output of scanner 



## input 

```cpp
 string sourceCode = R""""(
    function main() {
      print 'Hello, World!';
    }
  )"""";
```


## output 

```
KIND        STRING
-----------------------
function    function
#identifier main
(           (
)           )
{           {
print       print
#String     Hello, World!
;           ;
}           }
#EndOfToken
```



## Scanner/ 코드 해석 

1. 일단 Kind라는 이름의 enum만들어서 나올 수 있는 타입 다 때려박음. (라던지 String이라던지, ;이라던지 등..
2. Token이라는 클래스에 key:value로 Kind:매칭되는 string 묶음 
3. Scanner.cpp에서 scan할건데, Integer이건, String이건, WhiteSpace건, OperatorAndPunctuator이건, 미리 정해서 enum에 담아넣고, scan(code_in_string)으로 읽는데, 각 타입마다, 맞게 처리해서 Token{Kind:string}을 만들어 list에 담음.
4. Main에서 이 list를 돌면서 프린트 찍으면 output이 위처럼 나오는 것.



