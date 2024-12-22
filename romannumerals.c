#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to convert a single Roman numeral character to its decimal value
int romanCharToDecimal(char roman); 

// Function to convert a Roman numeral string to its decimal equivalent
int romanToDecimal(const char *roman); 

// Function to convert a decimal number to its Roman numeral representation
void decimalToRoman(int num, char *result); 

// Function to convert a decimal number to its English word representation
void decimalToWords(int num, char *result); 

// Function to process the input file, perform calculations, and write results to the output file
void processInputFile(const char *inputFile, const char *outputFile); 

// Function to display the calculation details on the console
void displayCalculation(const char *operand1, int decimal1, 
                         const char *operand2, int decimal2, 
                         char operator, int result, const char *wordsResult);

// Main function: Entry point of the program
int main() {
    // Process the input file and generate the output file
    processInputFile("input.txt", "output.txt"); 
    // Return 0 to indicate successful program execution
    return 0; 
}

// Converts a single Roman numeral character to its decimal value
int romanCharToDecimal(char roman) {
    switch (roman) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0; // Invalid Roman numeral character
    }
}

// Converts a Roman numeral string to its decimal equivalent
int romanToDecimal(const char *roman) { 
    // Initialize the total sum to 0
    int total = 0; 
    // Initialize the previous value to 0
    int prevValue = 0; 

    // Iterate through each character in the Roman numeral string
    for (int i = 0; roman[i] != '\0'; i++) { 
        // Get the decimal value of the current character
        int currentValue = romanCharToDecimal(roman[i]); 

        // Check for invalid Roman numerals
        if (currentValue == 0) 
            return -1; // Invalid Roman numeral

        // Handle subtractive notation (e.g., IV, IX)
        if (currentValue > prevValue) 
            total += currentValue - 2 * prevValue; 
        else 
            total += currentValue;

        // Update the previous value for the next iteration
        prevValue = currentValue; 
    }

    // Return the calculated decimal value
    return total; 
}

// Converts a decimal number to its Roman numeral representation
void decimalToRoman(int num, char *result) { 
    // Define an array of structures to represent Roman numerals and their values
    const struct {
        int value;
        const char *symbol;
    } roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, 
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"}, 
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    // Initialize the result string as an empty string
    result[0] = '\0'; 

    // Iterate through the Roman numeral values in descending order
    for (int i = 0; i < 13 && num > 0; i++) { 
        // Repeatedly append the Roman numeral symbol while the decimal number 
        // is greater than or equal to the current Roman numeral value
        while (num >= roman[i].value) { 
            strcat(result, roman[i].symbol); 
            num -= roman[i].value; 
        }
    }
}

// Converts a decimal number to its English word representation
void decimalToWords(int num, char *result) {
    // Define arrays for ones, teens, tens, and thousands place values
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    // Temporary string to build the word representation
    char temp[200] = ""; 
    // Counter for tracking thousands, millions, etc.
    int partCount = 0; 

    // Process the number in groups of three digits (hundreds, tens, ones)
    while (num > 0) { 
        // Extract the current three-digit part
        int part = num % 1000; 

        // If the current part is not zero
        if (part > 0) { 
            // Create a temporary string for the current part's word representation
            char partResult[200] = ""; 

            // Handle the hundreds place
            if (part / 100 > 0) { 
                sprintf(partResult, "%s Hundred ", ones[part / 100]); 
            }

            // Handle the tens and ones places (special case for numbers 10-19)
            if (part % 100 >= 10 && part % 100 < 20) { 
                strcat(partResult, teens[part % 100 - 10]);
            } else { 
                strcat(partResult, tens[(part % 100) / 10]); 
                strcat(partResult, " "); 
                strcat(partResult, ones[part % 10]); 
            }

            // Add the appropriate thousands place (e.g., "Thousand", "Million") if applicable
            if (strlen(thousands[partCount]) > 0) { 
                strcat(partResult, " "); 
                strcat(partResult, thousands[partCount]); 
            }

            // Concatenate the current part's word representation to the temporary string
            strcat(partResult, " "); 
            strcat(partResult, temp); 
            strcpy(temp, partResult); 
        }

        // Move to the next three-digit group
        num /= 1000; 
        partCount++; 
    }

    // Copy the final word representation to the result string
    strcpy(result, temp); 
}

// Reads input from a file, performs calculations, and writes results to an output file
void processInputFile(const char *inputFile, const char *outputFile) {
    // Open the input file in read mode ("r")
    FILE *input = fopen(inputFile, "r"); 
    // Open the output file in write mode ("w")
    FILE *output = fopen(outputFile, "w"); 

    // Check if files were opened successfully
    if (input == NULL || output == NULL) { 
        printf("Error opening files.\n"); 
        exit(1); 
    }

    // Buffer to store a line of input from the file
    char line[256]; 
    // Read lines from the input file until the end
    while (fgets(line, sizeof(line), input)) { 
        // Variables to store operands, operator, and results
        char operand1[100], operand2[100], wordsResult[200]; 
        char operator;
        int decimal1, decimal2, result;

        // Parse the input line using sscanf
        if (sscanf(line, "%s %c %s", operand1, &operator, operand2) == 3) { 
            // Convert Roman numerals to decimal values
            decimal1 = romanToDecimal(operand1); 
            decimal2 = romanToDecimal(operand2); 

            // Check for invalid Roman numerals
            if (decimal1 == -1 || decimal2 == -1) { 
                fprintf(output, "Invalid Roman numeral: %s\n", line); 
                continue; 
            }

            // Perform the arithmetic operation based on the operator
            switch (operator) {
                case '+': result = decimal1 + decimal2; break;
                case '-': result = decimal1 - decimal2; break;
                case '*': result = decimal1 * decimal2; break;
                case '/': 
                    if (decimal2 == 0) {
                        fprintf(output, "Error: Division by zero: %s\n", line); 
                        continue; 
                    }
                    result = decimal1 / decimal2; 
                    break;
                default: 
                    fprintf(output, "Invalid operator: %s\n", line); 
                    continue; 
            }

            // Convert the decimal result to words
            decimalToWords(result, wordsResult); 
            // Write the result in words to the output file
            fprintf(output, "%s\n", wordsResult); 

            // Display the calculation result on the console
            displayCalculation(operand1, decimal1, operand2, decimal2, 
                               operator, result, wordsResult); 
        } else {
            // Handle invalid input format
            fprintf(output, "Invalid input format: %s\n", line); 
        }
    }

    // Close the input and output files
    fclose(input); 
    fclose(output); 
}

// Displays calculation details on the console
void displayCalculation(const char *operand1, int decimal1, 
                         const char *operand2, int decimal2, 
                         char operator, int result, const char *wordsResult) {
    // Print the calculation result to the console 
    printf("%s (%d) %c %s (%d) = %d (%s)\n", 
           operand1, decimal1, operator, operand2, decimal2, result, wordsResult); 
}