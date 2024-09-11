#include <iostream>
#include <string>

// Model: Student
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

// View: Display the student data
class StudentView {
 public:
  void printStudentDetails(const std::string& studentName, int studentAge) {
    std::cout << "Student: " << std::endl;
    std::cout << "Name: " << studentName << std::endl;
    std::cout << "Age: " << studentAge << std::endl;
  }
};

// Controller: Handles the interaction between the Model and the View
class StudentController {
 public:
  StudentController(Student& model, StudentView& view)
      : model(model), view(view) {}

  void setStudentName(const std::string& name) { model.setName(name); }

  std::string getStudentName() const { return model.getName(); }

  void setStudentAge(int age) { model.setAge(age); }

  int getStudentAge() const { return model.getAge(); }

  void updateView() {
    view.printStudentDetails(model.getName(), model.getAge());
  }

 private:
  Student& model;
  StudentView& view;
};

int main() {
  // Create the Model, View, and Controller
  Student student("John Doe", 20);
  StudentView view;
  StudentController controller(student, view);

  // Display initial data
  controller.updateView();

  // Update student data and display updated data
  controller.setStudentName("Jane Smith");
  controller.setStudentAge(22);
  controller.updateView();

  return 0;
}
