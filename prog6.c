
int sum(int *a, int size) 
{
   int i, s = 0;

   for(i = 0; i < size; i++) 
       s += a[i];

   return s;
}


int main()
{
  int N = 5;
  int i[N] = {10, 9, 8, 7, 6};

  printf("sum is %d\n", sum(i,N));

  return 0;
}