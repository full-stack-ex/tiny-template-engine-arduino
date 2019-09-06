/*

This example expands a text template with the values of some variables and outputs the result to Serial.

The variables are marked as ${0}, ${1}, etc., and are substituted with the corresponding values.

The template is expanded line by line (terminated by \n), so the memory footprint is minimal.

*/


#include "Arduino.h"
#include <stdlib.h>

#include "TinyTemplateEngine.h"
#include "TinyTemplateEngineMemoryReader.h"

// The template. It is located in the program memory (ROM). That saves the precious memory (RAM)
// It contains some variables, including one out of range, which has no corresponding value and will be ignored
// The \n character is important: it denotes the end of the line. It is not included in the output
static PGM_P theTemplate PROGMEM = R"==(
========================
Running:
- ${0} seconds
- ${1} times

${9999}Extra parameters ignored
)==";

void setup() {
  Serial.begin(115200);
}

void loop() {

  // Allocate enough buffer memory
  #define A_LOT 20

  unsigned long seconds = millis() / 1000; // Parameter 1
  char secondsBuf[A_LOT];
  sprintf(secondsBuf, "%lu", seconds);

  static unsigned long times = 0; // Parameter 2
  times++;
  char timesBuf[A_LOT];
  sprintf(timesBuf, "%lu", times);

  // The reader reads the text line by line
  // This one reads from memory
  // See the source to see how to implement other readers, e.g., for SPIFFS
  TinyTemplateEngineMemoryReader reader(theTemplate);
  reader.keepLineEnds(true);  // Include trailing "\n" in the output

  TinyTemplateEngine engine(reader); // The engine. It needs a reader
  
  char* values[] = { // The values to be substituted
    secondsBuf, // For ${0}
    timesBuf,   // For {$1}
    0 // Guard against wrong parameters, such as ${9999} above
  };
  
  engine.start(values); // Get ready for the first line

  while (const char* line = engine.nextLine()) { // Read the lines and expand the variables
    // Memory for the line is allocated and deleted inside the engine
    Serial.print(line); // Might go to ESP8266WebServer::sendContent(line) or anywhere
    // Take a breath
    delay(1);
  }
  
  engine.end(); // Important: you must call this to free the resources
  
  
  delay(1000); // A chance for the user to read the result

}

/*

MIT License

Copyright (c) 2019 full-stack-ex

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
