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

#ifndef TINY_TEMPLATE_ENGINE_H_INCLUDED
#define TINY_TEMPLATE_ENGINE_H_INCLUDED

#include <stdlib.h>

class TinyTemplateEngine {

    public:

        // One line of template text
        struct Line {
            const char* text; size_t length;
            Line(const char* text, size_t length) {
                this->text = text;
                this->length = length;
            }
            virtual ~Line(){}
            bool eof() const { return text == 0; }
        };

        // Reads the template from the source (memory, file, stream, etc.) line by line
        class Reader {
            public:
                virtual Line nextLine() = 0;
                Reader(){}
                virtual ~Reader(){ reset(); }
                // Free resources, e.g., the memory, which may need to be allocated for the last line
                virtual void reset(){}

        };

        // The engine uses a Reader to read the template lines
        TinyTemplateEngine(Reader& reader);
        virtual ~TinyTemplateEngine();

        // Initialize the engine
        void start(const char * const * values = 0);

        // Read the next line, substitute the variables
        const char *nextLine();

        // Free the resources
        void end();

    private:

        const char* const* _values;
        char* _buffer;
        Reader& _reader;

        void reset();

        // The engine treats memory for time
        // It makes two passes over the source:
        // - Calculate the length of the output line after all the substitutions
        // - Actually do the substitutions
        // This one is the first step
        size_t calculateOutLength(const  Line& line) const;

        // Size known, allocate the memory
        void allocateBuffer(size_t size);

        // The second step: expand the variables
        void expandToBuffer(const  Line& line);

        // Free the memory
        void deleteBuffer();

};

#endif
