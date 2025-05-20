// patterns.h

#ifndef PATTERNS_H
#define PATTERNS_H
#include <Wire.h>
#include "grove_two_rgb_led_matrix.h"

extern uint64_t sunnyPattern1[] = {
  0xffff23181811ff5c, 0x17ffff17ffffffff, 0x1621ffffff13ffff, 0x57ffff18ffff1318,
  0xff1323ff181857ff, 0xffff21ff21ff2121, 0x1725ff17ffffff17, 0xff5bff25ff2525ff
};

extern uint64_t sunnyPattern2[] = {
  0xffffff1b1b14ffff, 0x24ffffff16ff24ff, 0x1e181eff1eff1e1e, 0x1dffff1dff1d1d18,
  0x18ff18ff181818ff, 0xff1c1c141c1c1c1c, 0x14ff141414ff1414, 0x0f0f1515ffff0f0f
};

extern uint64_t sunnyPattern3[] = {
  0x1a24ff24531953ff, 0x17ff17ff17ff2466, 0x172020521fff1f1f, 0xff622020ff112020,
  0xff1aff5e1a1aff62, 0x531b1bff1b1b1b1b, 0x161f161653ff1616, 0xffff1b1bff1b1212
};

extern uint64_t sunnyPattern4[] = {
  0x20733973fe272b0e, 0x3f227b2b51177b2b, 0xfefe76fe0efe3bfe, 0x2c2c0e522b0e3b27, 
  0x737b240e0efe2b7b, 0xfefefefe767bfe73, 0x2c2e7673fe2c2c16, 0x1c0e2efefe29fe2c
};

extern uint64_t* sunnyPatterns[] = {sunnyPattern1, sunnyPattern2, sunnyPattern3, sunnyPattern4};
extern const int numSunnyPatterns = 4;

