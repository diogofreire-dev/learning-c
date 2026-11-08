#include <stddef.h>
#include <stdio.h>

// A struct groups related variables (members) into one new type.
// typedef gives it a shorter name: Student instead of struct student.
typedef struct student
{
    char name[32];
    int age;
    double grades[3];
} Student;

// A struct is passed BY VALUE (copied) like an int; passing a pointer avoids the copy.
// p->age is short for (*p).age
void print_student(const Student *s)
{
    double sum = 0;
    for (int i = 0; i < 3; i++)
    {
        sum += s->grades[i];
    }
    printf("%-10s %2d years   average %.1f\n", s->name, s->age, sum / 3);
}

void birthday(Student *s)
{
    s->age++;
}

// Same 3 members, different order
struct bad_order
{
    char a;
    int b;
    char c;
};

struct good_order
{
    int b;
    char a;
    char c;
};

int main(void)
{
    // Members can be set by name (designated initialisers)
    Student class[] = {
        {.name = "Diogo", .age = 20, .grades = {16, 14, 18}},
        {.name = "Ana", .age = 19, .grades = {12, 15, 13}},
        {.name = "Rui", .age = 21, .grades = {10, 11, 17}},
    };
    size_t count = sizeof(class) / sizeof(class[0]);

    for (size_t i = 0; i < count; i++)
    {
        print_student(&class[i]);
    }

    birthday(&class[0]);
    printf("\nAfter birthday: ");
    print_student(&class[0]);

    // The CPU reads an int fastest from an address that is a multiple of 4, so the
    // compiler adds empty bytes (padding) to keep every member aligned
    printf("\nsizeof(Student)           = %zu\n", sizeof(Student));
    printf("sizeof(struct bad_order)  = %zu\n", sizeof(struct bad_order));
    printf("  a at offset %zu, b at offset %zu, c at offset %zu\n",
           offsetof(struct bad_order, a), offsetof(struct bad_order, b),
           offsetof(struct bad_order, c));
    printf("sizeof(struct good_order) = %zu\n", sizeof(struct good_order));
    printf("  b at offset %zu, a at offset %zu, c at offset %zu\n",
           offsetof(struct good_order, b), offsetof(struct good_order, a),
           offsetof(struct good_order, c));

    return 0;
}
