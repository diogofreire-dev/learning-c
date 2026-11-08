#include <stdio.h>

// Every call creates a stack frame. At the start of the function, ARM64 saves the
// caller's frame pointer (x29) and the return address (x30) on the stack:
//
//   higher addresses
//   ┌──────────────────────────┐
//   │ caller's frame           │
//   ├──────────────────────────┤ ← fp (x29) of this function
//   │ saved fp | return address│   (where to continue in the caller after ret)
//   │ local variables          │
//   └──────────────────────────┘ ← sp
//   lower addresses (the stack grows down)
//
// The __builtin_ functions are clang/gcc extensions that read these values.

void show_frame(int depth)
{
    int local = depth;

    printf("depth %d: local at %p   fp = %p   returns to %p\n",
           depth, (void *) &local, __builtin_frame_address(0), __builtin_return_address(0));

    if (depth < 3)
    {
        show_frame(depth + 1);
    }
}

int main(void)
{
    // Function addresses, to compare with the return addresses above.
    // A function pointer converted to void * is not standard C, but works here.
    printf("main       starts at %p\n", (void *) main);
    printf("show_frame starts at %p\n\n", (void *) show_frame);

    show_frame(0);
    return 0;
}
