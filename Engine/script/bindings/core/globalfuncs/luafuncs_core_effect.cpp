#include "luafuncs_core_effect.h"

#include <scenes/scene.h>



void Binding_Core_GlobalFuncs_Effect::runStaticEffect(long effectID, float startX, float startY, lua_State* L)
{
    LuaGlobal::getEngine(L)->getBaseScene()->launchEffect(effectID,
                                                                startX,
                                                                startY,
                                                                1,
                                                                0,
                                                                0,
                                                                0,
                                                                0);
}

void Binding_Core_GlobalFuncs_Effect::runStaticEffectCentered(long effectID, float startX, float startY, lua_State *L)
{
    LuaGlobal::getEngine(L)->getBaseScene()->launchStaticEffectC(effectID,
                                                                startX,
                                                                startY,
                                                                1,
                                                                0,
                                                                0,
                                                                0,
                                                                 0);
}


void Binding_Core_GlobalFuncs_Effect::runEffect(SpawnEffectDef &effectDef, bool centered, lua_State *L)
{
    LuaGlobal::getEngine(L)->getBaseScene()->launchEffect(effectDef, centered);
}


luabind::scope Binding_Core_GlobalFuncs_Effect::bindToLua()
{
    using namespace luabind;
    return
        namespace_("Effect")[
            def("runStaticEffect", &Binding_Core_GlobalFuncs_Effect::runStaticEffect),
            def("runStaticEffectCentered", &Binding_Core_GlobalFuncs_Effect::runStaticEffectCentered),
            def("runEffect", &Binding_Core_GlobalFuncs_Effect::runEffect)
        ];
}
