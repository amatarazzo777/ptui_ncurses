#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <streambuf>
#include <string>
#include <variant>
#include <cstdint>

#include <ptui_ncurses.h>

using namespace std;
using namespace ptui;

// enum of screen objects. can dynamically add these as well.
enum screen_objects { regular_expression, status_text, input_text, match_area };

// function prototypes
void run_regex(window_t &mw);

/**
 * main
 *
 */
int main(int argc, char **argv) {
  auto mw = window_t{};

  if (argc != 2) {
    mw << "retest input.txt\n";
    return EXIT_FAILURE;
  }

  /**
   *
   * After my research, this form is very appropriate for multiple types of
   * rendering containers. This form can be used with xterm and ncurses as a tui
   * or text user interface. Text applications are useful for embedded devices,
   * programming utilities, program debugging and terminal based usage. Text
   * only. Their execution can occur without xwindows. So it is also useful for
   * boot tools and command line tools. these run in the ptui_ncurses container
   * as a compiled unit.
   *
   * As well as an xwindow application That uses the same font type - monospace
   * and grid text character spacing as its coordinate. As well, the form
   * provides backwards compatibility. While applications written for gui may
   * employ more settings, as long as these base settings remain the domain
   * name, they will function similarly.
   *
   * The main problem with the approach and its shelf life remain within the
   * gui's capability to advance in its display and control capability's and
   * states where the operation is not implemented within the xterm ncurses
   * driver.
   *
   * The applications are built with exports that allow them to load into a gui
   * container without source compilation.
   *
   *
   */
  const char *screen_definition = R"(
	:regular_expression:
		class      text.edit;
		row        1;
		size       1 79;
		buffer     1 1024;
		foreground red;
		prompt     "Regular Expression :";

	:status text: 
		class text.area; 
		row 1;

	:input_text: 
		class text.area; 
		row 2; 
		size 15 79;

	:match_area: 
		class text.area; 
		row 1;)";

  // setup screen objects.
  if (mw.add(screen_definition) != EXIT_SUCCESS)
    throw std::runtime_error("screen definition could not be parsed.");

  // read file passed as command line argument
  mw["input text"] = std::filesystem::path(std::string(argv[1]));

  mw(10, 10) = "@at 10,10";
  mw(10, 10) << "@same at 10,10";

  // when the enter key is pressed within the regular_expression field,
  // run the regex;
  mw["regular_expression"].listen("KEY_ENTER", [&](auto e) {
    run_regex(mw);
    return false;
  });

  mw.listen("ESC", [&](auto e) {
    mw.terminate();
    return false;
  });

  return mw.wait_for_exit();
}

/**
 * @fn run_regex
 * @brief runs the expression on the screen and places the output and status
 * on the screen.
 */
void run_regex(window_t &mw) {
  using namespace std::regex_constants;
  using namespace std;
  smatch match;

  try {
    if (!std::regex_search(mw["input_text"].data(), match,
                           std::regex(mw["regular_expression"].data()))) {
      mw[status_text] = "No matches. ";
    } else {
      mw[status_text] << match.size() << " matches.";

      mw[match_area] = "";
      for (std::size_t idx = 0; idx < match.size(); idx++) {
        std::string n = match[idx];
        mw[match_area] << n;
      }
    }

  } catch (const std::exception &e) { // caught by reference to base
    mw[status_text] << e.what();
  }
}
