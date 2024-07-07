#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/*
 * Helper function that prints the help menu
 */
void print_help();

/*
 * Helper function that prints formats the final output
 */
void print_estimated_time(int word_count, int speed, int hours, int minutes, int seconds);

/*
 * Helper function to detect whether a word is valid or not.
 * Valid in this context means any word that contains alphanumeric
 * characters, ", ', or -.
 */
bool is_word(char* word);

const char delimiter[5] = ",. \n";
const char* SPEED_FLAG = "-s";
const char* NORMAL_FLAG = "-n"; // 150 normal
const char* MEDIUM_FLAG = "-m"; // 175 medium
const char* FAST_FLAG = "-f"; // 200 fast

// Average speeds in Words Per Minute
const int NORMAL_SPEED = 150;
const int MEDIUM_SPEED = 175;
const int FAST_SPEED = 200;

int words_per_minute = FAST_SPEED;

/*
This program gives a rough estimate on the speaking time of a markdown file 
*/
int main(int argc, char** argv)
{
    char* FILE_DIR;

    if (argc < 2)
    {
        print_help();
        return EXIT_SUCCESS;
    }
    else if (argc < 3)
    {
        FILE_DIR = argv[1];
    }
    else if (argc < 4)
    {
        FILE_DIR = argv[1];

        if (strcmp(argv[2], NORMAL_FLAG) == 0)
        {
            words_per_minute = NORMAL_SPEED; 
        }
        else if (strcmp(argv[2], MEDIUM_FLAG) == 0)
        {
            words_per_minute = MEDIUM_SPEED; 
        }
        else if (strcmp(argv[2], FAST_FLAG) == 0)
        {
            words_per_minute = FAST_SPEED; 
        }
        else
        {
            print_help();
            return EXIT_FAILURE;
        }
    }
    else if (argc == 4)
    {
        FILE_DIR = argv[1];

        if (strcmp(argv[2], SPEED_FLAG) != 0)
        {
            print_help();
            return EXIT_FAILURE;
        }

        words_per_minute = atoi(argv[3]);

        if (words_per_minute == 0)
        {
            perror("ERROR. Invalid argument\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        print_help();
        return EXIT_SUCCESS; 
    }

    // Open the file
    FILE* fptr;

    fptr = fopen(FILE_DIR, "r");

    if (fptr == NULL)
    {
        perror("ERROR. File not found\n");
        return EXIT_FAILURE;
    }

    // Read the content of the file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    bool is_code_block = false;
    unsigned long word_count = 0;

    while ((read = getline(&line, &len, fptr)) != -1)
    {
        // If the line is a code block
        if (strncmp("```", line, 3) == 0)
        {
            is_code_block = !is_code_block;

            // printf("\nCode block detected in line %s\n\n", line);
            continue;
        }

        // If line is not in a code block, heading nor a quote
        if (!is_code_block && line[0] != '#' && line[0] != '>')
        {
            // Split the line into words and count them
            char* token = strtok(line, delimiter);
            while (token != NULL)
            {
                if (is_word(token) == true)
                {
                    word_count++;
                }
                token = strtok(NULL, delimiter);
            }
        }
    }

    free (line);
    fclose(fptr);

    // Get the estimated time in minutes
    double estimated_time = (double) word_count / (double)(words_per_minute);
    
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    // Since estimated time will be in minutes, we can account for hours if needed
    if (estimated_time > 60)
    {
        hours = estimated_time / 60;
    }

    // Minutes will be equal to the estimated time minus the calculated hours in minute
    // For example, if estimated_time is 125.32, then minutes will be 125.32 - 60 * 2
    // => 5.32 minutes => int minutes = 5;
    minutes = (estimated_time - (hours * 60));

    // Using modulus here would generate floating point rounding errors.
    // Therefore, it is better to simply multiply.
    seconds = ((estimated_time - minutes) * 60);

    print_estimated_time(word_count, words_per_minute, hours, minutes, seconds);

    return EXIT_SUCCESS;
}

void print_help()
{
    printf("Markdown Speaking Time Estimator - mdspeak\n\n");
    printf("This program estimates the speaking time required to read aloud the content of a markdown file.\n");

    printf("Usage: mdspeak <FILE_PATH> [OPTIONS]\n\n");

    printf("Arguments:\n");
    printf(" <FILE_PATH>\t Path to the markdown file to be analyzed.\n\n");

    printf("Options:\n");
    printf("  -n\tEstimate speaking time at a normal speed (150 words per minute).\n");
    printf("  -m\tEstimate speaking time at a medium speed (175 words per minute).\n");
    printf("  -n\tEstimate speaking time at a fast speed (200 words per minute).\n");
    printf("  -s <SPEED>\tEstimate speaking time at a custom speed specified in words per minute.\n\n");

    printf("Examples:\n");
    printf("  mdspeak file.md\n");
    printf("  mdspeak file.md -n\n");
    printf("  mdspeak file.md -m\n");
    printf("  mdspeak file.md -f\n");
    printf("  mdspeak file.md -s 180\n\n");

    printf("Notes:\n");
    printf("  - If no speed option is provided, the default speed is 200 words per minute.\n");
    printf("  - Ensure the file path is correct and accessible.\n");
    printf("  - The program ignores code blocks, headings, and quotes while counting words.\n");
    printf("  - Valid words include those with alphanumeric characters, commas, single quotes, double quotes, and hyphens.\n");
}

void print_estimated_time(int word_count, int speed, int hours, int minutes, int seconds)
{
    printf("\n--------------------------\n");
    printf("Word Count Analysis\n");
    printf("--------------------------\n");
    printf("Total number of words: %lu\n", word_count);
    printf("Words Per Minute (WPM): %d\n", words_per_minute);
    
    printf("Estimated Speaking Time:\n");
    
    if (hours > 0)
    {
        printf("\t%d hour%s\n", hours, hours == 1 ? "" : "s");
    }
    printf("\t%d minute%s\n", minutes, minutes == 1 ? "" : "s");
    printf("\t%d second%s\n", seconds, seconds == 1 ? "" : "s");
    printf("--------------------------\n");
}

bool is_word(char* word)
{
    while (*word != '\0')
    {
        if (isalnum(*word) == 0 && *word != '\'' && *word != '-' && *word != '"')
        {
            return false;
        }
        word++;
    }

    return true;
}

