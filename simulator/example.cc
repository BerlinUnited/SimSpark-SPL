#include "simulator.h"

#include <console.h>
#include <forwarder.h>

#include <pthread.h>

pthread_mutex_t mutex;

void*
simulator_thread(void *)
{
    bool loop = true;
    rcss::Simulator sim;

    while (loop)
    {
        pthread_mutex_lock(&mutex);
        // loop =
        sim.execute();
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void*
console_thread(void *)
{
    flush(*Console::instance().getStream());
    Utility::smux.addStream(&cout);
    cin.tie(&Utility::smux);
    
    string s;
    long int i = 0;
    
    Console::instance().execute("help");
    while (true) 
    {    
        Utility::smux.normal() << "console:/ " << i << ") " << flush;
        ++i;
        getline(std::cin,s);
        pthread_mutex_lock(&mutex);
        Console::instance().execute(s);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

int
main(int argc, char **argv)
{
    pthread_t p1, p2;

    pthread_mutex_init (&mutex, 0);

    pthread_create(&p1, 0, simulator_thread, 0);
    pthread_create(&p2, 0, console_thread, 0);

    pthread_join(p1, 0);
    pthread_join(p2, 0);

    return 0;
}
