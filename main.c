#include <stdio.h>
#include <stdlib.h>

#include <vector.h>

char *readSourceFile(const char *path, int *size) {
    FILE *fd = fopen(path, "r");
    fseek(fd, 0, SEEK_END);
    int len = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    *size = len;
    char *res = malloc(sizeof(char) * len);
    fread(res, sizeof(char), len, fd);
    fclose(fd);
    return res;
}

#define MEM_CAP 30000
static char mem[MEM_CAP] = {0};
static int ip = 0;
static int dp = 0;

typedef struct {
    int *data;
    int cnt;
    int cap;
} jmpstack;

static jmpstack js = {0};

void panic(int code, const char *msg) {
    fprintf(stderr, "%s", msg);
    exit(code);
}

void interpet(const char *code, int size) {

    while (ip < size) {
        switch (code[ip]) {
        case '>': {
            dp++;
            printf("INC DP to %d\n", dp + 1);
        } break;
        case '<': {
            dp--;
            printf("DEC DP to %d\n", dp - 1);
        } break;
        case '+': {
            mem[dp] += 1;
            printf("INC MEM at %d to %d\n", dp, mem[dp] + 1);
        } break;
        case '-': {
            mem[dp] -= 1;
            printf("DEC MEM at %d to %d\n", dp, mem[dp] - 1);
        } break;
        case '.': {
            putc(mem[dp], stdout);
        } break;
        case ',': {
            mem[dp] = getchar();
        } break;
        case '[': {
            if (mem[dp] == 0) {
                int openCnt = 1;
                ip++;
                while (openCnt > 0) {
                    if (code[ip] == '[')
                        openCnt++;
                    if (code[ip] == ']')
                        openCnt--;
                    ip++;
                }
            } else {
                printf("Push ip: %d\n", ip);
                VEC_ADD(&js, ip);
            }
        } break;
        case ']': {
            if (mem[dp] != 0 && js.cnt > 0) {
                printf("Pop ip: %d\n", js.data[js.cnt - 1]);
                ip = js.data[--js.cnt];
            }
        } break;
        default:
            break;
        }
        ip++;
    }
}

int main(int argc, char **argv) {

    if (argc != 2)
        return 1;

    (void)*argv++;
    const char *path = *argv++;

    int size = 0;
    char *code = readSourceFile(path, &size);

    interpet(code, size);

    free(code);

    return 0;
}
