#include "document_management.h"
int document_management_t::optimize_expression() {

  s = "(:) *([_.a-zA-Z0-9]+) *(:)|([a-z]+) +([0-9]+) *(;)|([a-z]+) "
      "+([.a-z]+)*(;)|([a-z]+) +([0-9]+) +([0-9]+)(;)|([a-z]+) "
      "+\"([\\S\\s]+)\";";

  std::string re_object_id = "(:) *([_.a-zA-Z0-9]+) *(:)";
  std::string re_attribute_name = "([_.a-z]+) +";
  std::string re_value_string = "([.a-zA-Z]+) +";
  std::string re_value_string_literal = "\"([\\S\\s]*)\"";
  std::string re_value_numeric = "(-?[0-9]+.?[0-9]*) +";
  std::string re_attribute_end = "(;)";
  std::stringstream ss;

  ss << re_object_id;
  ss << "|";

  ss << re_attribute_name << re_value_string << re_attribute_end;
  ss << "|";

  ss << re_attribute_name << re_value_string_literal << re_attribute_end;
  re << "|";

  ss << re_attribute_name << re_value_numeric << re_attribute_end;
  ss << "|";

  ss << re_attribute_name << re_value_numeric << re_value_numeric
     << re_attribute_end;

  object_decl_re = std::regex(ss.str())
}

int document_management_t::add(const std::string_view sv) {
  smatch match;

  if (!std::regex_search(s match, object_decl_re)) {
    throw std::runtime_error(
        "Syntax error on add(). No matching recognition found.");

    auto obj = element_object_t{};
    bool bdone = false;
    bool bcapture_attribute = false;
    std::string token = {};
    std::size_t idx{};
    std::vector<std::string> param_stack = {};

    while (!bdone) {
      // clear
      obj = {};

      // start of declaration
      obj.id = match[idx];
      idx++;

      // the very next token must be a ':' which is within the capture of the
      // "re".
      token = match[idx];
      if (token != ":") {
        std::stringstream ss;
        ss << "Syntax error on add(), missing ':object_id:' in definition. "
           << token;
        throw std::runtime_error(ss.str());
      }
      idx++;

      bcapture_attribute = true;

      while (bcapture_attribute) {
        std::string attribute = match[idx];

        idx++;

        while (idx < match.size()) {
          std::string param = {};
          param = match[idx];
          // terminating condition, next object id found. the idx is reversed
          // to this name position for the logic to continue processing
          // element object definitions.
          if (param == ":") {
            idx--;
            bcapture_attribute = false;
            break;
          }

          if (param == ";")
            break;

          param_stack.push_back(param);
          idx++;
        }

        /** @brief  if the name exists, while it is immediately followed by a
         * ';' it is interpreted as a boolean. Otherwise leave the parsing up
         * to the recipient. The item will be replaced within the map as the
         * correct optimized method of manipulation and also provide
         * instantiation of the parameters effects through object member
         * functions. */
        if (param_stack.empty())
          obj[token] = true;
        else
          obj[token] = param_stack;

        idx++;
        idx >= match.size();
      }

      elements[obj.id] = obj;
    }
  }
  return EXIT_SUCCESS;
}
