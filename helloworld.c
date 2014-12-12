#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>          //task_struct
#include <linux/pid.h>            //pid
#include <linux/mm_types.h>       //mm_struct,vm_area_struct
#include <linux/fs.h>		  //struct filei
#include <linux/path.h>
#include <linux/dcache.h>         //d_path
#include <linux/slab.h>		  //kmalloc
#include <linux/string.h>         //memset
#include <asm/pgtable.h>   	  // offset function
#include <asm/pgtable_types.h>     //pgd,pud,pmd,pte
#include <asm/page.h>

struct pid *pid_struct;
struct task_struct *task;
struct mm_struct *mm_s;
struct vm_area_struct *vm_a_s,*vm_a_s2;
struct path my_path;
struct page *page=NULL,*page2=NULL;
asmlinkage int sys_project(long pid)
{
	char *ret_ptr=NULL;
	char *tpath=NULL;
	
	tpath=(char*)kmalloc(512,0);
	memset(tpath,'\0',512);
	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct,PIDTYPE_PID);
 	printk("process id=%ld  name :%s\n",pid,task->comm);

	mm_s=task->mm;
	vm_a_s=mm_s->mmap;
	for(vm_a_s2=vm_a_s;vm_a_s2!=NULL;vm_a_s2=vm_a_s2->vm_next)
	{
		unsigned long int vm_start,vm_end;
		vm_start=vm_a_s2->vm_start;
		vm_end=vm_a_s2->vm_end;

		printk("vm:%lx-%lx ",vm_start,vm_end);
		pgd_t *pgd;
		pud_t *pud;
		pmd_t *pmd;
		pte_t *ptep,pte;

		pgd= pgd_offset(mm_s,vm_start);
		if(pgd_none(*pgd)||pgd_bad(*pgd)){break;}
//		printk("pgd= %lx,",pgd);
		pud= pud_offset(pgd,vm_start);
		if(pud_none(*pud)||pud_bad(*pud)){break;}
//		printk("pud= %lx,",pud);
		pmd= pmd_offset(pud,vm_start);
		if(pmd_none(*pmd)||pmd_bad(*pmd)){break;}
//		printk("pmd= %lx,",pmd);
		ptep=pte_offset_kernel(pmd,vm_start);
		if(pte_none(*ptep)){break;}
		printk("pte= %lx ",pte_val(pte)>>12);
		
		//pte=*ptep;
		//page=pte_page(pte);			
		//if(page){printk("page start:%p ",page);}

		if(vm_a_s2->vm_file!=NULL)
		{	
			my_path=vm_a_s2->vm_file->f_path;
			ret_ptr=d_path(&my_path,tpath,512);	
			printk(" %s \n",ret_ptr);
		}
		else
		{
			printk("\n");
		}
	}

	return 1;
}
