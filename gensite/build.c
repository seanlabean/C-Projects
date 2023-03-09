#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the file extension for HTML files
#define HTML_EXTENSION ".html"

int main(int argc, char *argv[]) {
  // Check if a file name was provided as an argument
  if (argc != 2) {
    printf("Usage: %s <file_name>\n", argv[0]);
    return 1;
  }

  // Get the file name and create the HTML file name
  char *file_name = argv[1];
  size_t html_file_name_size = strlen(file_name) + strlen(HTML_EXTENSION) + 1;
  char html_file_name[html_file_name_size];
  strcpy(html_file_name, file_name);
  strcat(html_file_name, HTML_EXTENSION);

  // Open the input file and the output HTML file
  FILE *input_file = fopen(file_name, "r");
  FILE *html_file = fopen(html_file_name, "w");

  // Check if the input file was successfully opened
  if (input_file == NULL) {
    printf("Error opening input file %s\n", file_name);
    return 1;
  }

  // Check if the HTML file was successfully opened
  if (html_file == NULL) {
    printf("Error opening HTML file %s\n", html_file_name);
    return 1;
  }

  // Write the HTML header to the output file
  fprintf(html_file, "<html>\n");
  fprintf(html_file, "<head>\n");
  fprintf(html_file, "<title>%s</title>\n", file_name);
  fprintf(html_file, "</head>\n");
  fprintf(html_file, "<body>\n");

  // Read each line from the input file
  char line[1024];
  while (fgets(line, 1024, input_file) != NULL) {
    // Check if the line starts with "#" for a heading
    if (line[0] == '#') {
      // Determine the heading level based on the number of "#" characters
      int heading_level = 1;
      while (line[heading_level] == '#' && heading_level < 7) {
        heading_level++;
      }

      // Write the heading to the HTML file
      fprintf(html_file, "<h%d>%s</h%d>\n", heading_level, line + heading_level + 1, heading_level);
    } else if (strstr(line, "**") != NULL) {
      // Write bold text to the HTML file
      char *bold_start = strstr(line, "**");
      char *bold_end = strstr(bold_start + 2, "**");
      *bold_start = '\0';
      *bold_end = '\0';
      fprintf(html_file,



#include <string.h>

const char *img_markdown = "![alt text](image.jpg)";
const char *img_html = "<img src=\"image.jpg\" alt=\"alt text\" />";

int is_image(const char *line) {
  return strncmp(line, "!", 1) == 0 && strstr(line, "(") && strstr(line, ")");
}

void markdown_to_html(char *html, const char *markdown) {
  if (!is_image(markdown)) {
    strcpy(html, markdown);
    return;
  }

  const char *alt_start = markdown + 2;
  const char *alt_end = strstr(alt_start, "]");
  const char *url_start = strstr(alt_end, "(") + 1;
  const char *url_end = strstr(url_start, ")");

  int alt_len = alt_end - alt_start;
  int url_len = url_end - url_start;

  sprintf(html, "<img src=\"%.*s\" alt=\"%.*s\" />",
          url_len, url_start, alt_len, alt_start);
}

int main() {
  char html[1024];

  markdown_to_html(html, img_markdown);
  printf("markdown: %s\nhtml: %s\n", img_markdown, html);

  return 0;
}
