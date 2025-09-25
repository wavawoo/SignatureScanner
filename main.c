#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

int checkExe(FILE* file, int* IsExe)
{
    unsigned char buf[2];
    size_t elemSize = sizeof(buf[0]);
    size_t elemsNumber = sizeof(buf)/sizeof(buf[0]);

    if (file == NULL)
    {
        printf("error file in checkExe!\n");
        return 1;
    }
    if (IsExe == NULL)
    {
        printf("error result in checkExe!\n");
        return 2;
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        printf("error fseek in checkExe!\n");
        return 3;
    }

    if (fread(buf, elemSize, elemsNumber, file) != elemsNumber)
    {
        printf("error fread in checkExe!\n");
        return 4;
    }

    if ((buf[0] == 'M') && (buf[1] == 'Z')) *IsExe = 1;
    else *IsExe = 0;

    return 0;
}

int checkFileSize(FILE* file, size_t signature_offset, size_t size_bytes, int* result)
{
    long FileSize;
    long sum_size = signature_offset + size_bytes;

    if (file == NULL)
    {
        printf("error file in checkFileSize!\n");
        return 1;
    }
    if (result == NULL)
    {
        printf("error result in checkFileSize!\n");
        return 4;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        printf("error fseek in checkFileSize!\n");
        return 5;
    }

    FileSize = ftell(file);
    if (FileSize == -1)
    {
        printf("error ftell in checkFileSize\n");
        return 6;
    }

    if (FileSize < sum_size) *result = 0;
    else *result = 1;

    return 0;
}

int checkSignature(FILE* file, size_t signature_offset, unsigned char signature_bytes[], int* result)
{
    unsigned char buffer[8];
    size_t i, count = 0;
    size_t elementSize = sizeof(buffer[0]);
    size_t elementsNumber = sizeof(buffer)/sizeof(buffer[0]);

    if (file == NULL)
    {
        printf("error file in checkSignature!\n");
        return 1;
    }
    if (signature_bytes == NULL)
    {
        printf("error signature_bytes in checkSignature!\n");
        return 3;
    }
    if (result == NULL)
    {
        printf("error result in checkSignature!\n");
        return 4;
    }

    if (fseek(file, signature_offset, SEEK_SET) != 0)
    {
        printf("error fseek in checkSignature!\n");
        return 5;
    }

    if (fread(buffer, elementSize, elementsNumber, file) != elementsNumber)
    {
        printf("error fread in checkSignature!\n");
        return 6;
    }

    for(i = 0; i < elementsNumber; i++)
    {
        if (signature_bytes[i] == buffer[i]) count++;
    }

    if (count == elementsNumber) *result = 1;
    else *result = 0;

    return 0;
}

int main()
{
    FILE* signature_file;
    FILE* file;
    unsigned char signature_bytes[8];
    char path[100], signature_name[100];
    size_t i, signature_offset, size_bytes;
    int rez, IsExe, result;
    size_bytes = sizeof(signature_bytes)/sizeof(signature_bytes[0]);

    if (printf("=== Signature Scanner ===\n") < 0)
    {
        printf("error printf!\n");
        return 1;
    }
    if (printf("Write full path to file for scanning: ") < 0)
    {
        printf("error printf!\n");
        return 2;
    }

    signature_file = fopen("signDBase.txt", "r");
    if (signature_file == NULL)
    {
        printf("error fopen!\n");
        return 3;
    }

    if (scanf("%[^\n]", path) != 1)
    {
        printf("error scanf!\n");
        if (fclose(signature_file) != 0)
        {
            printf("error fclose!\n");
            return 4;
        }
        return 5;
    }

    rez = fscanf(signature_file, "%zx", &signature_offset);
    if (rez != 1)
    {
        printf("error fscanf!\n");
        if (fclose(signature_file) != 0)
        {
            printf("error fclose!\n");
            return 6;
        }
        return 7;
    }

    for(i = 0; i < size_bytes; i++)
    {
        rez = fscanf(signature_file, "%hhx", &signature_bytes[i]);
        if(rez != 1)
        {
            printf("error fcsanf!\n");
            if (fclose(signature_file) != 0)
            {
                printf("error fclose!\n");
                return 8;
            }
            return 9;
        }
    }

    rez = fscanf(signature_file, "%s", signature_name);
    if (rez != 1)
    {
        printf("error fscanf!\n");
        if (fclose(signature_file) != 0)
        {
            printf("error fclose!\n");
            return 10;
        }
        return 11;
    }

    if (fclose(signature_file) != 0)
    {
        printf("error fclose!\n");
        return 12;
    }

    file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("error fopen!\n");
        return 13;
    }

    rez = checkExe(file, &IsExe);
    if (rez != 0)
    {
        printf("error rez!\n");
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 14;
        }
        return 15;
    }
    if (IsExe == 0)
    {
        if (printf("File is not .exe, all OK!\n") < 0)
        {
            printf("error printf!\n");
            return 16;
        }
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 17;
        }
        return 18;
    }

    rez = checkFileSize(file, signature_offset, size_bytes, &result);
    if (rez != 0)
    {
        printf("error rez!\n");
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 19;
        }
        return 20;
    }
    if (result == 0)
    {
        if (printf("All OK!\n") < 0)
        {
            printf("error printf!\n");
            return 20;
        }
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 21;
        }
        return 22;
    }

    rez = checkSignature(file, signature_offset, signature_bytes, &result);
    if (rez != 0)
    {
        printf("error rez!\n");
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 23;
        }
        return 24;
    }
    if (result == 0)
    {
        if(printf("All OK, your file is safe!\n") < 0)
        {
            printf("error printf!\n");
            return 25;
        }
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 26;
        }
        return 27;
    }
    if (result == 1)
    {
        if (printf("qwertyA threat has been detected in the file. It`s recommended to remove it immediately!\n") < 0)
        {
            printf("error printf!\n");
            return 28;
        }
        if (printf("Detected signature: %s\nDanger file path: %s\n", signature_name, path) < 0)
        {
            printf("error printf!\n");
            return 29;
        }
        if (fclose(file) != 0)
        {
            printf("error fclose!\n");
            return 30;
        }
    }

    return 0;
}
