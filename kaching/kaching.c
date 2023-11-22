#include <stdio.h>
#include <unistd.h>

int
main()
{
	/* Init calculation floats, prompt and check user input */
	float s = 0, t = 0, c = 0;
	printf("Monthly Salary | $ ");
	if(!scanf("%f", &s)) {
		printf("Invalid format\n");
		return 0;
	}
	/* get tick (per second pay increase) */
	t = s / 20 / 8 / 60 / 60;
	putchar('\n');
	/* Print summed cash, step c by t */
	do {
		printf("\033[A$ %.2f\n",c);
		sleep(1);
		c+=t;
	}while(t >= 0);
	printf("Ended.\n");
	return 0;
}