#ifndef _STATUS_CODES_H_
#define _STATUS_CODES_H_

// Коды возврата
enum status_codes
{
    SUCCESS = 0,
    FAILURE,

    BAD_FILE,

    BAD_FSCANF,
    BAD_FPRINTF,
    ZERO_MAT,

    BAD_MAT_DIMS,
    BAD_ELEMS,
    BAD_FNAME
};

#endif // _STATUS_CODES_H_
