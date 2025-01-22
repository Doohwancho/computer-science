#include <iostream>
#include <cstdint>
#include <array>

//write print for array
void print_array(int arr[], int size) {
  for(int i = 0; i < size; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}
// Write print function for std::array
template <typename T, std::size_t N>
void print_std_array(const std::array<T, N>& arr) {
    for (const auto& element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

//case1)
int arr_global[10]; //global scope에 만들면 NULL로 초기화 됨

int32_t main() {
  print_array(arr_global, 10); //0 0 0 0 0 0 0 0 0 0 

  //case2)
  int arr_inside_main[5]; //main()안에 선언하면 NULL로 초기화되는게 아니라 쓰레기값으로 차있기 때문에 NULL과 !=, == 연산 불가능.
  print_array(arr_inside_main, 5); //1 82591840 1 1794419408 1 

  //case3) 오해 - initialize array with -1 
  int alphabet[26] = { -1 }; //이러면 맨 첫 element만 -1가 되고 나머지는 default-initialized
  print_array(alphabet, 26); //-1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

  //case4) initialize with 0 
  int arr_initialized_zero[26] = {}; //하면 0으로 initialized 된다. 
  print_array(arr_initialized_zero, 26); //0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

  //case5) std::array 사용해서 initialize 하기 
  std::array<int, 5> arr_lib= {1,2,3,4,5};
  print_std_array(arr_lib); //1 2 3 4 5

  return 0;
}