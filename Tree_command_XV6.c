#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


char* strcat(char* s1, const char* s2)
{
  char* b = s1;

  while (*s1) ++s1;
  while (*s2) *s1++ = *s2++;
  *s1 = 0;

  return b;
}


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
tree(char *path, int level)
{
  char buf[512], *p;
  // char *z;
  int fd;
  struct dirent de;
  struct stat st;

  // chdir(path);
  if((fd = open(path, 0)) < 0){
    printf(2, "tree: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "tree: cannot stat %s\n", path);
    close(fd);
    return;
  }
  

  if (st.type == 1)
  {
    for (int i = 0; i < level; ++i)
    {
      /* code */
      printf(1, "-");
    }
    printf(1, "d- %s\n", fmtname(path));
  }

  switch(st.type){
  case T_FILE:
    for (int i = 0; i < level; ++i)
    {
      /* code */
      printf(1, "-");
    }
    printf(1, "f- %s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "tree: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "tree: cannot stat %s\n", buf);
        continue;
      }
      if (fmtname(buf)[0]!='.'){
        // printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        // z = strcat(buf, "/");
        // printf(1, "%s\n", z);
        tree(buf, level+1);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  // char n[]="hello", m[]="world";

  // char *z = strcat(n,m);

  // printf(1, "%s", z);

  if(argc < 2){
    tree(".", 0);
    exit();
  }
  for(i=1; i<argc; i++)
    tree(argv[i], 0);
  exit();
}
