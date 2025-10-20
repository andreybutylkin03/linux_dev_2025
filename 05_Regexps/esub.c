#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>

#define MAX_ERROR_MSG 512
#define MAX_GROUPS 9
#define MAX_REFS 100

static void print_regerror(int errcode, regex_t *compiled) {
    char errbuf[MAX_ERROR_MSG];
    regerror(errcode, compiled, errbuf, sizeof(errbuf));
    fprintf(stderr, "Regex error: %s\n", errbuf);
}

static int append_buf(char **out, size_t *out_len, size_t *out_cap, const char *src, size_t n) {
    if (*out_len + n + 1 > *out_cap) {
        size_t newcap = (*out_cap == 0) ? 256 : (*out_cap * 2);
        while (newcap < *out_len + n + 1) newcap *= 2;
        char *tmp = realloc(*out, newcap);
        if (!tmp) return -1;
        *out = tmp;
        *out_cap = newcap;
    }
    memcpy(*out + *out_len, src, n);
    *out_len += n;
    (*out)[*out_len] = '\0';
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regexp> <substitution> <string>\n", argv[0]);
        return 2;
    }

    const char *pattern = argv[1];
    const char *replacement = argv[2];
    const char *input = argv[3];

    regex_t re;
    int rc = regcomp(&re, pattern, REG_EXTENDED);
    if (rc != 0) {
        print_regerror(rc, &re);
        return 3;
    }

    regmatch_t matches[MAX_GROUPS + 1];
    int nmatch = MAX_GROUPS + 1;

    rc = regexec(&re, input, nmatch, matches, 0);
    if (rc == REG_NOMATCH) {
        puts(input);
        regfree(&re);
        return 0;
    } else if (rc != 0) {
        print_regerror(rc, &re);
        regfree(&re);
        return 4;
    }

    if (matches[0].rm_so == -1 || matches[0].rm_eo == -1) {
        fprintf(stderr, "Internal: unexpected match positions\n");
        regfree(&re);
        return 5;
    }

    char *result = NULL;
    size_t res_len = 0;
    size_t res_cap = 0;

    if (matches[0].rm_so > 0) {
        if (append_buf(&result, &res_len, &res_cap, input, (size_t)matches[0].rm_so) != 0) {
            fprintf(stderr, "Out of memory\n");
            regfree(&re);
            free(result);
            return 6;
        }
    }

    int ref_count = 0;
    for (size_t i = 0; replacement[i] != '\0'; ++i) {
        char c = replacement[i];
        if (c == '\\') {
            char next = replacement[i+1];
            if (next == '\0') {
                if (append_buf(&result, &res_len, &res_cap, "\\", 1) != 0) {
                    fprintf(stderr, "Out of memory\n");
                    regfree(&re);
                    free(result);
                    return 6;
                }
                break;
            }

            if (next >= '0' && next <= '9') {
                int idx = next - '0';
                if (idx > MAX_GROUPS) {
                    fprintf(stderr, "Substitution error: backreference \\%d out of supported range (max %d)\n", idx, MAX_GROUPS);
                    regfree(&re);
                    free(result);
                    return 7;
                }
                if (matches[idx].rm_so == -1 || matches[idx].rm_eo == -1) {
                    fprintf(stderr, "Substitution error: backreference \\%d does not exist in the match.\n", idx);
                    regfree(&re);
                    free(result);
                    return 8;
                }
                size_t gstart = (size_t)matches[idx].rm_so;
                size_t glen = (size_t)(matches[idx].rm_eo - matches[idx].rm_so);
                if (append_buf(&result, &res_len, &res_cap, input + gstart, glen) != 0) {
                    fprintf(stderr, "Out of memory\n");
                    regfree(&re);
                    free(result);
                    return 6;
                }
                i++;
                ref_count++;
                if (ref_count > MAX_REFS) {
                    fprintf(stderr, "Substitution error: too many backreference occurrences (>%d)\n", MAX_REFS);
                    regfree(&re);
                    free(result);
                    return 9;
                }
            } else if (next == '\\') {
                if (append_buf(&result, &res_len, &res_cap, "\\", 1) != 0) {
                    fprintf(stderr, "Out of memory\n");
                    regfree(&re);
                    free(result);
                    return 6;
                }
                i++;
            } else {
                if (append_buf(&result, &res_len, &res_cap, &next, 1) != 0) {
                    fprintf(stderr, "Out of memory\n");
                    regfree(&re);
                    free(result);
                    return 6;
                }
                i++;
            }
        } else {
            if (append_buf(&result, &res_len, &res_cap, &c, 1) != 0) {
                fprintf(stderr, "Out of memory\n");
                regfree(&re);
                free(result);
                return 6;
            }
        }
    }

    size_t input_len = strlen(input);
    size_t suffix_start = (size_t)matches[0].rm_eo;
    if (suffix_start < input_len) {
        if (append_buf(&result, &res_len, &res_cap, input + suffix_start, input_len - suffix_start) != 0) {
            fprintf(stderr, "Out of memory\n");
            regfree(&re);
            free(result);
            return 6;
        }
    }

    printf("%s\n", result ? result : "");

    regfree(&re);
    free(result);
    return 0;
}

