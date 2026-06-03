#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

file_buf *file_buf_load(const char *path)
{
    if (!path) return NULL;

    FILE *file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length < 0) {
        fclose(file);
        return NULL;
    }

    file_buf *buf = malloc(sizeof(file_buf));
    if (!buf) {
        fclose(file);
        return NULL;
    }

    buf->size = (size_t)length;
    buf->data = malloc(buf->size + 1);
    if (!buf->data) {
        fclose(file);
        free(buf);
        return NULL;
    }

    size_t read_count = fread(buf->data, 1, buf->size, file);
    buf->data[read_count] = '\0';

    fclose(file);
    return buf;
}

void file_buf_destroy(file_buf *buf)
{
    if (!buf) return;

    free(buf->data);
    free(buf);
}

void pitch_template(const char *assembled_code, const char *template_file, const char *output_file)
{
    if (!assembled_code || !template_file || !output_file) return;

    FILE *in = fopen(template_file, "r");
    if (!in) {
        perror("Error opening input");
        return;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Error opening output");
        fclose(in);
        return;
    }

    char line[1024];
    const char *tape_tag = "$(code)";
    size_t tape_len = strlen(tape_tag) + 1;
    while (fgets(line, sizeof(line), in))
    {
        if (line[0] == '#') continue;

        char *tag_pos = strstr(line, tape_tag);
        if (tag_pos)
        {
            fwrite(line, 1, tag_pos - line, out);
            fputs(assembled_code, out);
            fputs(tag_pos + tape_len, out);
        }
        else
        {
            fputs(line, out);
        }
    }

    fclose(out);
    fclose(in);
}
