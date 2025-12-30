#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TARGET 1337

// Reversed x function
int64_t reverse_x(int64_t result) {
    // reverse: a1 = (2 * (result + 11) / 5 - 7) / 3
    return (2 * ((result + 11) / 5) - 7) / 3;
}

// Function to reverse y(a1) given a target value
void reverse_y(int64_t target) {
    // Brute force each possible length of the string s1 (max length)
    char available[] = "123456789";  // Available digits
    int max_length = 6;  // Max length of string (adjust as needed)
    
    // Try different string lengths
    for (int length = 1; length <= max_length; ++length) {
        for (int i = 0; i < (1 << (3 * length)); ++i) {  // Loop through all combinations
            char s1[length + 1];
            s1[length] = '\0';  // Null-terminate the string
            
            // Generate each combination of available characters
            for (int j = 0; j < length; ++j) {
                s1[j] = available[(i / (1 << (3 * j))) % 9];
            }

            unsigned int sum = 0;
            // Calculate y(s1)
            for (int j = 0; s1[j]; ++j) {
                sum += reverse_x(s1[j] + j);  // Reverse each character calculation
            }
            
            // Check if the sum matches the target
            if (sum == target) {
                printf("Found solution: %s\n", s1);
                return;
            }
        }
    }
}

int main() {
    reverse_y(TARGET);  // Find the solution that equals TARGET
    return 0;
}
