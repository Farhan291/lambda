# λ

a simple untyped lambda calculus interpreter.

# Examples

## Boolean

```
λ> true=(\x.\y.x);
λ> false=(\x.\y.y);
λ> true a b;
a
λ> false a b;
b
```

## Church numerals

```
λ> zero=\f.\x.x;
λ> succ=\n.\f.\x.f(n f x);
λ> one=succ(zero); one;
\f.\x.f x                   # one
λ> two=succ(one); two;
\f.\x.f (f x )              # two
λ> three=succ(two); three;
\f.\x.f (f (f x ))          # three
λ> add = \m.\n.\f.\x.m f (n f x);
λ> add one two;
\f.\x.f (f (f x ))          # three
λ> add two three;
\f.\x.f (f (f (f (f x ))))  # five
```

# References

- https://arxiv.org/pdf/1503.09060
- https://0xmukesh.github.io/blog/lambda-calculus.html
- https://blueberrywren.dev/blog/debruijn-explanation/
- https://github.com/0xmukesh/lambda
- https://github.com/cxinu/lambda
