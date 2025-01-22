#include <cstdint>
#include <iostream>

int32_t main() {
  int arr[3];

  arr[0] = 1;
  arr[1] = 2;
  arr[2] = 3;

  std::cout << "array elements: " << std::endl;
  for(int i = 0; i < 3; i++) {
    std::cout << "elements: " << i << ": " << arr[i] << std::endl;
  }

  return 0;
}