#include <stdio.h>
#include <string.h>

void markdown_to_html(char *html, FILE *fp) {
  char line[1024];
  int in_list = 0;
  int in_paragraph = 0;
  
  while (fgets(line, sizeof(line), fp)) {
    if (line[0] == '#') {
      int level = 1;
      while (line[level] == '#') {
        level++;
      }
      if (in_paragraph) {
        strcat(html, "</p>\n");
        in_paragraph = 0;
      }
      if (in_list) {
        strcat(html, "</ul>\n");
        in_list = 0;
      }
      sprintf(html + strlen(html), "<h%d>%s</h%d>\n", level, line + level + 1, level);
    } else if ((line[0] == '*') || (line[0] == '-')) {
      if (in_paragraph) {
        strcat(html, "</p>\n");
        in_paragraph = 0;
      }
      if (!in_list) {
        strcat(html, "<ul>\n");
        in_list = 1;
      }
      strcat(html, "<li>");
      strncat(html, line + 2, strlen(line) - 2);
      strcat(html, "</li>\n");
    } else {
      if (in_list) {
        strcat(html, "</ul>\n");
        in_list = 0;
      }
      if (!in_paragraph) {
        strcat(html, "<p>");
        in_paragraph = 1;
      }
      strncat(html, line, strlen(line) - 1);
    }
  }

  if (in_list) {
    strcat(html, "</ul>\n");
  }
  if (in_paragraph) {
    strcat(html, "</p>\n");
  }
}

int main(int argc, char *argv[]) {
  char html[1024];
  FILE *fp;

  if (argc != 2) {
    printf("Usage: %s <markdown file>\n", argv[0]);
    return 1;
  }

  fp = fopen(argv[1], "r");
  if (!fp) {
    perror(argv[1]);
    return 1;
  }

  markdown_to_html(html, fp);
  printf("html: %s\n", html);
}
