int evil_glob_var_1;
float evil_glob_var_2;

void fun1(int x)
{
  int y;
  int z;
  char i[10] = "Hello";
  char *string_literal = "String Literal";
  char *string_literal2;
  string_literal2 = "String Literal2";
  printf("%d \n", x+y+z); 
}

int fun2(float z)
{
  float x;
  char *a = malloc(sizeof(char));
  char **b = malloc(sizeof(char*));
  char *c = malloc(4*sizeof(char));
  return (int)(z+x);
}

int main(int argc, char **argv)
{
  int w, j, b;
  int i[N] = {10, 9, 8, 7, 6};

  fun1(w);
  fun2();

  return 0;
}