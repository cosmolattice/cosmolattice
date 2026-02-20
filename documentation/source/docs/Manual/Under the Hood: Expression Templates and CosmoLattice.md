While it is beyond the scope of this user-manual to expose the whole mechanics behind the code and most specifically the `TempLat` library, we want to elaborate on the main concept behind the implementation of the fields and their related algebra, the one of ''expression templates". Templates, which appear throughout CosmoLattice, are a `C++` mechanism which allows for ''generic" programming; types become themselves ''variables", to be specified at compile times. Templates can be used as their own programming language (this is often referred to as ''template metaprogramming", see
ref. [@10_5555_3175809] for more information on this paradigm)\footnote{Which is even Turing-complete, meaning that any software whatsoever could in principle be written only out of templates.}; atop of allowing for generic (type independent) functions, they can be used to move computations from being performed at run time to be performed at compile time. As we will see shortly, this extra level of abstraction can be used to implement ''symbolic" computations in `C++`, which is what ''expression templates" are.

By ''symbolic computations", we mean some software which is capable as understanding expressions of the type ''$a + b - b$", simplifying them to ''$a$" and evaluate them once  ''$a$" and ''$b$" have been assigned a value. Instead of spending time on the various subtleties on expression templates, we will refer the reader to Ref. [@Falcou15] and simply explain as an example how to create such a software using template metaprogramming. The challenge is to have an object which can at the same time represent the abstract expression ''$ a + b - b$", being able to manipulate it at the abstract level and then also being able to evaluate it. A key realisation is that this challenge can be addressed by using template metaprogramming, evaluated at compile time, to represent the abstract structure and use normal code, evaluated at run time, to take care of the evaluation. To achieve this, every member of an algebraic expression will be represented as a ''type".

Let us be specific. Let us first show how one use templates to implement abstract expressions. We first create two empty `Number` classes,
```cpp
class A{};
class B{};
```
and a class to represent the ''addition" operation. It can simply be implemented as follow
```cpp
template<class X, class Y>
class Addition{};
template<class X, class Y>
class Subtraction{};
```

Now, an object of the type `Addition<A, B>` can be used to represent ''$a+b$". `Addition<A,Subtraction<B,B>>` would then be interpreted as ''$a+b-b$". To actually be able to to manipulate these types, we add some operators
```cpp
template<class X, class Y>
Addition<X,Y> operator+(X, Y) // Not necessary to give names to function's arguments in C++.
{
return Addition<X,Y>();
}

template<class X, class Y>
Subtraction<X,Y> operator-(X, Y)
{
return Subtraction<X,Y>();
}
```
Now, atop of this generic definition, we can add more specific ones to deal with special case. First, let us define a type to represent the number
$0$, which will be the special case we will be dealing with, as for instance $a+0=a$ and $a-a=0$.
```cpp
class ZeroType{};
```

We can now specify our operators to behave differently when in presence of `ZeroType`
```cpp
template<class X>
X operator+(X, ZeroType)
{
return X();
}
template<class X>
X operator+(ZeroType, X)
{
return X();
}
```
We can also use this to simplify expression of the type ''$a-a$" to "$0$"
```cpp
template<class X>
ZeroType operator-(X, X)
{
return ZeroType();
};
```

Altogether, this set of definitions would simplify the expression `A()+B()-B()` to simply `A()`.

Note that at this point we have only taken care of the abstract expression; everything happens at compile-time and nothing happens at execution time. Note however that we have already achieved something interesting; we can now store and manipulate ''formulas", as you would do with a program such as Mathematica.

We can easily take care of the evaluation. For that we modify our class  `A` and `B` so that they represent some given number and give them a ''getter" to retrieve this number
```cpp
class A{
double get()
{
return 9.0;
}
};
class B{
double get()
{
return 0.5;
}
};
```
We also modify the operators so that they hold a copy of the object they operate on and provide them with a getter responsible for the evaluation.
```cpp
template<class X, class Y>
class Addition{
public:
Addition(X pX, Y pY):
x(pX),
y(pY)
{
}

double get()
{
return a.get() + b.get();
}

private:
X x;
Y y;

};
template<class X, class Y>
class Subtraction{
public:
Subtraction(X pX, Y pY):
x(pX),
y(pY)
{
}

double get()
{
return a.get() - b.get();
}

private:
X x;
Y y;

};
```

Now we can evaluate our expression. For instance, we can write
```cpp
A a;
A b;

auto expr = a + b; // Only store the expression, nothing is computed here.

std::cout << expr.get() << std::endl;  // When we call expr.get(), the addition is done.
```
This way, we achieved to have symbolic expression which can be stored, in a way which is completely unrelated to the evaluation.

While being simple, this example illustrate the most important features of expression templates. For a more complicated example, we invite the interested reader to explore the `Field` class of CosmoLattice, located in the `src/include/TempLat/lattice/field/` (the equivalent of $A$ and $B$) and the operators defined in the `src/include/TempLat/lattice/algebra/operators/` folder. Fields have a getter which takes as an argument the index of a lattice point. Operators are implemented in the same way as presented above. Evaluation happens only in the `operator=` of the `Field` class.

In a sophisticated situation like this, this separation between abstracts expression and evaluation also allow to affect the evaluation depending on the actual expression we want to evaluate, something which would not be possible otherwise. In particular, this is precisely this mechanism which allows CosmoLattice to completely hide under the hood the parallelization of the program. By having at hand the expression on an abstract level, we can analyze them to see whether or not they contain operator involving interaction between neighboring sites. If this is the case, we know this means that boundary needs to be synchronized before evaluation (see Section [*Parallelization*][subsec_para]) and we can trigger it automatically. In this way, it is completely hidden from the user.

