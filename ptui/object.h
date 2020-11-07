#pragma once

namespace ptui {

/**
 * @class object_t
 * @brief the base object which uses a curses pad type. The object provides
 * several operator overloads for placing text within the pad rectangle.
 */
class element_object_t : public event_dispatcher_t,
                         public data_layer_t,
                         public attribute_container_t {
public:
  using datalayer_t::operator=;
  using datalayer_t::operator<<;
  using datalayer_t::operator+=;

  element_object_t() {}

  element_object_t(const std::string &_id) : id(_id) {}

  element_object_t &operator()(const u_int16_t _row, const u_int16_t _col) {
    move(_row, _col);
    return *this;
  }
  /**
   * @fn flush()
   * @brief outputs the text in the stream to the screen
   */
  void flush(void) {
    if (!pad_handle) {
      pad_handle = newpad(max_h, max_w);
      pad_area = subpad(pad_handle, r, c, h, w);
    }

    char ch = {};
    while (out.get(ch))
      if (pechochar(pad_handle, ch) != OK)
        throw std::runtime_error("The 'pad' and 'subpad' "
                                 "could not be allocated.");
  }

  // @brief public data members
  std::string id = {};

private:
  WINDOW *pad_handle = {};
  WINDOW *pad_area = {};
};

} // namespace ptui
