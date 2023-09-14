#include <stdio.h>
#include <unistd.h>


#define UNUSED __attribute__((unused))

/**
 * @brief random delay function
 * (1) execute soem delay between printf function
 * @return int
**/
int delay(int status) {
  int x = 14;
  for (int i = 0; i < 1000000; i++) {
    x = x + 642;
  }
  printf("\nBytes last printed: %d\n", status);
  return x;
}

/**
 * @brief main function
 * (1) execute write function
 * (2) call printf function to test sys_sbrk
 * @return return function status
**/
int main(UNUSED int argc, char const *argv[]){
  int status;
  int i = 0;
  while( 1 ){
    status = write(1, "User hello world!\n", 18);
    if (status < 19) {
      delay(status);
      printf("argv[1]: %s\n", argv[1]);
    }
    i++;
  }
  return 0;
}
