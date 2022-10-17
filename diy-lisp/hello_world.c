#include <stdio.h>

// The basics of C. Printing "Hello, World!" using
// for and while loops and function calls.

/*
[[ Other fundamentals ]]

 - do loops (a.k.a. do-while loops):
 Executes statements then checks conditional at end of loop.
 Similar to while loop but is guarenteed to execute at least once.
 https://www.tutorialspoint.com/cprogramming/c_do_while_loop.htm

 - switch statement:
 Allows for a variable to be tested agains a list of values.
 Each value is a "case" which can be accompanied by statements to
 be executed. Optionally, a "break" statement is supplied at the end
 of each case. If not, the other statements will be executed as the
 program falls through until a break statement is reached.
 https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm

 - continue statement:
 Forces the start of the next loop iteration. In a way, the counterpart
 to the break statement.
 https://www.tutorialspoint.com/cprogramming/c_continue_statement.htm

 - typedef keyword:
 Can be used to give a type a new name (like a shorthand) typically
 using all uppercase for the new name to denote the pseudonym. Can also
 use typedef to name user-defined data types as well. Such user type
 (or "struct") function similar to classes in Python.
 https://www.tutorialspoint.com/cprogramming/c_typedef.htm
*/

int print_hello_world(int n) {
  // Testing function calls.
  // Must be defined before use in
  // main.
  for (int i = 0; i < n; i++) {
    puts("Hello, World!");
  }
  return 0;
}
  
int main(int argc, char** argv) {
  // For loop: init, condition, increment.
  puts("For Loop...");
  for (int i = 0; i < 3; i++) {
    puts("Hello, world!");
  }

  // While loop: condition
  puts("While Loop...");
  int i = 3;
  while (i > 0) {
    puts("Hello, World");
    i -= 1;
  }
  
  puts("Calling function...");
  print_hello_world(5);

  puts("Switch example...");
  // switch statements and break usage.
  char grade = 'B';

  switch(grade) {
     case 'A' :
        printf("Excellent!\n" );
        break;
     case 'B' :
     case 'C' :
        printf("Well done\n" );
        //break;
     case 'D' :
        printf("You passed\n" );
        break;
     case 'F' :
        printf("Better try again\n" );
        break;
     default :
        printf("Invalid grade\n" );
  }
   
  printf("Your grade is  %c\n", grade );
   
  return 0;
}


