#include <ctype.h>
#include <stdio.h>
#include <string.h>

int is_vowel(unsigned char c)
{
    int lower = tolower(c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

// Compares only the letters and digits, ignoring case: two indexes walk towards the middle
int is_palindrome(const char s[])
{
    size_t left = 0;
    size_t right = strlen(s);

    while (left < right)
    {
        // Skip anything that is not a letter or digit (spaces, commas, hyphens...)
        if (!isalnum((unsigned char) s[left]))
        {
            left++;
        }
        else if (!isalnum((unsigned char) s[right - 1]))
        {
            right--;
        }
        else
        {
            if (tolower((unsigned char) s[left]) != tolower((unsigned char) s[right - 1]))
            {
                return 0;
            }
            left++;
            right--;
        }
    }
    return 1;
}

int main(void)
{
    char line[200];

    printf("Write a sentence: ");
    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        return 1;
    }
    line[strcspn(line, "\n")] = '\0';

    int letters = 0, vowels = 0, digits = 0, spaces = 0;
    size_t len = strlen(line);

    // The ctype.h functions expect an unsigned char value: a plain char can be
    // negative (for example accented letters), which would be undefined behaviour
    for (size_t i = 0; i < len; i++)
    {
        unsigned char c = line[i];
        if (isalpha(c))
        {
            letters++;
            if (is_vowel(c))
            {
                vowels++;
            }
        }
        else if (isdigit(c))
        {
            digits++;
        }
        else if (isspace(c))
        {
            spaces++;
        }
    }

    printf("Letters: %d, vowels: %d, digits: %d, spaces: %d\n", letters, vowels, digits, spaces);

    printf("Uppercase: ");
    for (size_t i = 0; i < len; i++)
    {
        putchar(toupper((unsigned char) line[i]));
    }
    printf("\n");

    // Walking backwards with a size_t: i > 0 and use i - 1, because a size_t can never
    // be negative (i >= 0 would always be true and the loop would never end)
    printf("Reversed: ");
    for (size_t i = len; i > 0; i--)
    {
        putchar(line[i - 1]);
    }
    printf("\n");

    printf("Palindrome: %s\n", is_palindrome(line) ? "yes" : "no");

    return 0;
}
