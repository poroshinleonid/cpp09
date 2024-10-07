#include "BitcoinExchange.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Error: invalid arguments" << std::endl;
    std::cout << "usage: ./btc input_file_name" << std::endl;
    return 1;
  }
  try {
    BitcoinExchange database("data.csv");
    database.process_req_file(argv[1]);
    return 0;
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
