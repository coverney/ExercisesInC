/*
  1) After compiling the code and looking at the assembly language, I
  saw the following code corresponding to the integer 5:

  movl	$5, -4(%rbp)

  2) After turning on optimization using the flag -O2, the main section
  has fewer lines. Also, this section disappears:

  movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$5, -4(%rbp)

  which includes the movl part with 5. Instead this is added:

  addq	$8, %rsp

  3) When printing x,
  - both: has a .string	"%i\n" in the LC0 section
  - without optimization: there is call	printf instead of call puts
  - with optimization: there is call	__printf_chk instead of call puts.
  The movl	$0, %eax also disappears (potentially connected to return 0)

  4) What happens to x and y,
  - without optimization: there is a addl	$1, %eax to represent y = x+1
  - with optimization: there is no addl but instead movl	$1, %edi
  movl	$5, -8(%rbp) is replaced by movl	$6, %edx

  From this example, I can conclude that optimization condenses code by
  starting to combine simple operations (i.e addition). Instead of dealing with
  x and y separately, the compiler computes 5+1 and directly remembers 6.

*/
#include <stdio.h>
/*#include <stdlib.h>*/

/*Function added for class exercises*/
/*int add(int x, int y){
  int sum = x + y;
  return sum;
}*/

/*
  Creates x and y variable and prints y
*/
int main() {
    int x = 5;
    //int y = x + 1;
    printf("Hello, World!\n");
    //printf("%i\n", y);
    /* Added for class exercises
    int a = 3;
    int b = 4;
    int c = a + b;
    printf("c is %d\n", c);
    switch (c%3) {
      case 0:
        printf("c is divisible by 3\n");
      default:
        printf("c is not divisible by 3\n");
    }
    int sum = add(x, 3);
    printf("%i\n", sum);*/
    return 0;
}
