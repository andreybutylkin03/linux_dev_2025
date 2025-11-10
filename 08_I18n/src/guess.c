#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#define _(s) gettext(s)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", "locale");
    textdomain("guess");
    
    printf(_("Pick a number 1-100\n"));
    
    int low = 1, high = 100;
    
    while (low < high) {
        int mid = (low + high) / 2;
        printf(_(">%d? (y/n): "), mid);
        
        char answer[10];
        if (!fgets(answer, sizeof(answer), stdin)) {
            printf(_("Input error\n"));
            return 1;
        }
        
        answer[strcspn(answer, "\n")] = 0;
        
        if (strcmp(answer, _("y")) == 0) {
            low = mid + 1;
        } else if (strcmp(answer, _("n")) == 0) {
            high = mid;
        } else {
            printf(_("Invalid answer\n"));
        }
    }
    
    printf(_("Your number: %d\n"), low);
    return 0;
}
