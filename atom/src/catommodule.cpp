/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file COPYING.txt, distributed with this software.
|----------------------------------------------------------------------------*/
#include <cppy/cppy.h>
#include "behaviors.h"
#include "catom.h"
#include "member.h"
#include "memberchange.h"
#include "eventbinder.h"
#include "signalconnector.h"
#include "atomref.h"
#include "atomlist.h"
#include "atomset.h"
#include "atomdict.h"
#include "enumtypes.h"
#include "propertyhelper.h"


namespace
{


bool ready_types()
{
    using namespace atom;
    if( !AtomList::Ready() )
    {
        return false;
    }
    if( !AtomCList::Ready() )
    {
        return false;
    }
    if( !AtomDict::Ready() )
    {
        return false;
    }
    if( !AtomSet::Ready() )
    {
        return false;
    }
    if( !AtomRef::Ready() )
    {
        return false;
    }
    return true;
}

bool add_objects( PyObject* mod )
{
	using namespace atom;
    cppy::ptr atom_list( pyobject_cast( AtomList::TypeObject ) );
	if( PyModule_AddObject( mod, "atomlist", atom_list.get() ) < 0 )
	{
		return false;
	}
    atom_list.release();
    cppy::ptr atom_clist( pyobject_cast( AtomCList::TypeObject ) );
	if( PyModule_AddObject( mod, "atomclist", atom_clist.get() ) < 0 )
	{
		return false;
	}
    atom_clist.release();
    cppy::ptr atom_dict( pyobject_cast( AtomDict::TypeObject ) );
	if( PyModule_AddObject( mod, "atomdict", atom_dict.get() ) < 0 )
	{
		return false;
	}
    atom_dict.release();
    cppy::ptr atom_set( pyobject_cast( AtomSet::TypeObject ) );
	if( PyModule_AddObject( mod, "atomset", atom_set.get() ) < 0 )
	{
		return false;
	}
    atom_set.release();
    cppy::ptr atom_ref( pyobject_cast( AtomRef::TypeObject ) );
	if( PyModule_AddObject( mod, "atomref", atom_ref.get() ) < 0 )
	{
		return false;
	}
    atom_ref.release();
	return true;
}


int
catom_modexec( PyObject *mod )
{
    if( !ready_types() )
    {
        return -1;
    }
    if( !add_objects( mod ) )
    {
        return -1;
    }
    if( import_member() < 0 )
        return -1;
    if( import_memberchange() < 0 )
        return -1;
    if( import_catom() < 0 )
        return -1;
    if( import_eventbinder() < 0 )
        return -1;
    if( import_signalconnector() < 0 )
        return -1;
    if( import_enumtypes() < 0 )
        return -1;

    Py_INCREF( &Member_Type );
    Py_INCREF( &CAtom_Type );
    Py_INCREF( PyGetAttr );
    Py_INCREF( PySetAttr );
    Py_INCREF( PyDelAttr );
    Py_INCREF( PyPostGetAttr );
    Py_INCREF( PyPostSetAttr );
    Py_INCREF( PyDefaultValue );
    Py_INCREF( PyValidate );
    Py_INCREF( PyPostValidate );
    PyModule_AddObject( mod, "Member", pyobject_cast( &Member_Type ) );
    PyModule_AddObject( mod, "CAtom", pyobject_cast( &CAtom_Type ) );
    PyModule_AddObject( mod, "GetAttr", PyGetAttr );
    PyModule_AddObject( mod, "SetAttr", PySetAttr );
    PyModule_AddObject( mod, "DelAttr", PyDelAttr );
    PyModule_AddObject( mod, "PostGetAttr", PyPostGetAttr );
    PyModule_AddObject( mod, "PostSetAttr", PyPostSetAttr );
    PyModule_AddObject( mod, "DefaultValue", PyDefaultValue );
    PyModule_AddObject( mod, "Validate", PyValidate );
    PyModule_AddObject( mod, "PostValidate", PyPostValidate );

    return 0;
}


PyMethodDef
catom_methods[] = {
    { "reset_property", ( PyCFunction )reset_property, METH_VARARGS,
      "Reset a Property member. For internal use only!" },
    { 0 } // Sentinel
};


PyModuleDef_Slot catom_slots[] = {
    {Py_mod_exec, reinterpret_cast<void*>( catom_modexec ) },
    {0, NULL}
};


struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "catom",
        "catom extension module",
        0,
        catom_methods,
        catom_slots,
        NULL,
        NULL,
        NULL
};

}  // namespace


PyMODINIT_FUNC PyInit_catom( void )
{
    return PyModuleDef_Init( &moduledef );
}
