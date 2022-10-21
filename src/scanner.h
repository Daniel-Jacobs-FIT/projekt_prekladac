#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifndef SCANNER_H_
#define SCANNER_H_

const char *open_sign = "<?php";

bool sign_check(FILE *stream, const char *sign);

#endif
