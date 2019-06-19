# Line by line template processing for Arduino and ESP. Templates in PROGMEM, SPIFFS, and other

This library replaces placeholders in a text - ${0}, ${1}, ${2}, etc. - with the values of the variables associated with them.

It is different in that it can handle large texts (templates), which don't fit in the memoty (RAM), and it uses very little memory for it. The template can be stored in the program flash memory (PROGMEM), file system (SPIFFS) or any other source, which can be read one line at a time.

- The engine processes the text line by line. The lines must be separated by the line end ('\n'). The line end itself is not included in the output. Your code obtains the lines one by one in a loop and sends them to the destination. For example, ESP8266WebServer::sendContent(line) to send content to a Web server

- The engine only allocates the memory needed to hold a single output line - the current one. It uses no String variables, and reduces memory consumption and fragmentation to a minimum

- The engine is universal and designed to hanle different template sources. It uses an abstract Reader class as an interface to read the lines from the source. Currently, you can use a PROGMEM reader implementation called TinyTemplateEngineMemoryReader. SPIFFS support is coming next, and you can implement your own readers using the memory reader source as an example

Tested with an ESP8266 and Arduino Uno (Elegoo UNO R3).

## How to use it

---
It is simple. Please take a look ar the examples first. You may not need to read any further.

---


Anyway, here are the details in case you need them.


**1. The Template **

Create a template. It is a text with the placeholders. For example. to create it in the program memory:
```c++
static const char* const theTemplate PROGMEM = "\
========================\n\
Running:\n\
- ${0} seconds\n\
- ${1} times\n\
...
";
```
**2. The Individual Values**

Have all your values in a text format, like this (for example, the current run time):
```c++
  unsigned long seconds = millis() / 1000; // Parameter 1
  char secondsBuf[A_LOT];
  sprintf(secondsBuf, "%lu", seconds);
```

Of course, you don't need to convert any variables that are already in the text form (char *).


**3. Put the Values Together **

Make an array of all the individual values:
```c++
  char* values[] = { // The values to be substituted
    secondsBuf, // For ${0}
    timesBuf,   // For {$1}
    0 // Guard against wrong parameters, such as ${9999}
  };
```

**4. Create a Template Line Reader for the Template Lines **

Using the memory reader:
```c++
  TinyTemplateEngineMemoryReader reader(theTemplate);
```

**5. Create the engine **

```c++
  TinyTemplateEngine engine(reader); // The engine. It needs a reader
```


**6. Start **

```c++
  engine.start(values); // Get ready for the first line
```

**7. Read the Lines **

Make a loop to read the lines one by one.

The engine will allocate and free the memory for the line. Just take the data.

```c++
while (const char* line = engine.nextLine()) {
    // Your code here
    // Do what you need with the line variable
    // For example, send it to the web server
    ...
}
```

**8. Clean Up **

You must call this to free the memory:
```c++
engine.end();
```

**9. And Then... **

You are done!


As a result, the memory footprint imposed by the engine will be about the size of the longest line after variable substitution.
