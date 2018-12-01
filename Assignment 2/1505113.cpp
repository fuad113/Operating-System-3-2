#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<queue>
#include<string>

#define chococake 1
#define vanillacake 2

using namespace std;

queue <int> cakequeue;
queue <int> chocolatequeue;
queue <int> vanillaqueue;


sem_t CakequeueFull;
sem_t CakequeueEmpty;
sem_t ChocolatequeueFull;
sem_t ChocolatequeueEmpty;
sem_t VanillaqueueFull;
sem_t VanillaqueueEmpty;

pthread_mutex_t lock;
pthread_mutex_t Console_lock;

void init_semaphore()
{
    sem_init(&CakequeueFull,0,0);
    sem_init(&CakequeueEmpty,0,5);

    sem_init(&ChocolatequeueFull,0,0);
    sem_init(&ChocolatequeueEmpty,0,5);
    
    sem_init(&VanillaqueueFull,0,0);
    sem_init(&VanillaqueueEmpty,0,5);
    
    pthread_mutex_init(&lock,0);
    pthread_mutex_init(&Console_lock,0);

}


void * CHEF_X (void * arg)
{
    while(true)
    {
     
     pthread_mutex_lock(&Console_lock);
     cout<<"CHEF X is taking a tea break\n\n";
     pthread_mutex_unlock(&Console_lock);

     //wait for the queue to be non-empty
     sem_wait(&CakequeueEmpty);
     pthread_mutex_lock(&lock);
     
     cakequeue.push(chococake);

     pthread_mutex_lock(&Console_lock);
     cout<< "Cocolate Cake Produced\n";
     cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n\n";
     pthread_mutex_unlock(&Console_lock);

     pthread_mutex_unlock(&lock);
     sem_post(&CakequeueFull);

     sleep(5);      


    }

}


void * CHEF_Y(void * arg)
{
    while(true)
    {
     pthread_mutex_lock(&Console_lock);
     cout<<"CHEF Y is taking a tea break\n\n";
     pthread_mutex_unlock(&Console_lock);

     //wait for the queue to be non-empty
     sem_wait(&CakequeueEmpty);
     pthread_mutex_lock(&lock);
     
     cakequeue.push(vanillacake);

     pthread_mutex_lock(&Console_lock);
     cout<< "Vanilla Cake Produced\n";
     cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n\n";
     pthread_mutex_unlock(&Console_lock);

     pthread_mutex_unlock(&lock);
     sem_post(&CakequeueFull);

     sleep(5);      


    }
}

void * CHEF_Z (void * arg)
{

    while(true)
     {
      pthread_mutex_lock(&Console_lock);
      cout<<"CHEF Z is taking a coffee break\n\n";
      pthread_mutex_unlock(&Console_lock);   
    
      //waiting for the queue 1 to be filled
      sem_wait(&CakequeueFull);

      int frontcake=cakequeue.front();

      if(frontcake==chococake)
      {
        
        //now wait for the chocolate cake queue (Queue 3) to be empty
        pthread_mutex_lock(&Console_lock);
        cout<<"CHEF Z is taking a coffee break\n\n";
        pthread_mutex_unlock(&Console_lock); 

        sem_wait(&ChocolatequeueEmpty);

        chocolatequeue.push(chococake);
        cakequeue.pop();

        pthread_mutex_lock(&Console_lock);
        cout<< "Chef Z has decorated a Chocolate Cake\n";
        cout<< "Chocolate Cake is added to the Queue 3(Chocolate Cakes Queue)\n";
        cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n";
        cout<< "Number of cakes in the Queue 2(Vanilla Cakes Queue) is : " << vanillaqueue.size()<< "\n";
        cout<< "Number of cakes in the Queue 3(Chocolate Cakes Queue) is : " << chocolatequeue.size()<< "\n\n";
        pthread_mutex_unlock(&Console_lock);        
 
        sem_post(&ChocolatequeueFull);

      }

      else
      {
        //now wait for the vanilla cake queue (Queue 2) to be empty
        pthread_mutex_lock(&Console_lock);
        cout<<"CHEF Z is taking a coffee break\n\n";
        pthread_mutex_unlock(&Console_lock);
         
        sem_wait(&VanillaqueueEmpty);

        vanillaqueue.push(vanillacake);
        cakequeue.pop();

        pthread_mutex_lock(&Console_lock);
        cout<< "Chef Z has decorated a vanilla Cake\n";
        cout<< "Vanilla Cake is added to the Queue 2(Vanilla Cakes Queue)\n";
        cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n";
        cout<< "Number of cakes in the Queue 2(Vanilla Cakes Queue) is : " << vanillaqueue.size()<< "\n";
        cout<< "Number of cakes in the Queue 3(Chocolate Cakes Queue) is : " << chocolatequeue.size()<< "\n\n";
        pthread_mutex_unlock(&Console_lock);        
 
        sem_post(&VanillaqueueFull);  

      }

      
      sem_post(&CakequeueEmpty);
      sleep(5);


     }

}


void * WAITER_1(void * arg)
{
  while(true)
  {
      pthread_mutex_lock(&Console_lock);
      cout<<"Waiter 1 is taking a coffee break\n\n";
      pthread_mutex_unlock(&Console_lock);

      sem_wait(&ChocolatequeueFull);
      chocolatequeue.pop();

      pthread_mutex_lock(&Console_lock);
      cout<< "Waiter 1 delivers a chocolate cake to the customer\n";
      cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n";
      cout<< "Number of cakes in the Queue 2(Vanilla Cakes Queue) is : " << vanillaqueue.size()<< "\n";
      cout<< "Number of cakes in the Queue 3(Chocolate Cakes Queue) is : " << chocolatequeue.size()<< "\n\n";
      pthread_mutex_unlock(&Console_lock);

      sem_post(&ChocolatequeueEmpty);

      sleep(5);

  }

}


void * WAITER_2(void * arg)
{
  while(true)
  {
      pthread_mutex_lock(&Console_lock);
      cout<<"Waiter 2 is taking a coffee break\n\n";
      pthread_mutex_unlock(&Console_lock);

      sem_wait(&VanillaqueueFull);
      vanillaqueue.pop();

      pthread_mutex_lock(&Console_lock);
      cout<< "Waiter 2 delivers a vanilla cake to the customer\n";
      cout<< "Number of cakes in the Queue 1(Cakes Queue) is : " << cakequeue.size()<< "\n";
      cout<< "Number of cakes in the Queue 2(Vanilla Cakes Queue) is : " << vanillaqueue.size()<< "\n";
      cout<< "Number of cakes in the Queue 3(Chocolate Cakes Queue) is : " << chocolatequeue.size()<< "\n\n";
      pthread_mutex_unlock(&Console_lock);

      sem_post(&VanillaqueueEmpty);
      sleep(5);

  }

}


int main()
{

    init_semaphore();

    pthread_t chef_x;
    pthread_t chef_y;
    pthread_t chef_z;
    pthread_t waiter_1;
    pthread_t waiter_2;

    pthread_create(&chef_x,NULL,CHEF_X,(void *)0);
    pthread_create(&chef_y,NULL,CHEF_Y,(void *)0);
    pthread_create(&chef_z,NULL,CHEF_Z,(void *)0);
    pthread_create(&waiter_1,NULL,WAITER_1,(void *)0);
    pthread_create(&waiter_2,NULL,WAITER_2,(void *)0);

    while(true);
    return 0;
}