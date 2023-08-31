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

#include "Variant.h"
#include "CryptoKey.h"

class ConfigFile {
    Map<String, Variant> values;

public:
    void set_value(const String& p_key, const Variant& p_value);
    Variant get_value(const String& p_key, Variant p_default = Variant()) const;
    bool has_value(const String& p_key) const;
    bool erase_value(const String& p_key);
    void clear();

    const Map<String, Variant>& get_values() const;
    void set_values(Map<String, Variant>& p_values);

    void get_keys(Vector<String>& p_keys) const;

    Error save(const String& p_path) const;
    Error load(const String& p_path);

    Error save_encrypted(const String& p_path, const CryptoKey& p_key) const;
    Error load_encrypted(const String& p_path, const CryptoKey& p_key);

    ConfigFile();
};
