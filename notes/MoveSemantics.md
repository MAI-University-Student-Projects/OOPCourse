# Move Semantics  
 - [Rvalue References](#RvRef)  
 - [std::move](#stdmov)  
 - [Forwarding](#frwd)

Consider following block of code:  
```cpp
typedef std::map<std::string, std::string> dict_t;

dict_t build_dict(DbConnection db) {
    dict_t dict;
    if(db.is_open())
        // fill info to dict
    return dict; //(1)
}
...
void f() {
    dict_t dict; 
    dict = build_dict(getDbSuppl); // (2)
    // ... managing suppliers ...
    dict = build_dict(getDbCustm); // (3)
    // ... managing customers ...
}
```  
(1-3) - copy assignments of dict_t  
Assignment (2,3): destroy old value (freeing) -> copy (allocating, copying)-> destroy temporary. Instead of copying (allocating, copying resource) we want just get the resource ("point" to it), moreover there is no temporary anymore.  
C++11 intoduced ___MOVE SEMANTICS___, defined as Move Constructor/Assignment operator that:  
 - transfer ownership of resource, don't copy it  
 - used when the resource is an rvalue  
 _lvalue_ - appear on the left side & with a name & with an address  
 _rvalue_ - everything not lvalue, i.e temporaries, literal constants, return values(that aren't lvalues), results of built-in operators(that aren't lvalues)  
 _lvalue ref_ `_Type&` binds only to lvalue, const lvalue `const _Type&` binds to everything  
 - result in more efficiency  
 ## <a name="RvRef"></a> Rvalue References
This is managed by rvalue reference `_Type&&`, that only binds to rvalues, and `std::move` turning any expression into rvalue. Passing lvalue to rvalueref causes compile error  
The use of an rvalue ref is an lvalue:  
```cpp
void f(string&&);
void g(string&& x) { 
    f(x); //x used as an lvalue, compile error
}
```  
Guides(not rules):  
 - do not use const rvalue ref: most of the time, rvalue ref used to modify an object (semantics contadiction) into а valid unspecified state to be deleted and usually this object is not const  
 - no rvalue ref as return type, instead return by value (rvalue refs often bind to temporary objects in-scope), `std::move` returns one, cause it casts an argument to rvalue-ref and returns it  
 
## <a name="stdmov"></a> std::move  
`std::move` converts lvalue to rvalue to move constructor/assignment be applied. Use:  
 - if next operation after  `std::move`  is destruction or an assignment or an reset()-like method from stl, do not depend on a value of  `std::move`-d object  
 - with objects, that are passed by rvalue ref and assumed to be destroyed or assigned  
 - do not with the return of a local variable, c++ standart rule: the return expression is an rvalue if it is local variable or parameter, so in these cases move-constructor automatically works to initialize return value instead of copy (compile knows, that these objects would be destroyed after `return`)  
#### Guides for move constructor/assignment
Move contructor/assignment will be implicitly declared if there are no user-declared  
 - destructor  
 - copy constructor/assignment  
 - move assignment/constructor  
 
Deleted if any base or non-static data member cannot be move constructed   
The main goal of move contructor/assignment is to tranfer ownership of resources to object being constructed/assigned:  
 - Any members that doesn't represent ownership, i.e. `int`, `double` should just be copied.  
 - The ownership of anything, that ahandled to some resource, i.e. a pointer or an opened file descriptor, that object has to free at some point, should be transfered  
 - for ASSIGNMENT have to free old resources object already owns
    ```cpp
    struct S {
        int a;
        std::string b;
        S(S&& oth) noexcept : a{oth.a}, b(std::move(oth.b)) {} //noexcept means no-throwing 
        // may mark a(std::move(oth.a)) explicitly but it will call copy-constructor whatever
        //or S(S&& oth) = default;
        
        S& operator=(S&& oth) noexcept {
            a = oth.a
            b = std::move(oth.b) //managed by string class move-assignment
            return *this
        }
        //or S& operator=(S&& oth) = default;
    };

    struct P {
        double* pt;
        P(P&& oth) noexcept : pt(oth.pt) {
        //may mark pt(std::move(pt)) same way but it is just for code-review/documentation purpose
            oth.pt = nullptr 
        }
        // or pt(std::exchange(oth.pt, nullptr)) {} T exchange(T& obj, U&& value_to_assign_to_obj) - returns old value of obj
        // if pt cannot be nullptr then no way to tranfer ownership => class cannot have move constructor (=delete)
        
        S& operator=(S&& oth) noexcept {
            delete[] data; //freeing old resources, that class owns
            pt = std::exchange(oth.pt, nullptr);
            return *this;
            (!!!!)
        }
    };
    ```  
Move constructor/assignment guidlines:  
 - should be explicitly `noexcept`(even via `default`), transfering ownership of the resources should no fail  
 - moved-from object must be left in a valid state for compiler to call a destructor for this object later in a different scope, prefer to leave it in the default constructor state:  
     ```cpp
     struct S {
         size_t len;
         std::string b;
         S(S&& oth) noexcept : b(std::exchange(oth.bn std::string())), len(std::exchange(other.len, 0)) {}
     };
     ```  
 - use `=default` when possible  
 - make move assignment safe for self-assignment (it is bad code design but nevertheless)  (!!!! case):  
     ```cpp
     p = std::move(p) //bye-bye, undefined behaviour
     if(this != &oth) //add self assignment check
     ```  
 - follow _rule_of_five/rule_of_zero_  

Getting back to the first block of code, with C++11 (1), `return dict` - calls move constructor (local variable is treated like an rvalue), (2,3) - move assignments: no copies  
Move semantics allows to design noncopyable structures like `std::unique_ptr`, that will rise compiler errors, if try copying  
More examples:  
```cpp
template<typename _T>
void insert(const _T& x) { 
    vector.push_back(x); //emplacing invokes copy constructor 
}
template<typename _T>
void insert(_T&& x) { 
    //some non alloc operations on x like std::cout
    vector.push_back(std::move(x)); //emplacing invokes move constructor 
}
...
String str;
insert(str) // passed by const lvalue ref, will be copy constructed inside insert in push_back(x)
insert(getString()) // temporary obj passed by rvalue ref, will be move constructed
//some operations with str, i.e. need str more than twice
insert(std::move(str)) // "i do not need str anymore in the scope", passed by rvalue ref, will be move constructed
```
 - `emplace_back` - create an object in the end, `push_back` - put and then create. If certain about the type of emplaced object - may use `emplace_back`. `push_back`  have `emplace_back` inside of itself  
If `getString()` returns `cost string`, move semantics will broke (call back mechanism to copy semantics will invoke `f(const _T&)`) - do not return by const value  
Overloads of `insert()` generates garbage code with increase of arguments. It can be avoided.  
By First approach, when we insert, we create new object anyway, so we can simply do it in passing by-value and then moving created obj to structure. That causes one more move and for easy-movable types it doesn't affect speed:  
```cpp
//pass by const lvalue-ref: copy constructor in argument, then move constructor in vector
//pass by rvalue ref, move constructor in argument, then move constructor in vector (if x is movable)
template<typename _T>
void insert(_T x) { 
    vector.push_back(std::move(x)); //emplacing invokes copy constructor 
}
```  
That is good approach for easy-movable temporary objects, not for `double[10000]` or something needed condition to be created (will be copied in argument in that case). DO NOT PASS BY-VALUE in move semantics:  
 - for setters (do not need allocations)  
 - if there is some condition for constuction (`if` block inside `insert()`)  
 - move is expensive (`double[10000]`)  
For second approach forwarding is used  
## <a name="frwd"></a> Forwarding  
```cpp
template<typename _T>
void f(_T&& x); //forwarding reference
auto&& var2 = var1; //forwarding reference
```  
Forwarding reference represetnts:  
 - an lvalue if it is initialized by lvalue  
 - an rvalue if it is initialized by rvalue  
 - if rvalue reference involve type deduction or appear exactly as `T&&` or `auto&&` it is forwarding reference  

```cpp
...
Widget w;
f(w) //not compiler error, f(Widget& &&) -> f(Widget&)
//&& & -> &, & & -> &, && && -> &&
f(Widget{}) // f(Widget&& &&) -> f(Widget&&)
```  
