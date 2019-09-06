# Line by line template processing for Arduino and ESP. Templates in PROGMEM, SPIFFS, and other

This library replaces placeholders in a text - ${0}, ${1}, ${2}, etc. - with the values of the variables associated with them.

It is different in that it can handle large texts (templates), which don't fit in the memoty (RAM), and it uses very little memory for it. The template can be stored in the program flash memory (PROGMEM), file system (SPIFFS) or any other source, which can be read one line at a time.

- The engine processes the text line by line. The lines must be separated by the line end ('\n')

    - By default, the line end itself is not included in the output; there is an option to include it
 - Your code obtains the lines one by one in a loop and sends them to the destination. For example, ESP8266WebServer::sendContent(line) to send content to a Web server

- The engine only allocates the memory needed to hold a single output line - the current one. It uses no String variables, and it reduces memory consumption and fragmentation to a minimum

- The engine is universal and designed to handle different template sources. It uses an abstract Reader class as an interface to read the lines from the source. Currently, you can use a PROGMEM reader implementation called TinyTemplateEngineMemoryReader. SPIFFS support is coming next, and you can implement your own readers using the memory reader source as an example

Tested with an ESP8266, Arduino Uno (Elegoo UNO R3) and ESP32 (Arduino).

## How to use it

---
It is simple. Please take a look at the examples first. You may not need to read any further.

---


Anyway, here are the details in case you need them.
## 1. The Template

Create a template. It is a text with the placeholders. For example, to create it in the program memory:
```c++
static const char* const theTemplate PROGMEM = "\
========================\n\
Running:\n\
- ${0} seconds\n\
- ${1} times\n\
...
";
```
OR
```c++
static PGM_P theTemplate PROGMEM = R"foo(
========================
Running:
- ${0} seconds
- ${1} times

...
)foo";

```
## 2. The Individual Values

Have all your values in a text format, like this (for example, the current run time):
```c++
  unsigned long seconds = millis() / 1000; // Parameter 1
  char secondsBuf[A_LOT];
  sprintf(secondsBuf, "%lu", seconds);
```

Of course, you don't need to convert any variables that are already in the text form (char *).
## 3. Put the Values Together

Make an array of all the individual values:
```c++
  char* values[] = { // The values to be substituted
    secondsBuf, // For ${0}
    timesBuf,   // For {$1}
    0 // Guard against wrong parameters, such as ${9999}
  };
```
## 4. Create a Template Line Reader for the Template Lines

Using the memory reader:
```c++
  TinyTemplateEngineMemoryReader reader(theTemplate);
```
The line end character will not be included in the output. That allows splitting long single line texts seamlessly, like this:
```c++
const char* myLongLine = "This is a very long line, which I want to split somewhere to ma
ke my source code more readable. Please note that even if we split the word in the middle, it will be handled
 correctly. Also notice the leading space in this line.";
```
But you may as well want to keep the line ends - usually, when each source line is an actual line of text.

In particular, you may need this for a web server, which detects the end of a long stream by receiving an empty sting "" as input:
```c++
server.setContentLength(CONTENT_LENGTH_UNKNOWN);
...
server.sendContent(nonEmptyStringsOnly);
...
server.sendContent(""); // Done
```
To do so, add this call:
```c++
reader.keepLineEnds(true);
```
Then the traling "\n" in each source line will be included in the line read.
## 5. Create the Engine for This Template

```c++
  TinyTemplateEngine engine(reader); // The engine. It needs a reader
```
## 6. Initialize

```c++
  engine.start(values); // Get ready for the first line
```
## 7. Read the Lines

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
## 8. Clean Up

You must call this to free the memory:
```c++
engine.end();
```
## 9. And Then...

You are done!


As a result, you will get the template processed, and the memory footprint imposed by the engine will be about the size of the longest line after variable substitution.
