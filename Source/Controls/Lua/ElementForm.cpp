/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
 
#include "precompiled.h"
#include "ElementForm.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Controls/ElementForm.h>
#include <RmlUi/Core/Lua/Utilities.h>

namespace Rml {
namespace Controls {
namespace Lua {

//method
int ElementFormSubmit(lua_State* L, ElementForm* obj)
{
    int top = lua_gettop(L);
    const char* name = "";
    const char* value = "";
    if(top > 0)
    {
        name = luaL_checkstring(L,1);
        if(top > 1)
            value = luaL_checkstring(L,2);
    }
    obj->Submit(name,value);
    return 0;
}

Rml::Core::Lua::RegType<ElementForm> ElementFormMethods[] =
{
    LUAMETHOD(ElementForm,Submit)
    { NULL, NULL },
};

luaL_Reg ElementFormGetters[] =
{
    { NULL, NULL },
};

luaL_Reg ElementFormSetters[] =
{
    { NULL, NULL },
};


}
}
}
namespace Rml {
namespace Core {
namespace Lua {
template<> void ExtraInit<Rml::Controls::ElementForm>(lua_State* L, int metatable_index)
{
    //inherit from Element
    ExtraInit<Element>(L,metatable_index);
    LuaType<Element>::_regfunctions(L,metatable_index,metatable_index-1);
    AddTypeToElementAsTable<Rml::Controls::ElementForm>(L);
}
using Rml::Controls::ElementForm;
LUACONTROLSTYPEDEFINE(ElementForm,true)
}
}
}
