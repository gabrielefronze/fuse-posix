/*
Created by Gabriele Gaetano Fronzé on 2019-10-16.
Authors:
- Gabriele Gaetano Fronzé <gabriele.fronze at to.infn.it>, 2019
- Vivek Nigam <viveknigam.nigam3@gmail.com>, 2020
*/

#ifndef RUCIO_FUSE_UTILS_H
#define RUCIO_FUSE_UTILS_H

#include <string>
#include <vector>
#include <array>
#include <map>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

std::string to_string(char* contents, size_t size);

// Returns true if and only if path is root path
bool is_root_path(const char *path);

// This function returns -1 if path contains no token 0 if path is root
// POSIX format depth in other cases
int path_depth(const char *path, char token = '/');

// Returns true if the path is in the form /rucio/server1 or /rucio/server1/
bool is_server_mountpoint(const char *path);

// This function returns true is the depth is 2 (e.g. /rucio/server1/scope1 or /rucio/server1/scope1/)
bool is_main_scope(const char *path);

// Returns true if file is hidden.
bool is_hidden(const std::string &path);

// Returns true if file is Mac system file.
bool is_mac_specific(const std::string &path);

// Method to parse a string-printed python list into a vector with correct type content
template<class T>
void tokenize_python_list(std::string list, std::vector<T>& target, char separator = ',', std::array<char,2> unwanted_chars = {'"', ' '}){
  std::string list_copy = list;
  std::string element;

  list_copy.erase(0, 1);
  list_copy.erase(list_copy.size() - 1);
  std::stringstream stream(list_copy.c_str());
  while (getline(stream, element, separator)) {
    for(const auto& ch : unwanted_chars){
      element.erase(std::remove(element.begin(), element.end(), ch), element.end());
    }
    target.emplace_back((T)element);
  }
}

// Path strings manipulation methods
void remove_trailing_token(std::string& path, std::string token = "/");
void remove_leading_token(std::string& path, std::string token = "/");
std::string remove_substring(const std::string& path, const std::string& subs);
std::string remove_root_path(const std::string& path);
std::string extract_server_name(const std::string& path);
std::string extract_scope(const std::string& path);
std::string extract_name(const std::string& path);
std::string get_did(const std::string& path);

// Function which splits a string containing multiple dids and stores them individually in a strings vector
void split_dids(const std::string &line, std::vector<std::string>& did_strings);

// Function to merge together strings splitted in multiple curl responses
static std::string did_string_remainder;
void coherentize_dids(std::string &did_string);

// Rucio data types
enum rucio_data_type{
  rucio_file,
  rucio_container,
  rucio_dataset
};

// Struct to represent a rucio did
struct rucio_did{
  std::string scope;
  rucio_data_type type;
  std::string name;
  std::string parent;
  int level;

  bool operator==(const rucio_did& check){
    return this->scope == check.scope && this->name == check.name;
  }
};

// Translators from string-like rucio did descriptor to rucio_did structures
void structurize_did(const std::string& did_str, std::vector<rucio_did>& target);
void structurize_container_did(const std::string& did_str, std::vector<rucio_did>& target);

// Downloading status cache methods
static std::unordered_set<std::string> downloading_status_cache;
void set_downloading(const std::string& path);
bool is_downloading(const std::string& path);
void set_downloaded(const std::string& path);

#endif //RUCIO_FUSE_UTILS_H
