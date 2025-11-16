#pragma once
#include <string>
#include <vector>

#include "Adventurer.h"

class GuildDB {
 private:
  std::vector<Adventurer> database;
  std::string filename;

 public:
  GuildDB(const std::string& filename = "guild_db.txt");

  void addAdventurer();
  void viewAll() const;
  void search() const;
  void removeByID();
  void modifyReputation();

  void loadFromFile();
  void saveToFile() const;

  Adventurer* findByID(int id);
  void printMenu();
  void runMenu();
};
