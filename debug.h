#ifndef DEBUG_H_
#define DEBUG_H_

#include <string.h>
#include <stdio.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef DEBUG
#define DEBUG_PRINT(msg)\
do{   \
    (void)fprintf(stdout, "%s\t Error in line: %d, in file: %s\n", msg, __LINE__, __FILENAME__);   \
}while(0)
#else
#define DEBUG_PRINT(msg)
#endif



#endif /* DEBUG_H_ */
