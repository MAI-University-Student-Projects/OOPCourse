# Templates  
 - [Intoduction](#intro)
 - [Alias templates and general guidelines](#aliaG)
 - [Type Traits](#tptr)  
 - [Variadic templates](#vartm)  
 - [SFINAE](#sfinae)

## <a name="Intro"></a> Introduction
Generic programming is a method to implement algorithms and data structures in most sensible way. Algorithms are written in terms of types to-be-specified-LATER. Generic programming helps to reduce redunduncy and effort (reduce amount of code), while increases reusability and flexibility.  
Templates mean a pattern for the compiler; they can be instantiated with different types (known at compile-time) and initiated by `template` keyword  
 - function-templates  (not template functions)  
 - class-templates  
 - variable-templates (since C++14)  

The different kinds of parameters: concrete type, non-type parameters (ex. size_t sizes for arrays, not floats or c-strings), template-template  
```cpp
template<typename T, size_t N> //<type parameter, non-type>
constexpr auto Size(const T& [N]) { //arguments is array of type T and size N
    return N;
}
```  
Instatiation is when the compiler replaces a template with the concrete arguments. There are NO explicit convertions when istatiating templates. Compiler doesn't instantiate, if template instantiation isn't used later. Specialization for a concrete type implementation for template can be provided as well.  
Class templates cannot derive its arguments automatically, each template argument must be specified explicitly (unless type deduction in C++17). Classes can have method templates (even for class non-template or be different from a class template). Methods and attributes of the base class are not automatically available for the derived class; it could be made known in the scope as `using Base<T>::func`, or called directly   
## <a name="aliaG"></a> Alias templates and guidlines  
```cpp
template<typename T, size_t N>
using CharArray = std::array<char, N>;
...
CharArray<20> arr;
```  
This allows a partial specialization. Alias templates themselves cannot be later specialized. It is very convenient in case of using structure of certain type only (or mostly) in code. Frequent usage of templates causes code bloat (in compile-time) and heavy binaries  
Guide example to control this:
```cpp
//C-style
bool Send(const char* data, size_t size);
void Read(char* data, size_t size)
...
char buff[1024];
char buff2[2048];
Send(buff, sizeof(buff2)); // buffer overflow

//std::array
template<size_t N> // danger of code-bloat and heavy binary with different sized buffers
bool Send(const std::array<char, N>& data);
void Read(std::array<char, N>& data);
...
std::array<char, 1024> buff;
std::array<char, 2048> buff2;
Send(buff2);

//c++20
std::span
bool Send(const std::span<char>& data);
void Read(std::span<char>& data);

std::array<char, 1024> buff;
char buff2[2048];
Send(buff);
Send(buff2);
```  
`span` provides little overhead compared to other solutions, and it is range-based forready (`range` semantics appliable, C++20). `span` stores a pointer and a length  
Guides:
 1. Class templates  
    1. Move code, that stays the same for all instantiations, into a base class  
    2. Storing an additional type/value (like span or alias) can be better than passing as template parameter  (in exchange of RAM)  
 2. Function templates  
    3. if possible, Use only as API to delegate actual work to non-template  

## <a name="tptr"></a> Type Traits  
Types are known at a compile-time, so we can do checks or modifications in compile-time using `type_traits` from standard. Possible implementation of `is_pointer`:  
```cpp
//helper to store a value at compile time
template<class T, T v>
struct intgral_const { //takes type T and deduces it to create non-template parameter v
    static constexpr T value v;
};
//aliases for clean TMP (template metaprogramming)
using true_type = intgral_const<bool, true>;
using false_type = intgral_const<bool, false>;

template<class T>
struct is_pointer : false_type { };

// specialization of <T*> derived from true_type alias
template<class T>
struct is_pointer<T*> : true_type { };

static_assert(is_pointer<*int>::value) //value == true
static_assert(not is_pointer<int>::value) //value == false

//a variable template to access ::value for more readable code
template<typename T>
constexpr auto is_pointer_v = is_pointer<T>::value;

static_assert(is_pointer_v<*int>)
```  
Type traits exceptionally useful with `require` from C++20 or `if constexpr`, that evaluated with other branches in block in compile-time and only true branch goes to binary:  
```cpp
//pre c++20
template<typename T, size_t SIZE>
struct Array{
static_assert(not std::is_pointer<T>::value);
...
};

//c++20
template<typename T, size_t SIZE>
requires(not std::is_pointer<T>::value)
struct Array{
...
};

//if constexpr
template<typename T>
auto getValue(T t) {
    if constexpr(std::is_pointer<T>::value) {
        assert (t != nullptr);
        return *t;
    } else {
        return t;
    }
}
...
Array<int*, 2> arr; //compile error
```  
As well there are `is_convertable` to cast  trivial types  
## <a name="vartm"></a> Variadic templates  
```cpp
template <typename T, typename... Ts> //variadic template
constexpr auto min(const T& a, const T& b, const Ts&... ts) {
    const auto m = a < b ? a : b;
    if constexpr(sizeof...(ts) > 0) { //number of parameters is known in compile-time, ts - pack itself
        return min(m, ts...); //recursion for packed arguments, m -> a, ts -> b, ... -> ...ts
    }
    return m;
}
static_assert(min(3,2,3,4,2,1) == 1);
```  
`typename|class... Ts` generates a type template parameter pack with optional name. `Args... ts` a function argument parameter pack with optional name. `sizeof...(ts)` determine the number of arguments passed, `ts...` for unpacking (expanding) the arguments  

#### Fold expression with C++20 generic lambda(takes auto arguments)
used to unpack a parameter pack using an operation, saves the recursion. Syntax:  
 - (pack `operation` ...) right unary fold  
 - (... pack `operation`) left unary fold  
 - (pack `operation ... operation` [initial_value]) right binary fold  
 - ([initial_value] `operation ... operation` pack) left binary fold  

```cpp
template<typename... Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << '\n'; //left binary fold
}

//more detail example with th same action
template<typename T, typename... Args>
void Print(const T& t, const Args&... args) {
    std::cout << targ;
    auto PrintWithSpaces = [](const auto& arg) { std::cout << ' ' << arg; }
    (..., PrintWithSpaces(args)); //left unary fold
}
...
Print("Hello", "C++", 17, "folds"); //compiler will instantiate Print<char[6], char[4], int, char[6]>
```  
Example of generic function for array comparison  
```cpp
//c++17
namespace details::array_value_compare {

    template<typename T, size_t N, typename U, size_t... Id>
    constexpr bool Compare(const T (&a)[N], const U& b, std::index_sequence<Id...>) {
        return ((a[Id] == b) && ...); //left binary fold
    }

}
template<typename T, size_t N, typename U, size_t... Id>
constexpr bool Compare(const T (&a)[N], const U& b) { //compare a value to each element of the array
    //
    return details::array_value_compare::Compare(a, b, std::make_index_sequence<N>{} )
}

namespace details::array_compare {

    template<typename T, size_t N, size_t... Id>
    constexpr bool Compare(const T (&a)[N], const T (&b)[N], std::index_sequence<Id...>) {
        return ((a[Id] == b[Id]) && ...);
    }

}
template<typename T, size_t N, typename U, size_t... Id>
constexpr bool Compare(const T (&a)[N], const T (&b)[N]) { //compare two arrays
    return details::array_compare::Compare(a, b, std::make_index_sequence<N>{} )
}
//std::make_index_sequence<N> - make sequence 0...N-1

//c++20 with template-hat lambdas
template<typename T, size_t N, typename U>
constexpr bool Compare(const T (&a)[N], const U& b) { //compare each element with value
    return [&]<size_t... Id>(std::index_sequence<Id...>) { return ((a[Id] == b) && ...)}(std::make_index_sequence<N>{})
    //std::make_index_sequence<N>{} passed as argument to generic lambda
}

template<typename T, size_t N>
constexpr bool Compare(const T (&a)[N], const T (&b)[N]) { //compare each element with value
    return [&]<size_t... Id>(std::index_sequence<Id...>) { return ((a[Id] == b[Id]) && ...)}(std::make_index_sequence<N>{})
    //std::make_index_sequence<N>{} passed as argument to generic lambda
}
```  

## <a name="sfinae"></a> SFINAE  
___Substitution failure is not an error___  
When the compiler looks into an instantiation of a template and it fails, this failure is silently discarded. There needs to be a template that works for certain type otherwise compile error. Instead of doing our own specializations we can use SFINAE. For example, we have equality check template and specialization for `double` (because it's different in aproximation), what about `float`? We can double our code for `double`, or we just use SFINAE:  
```cpp
template<typename T>
//literally "use this template if type is not float-type and return bool"
std::enable_if_t<not std::is_floating_point_v<T>, bool> equal(const T& a, const T& b) { return a == b; }

template<typename T>
//literally "use this if type is float-type and return bool"
std::enable_if_t<std::is_floating_point_v<T>, bool> equal(const T& a, const T& b) { 
    return std::abs(a - b) < 1e-12; 
}
```  
SFINAE condition can be put in the template head as additional default parameter, as a default function parameter or in the return type (like above). First two cases are visible for user and can cause distraction  
In C++20 use `requires` instead of SFINAE, that passes understandable compile error if something wrong:  
```cpp
template<typename T>
requires(not std::is_floating_point_v<T>) 
bool equal(const T& a, const T& b) { return a == b; }

template<typename T>
requires(std::is_floating_point_v<T>) 
equal(const T& a, const T& b) { return std::abs(a - b) < 1e-12; }
```  

#### Template template parameters  
We have a template parameter, that itself is a template and its parameters are deduced  
```cpp
//ex. for template template of stl
template< template<class, class> class Container, class T, class Allocator = std::allocator<T> >
void foo(const Container<T, Allocator>& c) { ... }
```  
First declare named `Container` template template parameter `template<class, class>` with ommited type names (cannot say `typename` there). Then we provide arguments `class T` for template template parameter (for `Container`), we can also use defaults (ex. `Allocator` type)

## Sources  
[from cpp con](https://github.com/CppCon/CppCon2020/blob/main/Presentations/back_to_basics_templates_part_2/back_to_basics_templates_part_2__andreas_fertig__cppcon_2020.pdf)
