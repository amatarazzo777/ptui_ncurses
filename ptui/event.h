#pragma once

namespace ptui {

/**
 * @class event_t
 * @brief class used to communicate the event and its parameters
 */
class event_t {
public:
  enum code_t : u_int8_t { key_press, vkey_press, blur, focus, change };
  int key = {};
  std::string long_name = {};
  bool CTRL = {};
  bool SHIFT = {};
  bool ALT = {};
};

/**
 * @typedef fn_listen_t
 * @brief The listener type. Every function added through the event() must be
 * of this type. The function returns a bool for bubbling events and has as a
 * parameter an event_t object. The event_t object is used to represent all
 * events. Within the event handler, you should utilize the appropriate
 * parameters.
 */
using fn_listen_t = std::function<bool(const event_t &)>;

// @brief listener map
using event_listener_umap_t =
    std::unordered_map<std::string, std::list<fn_listen_t>>;

class event_dispatcher_t {
public:
  event_dispatcher_t() {}
  /**
   * @fn event
   * @brief  adds or clears an event listener.
   *
   * Adds an event listener for the named event type or particular virtual key.
   * The fn parameter is the function that is called when the named type has
   * been received from input. There can be only one event handler per object
   * for the event combination.
   *
   *
   * @tparam T The event name.
   *
   * @details The function is saved
   * along at that index.
   *
   * Calling the function with the template parameter with the fn not set will
   * remove the listener. There can be only one listener per type per object.
   * Event listeners can be described in several ways. The system provides the
   * most consolidated syntax in that the the virtual key may be named. These
   * are the keys that text interfaces are most concerned with in capturing
   * within an interface prospective. The text editing portion (edit control)
   * provides the base operations such as delete, display, string modification.
   * All other control object derive from this.
   *
   */
  void listen(const std::string &evt, const fn_listen_t &_fn = {}) {
    if (!_fn)
      event_listener_umap.erase(evt);
    else
      event_listener_umap[evt].push_back(_fn);
  }

  /**
   * @fn dispatch
   * @param int evt - the KEY_[NAME...] of the integer event defined within
   * curses.h
   * @brief class the mapped function if one is assigned.
   */
  void dispatch(int evt) {
    auto it = event_listener_umap.find(evt);
    if (it != event_listener_umap.end()) {
      for (auto fn : it->second) {
    	  // not bubbling stops dispatch.
        if (!fn(evt))
          break;
      }
    }
  }

  event_listener_umap_t event_listener_umap = {};
};

} // namespace ptui
