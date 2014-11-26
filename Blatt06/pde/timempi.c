/*
echo $(hostname): $(($(date +%s%N)/1000000))

exit
*/

/*
	srun -N [Anzahl der Nodes] mpiexec \-n [Anzahl der Prozesse] ./[Programm]
*/

#include <stdio.h>  //printf
#include <string.h> //strcat, snprintf
#include <time.h>   //time, localtime, strftime

#include <unistd.h> //gethostname
#include <sys/time.h> //gettimeofday

#include <mpi.h>    //MPI_*


#define BUF_SIZE 255

int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	char recieve[BUF_SIZE];
	
	if(MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
		printf("Error in MPI_Barrier\n");
	}
	
	if(rank == 0) {
		int i;
		for(i=1; i<size; ++i) {
			MPI_Recv(recieve, BUF_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, NULL);
			printf("%s\n", recieve);
		}
	} else {
		char message[BUF_SIZE];
	
		if(gethostname(message, BUF_SIZE) != 0) {
			printf("Error in gethostname\n");
		}
		strcat(message, ": ");
		
		time_t date = time(NULL);
		struct tm *tmptr = localtime(&date);
		if(tmptr == NULL) {
			printf("Error in localtime\n");
		}
		struct timeval time;
		if(gettimeofday(&time, NULL) != 0) {
			printf("Error in gettimeofday\n");
		}
		strftime(message+strlen(message), BUF_SIZE-strlen(message), "%Y-%m-%d %H:%M:%S.", tmptr);
		snprintf(message+strlen(message), BUF_SIZE-strlen(message), "%6ld", time.tv_usec);
		
			MPI_Send(message, BUF_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	
	if(MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
		printf("Error in MPI_Barrier\n");
	}
	printf("Rank %i beendet jetzt!\n", rank);
	
	
	MPI_Finalize();
	
	return 0;
}