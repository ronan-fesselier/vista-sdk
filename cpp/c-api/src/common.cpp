#include "dnv/vista/sdk/c/common.h"

void dnv_vista_sdk_string_free(char* str)
{
    delete[] str;
}
