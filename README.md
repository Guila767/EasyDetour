# EasyDetour
A c++ friendly interface for the Microsoft detour library

## What it does:
This project uses the Microsoft detour library to make the detouring, with the addition that the detour function can be a non-static class 
function or a lambda function with a pointer to the desired class.

## How to use:
```c++
// The target function to be detoured
void TargetFunction(int value);

// Example class
class Example
{
public:
  void DetourFunction(void(*lpOriginalFunction)(int), int value)
  {
    // Your code here...
    return lpOriginalFunction(value); // You don't need to call this function if you don't want to, but you need to return a value;
  }
}

// local instance
Example klass;

// Creating a local EasyDetour instance;
auto detourExample = EasyDetour::make_detour(&klass, &TargetFunction);

/* The two examples, and no, you can't have more than one detour function! */
detourExample.HookFunction(Example::DetourFunction);
// Or
detourExample.HookFunction([](auto klass, void(*lpOriginalFunction)(int), int value)
{
  // Code...
  return lpOriginalFunction(value); // Same thing here
});

// Keep it in mind that, when the EasyDetour leaves the scope, the detour will be detached! Unless you allocate it with 'new'
```

## Notice:
This project makes use of some "nasty tricks" to work, for sake of simplicity. The problem of using static fields can be solved by doing a custom trampoline function
(that needs to be encoded at runtime and then written to the process).
This code also can fail in some cases, so take a look at the source code before using it. This code only supports Windows, of course, and doesn't support ARM
