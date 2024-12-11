#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"


// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = myproc();
 
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_memsize(void);
extern int sys_trace(void);

static int (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
[SYS_getpid]  sys_getpid,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
[SYS_memsize]	sys_memsize,
[SYS_trace]   sys_trace,
};

void
syscall(void)
{
  int num;
  struct proc *curproc = myproc();

  num = curproc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    curproc->tf->eax = syscalls[num]();
	
	//trace() call, 비트연산자 & 로 mask값에 맞는 함수들만 trace()호출
	int mask_check = (curproc->mask & (1<<num));
	if(mask_check == (1<<1))
		cprintf("syscall traced: pid = %d, syscall = fork, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<2))
		cprintf("syscall traced: pid = %d, syscall = exit, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<3))
		cprintf("syscall traced: pid = %d, syscall = wait, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<4))
		cprintf("syscall traced: pid = %d, syscall = pipe, %d returned\n", curproc->pid, curproc->tf->eax);
	
	else if(mask_check == (1<<5))
		cprintf("syscall traced: pid = %d, syscall = read, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<6))
		cprintf("syscall traced: pid = %d, syscall = kill, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<7))
		cprintf("syscall traced: pid = %d, syscall = exec, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<8))
		cprintf("syscall traced: pid = %d, syscall = fstat, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<9))
		cprintf("syscall traced: pid = %d, syscall = chdir, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<10))
		cprintf("syscall traced: pid = %d, syscall = dup, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<11))
		cprintf("syscall traced: pid = %d, syscall = getpid, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<12))
		cprintf("syscall traced: pid = %d, syscall = sbrk, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<13))
		cprintf("syscall traced: pid = %d, syscall = sleep, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<14))
		cprintf("syscall traced: pid = %d, syscall = uptime, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<15))
		cprintf("syscall traced: pid = %d, syscall = open, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<16))
		cprintf("syscall traced: pid = %d, syscall = write, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<17))
		cprintf("syscall traced: pid = %d, syscall = mknod, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<18))
		cprintf("syscall traced: pid = %d, syscall = unlink, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<19))
		cprintf("syscall traced: pid = %d, syscall = link, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<20))
		cprintf("syscall traced: pid = %d, syscall = mkdir, %d returned\n", curproc->pid, curproc->tf->eax);

	else if(mask_check == (1<<21))
		cprintf("syscall traced: pid = %d, syscall = close, %d returned\n", curproc->pid, curproc->tf->eax);
	
	else if(mask_check == (1<<22))
		cprintf("syscall traced: pid = %d, syscall = memsize, %d returned\n", curproc->pid, curproc->tf->eax);
	
	else if(mask_check == (1<<23))
		cprintf("syscall traced: pid = %d, syscall = trace, %d returned\n", curproc->pid, curproc->tf->eax);

  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}
