#include "BitcoinExchange.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : table(other.table) {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this != &other) {
    table = other.table;
  }
  return *this;
}
BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string &db_filename) {
  std::ifstream ifile(db_filename);
  if (ifile.is_open() == false)
    throw std::runtime_error("could not open the subject's scv database file.");
  std::string line;
  std::getline(ifile, line);
  while (std::getline(ifile, line)) {
    std::stringstream ss(line);
    std::string date, price_str;
    std::getline(ss, date, ',');
    std::getline(ss, price_str);
    table[date] = std::stof(price_str);
  }
  ifile.close();
}

bool BitcoinExchange::is_date_valid(const std::string &date) const {
  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }
  int y, m, d;
  try {
    y = std::stoi(date.substr(0, 4));
    m = std::stoi(date.substr(5, 2));
    d = std::stoi(date.substr(8, 2));
  } catch (std::exception &e) {
    return false;
  }
  if (y < 1 || m < 1 || d < 1) { // REWORK
    return false;
  }
  return true;
}

void BitcoinExchange::process_req_file(const char *req_filename) const {
  std::ifstream ifile(req_filename);
  if (ifile.is_open() == false) {
    std::cout << "Error: could not open the requests file." << std::endl;
    return;
  }
  std::string line;
  std::getline(ifile, line);
  while (std::getline(ifile, line)) {
    std::stringstream ss(line);
    std::string date, amount_str;
    std::string dummy;
    if (std::getline(ss, date, ' ') && std::getline(ss, dummy, '|') &&
        std::getline(ss, dummy, ' ') && std::getline(ss, amount_str)) {
      if (!is_date_valid(date)) {
        std::cout << "Error: bad input. => " << date << std::endl;
        continue;
      }
      double amount;
      try {
        amount = std::stof(amount_str);
      } catch (...) {
        std::cout << "Error: not a number. => " << amount_str << std::endl;
        continue;
      }
      if (amount < 0) {
        std::cout << "Error: not a positive number. => " << amount_str
                  << std::endl;
        continue;
      }
      if (amount > 1000) {
        std::cout << "Error: too large a number. => " << amount_str
                  << std::endl;
        continue;
      }
      show_total_cost(date, amount);
    }
  }
  ifile.close();
}

std::string BitcoinExchange::get_database_date(const std::string &date) const {
  std::map<std::string, double>::const_iterator it = table.lower_bound(date);
  if (it != table.end() && it->first == date) {
    return it->first;
  }
  if (it == table.begin()) {
    return table.begin()->first;
  }
  --it;
  return it->first;
}

void BitcoinExchange::show_total_cost(const std::string &date,
                                      double amount) const {
  std::string database_date = get_database_date(date);
  std::cout << date << " => " << amount << " = "
            << table.at(database_date) * amount << std::endl;
}