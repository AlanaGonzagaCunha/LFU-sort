#include "Genops.h"

int toInt(char *string){
  int res;
  int p;
  res = 0;
  p = 1;
  while (('-' == (*string)) || ((*string) == '+'))
    {
      if (*string == '-')
        p = p * -1;
      string++;
    }
  while ((*string >= '0') && (*string <= '9'))
    {
      res = (res * 10) + ((*string) - '0');
      string++;
    }
  return (res * p);
}

int main(int argc, char *argv[]) {

	printf("\n< %s >",argv[1]);
	printf("\n< %s \n>",argv[2]);
	int capacity = toInt(argv[2]);

	execute(argv[1], capacity);
	return 0;
}



