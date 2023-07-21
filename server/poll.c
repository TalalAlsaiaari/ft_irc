#include <stdio.h>
#include <poll.h>

int main()
{
	struct pollfd	pfds[1];

	pfds[0].fd = 0;
	pfds[0].events = POLLIN;

	printf("You have 2.5 seconds to hit reutrn\n");

	int	num_events = poll(pfds, 1, 2500);

	if (!num_events)
		printf("Time out\n");
	else
	{
		int	pollin_happened = pfds[0].revents & POLLIN;

		if (pollin_happened)
			printf("Fd %d is ready to read\nnum_events: %d\n", pfds[0].fd, num_events);
		else
			printf("Error: %d\n", pfds[0].revents);
	}
	return 0;
}