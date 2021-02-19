## RULE OF ZERO/THREE/FIVE  
C++ treats variables of user-defined types with value/move semantics. This means that objects can be copied/moved in various contexts, so every user-defined type has at least default/copy/move constructors and copy/move assignment operators.  
Copy - construct a fresh object based on the state of an existing object. Move - construct a fresh object from grabbed ownership of other object, that also gain ownership of other object later or in most cases simply be destroyed.  
C++ standard says that: the implementation will implicitly declare these member functions member-wise in lexo-graphical order for some class types when the program does not explicitly declare them  

Resource managing    
"Resource" is an object, usually external, provided by an outside source, that a block of code uses.  
 - Allocated memory is resource operated by pointers and operations new/delete;  
 - File handlers by fopen/fclose; Threads by fork/join; Mutexes by _mutex_locks e.t.c.  
 
All of it needs manual resource management. Consider class with pointer-member, that points to some memory on the heap when object created. With implicitly declared copy-constructor compiler will copy the pointer, i.e the address of resource, not the resource itself. In that case we have to declare our own copy-constructor. Same goes for other special member functions and we have to declare them too. So to formalize:  

1. RULE OF THREE:  
If a class manages some resource(raw pointer, raw handler, raw mutex, e.t.c) you need to declare special member functions by yourself:  
   1. A destructor to free resource  
   2. A copy constructor to copy resource  
   3. An assignment operator to free an old value of lhs, and then copy new value from rhs to lhs  

If I need (i), then I need (ii), (iii)
#### COPY-SWAP 
An assignment operator can run into self-assignment situation or recursive structure assignment, that causes garbage-copying (deleted lhs then copy what? from lhs to lhs)  
First way of dealing with it is to check for self assignment:
```cpp
if(this != other) {
  delete[] _data; // (!)
  _data = nullptr;
  _size = oth.size; // (2)
  _data = _size ? new int[_size] : nullptr; //ternary: if _size > 0 -> new int[] else nullptr (1)
  std::copy(oth._data, oth._data + _size, _data); // (3)
}
```
(1) - could throw exception caused by memory exhaustion, but object would be in other state (already deleted (!)).  
We could place (1), (2), (3) above (!) and rewrite with creating temporary, thus we don't need 'if' anymore: self-assignment is rare case and 'if' slows execution.  
But that will be the same instructions that copy-constructor provides (code dublication), so (1-3) can be replaced with creation of temporary copy
```cpp
friend void swap(_T& lhs, _T& rhs) {
  std::swap(_rhs._size, _lhs._size);
  std::swap(_rhs._data, _lhs._data);
}
_T& operator=(_T oth) { //(4)
  swap(*this, oth);
  return *this;
}
```
(4) - constructor-call in argument, if there goes rvalue - move constructor is invoked, else copy-constructor  
Copy-Swap - choice between security and speed, it is not a neccesity  

 - If we don't want allow copies (in case of huge resource-objects), we explicitly mark copy-constructor/assignment as ```=delete;```  
 - When dealing with built-in type/wrapper members good practice to leave the work of declaring special member functions to compiler.  
That can be done implicitly by just not defining them or explicitly by marking such functions as ```=default;```, that tells user (and compiler) about usage of implicit declarations  

The ability to ```default``` special members leads to RULE OF ZERO:
2. RULE OF ZERO:  
If your class does not directly manage resources but merely use library components like ```std::vector```, it is better (just recommendation) not to write special member functions  
Let the compiler implicitly generate destructor and default/copy/move constuctors/assignments and mark them ```default```, but not swap (no default at the moment) or any excplicit constructors with args 

3. RULE OF FIVE:  
If a class manages some resource(raw pointer, raw handler, raw mutex, e.t.c) TO OPTIMISE PROGRAM in addition to (RULE OF THREE) you need to declare:  

   4. A move-constructor to transfer ownership of the resource
   5. A move-assignment operator to free lhs resource and tranfer ownership of rhs resource to lhs

In case of copy-swap you need to declare only value-assignment  
```std::move(_T arg)``` could be interpreted as rvalue declarator, so arg becomes rvalue, and we may pass it to move-constructor  
#### Prefer RULE OF ZERO where possible over RULE OF THREE and FIVE  
When using smart pointers - use RULE OF ZERO. For example, ```std::unique_ptr<_T>``` follows RULE OF FIVE (copy constructor/assignment ```=delete;```),  
so in user-defined class with ```std::unique_ptr<_T>``` member it is better to use ```=default;``` move constructor/assignment and destructor. As for copy constructor:  
```cpp
class _T {
  _copy_mov_able_Type _data;
  std::unique_ptr<_copy_mov_able_Type> _uniq_ptr
  ...
  _T(const _T& oth) {
  //invokes explicit template constructor for unique_ptr, no copying of std::unique_ptr here
    _uniq_ptr = std::make_unique<_copy_mov_able_Type>(oth._data); 
    ...
  }
};
```
