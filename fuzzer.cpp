#include <iostream>
#include <string>
#include <fstream>
//#include <vector>
#include <random>

using namespace std;

string generate_simple_expression(){
  random_device dev;
  mt19937 rng(dev());
  uniform_int_distribution<mt19937::result_type> binary_distribution(0,1);
  int num1 = 0;
  int num2 = 0;
  for (int i = 0; i <32; i++)
        num1 += binary_distribution(rng)<<i;
  for (int i = 0; i <32; i++)
        num2 += binary_distribution(rng)<<i;
  uniform_int_distribution<mt19937::result_type> op_distribution(0,2);
  int chsn_op = op_distribution(rng);
  string op = "!";
  if (chsn_op == 0)
    op = "-";
  if (chsn_op == 1)
    op = "+";
  if (chsn_op == 2)
    op = "*";
  return "(simplify (" + op + ' ' + to_string(num1)+' '+to_string(num2)+"))";
}
int main(int argc, char *argv[])
{
  int N = 50;
  for (int i = 0; i < N; i++)
    cout<< generate_simple_expression()<< endl;
  return 0;
}