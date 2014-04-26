extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "lualibrecadbridge.h"

#include "lua-intf/LuaIntf/LuaIntf.h"
#include "lua-intf/LuaIntf/QtLuaIntf.h"

#include <cad/geometry/geocoordinate.h>
#include <cad/primitive/line.h>
#include <cad/primitive/circle.h>
#include <cad/operations/builder.h>
#include <cad/operations/builderops.h>
#include <cad/dochelpers/documentimpl.h>
#include <cad/dochelpers/entitymanagerimpl.h>
#include <cad/dochelpers/layermanagerimpl.h>

namespace LuaIntf {
    LUA_USING_SHARED_PTR_TYPE(boost::shared_ptr)
}

using namespace LuaIntf;
using namespace lc;

void lua_openlckernel(lua_State* L) {

    LuaBinding(L)
    .beginClass <QString> ("QString")
    .addConstructor(LUA_ARGS(const char*))
    .endClass()

    .beginClass <ID> ("ID")
    .addFunction("id", &ID::id)
    .endClass()

    .beginClass <Variantable> ("Variantable")
    .endClass()
    .beginExtendClass <MetaType, Variantable> ("MetaType")
    .endClass()
    .beginExtendClass <Layer, MetaType> ("Layer")
    .endClass()

    .beginClass<geo::Coordinate>("Coord")
    .addConstructor(LUA_ARGS(double x, double y))
    .addFunction("x", &geo::Coordinate::x)
    .addFunction("y", &geo::Coordinate::y)
    .addFunction("z", &geo::Coordinate::z)
    .endClass()

    .beginClass<geo::Coordinate>("Coordinate")
    .addConstructor(LUA_ARGS(double x, double y))
    .addFunction("x", &geo::Coordinate::x)
    .addFunction("y", &geo::Coordinate::y)
    .addFunction("z", &geo::Coordinate::z)
    .endClass()


    .beginClass <geo::Vector> ("Vector")
    .addConstructor(LUA_SP(shared_ptr<const geo::Vector>), LUA_ARGS(const geo::Coordinate & start, const geo::Coordinate & end))
    .endClass()

    .beginClass <Document> ("Document")
    .endClass()
    .beginClass <DocumentImpl> ("DocumentImpl")
    .endClass()

    .beginClass <EntityManager> ("EntityManager")
    .endClass()
    .beginExtendClass <EntityManagerImpl, EntityManager > ("EntityManagerImpl")
    .addConstructor(LUA_SP(shared_ptr<EntityManagerImpl>), LUA_ARGS(Document * doc))
    .endClass()

    .beginClass <LayerManager> ("LayerManager")
    .endClass()
    .beginExtendClass <LuaLayerManagerImplProxy, LayerManager> ("LayerManagerImpl")
    .addConstructor(LUA_SP(shared_ptr<LuaLayerManagerImplProxy>), LUA_ARGS(Document * doc))
    .endClass()

    .beginExtendClass <CADEntity, ID> ("CADEntity")
    .endClass()

    .beginExtendClass <Line, CADEntity> ("Line")
    .addConstructor(LUA_SP(shared_ptr< Line>), LUA_ARGS(const geo::Coordinate & start, const geo::Coordinate & end, const shared_ptr<Layer> layer))
    .endClass()

    .beginExtendClass <Circle, CADEntity> ("Circle")
    .addConstructor(LUA_SP(shared_ptr< Circle>), LUA_ARGS(const geo::Coordinate & center, double radius, const shared_ptr<Layer> layer))
    .endClass()

    .beginClass <operation::DocumentOperation> ("DocumentOperation")
    .addFunction("execute", &operation::DocumentOperation::execute)
    .endClass()

    .beginExtendClass <LuaBuilderProxy, operation::DocumentOperation> ("Builder")
    .addConstructor(LUA_SP(shared_ptr<LuaBuilderProxy>), LUA_ARGS(Document * doc,   shared_ptr<lc::EntityManager> entityManager))
    .addFunction("append", &LuaBuilderProxy::append)
    .addFunction("move", &LuaBuilderProxy::move)
    .addFunction("copy", &LuaBuilderProxy::copy)
    .addFunction("rotate", &LuaBuilderProxy::rotate)
    .addFunction("push", &LuaBuilderProxy::push)
    .addFunction("loop", &LuaBuilderProxy::repeat)
    .addFunction("begin", &LuaBuilderProxy::begin)
    .endClass()

    .beginClass <operation::Base> ("Base")
    .endClass()
    .beginExtendClass <operation::Move, operation::Base> ("Move")
    .addConstructor(LUA_SP(shared_ptr< operation::Move>), LUA_ARGS(const geo::Coordinate & offset))
    .endClass()
    .beginExtendClass <operation::Begin, operation::Base> ("Begin")
    .addConstructor(LUA_SP(shared_ptr< operation::Begin>), LUA_ARGS())
    .endClass()
    .beginExtendClass <operation::Loop, operation::Base> ("Loop")
    .addConstructor(LUA_SP(shared_ptr< operation::Loop>), LUA_ARGS(const int numTimes))
    .endClass()
    .beginExtendClass <operation::Copy, operation::Base> ("Copy")
    .addConstructor(LUA_SP(shared_ptr< operation::Copy>), LUA_ARGS(const geo::Coordinate & offset))
    .endClass()
    .beginExtendClass <operation::Push, operation::Base> ("Push")
    .addConstructor(LUA_SP(shared_ptr< operation::Push>), LUA_ARGS())
    .endClass();

}

// .addConstructor(LUA_SP(shared_ptr<const CreateEntities>), LUA_ARGS(Document * doc))

