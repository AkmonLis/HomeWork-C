#include <stdio.h>
#include <stdint.h>



int main()
{
	uint32_t number;
	scanf("%u", &number);
	uint32_t mask = ((~number)>>24)<<24;
	number = ((number<<8)>>8)| mask;
	printf("%u", number);
	return 0;
}
