#include "syscalls.h"

int sys_time()
{
    return get_system_time();
}