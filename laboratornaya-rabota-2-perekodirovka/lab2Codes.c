#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void reverseMas(unsigned char* res, int size) {
    unsigned char* dopRes[4];
    for (int i = 0; i < size; i++) dopRes[i] = res[i];
    for (int i = 0; i < size; i++) res[i] = dopRes[size - i - 1];
}

void mergeMas(unsigned char* res, unsigned char* first, unsigned char* second, int size) {
    for (int i = 0; i < 2; i++)
    {
        res[i] = first[i];
    }
    for (int i = 0; i < 2; i++)
    {
        res[2 + i] = second[i];
    }
    return res;
}
unsigned long code32(const unsigned char* buf) {
    return (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
}

unsigned long code16(const unsigned char* buffer, int numB) {
    return (buffer[0] << 8) + buffer[1];
}

unsigned long code8(const unsigned char* buf, int numB) {
    if (numB == 1) return buf[0];
    if (numB == 2) return (buf[0] - (1 << 6) - (1 << 7)) * (0x40) + (buf[1] - (1 << 7));
    if (numB == 3) return (buf[0] - (1 << 6) - (1 << 7) - (1 << 5)) * (0x40 * 0x40) + (buf[1] - (1 << 7)) * (0x40) + (buf[2] - (1 << 7));
    if (numB == 4) return (buf[0] - (1 << 6) - (1 << 7) - (1 << 5) - (1 << 4)) * (0x40 * 0x40 * 0x40)
        + (buf[1] - (1 << 7)) * (0x40 * 0x40) + (buf[2] - (1 << 7)) * (0x40) + (buf[3] - (1 << 7));
}

void convert32(unsigned char* buf, const unsigned long cp) {
    buf[0] = cp >> 24;
    buf[1] = (cp << 8) >> 24;
    buf[2] = (cp << 16) >> 24;
    buf[3] = (cp << 24) >> 24;
}

int convert16(unsigned char* buf, unsigned long cp) {
    if (cp <= 0xFFFF) {
        buf[0] = cp >> 8;
        buf[1] = cp;
        return 2;
    }
    else {
        cp = cp - 0x10000;
        buf[0] = ((cp >> 10) + 0xD800) >> 8;
        buf[1] = ((cp >> 10) + 0xD800) >> 24;
        buf[2] = (((cp << 22) >> 22) + 0xDC00 + 0xD800) >> 8;
        buf[3] = (((cp << 22) >> 22) + 0xDC00 + 0xD800) >> 24;
        return 4;
    }
    return 0;
}

int convert8(unsigned char* buf, unsigned long cp) {
    if (cp <= 0x007F) {
        buf[0] = cp;
        return 1;
    }
    if (cp <= 0x07FF) {
        buf[0] = 0xC0 + (cp >> 6);
        buf[1] = 0x80 + ((cp << 24) >> 24);
        return 2;
    }
    if (cp <= 0xFFFF) {
        buf[0] = 0xE * (1 << 4) + (cp >> 12);
        buf[1] = 0x80 + ((cp << 16) >> 24);
        buf[2] = 0x80 + ((cp << 24) >> 24);
        return 3;
    }
    else {
        buf[0] = 30 * (1 << 3) + (cp >> 18);
        buf[1] = 0x80 + ((cp << 8) >> 24);
        buf[2] = 0x80 + ((cp << 16) >> 24);
        buf[3] = 0x80 + ((cp << 24) >> 24);
        return 4;
    }
    return -1;
}

void rewrite(FILE* in, FILE* out) {
    unsigned char byte;
    while (fread(&byte, 1, 1, in) == 1) {
        fwrite(&byte, 1, 1, out);
    }
}

void Utf16SwapE(FILE* in, FILE* out) {
    unsigned char buf[2];
    reverseMas(buf, 2);
    while (fread(&buf, 1, 2, in) == 2) {
        reverseMas(buf, 2);
        fwrite(buf, 1, 2, out);
    }
}

void Utf32SwapE(FILE* in, FILE* out) {
    unsigned char buf[4];
    while (fread(&buf, 1, 4, in) == 4) {
        reverseMas(buf, 4);
        fwrite(buf, 1, 4, out);
    }
}

void Utf32To16(FILE* in, FILE* out, int inIsLe, int outIsLe) {
    unsigned char buf[4];
    unsigned char converted[4];
    unsigned int ccp;
    unsigned int cp;
    while (fread(&buf, 1, 4, in) == 4) {
        if (inIsLe) reverseMas(buf, 4);
        cp = code32(buf);
        ccp = convert16(converted, code32(buf));
        if (outIsLe) reverseMas(converted, cp);
        for (int i = 0; i < ccp; i++) fwrite(converted[i], 1, 1, out);

    }
}

void Utf32To8(FILE* in, FILE* out, int isLe) {
    unsigned char buf[4];
    unsigned char converted[4];
    int countB;
    while (fread(&buf, 1, 4, in) == 4) {
        if (isLe) reverseMas(buf, 4);
        countB = convert8(converted, code32(buf));
        fwrite(converted, 1, countB, out);
    }
}

void Utf16To32(FILE* in, FILE* out, int inIsLe, int outIsLe) {
    unsigned char buf[4];
    unsigned char buf1[4];
    unsigned char buf2[4];
    unsigned char converted[4];
    unsigned long cp;
    while (fread(&buf1, 1, 2, in) == 2) {
        if (inIsLe) reverseMas(buf1, 2);
        if ((cp = code16(buf1, 2)) <= 0xD7FF) {
            convert32(converted, cp);
            if (outIsLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
            continue;
        }
        if (fread(&buf2, 1, 2, in) != 2) break;
        mergeMas(buf, buf1, buf2, 2);
        if ((cp = code16(buf, 4)) <= 0x10FFFF) {
            convert32(converted, cp);
            if (outIsLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
        }
    }
}

void Utf16To8(FILE* in, FILE* out, int isLe) {
    unsigned char buf1[4];
    unsigned char buf2[4];
    unsigned char buf[4];
    unsigned char converted[4];
    unsigned long cp;
    unsigned long countB;
    while (fread(buf1, 1, 2, in) == 2) {
        if (isLe) reverseMas(buf1, 2);
        if ((cp = code16(buf1, 2)) <= 0xD7FF) {
            countB = convert8(converted, cp);
            fwrite(converted, 1, countB, out);
            continue;
        }
        else if ((cp = fread(&buf2, 1, 2, in)) != 2) {
            break;
        }
        mergeMas(buf, buf1, buf2, 2);
        if ((cp = code16(buf, 4)) <= 0x10FFFF) {
            countB = convert8(converted, cp);
            fwrite(converted, 1, countB, out);
        }
    }
}
void Utf8To16(FILE* in, FILE* out, int isLe) {
    unsigned char buf[4];
    unsigned char converted[4];
    int numB;
    unsigned int cp;
    while (fread(&buf[0], 1, 1, in) != 0) {
        if (buf[0] <= 0x007F) {
            cp = code8(buf, 1);
            numB = convert16(converted, cp);
            if (isLe) reverseMas(converted, numB);
            fwrite(converted, 1, 2, out);
        }
        else if ((fread(&buf[1], 1, 1, in)) == 0) break;
        else if ((cp = code8(buf, 2)) <= 0x07FF) {
            numB = convert16(converted, cp);
            if (isLe) reverseMas(converted, numB);
            fwrite(converted, 1, 2, out);
        }
        else if ((fread(&buf[2], 1, 1, in)) == 0) break;
        else if ((cp = code8(buf, 3)) <= 0xFFFF) {
            numB = convert16(converted, cp);
            if (isLe) reverseMas(converted, numB);
            if (numB == 2) fwrite(converted, 1, 2, out);
            else if (numB >= 2) fwrite(converted, 1, 4, out);
        }
        else if ((fread(&buf[3], 1, 1, in)) == 0) break;
        else if ((cp = code8(buf, 4)) <= 0x10FFFF) {
            numB = convert16(converted, cp);
            if (isLe) reverseMas(converted, numB);
            if (numB == 2) fwrite(converted, 1, 2, out);
            else if (numB >= 2) fwrite(converted, 1, 4, out);
        }
    }
}

void Utf8To32(FILE* in, FILE* out, int isLe) {
    unsigned char buf[4];
    unsigned char converted[4];
    while (fread(&buf, 1, 1, in) == 1) {
        unsigned int cp;
        if (buf[0] <= 0x007F) {
            cp = code8(buf, 1);
            convert32(converted, code8(buf, 1));
            if (isLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
        }
        else if (fread(&buf[1], 1, 1, in) != 1) break;
        else if ((cp = code8(buf, 2)) <= 0x07FF) {
            convert32(converted, cp);
            if (isLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
        }
        else if (fread(&buf[2], 1, 1, in) != 1) break;
        else if ((cp = code8(buf, 3)) <= 0xFFFF) {
            convert32(converted, cp);
            if (isLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
        }
        else if (fread(&buf[3], 1, 1, in) != 1) break;
        else if ((cp = code8(buf, 4)) <= 0x10FFFF) {
            convert32(converted, cp);
            if (isLe) reverseMas(converted, 4);
            fwrite(converted, 1, 4, out);
        }
    }
}

int checkUtf(FILE* input) {
    unsigned int byte;
    byte = fgetc(input);
    if (byte == 0xFF) {
        fgetc(input);
        byte = fgetc(input);
        if (byte == 0x00) {
            fgetc(input);
            return 5;
        }
        else {
            fseek(input, -1, SEEK_CUR);
            return 2;
        }
    }
    if (byte == 0xEF) {
        fgetc(input);
        fgetc(input);
        return 1;
    }
    if (byte == 0xFE) {
        fgetc(input);
        return 3;
    }
    if (byte == 0x00) {
        fgetc(input);
        fgetc(input);
        fgetc(input);
        return 4;
    }
    fseek(input, -1, SEEK_CUR);
    return 0;
}

void startRecode(FILE* in, FILE* out, int typeUtfIn, int typeUtfOut) {
    unsigned const char Bom8[3] = { 0xEF, 0xBB, 0xBF };
    unsigned const char Be16[2] = { 0xFE, 0xFF };
    unsigned const char Le16[2] = { 0xFF, 0xFE };
    unsigned const char Be32[4] = { 0x00, 0x00, 0xFE, 0xFF };
    unsigned const char Le32[4] = { 0xFF, 0xFE, 0x00, 0x00 };
    if (typeUtfOut == 1) fwrite(Bom8, 3, 1, out);
    else if (typeUtfOut == 2) fwrite(Le16, 2, 1, out);
    else if (typeUtfOut == 3) fwrite(Be16, 2, 1, out);
    else if (typeUtfOut == 4) fwrite(Le32, 4, 1, out);
    else if (typeUtfOut == 5) fwrite(Be32, 4, 1, out);
    if (typeUtfIn <= 1)
    {
        if (typeUtfOut <= 1) rewrite(in, out);
        else if (typeUtfOut == 2) Utf8To16(in, out, 1);
        else if (typeUtfOut == 3) Utf8To16(in, out, 0);
        else if (typeUtfOut == 4) Utf8To32(in, out, 1);
        else if (typeUtfOut == 5) Utf8To32(in, out, 0);
    }
    else if (typeUtfIn == 2) {
        if (typeUtfOut <= 1) Utf16To8(in, out, 1);
        else if (typeUtfOut == 2) rewrite(in, out);
        else if (typeUtfOut == 3) Utf16SwapE(in, out);
        else if (typeUtfOut == 4) Utf16To32(in, out, 1, 1);
        else if (typeUtfOut == 5) Utf16To32(in, out, 1, 0);
    }
    else if (typeUtfIn == 3) {
        if (typeUtfOut <= 1) Utf16To8(in, out, 0);
        else if (typeUtfOut == 2) Utf16SwapE(in, out);
        else if (typeUtfOut == 3) rewrite(in, out);
        else if (typeUtfOut == 4) Utf16To32(in, out, 0, 1);
        else if (typeUtfOut == 5) Utf16To32(in, out, 0, 0);
    }
    else if (typeUtfIn == 4) {
        if (typeUtfOut == 1) Utf32To8(in, out, 1);
        else if (typeUtfOut == 2) Utf32To16(in, out, 1, 1);
        else if (typeUtfOut == 3) Utf32To16(in, out, 1, 0);
        else if (typeUtfOut == 4) rewrite(in, out);
        else if (typeUtfOut == 5) Utf32SwapE(in, out);
    }
    else if (typeUtfIn == 5) {
        if (typeUtfOut <= 1) Utf32To8(in, out, 0);
        else if (typeUtfOut == 2) Utf32To16(in, out, 0, 1);
        else if (typeUtfOut == 3) Utf32To16(in, out, 0, 0);
        else if (typeUtfOut == 4) Utf32SwapE(in, out);
        else if (typeUtfOut == 5) rewrite(in, out);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Неправильное число аргументов");
        return 1;
    }
    FILE* input = fopen(argv[1], "rb");
    if (input == NULL) {
        printf("Cant open file \n");
        return 1;
    }
    FILE* output = fopen(argv[2], "wb");
    if (output == NULL) {
        printf("Not enough memory to create a file for output \n");
        fclose(input);  
        return 1;
    }
    startRecode(input, output, checkUtf(input), atoi(argv[3]));
    fclose(input);
    fclose(output);
}



