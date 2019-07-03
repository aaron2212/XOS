#include "syscalls.h"

/*
    Func: sys_getuid
    Get the user ID (uid) for the currently logged in user

    Returns:
        The user ID for the currently logged in user
*/

int sys_getuid()
{
    return get_current_user().uid;
}