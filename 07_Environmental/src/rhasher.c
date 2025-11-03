#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <rhash.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

static int get_hash_id(const char *name) {
    if (strcasecmp(name, "MD5") == 0) return RHASH_MD5;
    if (strcasecmp(name, "SHA1") == 0) return RHASH_SHA1;
    if (strcasecmp(name, "TTH") == 0) return RHASH_TTH;
    return 0;
}

static void print_hash(int hash_id, unsigned char *digest, int lower) {
    char output[256];
    int flags = lower ? RHPR_BASE64 : RHPR_HEX;
    rhash_print_bytes(output, digest, rhash_get_digest_size(hash_id), flags);
    printf("%s\n", output);
}

static void process_line(char *line) {
    char algo[32], input[1024];
    
    if (sscanf(line, "%31s %1023[^\n]", algo, input) != 2) return;
    
    int hash_id = get_hash_id(algo);
    if (!hash_id) {
        fprintf(stderr, "Unknown algorithm\n");
        return;
    }
    
    unsigned char digest[64];
    int lower = islower(algo[0]);
    
    if (input[0] == '"') {
        size_t len = strlen(input);
        if (len > 1 && input[len-1] == '"') input[len-1] = '\0';
        if (rhash_msg(hash_id, input+1, strlen(input+1), digest) < 0) {
            fprintf(stderr, "Hashing error\n");
        } else {
            print_hash(hash_id, digest, lower);
        }
    } else {
        if (rhash_file(hash_id, input, digest) < 0) {
            fprintf(stderr, "File error\n");
        } else {
            print_hash(hash_id, digest, lower);
        }
    }
}

int main(void) {
    rhash_library_init();
    char *line = NULL;
    size_t len = 0;
    
#ifdef HAVE_READLINE
    while ((line = readline("")) != NULL) {
        if (line[0]) add_history(line);
        process_line(line);
        free(line);
    }
#else
    while (getline(&line, &len, stdin) != -1) {
        line[strcspn(line, "\n")] = '\0';
        process_line(line);
    }
    free(line);
#endif
    
    return 0;
}
