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

#include "Variant.h"

static const char* variant_type[] = {
    "Null",
    "Bool",
    "Int",
    "String",
    "StringVector"
};

String Variant::get_type_string(Type p_type) {
    if (p_type >= static_cast<Type>(0) && p_type < Type::Max) {
        return variant_type[static_cast<size_t>(p_type)];
    } else {
        return "Unknown";
    }

}

Variant::Type Variant::find_type(const String& p_type) {
    for (size_t i = 0; i < static_cast<size_t>(Type::Max); ++i) {
        if (p_type == variant_type[i]) {
            return static_cast<Type>(i);
        }
    }
    return Type::Unknown;
}

void Variant::_construct() {
    switch (type) {
    case Type::String:
        _data = new String();
        break;
    case Type::StringVector:
        _data = new StringVector();
        break;
    }
}

void Variant::_destruct() {
    switch (type) {
    case Type::String:
        delete static_cast<String*>(_data);
        break;
    case Type::StringVector:
        delete static_cast<StringVector*>(_data);
        break;
    }
}

void Variant::_copy(const Variant& p_value) {
    switch (p_value.type) {
    case Type::String:
        *static_cast<String*>(_data) = *static_cast<const String*>(p_value._data);
        break;
    case Type::StringVector:
        *static_cast<StringVector*>(_data) = *static_cast<const StringVector*>(p_value._data);
        break;
    default:
        _data = p_value._data;
        break;
    }
}

void Variant::set_type(Type p_type) {
    if (type == p_type) {
        return;
    }
    _destruct();
    type = p_type;
    _construct();
}

Variant::Type Variant::get_type() const {
    return type;
}

Variant& Variant::operator=(const Variant& p_value) {
    if (this == &p_value) {
        return *this;
    }
    set_type(p_value.type);
    _copy(p_value);
    return *this;
}

Variant& Variant::operator = (bool p_value) {
    set_type(Type::Bool);
    _bool = p_value;
    return *this;
}

Variant& Variant::operator = (int p_value) {
    set_type(Type::Int);
    _int = p_value;
    return *this;
}

Variant& Variant::operator = (const String& p_value) {
    set_type(Type::String);
    *static_cast<String*>(_data) = p_value;
    return *this;
}

Variant& Variant::operator = (const char* p_value) {
    set_type(Type::String);
    *static_cast<String*>(_data) = p_value;
    return *this;
}

Variant& Variant::operator = (const StringVector& p_value) {
    set_type(Type::StringVector);
    *static_cast<StringVector*>(_data) = p_value;
    return *this;
}

Variant::operator const bool() const {
    if (type != Type::Bool) {
        return false;
    }
    return _bool;
}

Variant::operator bool() {
    if (type != Type::Bool) {
        return false;
    }
    return _bool;
}

Variant::operator const int() const {
    if (type != Type::Int) {
        return 0;
    }
    return _int;
}

Variant::operator int() {
    if (type != Type::Int) {
        return 0;
    }
    return _int;
}

Variant::operator const String& () const {
    if (type != Type::String) {
        static const String ret;
        return ret;
    }
    return *static_cast<const String*>(_data);
}

Variant::operator String& () {
    if (type != Type::String) {
        static String ret;
        return ret;
    }
    return *static_cast<String*>(_data);
}

Variant::operator const StringVector& () const {
    if (type != Type::StringVector) {
        static const StringVector ret;
        return ret;
    }
    return *static_cast<const StringVector*>(_data);
}

Variant::operator StringVector& () {
    if (type != Type::StringVector) {
        static StringVector ret;
        return ret;
    }
    return *static_cast<StringVector*>(_data);
}


bool Variant::operator == (const Variant& p_value) {
    if (type != p_value.type) {
        return false;
    }
    switch (type) {
    case Type::Bool: return _bool == p_value._bool;
    case Type::Int: return _int == p_value._int;
    case Type::String: return (*static_cast<const String*>(_data)) == (*static_cast<const String*>(p_value._data));
    case Type::StringVector: return (*static_cast<const StringVector*>(_data)) == (*static_cast<const StringVector*>(p_value._data));
    default: return false;
    }
}

bool Variant::operator == (const bool& p_value) {
    if (type != Type::Bool) {
        return false;
    }
    return _bool == p_value;
}

bool Variant::operator == (const int& p_value) {
    if (type != Type::Int) {
        return false;
    }
    return _int == p_value;
}

bool Variant::operator == (const String& p_value) {
    if (type != Type::String) {
        return false;
    }
    return *static_cast<String*>(_data) == p_value;
}

