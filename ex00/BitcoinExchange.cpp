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
  std::tm tm = {};
  std::istringstream ss(date);
  ss >> std::get_time(&tm, "%Y-%m-%d");
  if (ss.fail()) {
    return false;
  }
  tm.tm_isdst = -1;
  std::time_t time = std::mktime(&tm);
  if (time == -1) {
    return false;
  }
  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }
  std::tm *tm_check = std::localtime(&time);
  return (tm_check->tm_year == tm.tm_year && tm_check->tm_mon == tm.tm_mon &&
          tm_check->tm_mday == tm.tm_mday);
}

void BitcoinExchange::process_req_file(const char *req_filename) const {
  std::ifstream ifile(req_filename);
  if (ifile.is_open() == false) {
    std::cout << "Error: could not open the requests file." << std::endl;
    return;
  }
  std::string line;
  std::getline(ifile, line);
  if (line != "date | value") {
    std::cout << "Error in the input file format" << std::endl;
    return;
  }
  while (std::getline(ifile, line)) {
    if (line.size() < 14 || line[4] != '-' || line[7] != '-' ||
        line[10] != ' ' || line[11] != '|' || line[12] != ' ') {
      std::cout << "Error: bad input. => " << line << std::endl;
      continue;
    }
    std::string date = line.substr(0, 10);
    if (!is_date_valid(date)) {
      std::cout << "Error: bad date. => " << date << std::endl;
      continue;
    }
    line.erase(0, 13);
    std::stringstream ss(line);
    std::string amount_str;
    if (std::getline(ss, amount_str)) {
      double amount;
      size_t endpos;
      try {
        amount = std::stof(amount_str, &endpos);
      } catch (std::invalid_argument) {
        std::cout << "Error: not a number. => " << amount_str << std::endl;
        continue;
      } catch (std::out_of_range) {
        std::cout << "Error: too large a number. => " << amount_str
                  << std::endl;
        continue;
      }
      if (endpos != amount_str.size()) {
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
    } else {
      std::cout << "Error: bad input. => " << line << std::endl;
      continue;
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
    throw std::out_of_range("date too old");
    // return table.begin()->first;
  }
  --it;
  return it->first;
}

void BitcoinExchange::show_total_cost(const std::string &date,
                                      double amount) const {
  try {
    std::string database_date = get_database_date(date);
    std::cout << date << " => " << amount << " = "
              << table.at(database_date) * amount << std::endl;
  } catch (std::out_of_range &e) {
    std::cout << "Error: " << e.what() << " => " << date << std::endl;
  }
}