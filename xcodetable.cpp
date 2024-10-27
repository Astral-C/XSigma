#include <array>
#include "xsigma.hpp"

namespace XSigma {

/*
    Default Code Table, there was probably a better way to do this but like. Whatever

        TYPE      SIZE     MODE    TYPE     SIZE     MODE     INDEX
       ---------------------------------------------------------------
    1.  RUN         0        0     NOOP       0        0        0
    2.  ADD    0, [1,17]     0     NOOP       0        0      [1,18]
    3.  COPY   0, [4,18]     0     NOOP       0        0     [19,34]
    4.  COPY   0, [4,18]     1     NOOP       0        0     [35,50]
    5.  COPY   0, [4,18]     2     NOOP       0        0     [51,66]
    6.  COPY   0, [4,18]     3     NOOP       0        0     [67,82]
    7.  COPY   0, [4,18]     4     NOOP       0        0     [83,98]
    8.  COPY   0, [4,18]     5     NOOP       0        0     [99,114]
    9.  COPY   0, [4,18]     6     NOOP       0        0    [115,130]
   10.  COPY   0, [4,18]     7     NOOP       0        0    [131,146]
   11.  COPY   0, [4,18]     8     NOOP       0        0    [147,162]
   12.  ADD       [1,4]      0     COPY     [4,6]      0    [163,174]
   13.  ADD       [1,4]      0     COPY     [4,6]      1    [175,186]
   14.  ADD       [1,4]      0     COPY     [4,6]      2    [187,198]
   15.  ADD       [1,4]      0     COPY     [4,6]      3    [199,210]
   16.  ADD       [1,4]      0     COPY     [4,6]      4    [211,222]
   17.  ADD       [1,4]      0     COPY     [4,6]      5    [223,234]
   18.  ADD       [1,4]      0     COPY       4        6    [235,238]
   19.  ADD       [1,4]      0     COPY       4        7    [239,242]
   20.  ADD       [1,4]      0     COPY       4        8    [243,246]
   21.  COPY        4      [0,8]   ADD        1        0    [247,255]
*/

CodeTable DefaultCodeTable = {
    {
        {{CodeType::RUN, 0, 0},{CodeType::NOP, 0, 0}},

        {{CodeType::ADD, 0, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 1, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 2, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 3, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 4, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 5, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 6, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 7, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 8, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 9, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 10, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 11, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 12, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 13, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 14, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 15, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::ADD, 16, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::ADD, 17, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 0}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 0}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 1}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 1}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 2}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 2}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 5, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 7, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 9, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 11, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 13, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 15, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 3}, {CodeType::NOP, 0, 0}}, 
        {{CodeType::COPY, 17, 3}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 3}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 4}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 4}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 5}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 5}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 6}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 6}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 7}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 7}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 0, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 4, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 5, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 6, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 7, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 8, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 9, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 10, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 11, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 12, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 13, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 14, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 15, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 16, 8}, {CodeType::NOP, 0, 0}},
        {{CodeType::COPY, 17, 8}, {CodeType::NOP, 0, 0}}, {{CodeType::COPY, 18, 8}, {CodeType::NOP, 0, 0}},

        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 0}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 0}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 0}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 0}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 0}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 0}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 0}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 0}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 0}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 0}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 0}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 0}}, 
        
        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 1}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 1}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 1}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 1}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 1}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 1}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 1}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 1}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 1}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 1}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 1}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 1}}, 
        
        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 2}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 2}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 2}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 2}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 2}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 2}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 2}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 2}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 2}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 2}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 2}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 2}},
        
        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 3}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 3}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 3}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 3}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 3}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 3}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 3}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 3}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 3}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 3}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 3}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 3}},         
        
        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 4}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 4}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 4}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 4}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 4}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 4}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 4}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 4}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 4}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 4}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 4}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 4}}, 
        
        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 5}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 5, 5}}, {{CodeType::ADD, 1, 0}, {CodeType::COPY, 6, 5}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 5}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 5, 5}}, {{CodeType::ADD, 2, 0}, {CodeType::COPY, 6, 5}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 5}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 5, 5}}, {{CodeType::ADD, 3, 0}, {CodeType::COPY, 6, 5}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 5}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 5, 5}}, {{CodeType::ADD, 4, 0}, {CodeType::COPY, 6, 5}}, 

        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 6}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 6}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 6}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 6}},

        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 7}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 7}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 7}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 7}},

        {{CodeType::ADD, 1, 0}, {CodeType::COPY, 4, 8}},
        {{CodeType::ADD, 2, 0}, {CodeType::COPY, 4, 8}},
        {{CodeType::ADD, 3, 0}, {CodeType::COPY, 4, 8}},
        {{CodeType::ADD, 4, 0}, {CodeType::COPY, 4, 8}},

        {{CodeType::COPY, 4, 0}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 1}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 2}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 3}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 4}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 5}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 6}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 7}, {CodeType::ADD, 1, 0}},
        {{CodeType::COPY, 4, 8}, {CodeType::ADD, 1, 0}}
    },
};

}