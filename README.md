# btest
A simple unit test framework

---

So what is *btest*?

This `btest` library is a vastly reduced implementation of a
testing framework of a similar flavor. I think most users who use
googletest have a good idea of how it works, but I wanted to try
to implement something similar, to really see how it's done.

The internals of `btest` or googletest must:
* define tests during static initialization
* register tests into a central registry/controller
* provide runtime mechanisms to flag tests' results in registry
* use abstract classes and object factories to use instances

As you can probably tell, `btest` is more of a framework for
trying implementation ideas, and not a replacement for
googletest.

## Features

`btest` has a drastically reduced feature set compared to googletest.
However, it implements the most important testing features, most easily
reported in this table.

Feature                 | btest | gtest
----------------------- | ----- | -----
basic tests             | yes   | yes
test fixtures           | yes   | yes
DISABLED_ tests         | yes   | yes
Test filtering          | no    | yes
Global environment      | no    | yes
Run order randomization | no    | yes
Death tests             | no    | yes

If you'd like to play with this, go ahead.

## Building btest

`btest` is currently managed with cmake, and uses C++03 for its
implementation. From the toplevel directory, run these commands:

```
    $ cd path/to/btest
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j
```

---

Brent - Nov 2018
