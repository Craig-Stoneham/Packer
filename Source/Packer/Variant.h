/**************************************************************************/
/* Copyright (c) 2023 Craig Stoneham.                                     */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "Error.h"

class Variant {
public:
    enum class Type {
        Unknown = -1,
        Null,
        Bool,
        Int,
        String,
        StringVector,
        Max
    };

private:
    Type type;
    union {
        bool _bool;
        int _int;
        void* _data;
    };

    void _construct();
    void _destruct();
    void _copy(const Variant& p_value);

public:
    static String get_type_string(Type p_type);
    static Type find_type(const String& p_type);

    void set_type(Type p_type);
    Type get_type() const;

    Variant& operator=(const Variant& p_value);
    Variant& operator = (bool p_value);
    Variant& operator = (int p_value);
    Variant& operator = (const String& p_value);
    Variant& operator = (const char* p_value);
    Variant& operator = (const StringVector& p_value);

    operator const bool() const;
    operator bool();
    operator const int() const;
    operator int();
    operator const String& () const;
    operator String& ();
    operator const StringVector& () const;
    operator StringVector& ();

    bool operator == (const Variant& p_value);
    bool operator == (const bool& p_value);
    bool operator == (const int& p_value);
    bool operator == (const String& p_value);
    bool operator == (const StringVector& p_value);

    bool operator != (const Variant& p_value);
    bool operator != (const bool& p_value);
    bool operator != (const int& p_value);
    bool operator != (const String& p_value);
    bool operator != (const StringVector& p_value);

    String serialize() const;
    Error parse(const String& p_value);

    Variant();
    Variant(Type p_type);
    Variant(const Variant& p_value);
    Variant(bool p_value);
    Variant(int p_value);
    Variant(const String& p_value);
    Variant(const char* p_value);
    Variant(const StringVector& p_value);
    ~Variant();
};
