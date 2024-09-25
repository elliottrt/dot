# dot

## Description

### General

A programming language with a lot of dot syntax.
Note that the dot operator is left associative, so
`a.b.c`
is equivalent to
`(a.b).c`

### Variables
Like Python, variables are created simply by using giving them a name. They begin with null values, which may be changed using the `set` function:  
For example, `a.set.10` creates a null-valued variable named `a`, finds the system function called `set`, and calls `set` with caller = `a` and argument = `10`, setting `a` to the integer `10`.

Integer literals are represented in decimal and may be negative:  ```0```, ```10000```, ```-434234```, ```3``` etc.

String literals are enclosed in double quotation marks and may have escape characters such as `\t`, `\n`, and `\"`:  
```"Hello, World!\n"```

Array literals may have differently-typed elements and are enclosed in square brackets:  
```[<element1>, <element2>, ...]```

Function Definitions are enclosed in curly brackets and contain 0 or more lines to execute. `@` represents the caller of the function and `$` represents the argument of the function. Functions return the last object that was referenced, and if no object was referenced a new null object is returned.
```
{
	...
	...
	... (<- note that the last object referenced is returned)
}
```

### Data Types
#### null
 - has no inherent value
 - has attributes
#### integer
 - inherent value: integer
 - has attributes
#### string
 - inherent value: string
 - has attributes
#### array
 - inherent value: array of objects
 - may be indexed by integers or with the `get` function
 - setting elements may be done by calling the `set` function on an element like so: `array.get.<index>.set.<new value>`
 - has attributes
#### function
 - inherent value: a function
 - does NOT have attributes
 - is always called with two arguments: caller (represented by `@`) and argument (represented by `$`)
	- it is possible for the caller to be left out, in which case it is a new null object
 - functions are called like so: `<caller>.<function>.<argument>`
