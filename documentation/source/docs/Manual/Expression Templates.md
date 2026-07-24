While it is beyond the scope of this user-manual to expose the whole mechanics behind the code and most specifically [`TempLat`](https://github.com/cosmolattice/templat) — the lattice library that forms the backbone of CosmoLattice — we want to elaborate on the main concept behind the implementation of the fields and their related algebra: ***Expression Templates***.

Templates, which appear throughout CosmoLattice, are a `C++` mechanism which allows for *generic* programming: types become themselves dependent on other types and constants at compile time. 
Templates therefore allow for complex logic to be used within the compilation (this is often referred to as ''template metaprogramming", see ref. [@10_5555_3175809] for more information on this paradigm)[^turing]. 
On top of allowing for generic (type independent) functions, they can be used to move computations from being performed at run time to be performed at compile time. As we will see shortly, this extra level of abstraction can be used to implement *symbolic* computations in `C++`, which is what *expression templates* are.

[^turing]: Template metaprogramming is even Turing-complete, meaning that any software whatsoever could in principle be written only out of templates.

By *symbolic computations*, we mean some software which is capable of understanding expressions of the type $a + b - b$, simplifying them to $a$ and evaluating them once  $a$ and $b$ have been assigned a value. Instead of spending time on the various subtleties of expression templates, we will refer the reader to Ref. [@Falcou15] and simply explain as an example how to create such a software using template metaprogramming.

The challenge is to have an object that can, at the same time:
- represent the abstract expression $a + b - b$
- manipulate it at the abstract level 
- evaluate it. 

A key realisation is that this challenge can be addressed by using template metaprogramming, evaluated at compile time, to represent the abstract structure and use normal code, evaluated at run time, to take care of the evaluation. To achieve this, every member of an algebraic expression will be represented as a *type*.

Let us be specific. Let us first show how to use templates to implement abstract expressions. We first create two empty classes that represent numbers or fields $a$ and $b$:
```cpp
class A
{
};

class B
{
};
```
Next, we add classes to represent $+$ and $-$ operations. They can simply be written as follows
```cpp
template <class X, class Y> class Addition
{
};

template <class X, class Y> class Subtraction
{
};
```

Now, an object of the type `Addition<A, B>` can be used to represent $a+b$. `Addition<A,Subtraction<B,B>>` would then be interpreted as $a+b-b$. To actually be able to manipulate these types, we add some operators
```cpp
template <class X, class Y> Addition<X, Y> operator+(X, Y) // Not necessary to name the function's arguments in C++.
{
  return Addition<X, Y>();
}

template <class X, class Y> Subtraction<X, Y> operator-(X, Y)
{
  return Subtraction<X, Y>();
}
```
Now, on top of this generic definition, we can add more specific ones to deal with special cases. First, let us define a type to represent the number
$0$, as for instance $a+0=a$ and $a-a=0$.
```cpp
class ZeroType
{
};
```

We can now specify our operators to behave differently when in presence of `ZeroType`
```cpp
template <class X> X operator+(X, ZeroType)
{
  return X();
}

template <class X> X operator+(ZeroType, X)
{
  return X();
}
```
We can also use this to simplify expressions of the type ''$a-a$" to "$0$"
```cpp
template <class X> ZeroType operator-(X, X)
{
  return ZeroType();
}
```

Altogether, this set of definitions would simplify the expression `A() + B() - B()` to simply `A()`.

Note that at this point we have only taken care of the abstract expression; everything happens at compile-time and nothing happens at execution time. However, we have already achieved something interesting: we can now store and manipulate *formulas*, as you would do with a program such as Mathematica.

We can easily take care of the evaluation. For that we modify our classes `A` and `B` so that they each hold the field values on a 1-dimensional lattice and give them an `eval` method which, given the index `i` of a lattice site, returns the value stored there
```cpp
constexpr int N = 100; // Number of sites in the lattice 

class A
{
public:
  double eval(int i) { return data[i]; }

private:
  double data[N];
};

class B
{
public:
  double eval(int i) { return data[i]; }

private:
  double data[N];
};
```
`eval(i)` is parametrised by the lattice site and retrieves the data at lattice site $i$. We also modify the operators so that they hold a copy of the objects they operate on and provide them with an `eval` of their own, which simply forwards the site index to their operands
```cpp
template <class X, class Y> class Addition
{
public:
  Addition(X pX, Y pY) : x(pX), y(pY) {}

  double eval(int i) { return x.eval(i) + y.eval(i); }

private:
  X x;
  Y y;
};

template <class X, class Y> class Subtraction
{
public:
  Subtraction(X pX, Y pY) : x(pX), y(pY) {}

  double eval(int i) { return x.eval(i) - y.eval(i); }

private:
  X x;
  Y y;
};
```

Now we can evaluate our expression. For instance, we can write
```cpp
A a;
B b;

auto expr = a + b; // Only the expression is stored here; nothing is computed.

for (int i = 0; i < N; ++i)
  result[i] = expr.eval(i); // When we call expr.eval(i), the addition is performed at site i.
```
This way, we achieved to have symbolic expressions which can be stored, in a way which is completely unrelated to the evaluation.

Passing the site index `i` all the way down to the leaves of the expression is what unlocks the real power of the mechanism, and is the reason CosmoLattice uses `eval` everywhere:

- **A single expression sweeps the whole lattice.** The compiled expression object is built once and then evaluated at every site in a simple loop, with no per-site overhead and no rebuilding of the formula.
- **Operators can offset the index before forwarding it.** Because an operator is free to evaluate its operands at a *shifted* index, finite-difference stencils, gradients and other neighbour interactions become ordinary algebra: a derivative simply evaluates its argument at `i+1` and `i-1`. This is impossible with an argument-less getter.
- **Fields, constants and scalars mix freely.** A small helper, [`DoEval`](https://github.com/cosmolattice/templat/blob/main/include/TempLat/lattice/algebra/helpers/doeval.h), dispatches `eval` uniformly: leaves that carry data (like a field) use the index, while compile-time constants (such as `ZeroType`) ignore it and return their static value. The same helper also supports intermediate evaluation, which is what makes the matrix algebra used for gauge fields possible.

While being simple, this example illustrates the most important features of expression templates. For more complicated examples, and much better optimized, we invite the interested reader to explore the [`Field`](https://github.com/cosmolattice/templat/blob/main/include/TempLat/lattice/field/field.h) class of CosmoLattice, located in the [`include/TempLat/lattice/field/`](https://github.com/cosmolattice/templat/tree/main/include/TempLat/lattice/field) folder (the equivalent of $A$ and $B$), and the operators defined in the [`include/TempLat/lattice/algebra/operators/`](https://github.com/cosmolattice/templat/tree/main/include/TempLat/lattice/algebra/operators) folder. Fields have an `eval` method which takes as an argument the $d$-dimensional index of a lattice point, as in the toy example above. Operators are implemented in the same way as presented above. Evaluation happens only in the `operator=` of a field view class, which loops over the lattice and assigns `eval(i, j, k)` of the right-hand-side expression to each site.

In a sophisticated situation like this, this separation between abstract expression and evaluation also allows to affect the evaluation depending on the actual expression we want to evaluate, something which would not be possible otherwise. 
In particular, this is precisely this mechanism which allows CosmoLattice to completely hide under the hood the parallelization of the program. By having at hand the expression on an abstract level, we can analyze them to see whether or not they contain operators involving interaction between neighboring sites. If this is the case, we know this means that boundary needs to be synchronized before evaluation (see Section [Parallelization](Parallelization.md#subsec_para)) and we can trigger it automatically. In this way, it is completely hidden from the user.
