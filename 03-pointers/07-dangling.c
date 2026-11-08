#include <stdio.h>

// BUG ON PURPOSE – do not fix it here; the fix is in notes.md.
//
// Run it twice:
//   make run/03-pointers/07-dangling
//   ASAN_OPTIONS=detect_stack_use_after_return=1 ./build/03-pointers/07-dangling

int *make_number(void)
{
    int local = 42;
    // local lives in make_number's stack frame, which is thrown away when the
    // function returns: the address stays, but nothing valid is there anymore.
    // (Going through p hides the bug from the compiler's -Wreturn-stack-address.)
    int *p = &local;
    return p;
}

// Any other call reuses the same stack area. This one has exactly the same shape as
// make_number, so its local lands where make_number's local was.
int *other_function(void)
{
    int other = 999;
    int *p = &other;
    return p;
}

int main(void)
{
    int *n = make_number();
    int first = *n;     // may still read 42 – pure luck, the memory was not reused yet

    other_function();
    int second = *n;    // same address, but the memory now belongs to someone else

    printf("First read:  %d\n", first);
    printf("Second read: %d\n", second);

    return 0;
}
