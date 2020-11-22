<h1 align="center">
	<a href="https://github.com/KeyC0de/DllLinkingDemo">DLL Linking Demo</a>
</h1>
<hr>


A DLL (`.dll`) or dynamic link library (or shared library for *nix connoisseurs - `.so` for Linux and `.dylib` for MAC) is a library containg code and data that can be used by multiple programs at the same time. Dlls reduce the size of the executable and the memory usage of the operating system in general, since all applications just need to refer to a single place in memory to use a function in the library (instead of each application having its own copy of it).

Dlls help with separating concerns and reasoning about your code. It also serves for separate compilation, if we change the dll then we need to compile only the dll; the application code remaining unaffected and unaware of the change (in this context it resembles like the [PIMPL](https://github.com/KeyC0de/PIMPLIdiom) idiom). It's worth noting that dll's are a little slower compared to static libraries (which are included in the executable/process), but that speed difference can be considered for almost all intents and purposes negligible (unless you're linking lots of dlls at run time - that's not ideal).

Application |DLL
------------|---------------------
An application can have multiple instances of itself| a dll is single instanced
application has its own local mmeory, stack, file descriptors, message queue and address space| dll has none of those
an application can be run independently| dll can only exist as a component of another application

There are 2 ways to link a dll to an executable:

1. Implicit Linking aka Static Loading aka Link Time Dynamic Linking
2. Explicit Linking aka Dynamic Loading aka Run Time Dynamic Linking

In both cases the .dll must be visible in the "environment" of the process at the time of the execution of its program. In other words the dll can either be in the same folder as the program or in the `PATH` environment variable. Otherwise the application will fail at load time and produce an exception message.

Also the dll must specify its exported function(s) with `__declspec( dllexport )` otherwise they will not be found by the executable. Best to do this like so:

```
extern "C" __declspec( dllexport ) functionDeclaration;
```

In short the main difference between those 2 methods is that with implicit linking the dll is linked at link time and with explicit linking it is loaded at run time whenever you make the `loadLibrary` call. For more information read on.

I should point out that both the dll and the program must share the same platform configuration (ie either both 32 bit or both 64 bit).


## Implicit Linking

With implicit linking the dll is loaded when the process is loaded and it sticks to the process until its termination.

The application:

1. must include the necessary dll's source code headers (that declare the target functions)
2. import the `.lib` which is produced along with `.dll` to link external symbols to the program. This is often the point of confusion for many. This `.lib` here is not a static library as you may have thought even though they share the same file extension. It is known as an "import library". What it does is it produces some glue code, by telling the linker to add the functions to the program's import table and informs the program of the DLL storage space that will be needed later. So all it does is write some information into the executable that will allow the loader to automatically load the required libraries when the application runs.
3. must be able to "see" and access the `.dll` as always

If you're wondering why we must also include this `.lib` in the Project's includes it is because header file inclusion (as is customary) is not enough. Remember in C++ `#include` is simply telling that a function or class or something exists (is "declared"), it doesn't give its implementation (except for static functions and templates).

If you don't want to include the header files you can alternatively specify `dllimport` on those functions inside your application code. You can see in `main.cpp` where I make reference to this in the comments. Use `dllimport` like so:

```
extern "C" __declspec( dllimport ) functionDeclaration;
// no need for dll's header includes!
```

In this project we will only demonstrate implicit linking. But for completion sake I will describe explicit linking as well. So if you're interested continue.


## Explicit Linking

Here the application doesn't include the dll's headers or use its import library, rather it loads the dll's function at runtime using `LoadLibrary`. The OS loads the dll on demand at runtime. To do this it reserves memory for the dll to be placed in the process's address space and later frees it when it's not needed. This can occur multiple times during a process's lifetime. Most dlls are loaded like this as system dlls during OS startup and they are resident in C:\Windows\system32.

What you have to do in this case in your program code is:

1. specify function pointers to the target `dll`'s functions you'll be using eg. `typedef void (__stdcall *DllFunc)();`
2. `HMODULE hModule = loadLibrary( "path\\to\\dllModule.dll" )` gets a handle to the dll module. Make sure `hModule` is not `nullptr`. Remember a Windows process has a list of module handles.
3. `DllFunc func = getProcAddress( hModule, "functionName" )` get the address of the dll function. Error check that `func` is not `nullptr`.
4. `func(...)` calls the function.
5. `FreeLibrary( hModule )`frees the dll module.

If you are inquisitive you can use [dependency walker](http://www.dependencywalker.com/) application to identify all the dlls that an application requires.

I used Windows 8.1 x86_64, Visual Studio 2017, Modern C++17 to build the project.


# Contribute

Please submit any bugs you find through GitHub repository 'Issues' page with details describing how to replicate the problem. If you liked it or you learned something new give it a star, clone it, laugh at it, contribute to it whatever. I appreciate all of it. Enjoy.


# License

Distributed under the GNU GPL V3 License. See "GNU GPL license.txt" for more information.


# Contact

email: *nik.lazkey@gmail.com*</br>
website: *www.keyc0de.net*


# Acknowledgements

Microsoft [docs](https://docs.microsoft.com/en-us/cpp/build/importing-into-an-application-using-declspec-dllimport?view=msvc-160)
