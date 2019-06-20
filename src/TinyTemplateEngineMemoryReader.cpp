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

#include "TinyTemplateEngineMemoryReader.h"
#include <string.h>

TinyTemplateEngineMemoryReader::TinyTemplateEngineMemoryReader(const char* text):
    Reader(),
    _text(text),
    _position(text) {}

void TinyTemplateEngineMemoryReader::reset() {
    _position = _text;
}

TinyTemplateEngine::Line TinyTemplateEngineMemoryReader::nextLine() {
    if (! *_position) return TinyTemplateEngine::Line(0, 0);
    const char* end = strchr(_position, '\n');
    size_t len =
        end ?
            end - _position
        :
            strlen(_position)
    ;
    const char* curentLine = _position;
    _position += len; if (*_position /* \n */) _position++;
    return TinyTemplateEngine::Line(curentLine, len);
}
