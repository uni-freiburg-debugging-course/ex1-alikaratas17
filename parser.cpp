#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

enum TOKEN_TYPE
{
  SIMPLIFY_KEYWORD,
  NUMBER,
  NEGATIVE_NUMBER,
  OPEN_PARANTHESIS,
  CLOSE_PARANTHESIS,
  OPERATOR,
  UNDEFINED
};

string get_next_token(string word)
{
  return " < " + word + " > ";
}
string process_line(string line)
{
  int i = 0;
  int length = line.length();
  if (length == 0)
    return "";
  // Store tokens in a vector
  vector<string> tokens;
  while (i < length)
  {
    // Get Next Token
    string next_word = "";
    enum TOKEN_TYPE next_type;
    if (line[i] == 's')
    {
      next_type = SIMPLIFY_KEYWORD;
    }
    else if (isdigit(line[i]))
    {
      next_type = NUMBER;
    }
    else if (line[i] == '-' && i + 1 < length && isdigit(line[i]))
    {
      next_type = NEGATIVE_NUMBER;
    }
    else if (line[i] == '(')
    {
      next_type = OPEN_PARANTHESIS;
    }
    else if (line[i] == ')')
    {
      next_type = CLOSE_PARANTHESIS;
    }
    else if (line[i] == '*' || line[i] == '+' || line[i] == '/' || line[i] == '-')
    {
      next_type = OPERATOR;
    }
    else if (isspace(line[i]))
    {
      i++;
      continue;
    }
    else
    {
      next_type = UNDEFINED;
      return "Syntax Error1";
    }
    next_word.push_back(line[i]);
    i++;
    if (next_type == OPEN_PARANTHESIS || next_type == CLOSE_PARANTHESIS || next_type == OPERATOR)
    {
      while (i < length && isspace(line[i]))
        i++;
    }
    else if (next_type == SIMPLIFY_KEYWORD)
    {
      while (i < length && !isspace(line[i]))
      {
        next_word.push_back(line[i]);
        i++;
      }
      if (next_word.compare("simplify"))
      {
        cout << next_word << endl;
        return "Syntax Error";
      }
    }
    else if (next_type == NUMBER || next_type == NEGATIVE_NUMBER)
    {

      while (i < length && isdigit(line[i]))
      {
        next_word.push_back(line[i]);
        i++;
      }
    }
    tokens.push_back(get_next_token(next_word));
  }
  for (string s : tokens)
  {
    cout << s << endl;
  }
  // Process tokens recursively
  return "<Processed>";
}

void parse_input(string fname)
{
  fstream input;
  input.open(fname);
  if (!input.is_open())
  {
    cout << "Couldn't open file!" << endl;
    return;
  }

  while (input)
  {
    string line;
    getline(input, line);
    string processed_line = process_line(line);

    cout << endl;
    cout << line << endl;
    cout << processed_line << endl;
  }
  input.close();
}

int main()
{
  string filename = "small_example.smt2";
  parse_input(filename);
  return 0;
}