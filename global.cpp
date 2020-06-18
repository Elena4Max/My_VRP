#include "global.h"

std::vector< std::vector<char> > _bonus_map;

void print_bonus_map(int N, int M)
{
	system("clear");
	#ifdef _BONUS2
		printf("Iteration # %llu \n", k);
	#endif
	for(long long i = 0; i < M; i++) {
		for(long long j = 0; j < N; j++) {
			if(_bonus_map[i][j] == 'V') {
				printf("\033[31m%c%s", 'V', "\033[0m");
			}	
			else {
				if( _bonus_map[i][j] == 'S' ) {
					printf("\033[32m%c%s", 'S', "\033[0m");
				}
				else {
					if( _bonus_map[i][j] == 'D' ) {
						printf("\033[32m%c%s", 'D', "\033[0m");
					}
					else {
						printf("%s", " ");
					}
				}
			}
		}
		printf("\n");	
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
