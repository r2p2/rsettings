rsettings
=========

Is yet another ini file format parser which was created
to eliminate my Qt dependency. Sure, I could use one of
the thousand already existing ones which are also small
and cute, but there is no fun along this path.

Features
--------

All the fancy things you can see in this document are
just a way to specify what I want to achieve on one day
or another with this library. I don't know when the real
api will match the following examples and maybe they will
change over time.

So in order to figure out what you can actually do and
how, you have to look into the source code for now. As
soon as I reach version 1, I promise to keep this readme
file in sync with the code.

Have fun...

Example INI file
----------------

```C++
; This is a comment starting with an semicolon.
# You could also use the diamond as well.
# But please be sure not to put anything in front.
root_key = value
numbers = 1024
boolean1 = true
boolean2 = t
boolean3 = 1

[group]
# Does not hurt to put comments in a group
group_key = "another \"small\" value"
with_line_breaks = long\ntext
```

Example usage
-------------

The library itself gives nothing about the source of
the INI data. You just have to pass a string and hope
for no exceptions.

```C++
std::string init_data = "...";

// Create the object first ...
RSettings settings;

// ... and parse later ...
RSettings::Result res = settings.read(ini_data);

// ... so we can check for errors without the use of exceptions
if (not res.is_ok())
{
	std::cout << "Error while parsing INI format!" << std::endl;
	std::cout << res.error_text() << " in line " << res.error_line()
		  << " in column " << res.error_line() << std::endl;
	std::cout << std::endl;
	std::cout << res.annotated_input() << std::endl;
}

```

One day you might get the fancy idea to convert the
RSettings object back into the INI format after doing
a change or two. 

```C++
ini_data = settings.write();
```
