## OBJECT SLICING  
Consider situation: we have some base class with some significant amount of fundamental code for derived classes. We want to implement some inheritance logic, ex. assignment:  
```cpp
class Base {
private:
    _T _membBase;
public:
    Base& operator=(const Base& oth) { ... }
    ...
};
class Der_one : public Base {
private:
    _T _membDerOn;
public:
    Der_one& operator=(const Der_one& oth) { ... }
    ...
};
class Der_two : public Base {
private:
    _T _membDerTw;
public:
    Der_two& operator=(const Der_two& oth) { ... }
    ...
};
...
Der_one d1;
Der_one d1_2;
Base* b1 = &d1; //(1)
Base* b1_2 = &d1_2; //(2)
...
*b1 = *b2; //(!)
```  
Line (!) invokes Base assignment to Der_one, causing partial assignment (Base part members will assign, Der_one's not). Lines (1, 2, !) are very common in C++ and provide polymorphism.  
Basic solution is to define interface, but it could cause huge code dublication of _T _membDerTw-s and general functions declared in Base for all derived classes.  
With usage of  C++ ```virtual``` mechanism we will achive other problem:  
```cpp
...
    virtual Base& operator=(const Base& oth) { ... }
...
    virtual &Der_one operator=(const Base& oth) { ... } // override demands same declaration
...
    virtual &Der_two operator=(const Base& oth) { ... }
...
Der_one d1;
Der_two d2; //now Def_two
Base* b1 = &d1;
Base* b2 = &d2;
...
*b1 = *b2; //(!)
```  
Now we assign Der_two to Der_one causing Mixed-Type assignment, that leads to run-time errors.  First solution is to ```dynamic_cast<>``` base-type arguments to derived types, but it is hardly optimal, because it makes us to check later for ```bad_cast``` exceptions and operation of casting itself might be heavy. Then we could specify ```protected``` for Base ```operator=```, not  ```private``` as it makes impossible to implement assignments in derived classes, it will work <=> Base is abstract class (i.e. cannot be initialized and provide general fundamental information for all derived classes), otherwise we provide AbstractBase and put there generilized in Base data:
```cpp
class AbstractBase {
private:
    _T _membBase;
protected:
    AbstractBase& operator=(const AbstractBase& oth) { ... }
public:
    virtual ~AbstractBase() {} // =0 if no other plain-virtual functions declared
    ...
};
class Base : public AbstractBase {
public:
    Base& operator=(const Base& oth) { ... AbstractBase(oth) ... }
    ...
};
class Der_one : public AbstractBase {
private:
    _T _membDerOn;
public:
    Der_one& operator=(const Der_one& oth) { ... AbstractBase(oth) ... }
    ...
};
class Der_two : public AbstractBase {
private:
    _T _membDerTw;
public:
    Der_two& operator=(const Der_two& oth) { ... AbstractBase(oth) ... }
    ...
};
...
Der_one d1;
Der_two d2;
AbstractBase* b1 = &d1; 
AbstractBase* b2 = &d2;
...
*b1 = *b2; //(!) Compile error
```  
Note, that compiler will implicitly create copy constuctors in derived classes and chain it to abstract class, special member functions could be marked as ```default``` in case of RULE_OF_ZERO. Question of managing resources in abstract class is different of current topic and I'm not sure about its correctness at the moment.  
If you need an explicit constructor to initilize _T _membBase - declare it in AbstractBase as ```protected``` so you could reach it from derived class constructors

#### Sources  
Mayers, More Effective C++ 
