RSettings (1.0.1)
=================

Is yet another ini file format parser which was created
to eliminate my Qt dependency. Sure, I could use one of
the thousand already existing ones which are also small
and cute, but there is no fun along this path.

Build status
------------

* master [![Build Status](https://travis-ci.org/r2p2/rsettings.svg?branch=master)](https://travis-ci.org/r2p2/rsettings)
* develop [![Build Status](https://travis-ci.org/r2p2/rsettings.svg?branch=develop)](https://travis-ci.org/r2p2/rsettings)

Currently implemented
---------------------

* Groups
* Comments which start at the beginning of a line
* `;` and `#` can be used to start a commet
* Key/value pairs separated by `=`
* UTF8 seems to work by magic
* Values of the folowing types: string, integer
* Writing an Ini object back to string
* Converting `true` and `false` into equivalent boolean type

Not implemented
---------------

* Interpreting line breaks in values

Known bugs
----------

* If an Ini object is written back into string it ignores comments and
  the order used in the original file.

Example INI file
----------------

```INI
; This is a comment starting with an semicolon.
# You could also use the diamond as well.
# But please be sure not to put anything in front.
root_key=value
numbers=1024
boolean1=true
boolean2=t
boolean3=1

[group]
# Does not hurt to put comments in a group
group_key="another \"small\" value"
with_line_breaks=long\ntext
```

Example usage
-------------

The library itself gives nothing about the source of
the INI data. You just have to pass a string and check
the result value for success. If anything went wrong,
it will tell you why if possible.

```C++
std::string init_data = "...";

rsettings::Ini settings;
rsettings::Result res = settings.read(ini_data);

if (not res.is_successful())
{
	std::cout << res.reason() << std::endl;
	return;
}

bool value = settings.get<bool>("variable_name", false)
```

One day you might get the fancy idea to convert the
Ini object back into the INI format after doing
a change or two. 

```C++
std::string ini_data = settings.write();
```
