#pragma once

namespace ptui {

class data_layer_t {
public:
  template <typename T2> data_layer_t &operator=(const T2 &s) {
    out.clear();
    buffer.clear();
    return operator<<(s);
  }

  /**
   * @overload
   * @fn operator=
   * @param const std::filesystem::path &fpath - the file location
   * @@brief read a file and place into object view.
   */
  data_layer_t &operator=(const std::filesystem::path &fpath) {
    out.clear();
    buffer.clear();
    return operator<<(fpath);
  }

  /**
   * @overload
   * @fn operator<<
   * @param const std::string &s
   * @brief Adds a stream to the output.
   */
  template <typename T2> data_layer_t &operator<<(const T2 &a) {
    out << a;
    buffer << a;
    return *this;
  }

  /**
   * @overload
   * @fn operator<<
   * @param const std::string &s
   * @brief Adds a stream to the output.
   */
  template <typename T2>
  data_layer_t &operator<<(const std::filesystem::path &f) {
    return operator<<(read_file(f));
  }

  /**
   * @overload
   * @fn operator+=
   * @brief appends data, same as operator<<;
   */
  template <typename T2> data_layer_t &operator+=(const T2 &s) {
    return operator<<(s);
  }

  std::string data(void) { return buffer.str(); }

  std::stringstream read_file(const std::filesystem::path &filename);

  std::stringstream out = {};
  std::stringstream buffer;

}; // namespace ptui

} // namespace ptui
