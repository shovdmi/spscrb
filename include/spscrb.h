/* Single Producer Single Consumer Ring Buffer (SPSCRB)
* 
* Copyright (C) 2023 Dmitry Shovkun 
*/

#ifndef SPSCRB_H
#define SPSCRB_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SPSCRB_SIZE_TYPE
#define SPSCRB_SIZE_TYPE size_t
#endif

#ifndef SPSCRB_DIFFERENCE_TYPE
#define SPSCRB_DIFFERENCE_TYPE ptrdiff_t
#endif

#ifndef SPSCRB_VALUE_TYPE
#define SPSCRB_VALUE_TYPE char
#endif

typedef struct {
    SPSCRB_VALUE_TYPE* const buf;
    const SPSCRB_DIFFERENCE_TYPE capacity;
    SPSCRB_SIZE_TYPE pops;
    SPSCRB_SIZE_TYPE pushes;
//    SPSCRB_SIZE_TYPE peeks;
} spscrb_t;


/* static_assert(rb->capacity != SIZE_MAX)
 if "pops"==0 and "pushes" overflowed (now is equal to 0) we expect: size()==SIZE_MAX+1, 
 but we have pushes == pops == 0 == (pushes-pops) i.e size()==0

 This is the reason why we use "ptrdiff_t capacity, ptrdiff_t size()", for capacity to be two times lower than max values of pushes and pops, 
 and be able to represent corner cases such as when pushes has overlaped and "pushes == 0" and "size() == capacity"
 */


/*
* if pushes < pops in unsigned arithmetics, let's use r' such as "pops = ((SIZE_MAX + 1) - r') == 0 - r' == -r' "
* pushes - pops == pushes - (-r') == pushes + r' ==  pushes + r' - (SIZE_MAX + 1)
*
*   pops          0    pushes
*     |           |       |
*     |<----r'--->|       |
* ..|253|254|255| 0 | 1 | 2 | 3 | 4 | 5 |..
* ..| -3| -2| -1| 0 | 1 | 2 | 3 | 4 | 5 |..
*             |   |
*             |  (SIZE_MAX + 1)
*       SIZE_MAX
*/


static inline SPSCRB_DIFFERENCE_TYPE spscrb_capacity(const spscrb_t *rb) {
    return rb->capacity; 
}


static inline bool spscrb_empty(const spscrb_t *rb) {
    return rb->pushes == rb->pops;
}


static inline bool spscrb_full(const spscrb_t* rb) {
    return (SPSCRB_SIZE_TYPE)(rb->pushes - rb->pops) >= rb->capacity;
}


static inline SPSCRB_DIFFERENCE_TYPE spscrb_size(const spscrb_t* rb) {
    return (SPSCRB_DIFFERENCE_TYPE)(rb->pushes - rb->pops);
}


//static inline SPSCRB_SIZE_TYPE spscrb_peek_avail(const spscrb_t* rb) {
//    return (SPSCRB_SIZE_TYPE)(rb->pushes - rb->peeks);
//}


static inline void spscrb_push_back(spscrb_t *rb, SPSCRB_VALUE_TYPE c) {
    assert(spscrb_size(rb) < spscrb_capacity(rb));
    rb->buf[rb->pushes % rb->capacity] = c;
    rb->pushes++;  
}


static inline SPSCRB_VALUE_TYPE spscrb_top(const spscrb_t *rb) {
    return rb->buf[rb->pops % rb->capacity];
}


//static inline SPSCRB_VALUE_TYPE spscrb_peek_top(const spscrb_t *rb) {
//    return rb->buf[rb->peeks % rb->capacity];
//}


static inline SPSCRB_VALUE_TYPE spscrb_at(const spscrb_t *rb, SPSCRB_DIFFERENCE_TYPE offset) {
    assert(offset < rb->capacity);
    //assert(offset < rb->pushes - rb->pops);
    assert(offset < spscrb_size(rb));
    return rb->buf[(rb->pops + offset) % rb->capacity];
}


static inline void spscrb_pop_front(spscrb_t *rb) {
    assert(spscrb_size(rb) > 0);
    rb->pops++;
}


//static inline void spscrb_peek_front(spscrb_t *rb) {
//    assert(spscrb_peek_avail(rb) > 0);
//    rb->peeks++;
//}


//static inline void spscrb_peek_discard(spscrb_t *rb) {
//    rb->peeks = rb->pops;
//}


//static inline void spscrb_peek_drop(spscrb_t *rb) {
//    rb->pops = rb->peeks;
//}


#ifdef __cplusplus
}
#endif

#endif //SPSCRB_H
