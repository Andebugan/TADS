#ifndef _STATUS_CODES_H_
#define _STATUS_CODES_H_

// Коды ошибок и состояний
enum status_codes
{
    SUCCESS = 0,
    FAILURE,

    SCANF_BAD_DELIM,
    SCANF_BAD_UINT,
    SCANF_BAD_FLOAT,
    SCANF_EMPTY_STRING,
    SCANF_BAD_OPTION,

    INVALID_FILENAME,
    MAX_COUNTRY_TABLE_SIZE_REACHED,
};

#endif // _STATUS_CODES_H_
