# C-Generic-Type-Data-Structs
Generic Type Data Structures that are compatible with both c an c++

One of the most prominent limmitations of c is the lack of templates. Due to
this, there's no supported way to make general use functions that can take user
defined types or return user defined types. Macros however, can bypass this.

this library includes 3 popular data structures: dynamic arrays, linked lists
(singly or doubly linked), and hash tables which can hold any type of data -
pointer or non pointer, struct or standard type.

features include:
multidimensional support for dynamic arrays,
free_func parameters for destroying data structures holding your allocated data,
support for multiple types in the same data structure,
and for_each loops.
