# Static tester

This tester will just try to instantiate your containers and all their methods and check their type and return values.

It doesn't produce any executable because the goal is just to make the code compile.

If you pass this tester, you shouldn't have any compilation error on the monkey_tester

## Usage

Set the path of your headers directory in the Makefile and launch `make` or `make <container>`

## PATCH

- Fix assert on vector insert which was waiting for a void return unstead of an iterator.  
- Fix assert on set and multiset iterators operator *() which was waiting for a T& return unstead of a const T&.  