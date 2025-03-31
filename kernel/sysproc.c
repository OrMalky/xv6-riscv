#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

extern int forkn(int n, int *pids);
extern int waitall(int* n, int* statuses);
uint64
sys_exit(void)
{
  int n;
  char msg[32];
  argint(0, &n);
  argstr(1, msg, sizeof(msg));
  strncpy(myproc()->exit_msg, msg, sizeof(myproc()->exit_msg));
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  uint64 msg_addr;

  argaddr(0, &p);
  argaddr(1, &msg_addr);

  return wait(p, msg_addr);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_memsize(void)
{
  struct proc *p = myproc();
  return p->sz;
}

uint64
sys_forkn(void)
{
  int n;
  int pids[16];
  uint64 pids_addr;
  struct proc *p = myproc();
  argint(0, &n);
  argaddr(1, &pids_addr);
  int ret = forkn(n, pids);
  copyout(p->pagetable, pids_addr, (char*)pids, sizeof(int) * n);
  return ret;
}

uint64
sys_waitall(void)
{
  uint64 n;
  uint64 statuses;
  int num[1];
  int stat[NPROC];
  struct proc *p = myproc();
  argaddr(0, &n);
  argaddr(1, &statuses);
  int ret = waitall(num, stat);
  if (ret!=-1){
    copyout(p->pagetable, n, (char*)num, sizeof(int));
    copyout(p->pagetable, statuses, (char*)stat, sizeof(int)*(*num));
  }
  return ret;
}
