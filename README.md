Unsynchronized Object Pool
==========================

This single-header library contains an implmementation of an
unsynchronized object pool. The pool allows construction of
objects and gives access to the objects via smart pointers that
track the lifetime of the object. When the last smart pointer
associated with an object is destroyed, the object is released
back to the pool.

Copyright 2020 Florin Iucha <florin@signbit.net>

Licensed under [The "BSD 3-clause license"]
