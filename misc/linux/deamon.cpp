#include "deamon.h"
#include <unistd.h> 
#include <signal.h> 
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

void init_daemon()
{
    int pid, fd;
    
    // 1.ת��Ϊ��̨����
    if ((pid = fork()) == -1) exit(1);
    if (pid != 0) exit(0); // �����̣�ǰ̨���̣��˳�
    
    // 2.�뿪ԭ�ȵĽ����飬�Ự
    if (setsid() == -1) exit(1); // ����һ���»Ự
    
    // 3.��ֹ�ٴδ򿪿����ն�
    if ((pid = fork()) == -1) exit(1);
    if (pid != 0) exit(0); // �����̣��Ự��ͷ���̣��˳�
    
    // 4.�رմ򿪵��ļ��������������˷�ϵͳ��Դ
    for (int i = 0; i <NOFILE; i++)
      close(i);
    
    // 5.�ı䵱ǰ�Ĺ���Ŀ¼������ж�ز����ļ�ϵͳ
    //if (chdir("/") == -1) exit(1);
    
    // 6.�����ļ����룬��ֹĳЩ���Ա�����������
    //if (umask(0) == -1) exit(1);
    
    // 7.�ض����׼���룬���������������Ϊ�ػ�����û�п����ն�
    if ((fd = open("/dev/null", O_RDWR)) == -1) exit(1); // ��һ��ָ��/dev/null���ļ�������
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
    
    // 8.���ػ����̵��ӽ���������Ҫ������Ϣ����ô����init���̻��գ����������ʬ����
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) exit(1);
}
