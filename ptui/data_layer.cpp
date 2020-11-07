#include "../ptui/std_base.h"
#include "../ptui/data_layer.h"

/**
 * @fn read_file
 * @brief reads a file, text , as a stringstream. Other formats can be added
 * here such as treeviews easily with text. XML, json object displayer. format
 * parsers. Syntax highlighting. But the code only reads a text file.
 *
 */
std::stringstream
ptui::data_layer_t::read_file(const std::filesystem::path &filename) {
  std::stringstream data = {};
  std::ifstream finput(filename);
  data << finput.rdbuf();
  finput.close();
  return data;
}
