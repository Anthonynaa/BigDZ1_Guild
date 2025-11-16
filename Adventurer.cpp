#include "Adventurer.h"

#include <iostream>
#include <sstream>

Adventurer::Adventurer(int id, const std::string& name,
                       const std::string& surname, const std::string& role,
                       int level, int reputation)
    : id(id),
      name(name),
      surname(surname),
      role(role),
      level(level),
      reputation(reputation) {
  calculateRank();
}

void Adventurer::calculateRank() {
  if (reputation < 50)
    rank = "Newbie";
  else if (reputation < 100)
    rank = "Venturer";
  else if (reputation < 200)
    rank = "Guardian";
  else if (reputation < 300)
    rank = "Veteran";
  else if (reputation < 350)
    rank = "Hero";
  else if (reputation < 400)
    rank = "Renowned Hero";
  else
    rank = "Legend";
}

std::string Adventurer::serialize() const {
  std::ostringstream out;
  out << id << ";" << name << ";" << surname << ";" << role << ";" << level
      << ";" << reputation << ";" << rank;
  return out.str();
}

Adventurer Adventurer::deserialize(const std::string& line) {
  std::istringstream in(line);
  std::string temp;

  int id, level, reputation;
  std::string name, surname, role, rank;

  std::getline(in, temp, ';');
  id = std::stoi(temp);
  std::getline(in, name, ';');
  std::getline(in, surname, ';');
  std::getline(in, role, ';');
  std::getline(in, temp, ';');
  level = std::stoi(temp);
  std::getline(in, temp, ';');
  reputation = std::stoi(temp);
  std::getline(in, rank, ';');

  Adventurer a(id, name, surname, role, level, reputation);
  a.rank = rank;
  return a;
}

void Adventurer::printInfo() const {
  std::cout << "ID: " << id << "\n"
            << "Name: " << name << " " << surname << "\n"
            << "Role: " << role << "\n"
            << "Level: " << level << "\n"
            << "Reputation: " << reputation << "\n"
            << "Rank: " << rank << "\n\n";
}
