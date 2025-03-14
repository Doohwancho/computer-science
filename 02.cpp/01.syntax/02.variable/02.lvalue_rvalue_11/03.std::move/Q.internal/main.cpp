//Q. what is std::move()의 내부구현?
//A. 해당 데이터의 pointer를 해제시켜주는 함수인가보다. 
//   다른 포인터가 가르킬 수 있게 

template<typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}