bool Variant::operator == (const StringVector& p_value) {
    if (type != Type::StringVector) {
        return false;
    }
    return *static_cast<StringVector*>(_data) == p_value;
}

bool Variant::operator != (const Variant& p_value) {
    if (type == p_value.type) {
        return false;
    }
    switch (type) {
    case Type::Bool: return _bool != p_value._bool;
    case Type::Int: return _int != p_value._int;
    case Type::String: return (*static_cast<const String*>(_data)) != (*static_cast<const String*>(p_value._data));
    case Type::StringVector: return (*static_cast<const StringVector*>(_data)) != (*static_cast<const StringVector*>(p_value._data));
    default: return true;
    }
}

bool Variant::operator != (const bool& p_value) {
    if (type != Type::Bool) {
        return false;
    }
    return _bool != p_value;
}

bool Variant::operator != (const int& p_value) {
    if (type != Type::Int) {
        return false;
    }
    return _int != p_value;
}

bool Variant::operator != (const String& p_value) {
    if (type != Type::String) {
        return false;
    }
    return *static_cast<String*>(_data) != p_value;
}

bool Variant::operator != (const StringVector& p_value) {
    if (type != Type::String) {
        return false;
    }
    return *static_cast<StringVector*>(_data) != p_value;
}

String Variant::serialize() const {
    switch (type) {
    case Type::Bool:
        return _bool ? "true" : "false";
    case Type::Int:
        return std::to_string(_int);
    case Type::String:
        return "\"" + *static_cast<String*>(_data) + "\"";
    case Type::StringVector: {
        const StringVector& vector = *static_cast<StringVector*>(_data);
        String variant;
        variant += '{';
        for (int i = 0; i < vector.size(); ++i) {
            variant += "\"" + vector[i] + "\"";
            if (i != vector.size() - 1) {
                variant += ",";
            }
        }
        variant += '}';
        return variant;
    }
    default:
        return "null";
    }
}

Error Variant::parse(const String& p_value) {
    String value = p_value;

    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int ch) { return !std::isspace(ch);  }));
    value.erase(std::find_if(value.rbegin(), value.rend(), [](int ch) { return !std::isspace(ch); }).base(), value.end());

    if (value == "true") {
        set_type(Type::Bool);
        _bool = true;
        return Error::OK;
    } else if (value == "false") {
        set_type(Type::Bool);
        _bool = false;
        return Error::OK;
    } else if (value.front() == '"' && value.back() == '"') {
        set_type(Type::String);
        *static_cast<String*>(_data) = value.substr(1, value.size() - 2);
        return Error::OK;
    } else if (value.front() == '{' && value.back() == '}') {
        set_type(Type::StringVector);
        StringVector vector;
        String content = value.substr(1, value.size() - 2);
        size_t start = 0, end;
        while ((end = content.find(',', start)) != String::npos) {
            String element = content.substr(start, end - start);
            if (element.front() == '"' && element.back() == '"') {
                element = element.substr(1, element.size() - 2);
                vector.push_back(element);
            } else {
                return Error::InvalidData;
            }
            start = end + 1;
        }
        String last_element = content.substr(start);
        if (last_element.front() == '"' && last_element.back() == '"') {
            last_element = last_element.substr(1, last_element.size() - 2);
            vector.push_back(last_element);
        } else {
            return Error::InvalidData;
        }
        *static_cast<StringVector*>(_data) = vector;
        return Error::OK;
    } else {
        try {
            int int_value = std::stoi(value);
            set_type(Type::Int);
            _int = int_value;
            return Error::OK;
        } catch (const std::invalid_argument&) {
            return Error::InvalidData;
        } catch (const std::out_of_range&) {
            return Error::RangeError;
        }
    }
    return Error::InvalidData;
}

Variant::Variant() :
    type(Type::Null),
    _data(nullptr) {
}

Variant::Variant(Type p_type) :
    type(p_type),
    _data(nullptr) {
    _construct();
}

Variant::Variant(const Variant& p_value) :
    type(p_value.type),
    _data(nullptr) {
    _construct();
    _copy(p_value);
}

Variant::Variant(bool p_value) :
    type(Type::Bool),
    _bool(p_value) {
}

Variant::Variant(int p_value) :
    type(Type::Int),
    _int(p_value) {
}

Variant::Variant(const String& p_value) :
    type(Type::String),
    _data(new String(p_value)) {
}

Variant::Variant(const char* p_value) :
    type(Type::String),
    _data(new String(p_value)) {
}

Variant::Variant(const StringVector& p_value) :
    type(Type::StringVector),
    _data(new StringVector(p_value)) {
}

Variant::~Variant() {
    _destruct();
}
