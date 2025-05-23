#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/file.h"

#define STDOUT_FILENO 1
#define MAX_FD 128

static struct file* fd_table[MAX_FD];

static int sys_open(const char *filename) {
  struct file *f = filesys_open(filename);
  if (f == NULL) return -1;

  for (int fd = 2; fd < MAX_FD; fd++) {
    if (fd_table[fd] == NULL) {
      fd_table[fd] = f;
      return fd;
    }
  }
  file_close(f);
  return -1;
}
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


int sys_write (int fd, const void *buffer, unsigned size) {
  if (fd == STDOUT_FILENO) {
    putbuf (buffer, size);
    return size;
  }
  // Chưa hỗ trợ các fd khác
  return -1;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int syscall_num = *(int *) f->esp; // Lấy số syscall từ user stack
  printf("[DEBUG] syscall number: %d\n", syscall_num);
  switch (syscall_num) {
    case SYS_EXIT:
      {
        int status = *((int *)f->esp + 1);
        printf("%s: exit(%d)\n", thread_name(), status);
        thread_exit();
      }
      break;

    case SYS_WRITE:
      {


        int fd = *((int *)f->esp + 1);
        void *buf = *((void **)f->esp + 2);
        unsigned size = *((unsigned *)f->esp + 3);

        printf("[DEBUG] SYS_WRITE: fd=%d, size=%u, buffer=%p\n", fd, size, buf);
        hex_dump(0, buf, size, true);
        if (fd == 1) {
          putbuf(buf, size);
          f->eax = size;
        } else {
          f->eax = -1;
        }
      }
      break;

    case SYS_OPEN:
      {
        const char *filename = *((const char **)f->esp + 1);
        f->eax = sys_open(filename);
      }
      break;

    default:
      printf("Unimplemented syscall\n");
      thread_exit();
  }
}
