#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Observer 인터페이스
class Observer {
 public:
  virtual void update() = 0;
};

// Subject 클래스
class Observable {
 private:
  std::vector<Observer*> observers;

 public:
  void addObserver(Observer* observer) { observers.push_back(observer); }

  void removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void notifyObservers() {
    for (Observer* observer : observers) {
      observer->update();
    }
  }
};

// Entity
class Student {
 public:
  Student(const std::string& name, int age) : name(name), age(age) {}

  std::string getName() const { return name; }
  void setName(const std::string& name) { this->name = name; }

  int getAge() const { return age; }
  void setAge(int age) { this->age = age; }

 private:
  std::string name;
  int age;
};

// Use Cases
class UpdateStudentNameUseCase {
 public:
  UpdateStudentNameUseCase(Student& student) : student(student) {}

  void execute(const std::string& newName) {
    student.setName(newName);
    // 추가적인 비즈니스 로직을 수행할 수 있음
  }

 private:
  Student& student;
};

class UpdateStudentAgeUseCase {
 public:
  UpdateStudentAgeUseCase(Student& student) : student(student) {}

  void execute(int newAge) {
    student.setAge(newAge);
    // 추가적인 비즈니스 로직을 수행할 수 있음
  }

 private:
  Student& student;
};

// ViewModel
class StudentViewModel : public Observable {
 public:
  StudentViewModel(Student& student)
      : student(student),
        updateNameUseCase(student),
        updateAgeUseCase(student) {}

  std::string getStudentName() const { return student.getName(); }

  void setStudentName(const std::string& name) {
    updateNameUseCase.execute(name);
    notifyObservers();  // 데이터를 변경할 때마다 옵저버들에게 알림
  }

  int getStudentAge() const { return student.getAge(); }

  void setStudentAge(int age) {
    updateAgeUseCase.execute(age);
    notifyObservers();  // 데이터를 변경할 때마다 옵저버들에게 알림
  }

 private:
  Student& student;
  UpdateStudentNameUseCase updateNameUseCase;
  UpdateStudentAgeUseCase updateAgeUseCase;
};

// View
class StudentView : public Observer {
 public:
  StudentView(StudentViewModel& viewModel) : viewModel(viewModel) {
    viewModel.addObserver(this);  // ViewModel을 구독
  }

  void update() override {
    // ViewModel에서 데이터를 가져와 화면에 표시
    std::cout << "Student: " << std::endl;
    std::cout << "Name: " << viewModel.getStudentName() << std::endl;
    std::cout << "Age: " << viewModel.getStudentAge() << std::endl;
  }

  ~StudentView() {
    viewModel.removeObserver(this);  // 메모리 관리: 옵저버 해제
  }

 private:
  StudentViewModel& viewModel;
};

int main() {
  // 도메인 객체 생성
  Student student("John Doe", 20);

  // ViewModel 생성 (Use Case와 함께)
  StudentViewModel viewModel(student);

  // View 생성 (ViewModel 구독)
  StudentView view(viewModel);

  // 초기 데이터 출력
  view.update();

  // ViewModel을 통해 데이터 변경
  viewModel.setStudentName("Jane Smith");
  viewModel.setStudentAge(22);

  // 변경된 데이터 자동 업데이트
  return 0;
}
