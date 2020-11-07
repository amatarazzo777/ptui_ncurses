
// clang-format off

#include "../ptui/std_base.h"

#include "curses.h"

#include "../ptui/data_layer.h"
#include "../ptui/event.h"
#include "../ptui/object.h"
#include "../ptui/window.h"

// clang-format on

#pragma once

namespace ptui {

/**
 * @class
 * @brief tui_window_t
 * @tparam T the index type or range. It is nice to use a small value, like an
 * enumeration. The enum type provides compile time assessment rather than a
 * dynamic type which can be fumbled such as a string, or int, float. But using
 * those types might lead to some form of syntax derivative that exposes
 * dynamic additions, or dynamic ids within an interface. Either is fine.
 */
class window_t : public event_dispatcher_t,
                 public data_layer_t,
                 public document_management_t<element_object_> {
public:
  using datalayer_t::operator=;
  using datalayer_t::operator<<;
  using datalayer_t::operator+=;
  using attribute_container_t::operator[];

  /**
   * @fn tui_window_t
   * @brief initialize the ncurses library.
   * ncurses is the low level console for linux.
   */
  window_t() {
    initscr();
    cbreak();
    noecho();

    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    if (has_colors())
      start_color();
  }

  /**
   * @fn ~tui_window_t()
   * @brief destructor, cleanup curses
   */
  ~window_t() { endwin(); }

  /**
   * @fn operator()
   * @brief locate cursor row and column.
   */
  window_t &operator()(const u_int16_t &_row, const u_int16_t &_col) {
    move(_row, _col);
    return *this;
  }

  /**
   * @fn flush
   * @brief writes the output buffer to the screen.
   */
  int flush() {
    out << a;
    char ch = {};
    while (out.get(ch))
      addch(ch);
    return EXIT_SUCCESS;
  }

  /**
   * @fn tab_order
   * @tparam index_t...
   */
  template <index_t...> int tab_order(const index_t... args) {
    field_tab_order = {args...};
    return EXIT_SUCCESS;
  }

  /**
   * @fn wait_for_close
   * @brief the message process loop, wait on a key and dispatch to handler.
   * getch is setup to wait for a complete keystroke key and return it. This
   * includes the KEY_[NAME..] values as well as the regular printable
   * character keys.
   */
  int wait_for_close(void) {
    while (!bdone)
      dispatch(getch());
    return EXIT_SUCCESS;
  }

  int terminate(void) {
    bdone = true;
    return EXIT_SUCCESS;
  }

  std::stringstream out = {};

  u_int16_t line_offset = {};
  u_int16_t cur_x = {};
  u_int16_t cur_y = {};
  std::list<index_t> field_tab_order = {};

  bool bdone = {};
};

} // namespace ptui
