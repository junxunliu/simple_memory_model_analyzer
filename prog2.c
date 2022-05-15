void f1(int *i)
{
 int *j = malloc(sizeof(int) * 12);
 i = malloc(sizeof(int));
}


int f2()
{
  float x[5];
}

int main(int argc, char **argv)
{
  int i;
  int j;

  i = f2();
  f1(j);
}