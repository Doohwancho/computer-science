#include <string>
#include <iostream>
#include <fstream>

class FileManager {
  std::unique_ptr<File> current_file_;  // 이 클래스만 이 파일 소유
  public:
    void OpenFile(const std::string& path) {
      current_file_ = std::make_unique<File>(path);
    }
    // 이 함수는 생성한 객체의 소유권을 호출자에게 넘김
    std::unique_ptr<File> ReleaseFile() {
      return std::move(current_file_);
    }
  };
