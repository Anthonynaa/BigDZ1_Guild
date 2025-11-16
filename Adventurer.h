#pragma once
#include <string>

class Adventurer {
 public:
  int id;
  std::string name;
  std::string surname;
  std::string role;
  int level;
  int reputation;
  std::string rank;

  Adventurer() = default;
  Adventurer(int id, const std::string& name, const std::string& surname,
             const std::string& role, int level, int reputation);

  void calculateRank();

  std::string serialize() const;
  static Adventurer deserialize(const std::string& line);

  void printInfo() const;
};
