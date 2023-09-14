/** @file   default/main.c
 *
 *  @brief  user-space project "default", demonstrates system call use
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
 *
 *  @output repeatedly prints provided argument to console
**/

#include <stdio.h>

#define UNUSED __attribute__((unused))

int main(UNUSED int argc, char* const argv[]) {
	while(1){
		printf("Hello World from User : %s\n", argv[1]);
	}
	return 0;
}
