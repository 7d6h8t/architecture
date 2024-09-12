#include <algorithm>
#include <format>
#include <iostream>
#include <observable/observable.hpp>
#include <string>
#include <vector>

// Model: Character
struct Character {
  uint32_t exp = 0;
  uint32_t att = 10;
  uint32_t def = 5;
};

// ViewModel: Handles communication between the View and the Model
class CharacterViewModel {
  OBSERVABLE_PROPERTIES(CharacterViewModel)
 public:
  CharacterViewModel() = default;
  ~CharacterViewModel() = default;

  void UpgradeAtt() {
    Character user = character_model_.get();
    user.att++;

    character_model_ = user;
    // 데이터 변경 시 옵저버에게 알림
  }

  void UpgradeDef() {
    Character user = character_model_.get();
    user.def++;

    character_model_ = user;
    // 데이터 변경 시 옵저버에게 알림
  }

  void UpgradeExp() {
    Character user = character_model_.get();

    if (++user.exp % 10 == 0) {
      // 데이터 변경 시 옵저버에게 알림
      character_levelup_subscription_.notify();
    }

    character_model_ = user;
    // 데이터 변경 시 옵저버에게 알림
  }

  observable::unique_subscription SubscribeLevelup(
      std::function<void()> notify) {
    return character_levelup_subscription_.subscribe(std::move(notify));
  }

  // private 처럼 동작 (외부에서 set 하면 error 발생)
  observable_property<Character> character_model_;

 private:
  observable::subject<void()> character_levelup_subscription_;
};

// View: Display the student data
class View {
 public:
  View(CharacterViewModel& character) : character_viewmodel_(character) {
    character_subscription_ = character_viewmodel_.character_model_.subscribe(
        [this](const Character& character) { ShowCharacterInfo(character); });

    character_levelup_subscription_ =
        character_viewmodel_.SubscribeLevelup([this]() { ShowLevelupEvent(); });
  }

  ~View() = default;

  void OnBtnClickAttUpgrade() { character_viewmodel_.UpgradeAtt(); }
  void OnBtnClickDefUpgrade() { character_viewmodel_.UpgradeDef(); }

 private:
  void ShowCharacterInfo(const Character& character) {
    std::cout << std::format(
                     "<< info >>\n===========\natt : {}\ndef : {}\nexp : "
                     "{}\n===========",
                     character.att, character.def, character.exp)
              << std::endl;
  }

  void ShowLevelupEvent() {
    std::cout << "=========== level up!! ===========" << std::endl;
  }

  observable::unique_subscription character_subscription_;
  observable::unique_subscription character_levelup_subscription_;

  CharacterViewModel& character_viewmodel_;
};

int main() {
  CharacterViewModel model;
  View view(model);

  std::cout << "att up : a\ndef up : b\nexp up : c" << std::endl;

  while (std::cin) {
    std::string input;
    std::cin >> input;

    if (input == "q") break;

    if (input == "a")
      view.OnBtnClickAttUpgrade();
    else if (input == "b")
      view.OnBtnClickDefUpgrade();
    else if (input == "c")
      model.UpgradeExp();
  }
  return 0;
}
