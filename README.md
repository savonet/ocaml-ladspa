ocaml-ladspa
============

This package contains an OCaml interface for LADSPA audio plugins library.

Please read the COPYING file before using this software.

Prerequisites
-------------

- ocaml >= 3.0.6
- LADSPA sdk >= 1.1
- findlib >= 0.8.1

Compilation
-----------

```sh
make all
```

This should build both the native and the byte-code version of the extension
library.

Installation
------------

```sh
make install
```

This should install the library file (using ocamlfind) in the appropriate place.

Author
------

This author of this software may be contacted by electronic mail at the
following address: savonet-users@lists.sourceforge.net.
