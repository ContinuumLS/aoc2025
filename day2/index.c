/* Compile in root dir: gcc day2/index.c -o day2/a.out */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 512
#define TRUE 1
#define FALSE 0

typedef struct
{
    unsigned long begin, end;
} Range;

char *get_input();
int get_range_count(char *input);
Range **input_to_ranges(char *input, int size);
Range *create_range(unsigned long begin, unsigned long end);
unsigned long sum_invalid_ids(Range **ranges, int count);
unsigned long sum_invalid_ids2(Range **ranges, int count);

int main()
{
    char *input = get_input();
    int count = get_range_count(input);
    Range **ranges = input_to_ranges(input, count);
    unsigned long invalid1 = sum_invalid_ids(ranges, count);
    unsigned long invalid2 = sum_invalid_ids2(ranges, count);
    printf("%ld\n%ld\n", invalid1, invalid2);
    for (int i = 0; i < count; ++i)
        free(ranges[i]);
    free(ranges);
}

char *get_input()
{
    size_t size = BUF_SIZE;
    char *input = NULL;
    FILE *file = fopen("day2/input.txt", "r");
    if (file == NULL)
        exit(1);
    getline(&input, &size, file);
    return input;
}

int get_range_count(char *input)
{
    size_t size = strlen(input);
    int ranges = 0;
    for (int i = 0; i < size; ++i)
    {
        if (input[i] == '-')
            ++ranges;
    }
    return ranges;
}

Range **input_to_ranges(char *input, int size)
{
    const char delim[] = ",";
    Range **list = (Range **)malloc(size * sizeof(Range *));
    char *token = strtok(input, delim);
    for (int i = 0; token != NULL; ++i)
    {
        unsigned long begin, end;
        sscanf(token, "%ld-%ld", &begin, &end);
        list[i] = create_range(begin, end);
        token = strtok(NULL, delim);
    }
    free(input);
    return list;
}

Range *create_range(unsigned long begin, unsigned long end)
{
    Range *range = (Range *)malloc(sizeof(Range));
    if (range == NULL)
        return NULL;
    range->begin = begin;
    range->end = end;
    return range;
}

unsigned long sum_invalid_ids(Range **ranges, int count)
{
    unsigned long invalid_sum = 0l;
    for (int i = 0; i < count; ++i)
    {
        Range *range = ranges[i];
        for (unsigned long j = range->begin; j <= range->end; ++j)
        {
            char product_id[12];
            sprintf(product_id, "%ld", j);
            int id_len = strlen(product_id);
            if (id_len % 2 == 0)
            {
                int mid = id_len / 2;
                char left[mid + 1];
                char right[mid + 1];
                strncpy(left, product_id, mid);
                strncpy(right, product_id + mid, id_len - mid);
                left[mid] = right[mid] = '\0';
                if (strcmp(left, right) == 0)
                    invalid_sum += j;
            }
        }
    }
    return invalid_sum;
}

unsigned long sum_invalid_ids2(Range **ranges, int count)
{
    unsigned long invalid_sum = 0l;
    char product_id[32];
    for (int i = 0; i < count; ++i)
    {
        Range *range = ranges[i];
        for (unsigned long j = range->begin; j <= range->end; ++j)
        {
            sprintf(product_id, "%ld", j);
            int id_len = strlen(product_id);
            int flagged = FALSE;
            for (int k = 1; k <= id_len / 2 && !flagged; ++k)
            {
                if (id_len % k != 0)
                    continue;
                char segment[k];
                char next[k];
                strncpy(segment, product_id, k);
                segment[k] = '\0';
                int invalid = TRUE;
                for (int l = k; l < id_len && invalid; l += k)
                {
                    strncpy(next, product_id + l, l + k);
                    next[k] = '\0';
                    if (strcmp(segment, next) != 0)
                        invalid = FALSE;
                }
                if (invalid)
                    flagged = TRUE;
            }
            if (flagged)
                invalid_sum += j;
        }
    }
    return invalid_sum;
}