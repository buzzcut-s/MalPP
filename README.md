# MalPP
Mal (Make A Lisp) interpreter written in C++.

# Overview
[mal](https://github.com/kanaka/mal) is a Clojure inspired Lisp interpreter created by [Joel Martin](https://github.com/kanaka). Please refer to the upstream documentation to learn more about the mal. You can also watch the playlist [Joel Martin: Make Your Own Lisp Interpreter in 10 Incremental Steps -  λC 2016](https://www.youtube.com/playlist?list=PL7Y9z9ogGndCkFWWv7hzBuBa7OimkAASo) to know more about the project and the process behind it, by the creator himself.

MalPP is a an implementation of mal written in C++. 

Here's how you would define a recursive function ```fib``` taking a single argument ```N``` for the fibonacci sequence in mal : 

```
(def! fib 
        (fn* (N) 
                (if (= N 0) 1 
                (if (= N 1) 1 
                (+ (fib (- N 1)) (fib (- N 2)))))
        )
)
```

and call it as :

```
( fib 10 )
```

The goal of writing MalPP was to see how much of a language I can implement - having very little (see : none) previous knowledge on how programming languages are implemented, and/or designed. (Please note that the language was designed by https://github.com/kanaka, which left only the effort of implementing the interpreter to me.)

MalPP was written with little to no long term goals, (thus, it being already rewritten thrice in its short development period) except to get it working and spec compliant as quickly as I can.

# Implemented
Currently, has support for the following : 
- REPL environment with complete line editing and command history support. Uses the [cpp-linenoise](https://github.com/yhirose/cpp-linenoise) library
- Built-in types : ```List```, ```Symbol```, ```Vector```, ```HashMap```, ```Integer```, ```Keyword```, ```String```, ```Function```
- Built-in arithmetic functions (```+```,```-```, ```*```, ```/```)
- Built-in print function (```prn```)
- Built-in list/vector operations (```list```, ```list?```, ```empty?```, ```count```, ```=```)
- Built-in comparision operations (```=```, ```<```, ```<=```, ```>```, ```>=```)
- Support for Lisp special atoms ```def!``` and ```let*```
- Ability to create new environments, using the ```let*``` special form
- Ability to modify existing environments (eg, global and local variables), using the ```def!``` special form
- User-defined functions with lexical scope, using the ```fn*``` special form
- Clojure-style variadic function parameters, using the ```&``` special form
- Flow control conditionls, using the ```if``` special form
- Comments, starting with ```,```
- A complete tokenizer, can also handle irregular input (uses ```std::regex```)
- Lexer and parser handwritten in pure C++

# Build
MalPP is built using CMake and requires at least C++17. clang-tidy and clang-format config files are provided.

MalPP requires no external dependencies.

```
git clone https://github.com/buzzcut-s/MalPP.git
cd MalPP/
mkdir build
cd build/
cmake ..
cmake --build .
```

To run :
``` 
./MalPP
```

# Usage  
MalPP provides a REPL environment to play around in. 
After executing, use the ```user>``` shell to provide input. 

Please read upstream documentation, or follow until this README's end, for mal's syntax and examples.

# (extra)
You can pass in a second string argument to the ```readline::read(input)``` call at ```main.cpp:19:36```[ (here) ](https://github.com/buzzcut-s/MalPP/blob/nude/src/main.cpp#L19) to change the shell prompt from ```user>``` to anything else that your heart desires :D

The source code for MalPP is heavily documented at parts which need it. [See](https://github.com/buzzcut-s/MalPP/blob/nude/src/Data.cpp#L8). Further, the source code is cleanly organized and structured, and the author (I) thinks that it can be read and understood by someone familiar with C++, with somewhat ease.

# What's missing 
In short, still a lot. 
- Tail call optimization 
- String functions
- Reading files 
- CLI function to run another .mal file
- Quoting
- Macros
- Exception handling (in language) using ```try*/catch*```
- Exception handling in the interpreter
- Self hosting :)

However, the only goals left for this project (for me) are adding tail call optimization and reading/executing .mal files. Further efforts will not be put into this project after (possibly - and much more realistically, even before) that.

# Implementation Details
These are the parts I do not particularly like.
- Everything is allocated on the heap using new (raw pointers) and never cleaned up.
    - The Second Rewrite (Aptly named The Great std::unique_ptr-isation) was aimed at improving this pathetic situation and converted all of the raw pointer usage with ```std::unique_ptr```s. It was met with huge success at the time and the kingdom flourished. This however took some effort on my part to maintain and was before the time of The Addition of the REPL Environment, which made everything much more involved and complicated. Thus there was the third rewrite - The not-so-great un-std::unique_ptr-isation. Everything is now back to being new'd. Too bad.
    - Owning to that, this new branch, is thus, called nude.
    - It should, nay, must, be noted that the 14 allocations done before our REPL loop for initializing repl_env are duly deleted before exiting. These unfortunate new'd objects are the only unlucky allocations that are deleted in the entire 2000+ line source code. +14 for me. (-14?)
    - This makes me feel bad and keeps me up at night. I'm sorry.
    - I shall take this bullet to also mention that the, hitherto unmentioned, ill-fated, First Rewrite shall not ever be spoken of. (It, now, resides in the switch-tokenzier branch.)
- The base abstract class (```mal::Data```) for every MAL datatypes exposes all the underlying derived types, and objects are generally not accessed as instances of the base class.
    - This, also, makes me feel bad, but is this is how the mal spec defines a lot of functions as; and so is = is.
- The CMakeList file is altogether and utterly devoid of any and all common decency.
   - I will learn some CMake soon.
- A lack of strong (see :  any) exception guarantees and nullptr checks. Currently just outputs to std::cerr at places where exceptions should be thrown.
- The lexer can be improved a lot. We're currently transforming a string to a vector of strings. With that, we get : large amounts of allocations for no reason, wasted memory space and loss of ergonomics.
    - Should use something like a enum class TokenType and struct Token which has a TokenType and some optional of a string_view. You know, like most lexers out there. (I've done my research now)
    - Can omit regex (pretty overkill); manually skip whitespaces etc and convert input stream to required tokens. 
- A lot of boilerplate written for defining every MAL type. 
    - This can probably be helped by using templates but I don't feel bad about this!

# Examples
There are only examples beyond this point, just a heads-up.
- Arithmetic evaluation

```
buzzcut > (+ 1 2)
3
buzzcut > (/ (- (+ 515 (* -87 311)) 296) 27)
994
```

- Evaluation with collection literals
```
buzzcut > [1 2 (+ 1 2)]
[1 2 3]
```

- Evaluation with hashmaps
```
buzzcut > {"a" (+ 7 8)}
{"a" 15}
```

- ```def!``` - Defining global environmets (variables, specifically)
```
buzzcut > (def! x 3)
3
buzzcut > x
3
buzzcut > (def! y (+ 1 7))
8
buzzcut > y
8
```

- ```let*``` - Evaluating user-defined environments
```
buzzcut > (let* (z 9) z)
9
buzzcut > (let* (z (+ 2 3)) (+ 1 z))
6
buzzcut > (let* (p (+ 2 3) q (+ 2 p)) (+ p q))
12 
buzzcut > (let* (a 5 b 6) [3 4 a [b 7] 8])
[3 4 5 [6 7] 8]
```

- ```def!``` - Defining new environments
```
buzzcut > (def! n (let* (z 7) z))
7
buzzcut > n
7
```

- List operations
```
buzzcut > (list)
()
buzzcut > (empty? (list))
true
buzzcut > (empty? (list 1))
false
buzzcut > (count (list 1 2 3))
3
buzzcut > (count nil)
0
```

- ```if``` - Control flow conditionals
```
buzzcut > (if (> (count (list 1 2 3)) 3) 89 78)
78
buzzcut > (if true (+ 1 7) (+ 1 8))
8
buzzcut > (if false (+ 1 7) (+ 1 8))
9
```

- Comparision operators
```
buzzcut > (= 2 1)
false
buzzcut > (= 2 (+ 1 1))
true
buzzcut > (= nil nil)
true
buzzcut > (= (list 1 2) (list 1 2))
true
```

- Calling user-defined functions
```
buzzcut > ( (fn* (f x) (f x)) (fn* (a) (+ 1 a)) 7)
8 
buzzcut > ( ( (fn* (a) (fn* (b) (+ a b))) 5) 7)
12
```

- Closures
```
buzzcut > (def! gen-plus5 (fn* () (fn* (b) (+ 5 b))))
buzzcut > (def! plus5 (gen-plus5))
buzzcut > (plus5 7)
12
buzzcut > (def! gen-plusX (fn* (x) (fn* (b) (+ x b))))
buzzcut > (def! plus7 (gen-plusX 7))
buzzcut > (plus7 8)
15
```

- ```do``` form with ```prn``` and ```def!```
```
buzzcut > (do (prn 101))
101
buzzcut > (do (prn 101) (prn 102) (+ 1 2))
102
7
3
buzzcut > (do (def! a 6) 7 (+ a 8))
14
buzzcut > a 
6
```

- Recursive sumdown function
```
buzzcut > (def! sumdown (fn* (N) (if (> N 0) (+ N (sumdown  (- N 1))) 0)))
buzzcut > (sumdown 6)
21
```

- Variadic functions
```
buzzcut > ( (fn* (& more) (count more)) 1 2 3)
3
buzzcut > ( (fn* (& more) (list? more)) 1 2 3)
true
buzzcut > ( (fn* (a & more) (count more)) 1 2 3)
2
buzzcut > ( (fn* (a & more) (list? more)) 1)
true
```

- ```not``` function
```
buzzcut > (not false)
true
buzzcut > (not 0)
false
```

- Printing, using the ```prn``` function
```
buzzcut > (prn "abc")
abc
nil
buzzcut > (prn (list 1 2 "abc" "\"") "def")
(1 2 "abc" "\"")
nil
````

- Keywords, starting with ```:```
```
buzzcut > (= (list :abc) (list :abc))
true
```

- Vector parameter lists
```
buzzcut > ( (fn* [f x] (f x)) (fn* [a] (+ 1 a)) 7)
8
```

- Nested Vector/List equality
```
buzzcut > (= [(list)] (list []))
true
buzzcut > (= [1 2 (list 3 4 [5 6])] (list 1 2 [3 4 (list 5 6)]))
true
```

This is everything that has been implemented. In other words, this is everything that is guaranteed to work :p


# License
MalPP is licensed under the MIT License. See [LICENSE](https://github.com/buzzcut-s/MalPP/blob/nude/LICENSE) for more details.

MalPP uses the cpp-linenoise library. The file ```linenoise.hpp``` is taken from the [cpp-linenoise](https://github.com/yhirose/cpp-linenoise) library, © 2015 Yuji Hirose, licensed under the BSD license.


