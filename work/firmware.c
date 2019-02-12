
int toto;
int tata;
int titi;

int fibo(int n)
{
  if (n < 2)
    return n;
  else
    return fibo(n-1) + fibo(n-2);
}

int main(int ac, char **av)
{
  return fibo(8);
}
