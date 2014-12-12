#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>          //task_struct
#include <linux/mm_types.h>       //mm_struct,vm_area_struct
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/pgtable_types.h>

struct mm_struct *mm;
struct vm_area_struct *vma;
struct vm_fault *vmf;
asmlinkage int nonwritable(unsigned long begin,unsigned long end)
{

	mm=current->mm;
	vma=find_vma(mm,begin);
//	vma->vm_flags=vma->vm_flags&VM_READ;

	pgd_t *pg;
        pud_t *pu;
        pmd_t *pm;
        pte_t *ptep2,*pte2,temp_pte;

        pg= pgd_offset(mm,begin);
        if(pgd_none(*pg)||pgd_bad(*pg)){return 0;}
        pu= pud_offset(pg,begin);
        if(pud_none(*pu)||pud_bad(*pu)){return 0;}
        pm= pmd_offset(pu,begin);
        if(pmd_none(*pm)||pmd_bad(*pm)){return 0;}
        ptep2=pte_offset_kernel(pm,begin);
        if(pte_none(*ptep2)){return 0;}
	printk("before %lx",pte_val(*ptep2));
	//temp_pte=*ptep2;
	pte_wrprotect(*ptep2);
	printk("after %lx",pte_val(*ptep2));
	return 1;
}