// Cloudy Patterns
extern uint64_t cloudyPattern1[] = {
  0xfefefefefefefefe, 0xfefefefefefefefe, 0xffffffffffffffff, 0xffffffffffffffff,
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t cloudyPattern2[] = {
  0xfefefefefefefefe, 0xfefefefefefefefe, 0xfefefefefefefefe, 0xfefefefefefefefe,
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t cloudyPattern3[] = {
  0xfefefefefefefefe, 0xfefefefefefefefe, 0xfefefefefefefefe, 0xfefefefefefefefe,
  0xfefefefefefefefe, 0xfefefefefefefefe, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t cloudyPattern4[] = {
  0x8c8c8c8c8c8c8c8c, 0x9b9b9b9b9b9b9b9b, 0xadadadadadadadad, 0xc1c1c1c1c1c1c1c1,
  0xcbcbcbcbcbcbcbcb, 0xb6b6b6b6b6b6b6b6, 0x9999999999999999, 0x8d8d8d8d8d8d8d8d
};

extern uint64_t* cloudyPatterns[] = {cloudyPattern1, cloudyPattern2, cloudyPattern3, cloudyPattern4};
extern const int numCloudyPatterns = 4;

// Windy Patterns
extern uint64_t windyPattern1[] = {
  0xffff1515767f87af, 0x1515ffffffffffff, 0xffffff82828995ab, 0xffffffffffff1523,
  0x8396ab15ffffffff, 0xffffffffffffff83, 0x152315ff23158ba2, 0xffffffffffff9393
};

extern uint64_t windyPattern2[] = {
  0xffffffff17178484, 0xff9c911616ffffff, 0xffffffffff7f8888, 0xff1f188793a1ffff,
  0x7f94a31fffffffff, 0xffffffffffffff1b, 0xffffff1b1b1b237a, 0x7a8c9cffffffffff
};

extern uint64_t windyPattern3[] = {
  0xfe231414a5988826, 0xc1a2891e1e968bb9, 0x8e8e7c1b1b22c1af, 0x2626a2937f191926,
  0x181828b2a1852626, 0x24a8957d2626269e, 0x28281717179b8ba9, 0xab19252525ae9a83
};

extern uint64_t* windyPatterns[] = {windyPattern1, windyPattern2, windyPattern3};
extern const int numWindyPatterns = 3;

extern uint64_t rainyPattern1[] = {
  0xfefffffffeffffff, 0xff68ffffff95fffe, 0xffbdffff65ffffff, 0xfffffffffffffffe, 
  0xfeff68ff95ffbdff, 0xfffffffffffeff68, 0xffffbdffbdfffeff, 0x95fffffeffffffbd
};

extern uint64_t rainyPattern2[] = {
  0xffff9efeff9effff, 0xff73ffffffffff73, 0xfeffffff73ffffff, 0xfffffffffffeff9e,
  0xffff73ffffff9eff, 0x73ffffff77feffff, 0xffff9efffffffffe, 0x9efffffffeff73ff 
};

extern uint64_t rainyPattern3[] = {
  0xccfeccbcfe8c758d, 0x8d73fe758dfefefe, 0xfeccfefe73bccc75, 0x75bc8efebcfebc9e,
  0xfecca375fe8d8d75, 0xfefe75cc77fe75fe, 0x75cc9ebca3bcfecc, 0xfefea3fe75cca3fe
};

extern uint64_t* rainyPatterns[] = {rainyPattern1, rainyPattern2, rainyPattern3};
extern const int numRainyPatterns = 3;

extern uint64_t snowyPattern1[] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t snowyPattern2[] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xfffffffefeffffff, 
  0xfffffefefefeffff, 0xfffffffefeffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t snowyPattern3[] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xfffffefffffeffff, 0xfffffffefeffffff, 
  0xfffefefefefefeff, 0xfffffffefeffffff, 0xfffffefffffeffff, 0xffffffffffffffff
};

extern uint64_t snowyPattern4[] = {
  0xfefffffefefffffe, 0xfffefffefefffeff, 0xfffffefefefeffff, 0xfffefffefefffeff, 
  0xfefefefefefefefe, 0xfffefffefefffeff, 0xfffffefefefeffff, 0xfffefffefefffeff
};

extern uint64_t snowyPattern5[] = {
  0xfefffffefeff19fe, 0x19fe27fefe27fe27, 0x2719fefefefe2719, 0xfffe27fefe27feff, 
  0xfefefefefefefefe, 0xfffe19fefe19feff, 0x27fffefefefeff27, 0x27fe19fefe27fe19
};

extern uint64_t snowyPattern6[] = {
  0xd710e2fefee219fe, 0x161e27fefe27fe27, 0x2719fefe22fe2719, 0xe2fefe25fe1efee2, 
  0xfefe25fe1bfefefe, 0x22fe19fefe1924e2, 0x27e2fe22fefee227, 0x27fe19fee227fe19
};

extern uint64_t* snowyPatterns[] = {snowyPattern1, snowyPattern2, snowyPattern3, snowyPattern4, snowyPattern5, snowyPattern6};
extern const int numSnowyPatterns = 6;

extern uint64_t hailPattern1[] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xfffffffefeffffff,
  0xfffffffefeffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t hailPattern2[] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xfffffffefeffffff, 0xfffffefffffeffff,
  0xfffffefffffeffff, 0xfffffffefeffffff, 0xffffffffffffffff, 0xffffffffffffffff
};

extern uint64_t hailPattern3[] = {
  0xffffffffffffffff, 0xfffffefefefeffff, 0xfffefffffffffeff, 0xfffefffefefffeff,
  0xfffefffefefffeff, 0xfffefffffffffeff, 0xfffffefefefeffff, 0xffffffffffffffff
};

extern uint64_t hailPattern4[] = {
  0xfffffefefefeffff, 0xfffefffffffffeff, 0xfefffffefefffffe, 0xfefffefefefefffe,
  0xfefffefefefefffe, 0xfefffffefefffffe, 0xfefffffefefffffe, 0xfffffefefefeffff
};

extern uint64_t hailPattern5[] = {
  0xfffefefefefefeff, 0xfefefefffffefefe, 0xfefefffefefffefe, 0xfefffefefefefffe,
  0xfefffefefefefffe, 0xfefefffefefffefe, 0xfefefefffffefefe, 0xfffefefefefefeff
};

extern uint64_t* hailPatterns[] = {hailPattern1, hailPattern2, hailPattern3, hailPattern4, hailPattern5};
extern const int numHailPatterns = 5;

extern uint64_t thunderPattern1[] = {
  0xfffffffffefeffff, 0xfffffffefeffffff, 0xfffffefeffffffff, 0xfffffffefeffffff, 
  0xfffffffffefeffff, 0xfffffffffffefeff, 0xfffffffefefeffff, 0xfffffefefeffffff
};

extern uint64_t thunderPattern2[] = {
  0xff4646ffffffffff, 0xffff4646ffffffff, 0xffffff4646ffffff, 0xffff4646ffffffff, 
  0xff4646ffffffffff, 0x4646ffffffffffff, 0xff4646ffffffffff, 0xffff4646ffffffff
};

extern uint64_t thunderPattern3[] = {
  0xff4646ffff7f7fff, 0xffff46467f7fffff, 0xffffff7f46ffffff, 0xffff46467f7fffff,
  0xff4646ffff7f7fff, 0x4646ffffffff7f7f, 0xff4646ffff7f7fff, 0xffff46467f7fffff
};

extern uint64_t thunderPattern4[] = {
  0xfe46468080fefe5c, 0x5c5c8080fefe5c5c, 0x5c8080fefe5c5c80, 0x8080fefe5c5c8080, 
  0x80fefe5c5c7f7ffe, 0xfefe5c5c8080fefe, 0xfe46468080fefe5a, 0x5c5c8080fefe5a5a
};

extern uint64_t* thunderPatterns[] = {thunderPattern1, thunderPattern2, thunderPattern3, thunderPattern4};
extern const int numThunderPatterns = 4; 

#endif
