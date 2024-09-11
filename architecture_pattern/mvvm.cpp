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

// Model: Student
class Student {
 private:
  std::string name;
  int age;

 public:
  Student(const std::string& name, int age) : name(name), age(age) {}

  std::string getName() const { return name; }
  void setName(const std::string& name) { this->name = name; }

  int getAge() const { return age; }
  void setAge(int age) { this->age = age; }
};

// ViewModel: Handles communication between the View and the Model
class StudentViewModel : public Observable {
 private:
  Student& model;

 public:
  StudentViewModel(Student& model) : model(model) {}

  std::string getStudentName() const { return model.getName(); }

  void setStudentName(const std::string& name) {
    model.setName(name);
    notifyObservers();  // 데이터 변경 시 옵저버에게 알림
  }

  int getStudentAge() const { return model.getAge(); }

  void setStudentAge(int age) {
    model.setAge(age);
    notifyObservers();  // 데이터 변경 시 옵저버에게 알림
  }
};

// View: Display the student data
class StudentView : public Observer {
 private:
  StudentViewModel& viewModel;

 public:
  StudentView(StudentViewModel& viewModel) : viewModel(viewModel) {
    // ViewModel에 옵저버로 등록
    viewModel.addObserver(this);
  }

  // ViewModel의 변경 사항을 반영하여 화면을 업데이트
  void update() override {
    std::cout << "Student: " << std::endl;
    std::cout << "Name: " << viewModel.getStudentName() << std::endl;
    std::cout << "Age: " << viewModel.getStudentAge() << std::endl;
  }

  ~StudentView() {
    viewModel.removeObserver(
        this);  // 메모리 누수를 방지하기 위해 옵저버에서 제거
  }
};

int main() {
  // Model, ViewModel, View 생성
  Student student("John Doe", 20);
  StudentViewModel viewModel(student);
  StudentView view(viewModel);

  // 초기 데이터 출력
  view.update();

  // 데이터 변경 -> View 자동 업데이트
  viewModel.setStudentName("Jane Smith");
  viewModel.setStudentAge(22);

  return 0;
}
