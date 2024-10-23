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

enum EXP_TYPES
{
  SUBTRACTION_EXP,
  ADDITION_EXP,
  MULTIPLICATION_EXP,
  /* DIVISION NOT REQUIRED YET
  DIVISION_EXP,
  */
  SIMPLIFICATION_EXP,
  NUMBER_EXP,
};

struct ast_tree_node
{
  enum EXP_TYPES exp_type;
  int leaf_num;
  vector<ast_tree_node> args;
};

string ast_to_str(ast_tree_node ast)
{
  int leaf_num = ast.leaf_num;
  if (leaf_num < 0)
    return "(- " + to_string(leaf_num * -1) + ")";
  return to_string(leaf_num);
}
ast_tree_node evaluate_ast(ast_tree_node ast)
{
  if (ast.exp_type == SIMPLIFICATION_EXP)
    return evaluate_ast(ast.args[0]);
  ast_tree_node tmp;
  if (ast.exp_type == ADDITION_EXP)
  {
    tmp.exp_type = NUMBER_EXP;
    tmp.leaf_num = ast.args[0].leaf_num + ast.args[1].leaf_num;
    return tmp;
  }
  /* DIVISION NOT REQUIRED YET, ALSO NEED TO CHANGE INT
  if (ast.exp_type == DIVISION_EXP)
  {
    tmp.exp_type = NUMBER_EXP;
    tmp.leaf_num = ast.args[0].leaf_num / ast.args[1].leaf_num;
    return tmp;
  }*/
  if (ast.exp_type == SUBTRACTION_EXP)
  {
    tmp.exp_type = NUMBER_EXP;
    tmp.leaf_num = ast.args[0].leaf_num - ast.args[1].leaf_num;
    return tmp;
  }
  if (ast.exp_type == MULTIPLICATION_EXP)
  {
    tmp.exp_type = NUMBER_EXP;
    tmp.leaf_num = ast.args[0].leaf_num * ast.args[1].leaf_num;
    return tmp;
  }
  return tmp;
}
ast_tree_node tokens_to_ast(vector<string> tokens)
{
  int size = tokens.size();
  ast_tree_node node;
  if (size >= 2 && !tokens[0].compare("<START_EXP>") && !tokens[1].compare("<SIMPLIFY>") && !tokens[size - 1].compare("<END_EXP>"))
  {
    node.exp_type = SIMPLIFICATION_EXP;
    vector<string> rest_of_tokens;
    for (int i = 2; i < size - 1; i++)
      rest_of_tokens.push_back(tokens[i]);
    node.args.push_back(tokens_to_ast(rest_of_tokens));
    return node;
  }
  if (size == 1)
  {
    node.exp_type = NUMBER_EXP;
    node.leaf_num = atoi(tokens[0].c_str());
    return node;
  }
  if (size >= 2 && !tokens[0].compare("<START_EXP>") && !tokens[1].compare("<ADDITION>") && !tokens[size - 1].compare("<END_EXP>"))
  {
    node.exp_type = ADDITION_EXP;
    vector<string> left_tokens;
    left_tokens.push_back(tokens[2]);
    node.args.push_back(tokens_to_ast(left_tokens));
    vector<string> right_tokens;
    right_tokens.push_back(tokens[3]);
    node.args.push_back(tokens_to_ast(right_tokens));
    return node;
  }
  /* DIVISION NOT REQUIRED YET
  if (size >= 2 && !tokens[0].compare("<START_EXP>") && !tokens[1].compare("<DIVISION>") && !tokens[size - 1].compare("<END_EXP>"))
  {
    node.exp_type = DIVISION_EXP;
    vector<string> left_tokens;
    left_tokens.push_back(tokens[2]);
    node.args.push_back(tokens_to_ast(left_tokens));
    vector<string> right_tokens;
    right_tokens.push_back(tokens[3]);
    node.args.push_back(tokens_to_ast(right_tokens));
    return node;
  }
  */
  if (size >= 2 && !tokens[0].compare("<START_EXP>") && !tokens[1].compare("<MULTIPLICATION>") && !tokens[size - 1].compare("<END_EXP>"))
  {
    node.exp_type = MULTIPLICATION_EXP;
    vector<string> left_tokens;
    left_tokens.push_back(tokens[2]);
    node.args.push_back(tokens_to_ast(left_tokens));
    vector<string> right_tokens;
    right_tokens.push_back(tokens[3]);
    node.args.push_back(tokens_to_ast(right_tokens));
    return node;
  }
  if (size >= 2 && !tokens[0].compare("<START_EXP>") && !tokens[1].compare("<SUBTRACTION>") && !tokens[size - 1].compare("<END_EXP>"))
  {
    node.exp_type = SUBTRACTION_EXP;
    vector<string> left_tokens;
    left_tokens.push_back(tokens[2]);
    node.args.push_back(tokens_to_ast(left_tokens));
    vector<string> right_tokens;
    right_tokens.push_back(tokens[3]);
    node.args.push_back(tokens_to_ast(right_tokens));
    return node;
  }
  return node;
}

string get_next_token(enum TOKEN_TYPE type, string word)
{
  if (type == OPERATOR)
  {
    if (!word.compare("-"))
      return "<SUBTRACTION>";
    if (!word.compare("*"))
      return "<MULTIPLICATION>";
    /* DIVISION NOT REQUIRED YET
    if (!word.compare("/"))
      return "<DIVISION>";
      */
    if (!word.compare("+"))
      return "<ADDITION>";
    return "<UNDEFINED>";
  }
  if (type == OPEN_PARANTHESIS)
    return "<START_EXP>";
  if (type == CLOSE_PARANTHESIS)
    return "<END_EXP>";
  if (type == NUMBER || type == NEGATIVE_NUMBER)
    return word;
  if (type == SIMPLIFY_KEYWORD)
    return "<SIMPLIFY>";

  return "<UNDEFINED>";
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
      return "Syntax Error";
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
    tokens.push_back(get_next_token(next_type, next_word));
  }
  // Process tokens recursively using ast
  ast_tree_node ast = tokens_to_ast(tokens);
  ast_tree_node result_ast = evaluate_ast(ast);
  return ast_to_str(result_ast);
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
    if (line.size() == 0)
      continue;
    string processed_line = process_line(line);

    cout << processed_line << endl;
  }
  input.close();
}

int main(int argc, char *argv[])
{
  string filename = "small_example.smt2";
  if (argc == 2)
    filename = argv[1];
  parse_input(filename);
  return 0;
}