/**
 * @Author: B159973
 * @Date:	10/4/2019
 * @Course: Parallel Design Patterns - 2020
 * @University of Edinburgh
*/
/*************************LIBRARIES**********************************/
/********************************************************************/
#include "mpi.h"
#include "omp.h"
#include <math.h>
#include "stdio.h"
#include <time.h>
/********************************************************************/
/***************************ACTORS***********************************/
/********************************************************************/
#include "actor.h"
#include "registry.h"
#include "cell.h"
#include "clock.h"
#include "squirrel.h"
/*************************PROCESS************************************/
/********************************************************************/
#include "process_pool.h"
#include "master.h"
#include "worker.h"

#include "main.h"
#include "ran2.h"
/********************************************************************/

int main(int argc, char *argv[])
{

  /*Create a registry*/
  Registry_cell *registry = NULL;

  /****Initialize MPI****/
  int rank;
  int size;
  time_t t;

   /* Intializes random number generator */
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  srand((unsigned) time(&t));
  int *workers = (int *)malloc(size * sizeof(int));
  int statusCode = create_pool();

  /*Give work to a worker*/
  if (statusCode == 1)
  {
    worker(rank, size - 1);
  }
  else if (statusCode == 2)
  {
    /*Start the workers*/
    startworkers(size - 1, workers);
    /*Send the workload to the workers*/
    master_send_instructions(size - 1, &registry, workers);
    /*While the master lives*/
    masterlives(registry, size - 1);
  }
  terminate_pool();
  MPI_Finalize();

  return 0;
}

static void initialiseRNG(long *seed)
{
  ran2(seed);
}

