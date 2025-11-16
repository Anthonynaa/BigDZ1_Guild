#include "GuildDB.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

// Add. func for safe int read from console
static int readInt(const std::string& prompt) {
  while (true) {
    std::cout << prompt;
    int x;
    if (std::cin >> x) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return x;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Incorrect input, try again.\n";
  }
}

GuildDB::GuildDB(const std::string& filename_) : filename(filename_) {
  loadFromFile();
}

void GuildDB::loadFromFile() {
  database.clear();
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    return;
  }

  std::string line;
  while (std::getline(fin, line)) {
    if (line.empty()) continue;
    Adventurer a = Adventurer::deserialize(line);
    a.calculateRank();
    database.push_back(a);
  }
  fin.close();
}

void GuildDB::saveToFile() const {
  std::ofstream fout(filename, std::ios::trunc);
  if (!fout.is_open()) {
    std::cout << "Couldn't open file for writing: " << filename << '\n';
    return;
  }
  for (const auto& a : database) {
    fout << a.serialize() << '\n';
  }
  fout.close();
}

void GuildDB::addAdventurer() {
  std::string name, surname, role;
  int level = 0;
  int reputation = 0;

  std::cout << "=== New Member Registration ===\n";
  std::cout << "Name: ";
  std::getline(std::cin, name);
  std::cout << "Surname: ";
  std::getline(std::cin, surname);
  std::cout << "Role: ";
  std::getline(std::cin, role);

  while (true) {
    level = readInt("Level: ");
    if (level > 0) break;
    std::cout << "Level must be greater than 0.\n";
  }

  std::cout << "Reputation (Press Enter for 0): ";
  std::string repLine;
  std::getline(std::cin, repLine);
  if (!repLine.empty()) {
    try {
      reputation = std::stoi(repLine);
    } catch (...) {
      std::cout << "Incorrect reputation format. Set to 0.\n";
      reputation = 0;
    }
  } else {
    reputation = 0;
  }

  int nextId = 1;
  for (const auto& a : database)
    if (a.id >= nextId) nextId = a.id + 1;

  Adventurer a(nextId, name, surname, role, level, reputation);
  a.calculateRank();

  database.push_back(a);
  std::cout << "New Adventurer Added:\n";
  a.printInfo();
}

void GuildDB::viewAll() const {
  if (database.empty()) {
    std::cout << "Database is empty!\n";
    return;
  }
  std::cout << "=== Adventurer's Guild Database ===\n";
  for (const auto& a : database) a.printInfo();
}

Adventurer* GuildDB::findByID(int id) {
  for (auto& a : database) {
    if (a.id == id) return &a;
  }
  return nullptr;
}

void GuildDB::search() const {
  if (database.empty()) {
    std::cout << "Database is empty!\n";
    return;
  }

  std::cout << "Search Criteria:\n"
            << "1) ID\n2) Name/Surname\n3) Role\n4) Level\n5) Reputaion\n6) "
               "Rank\nChoose criteria (1-6): ";
  int k;
  if (!(std::cin >> k)) {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return;
  }
  std::cin.ignore(10000, '\n');

  std::vector<const Adventurer*> results;

  if (k == 1) {
    int id = readInt("ID: ");
    for (const auto& a : database)
      if (a.id == id) results.push_back(&a);
  } else if (k == 2) {
    std::string s;
    std::cout << "Name/Surname: ";
    std::getline(std::cin, s);
    for (const auto& a : database)
      if (a.name == s || a.surname == s) results.push_back(&a);
  } else if (k == 3) {
    std::string r;
    std::cout << "Role: ";
    std::getline(std::cin, r);
    for (const auto& a : database)
      if (a.role == r) results.push_back(&a);
  } else if (k == 4) {
    int lvl = readInt("Level: ");
    for (const auto& a : database)
      if (a.level == lvl) results.push_back(&a);
  } else if (k == 5) {
    int rep = readInt("Reputation: ");
    for (const auto& a : database)
      if (a.reputation == rep) results.push_back(&a);
  } else if (k == 6) {
    std::string rk;
    std::cout << "Rank: ";
    std::getline(std::cin, rk);
    for (const auto& a : database)
      if (a.rank == rk) results.push_back(&a);
  } else {
    std::cout << "Incorrect criteria.\n";
    return;
  }

  if (results.empty()) {
    std::cout << "No reuslts found.\n";
  } else {
    std::cout << "Found entries:\n";
    for (const auto* p : results) p->printInfo();
  }
}

void GuildDB::removeByID() {
  if (database.empty()) {
    std::cout << "Database is empty!\n";
    return;
  }
  int id = readInt("Enter adventurer's ID: ");
  auto it = std::remove_if(database.begin(), database.end(),
                           [id](const Adventurer& a) { return a.id == id; });
  if (it == database.end()) {
    std::cout << "Adventurer not found! << \n";
    return;
  }
  bool found = (it != database.end());
  database.erase(it, database.end());
  if (found) std::cout << "Adventurer's entry deleted.\n";
}

void GuildDB::modifyReputation() {
  if (database.empty()) {
    std::cout << "Database is empty!\n";
    return;
  }
  int id = readInt("Adventurer's ID: ");
  Adventurer* p = findByID(id);
  if (!p) {
    std::cout << "Adventurer not found!\n";
    return;
  }

  int delta = readInt("Add reputation (Demote with a negative value): ");
  p->reputation += delta;
  p->calculateRank();
  std::cout << "Reputation Updated:\n";
  p->printInfo();
}

void GuildDB::printMenu() {
  std::cout << "\n=== Adventurer's Guild Menu ===\n"
            << "1) Register a New Member\n"
            << "2) Guild Members List\n"
            << "3) Search an Adventurer\n"
            << "4) Deleting from the Guild\n"
            << "5) Modify Reputation\n"
            << "6) Save Changes\n"
            << "0) Exit\n"
            << "Choose an option: ";
}

void GuildDB::runMenu() {
  while (true) {
    printMenu();
    int choice;
    if (!(std::cin >> choice)) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      continue;
    }
    std::cin.ignore(10000, '\n');

    if (choice == 0) break;
    switch (choice) {
      case 1:
        addAdventurer();
        break;
      case 2:
        viewAll();
        break;
      case 3:
        search();
        break;
      case 4:
        removeByID();
        break;
      case 5:
        modifyReputation();
        break;
      case 6:
        saveToFile();
        std::cout << "Saved in: " << filename << '\n';
        break;
      default:
        std::cout << "Incorrect option.\n";
        break;
    }
  }

  std::cout << "Save changes before exiting? 1-Yes / 0-No: ";
  int ans;
  if (std::cin >> ans && ans == 1) {
    saveToFile();
    std::cout << "Changes saved.\n";
  }
}
