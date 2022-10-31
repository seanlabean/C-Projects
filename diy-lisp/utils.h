#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

long factorial(int n)  
{  
  if (n == 0)  
    return 1;  
  else  
    return(n * factorial(n-1));  
}  
