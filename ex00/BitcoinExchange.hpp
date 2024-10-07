#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

class BitcoinExchange {
private:
  BitcoinExchange();

public:
  BitcoinExchange(const BitcoinExchange &other);
  BitcoinExchange &operator=(const BitcoinExchange &other);
  ~BitcoinExchange();

public:
  BitcoinExchange(const std::string &db_filename);

public:
  void process_req_file(const char *req_filename) const;

private:
  void show_total_cost(const std::string &date, double amount) const;
  std::string get_database_date(const std::string &date) const;
  bool is_date_valid(const std::string &date) const;

private:
  std::map<std::string, double> table;
};

#endif // BITCOINEXCHANGE_HPP
