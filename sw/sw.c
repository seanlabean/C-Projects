#include <stdio.h>
#include <unistd.h>

int main()
{
	/* Init timing ints, prompt and check user input */
	int h = -1, m = -1, s = -1, t = 0;
	printf("Timeout(HH:MM:SS) | ");
	if(!scanf("%d:%d:%d", &h, &m, &s)) {
		printf("Invalid timestamp\n");
		return 0;
	}
	/* More natural interpretation of time inputs */
	if(m == -1 && s == -1){
		s = h;
		m = 0;
		h = 0;
	} else if(s == -1){
		s = m;
		m = h;
		h = 0;
	} 
	t = h * 3600 + m * 60 + s;
	putchar('\n');
	/* Print remaining time, sleep 1s, step t.
	 Display as HH:MM:SS until t < 0 */
	do {
		if(t > 3600){
			printf("\033[A%02d:%02d:%02d\n",
					t / 3600,
					(t / 60) % 60,
					t % 60);
		} else if(t > 60){
			printf("\033[A00:%02d:%02d\n",
					(t / 60) % 60,
					t % 60);
		} else{
			printf("\033[A00:00:%02d\n",
					t);
		}
		sleep(1);
		t--;
	}while(t >= 0);
	printf("Ended.\n");
	return 0;
}
