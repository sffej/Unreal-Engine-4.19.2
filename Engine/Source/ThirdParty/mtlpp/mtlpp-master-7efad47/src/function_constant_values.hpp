/*
 * Copyright 2016-2017 Nikolay Aleksiev. All rights reserved.
 * License: https://github.com/naleksiev/mtlpp/blob/master/LICENSE
 */
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Modifications for Unreal Engine

#pragma once


#include "declare.hpp"
#include "ns.hpp"
#include "argument.hpp"

MTLPP_BEGIN

namespace ue4
{
	template<>
	inline ITable<MTLFunctionConstantValues*, void>* CreateIMPTable(MTLFunctionConstantValues* handle)
	{
		static ITable<MTLFunctionConstantValues*, void> Table(object_getClass(handle));
		return &Table;
	}
}

namespace mtlpp
{
    class FunctionConstantValues : public ns::Object<MTLFunctionConstantValues*>
    {
    public:
        FunctionConstantValues();
        FunctionConstantValues(MTLFunctionConstantValues* handle, ns::Ownership const retain = ns::Ownership::Retain) : ns::Object<MTLFunctionConstantValues*>(handle, retain) { }

        void SetConstantValue(const void* value, DataType type, NSUInteger index);
        void SetConstantValue(const void* value, DataType type, const ns::String& name);
        void SetConstantValues(const void* value, DataType type, const ns::Range& range);

        void Reset();
    }
    MTLPP_AVAILABLE(10_12, 10_0);
}

MTLPP_END
