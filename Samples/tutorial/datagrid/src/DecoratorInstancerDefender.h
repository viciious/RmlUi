/*
 * Copyright (c) 2006 - 2008
 * Wandering Monster Studios Limited
 *
 * Any use of this program is governed by the terms of Wandering Monster
 * Studios Limited's Licence Agreement included with this program, a copy
 * of which can be obtained by contacting Wandering Monster Studios
 * Limited at info@wanderingmonster.co.nz.
 *
 */

#ifndef DECORATORINSTANCERDEFENDER_H
#define DECORATORINSTANCERDEFENDER_H

#include <RmlUi/Core/DecoratorInstancer.h>

/**
	Decorator instancer for the Defender decorator.
	@author Robert Curry
 */

class DecoratorInstancerDefender : public Rml::Core::DecoratorInstancer
{
public:
	DecoratorInstancerDefender();
	virtual ~DecoratorInstancerDefender();

	/// Instances a decorator given the property tag and attributes from the RCSS file.
	/// @param name The type of decorator desired. For example, "background-decorator: simple;" is declared as type "simple".
	/// @param properties All RCSS properties associated with the decorator.
	/// @return The decorator if it was instanced successful, NULL if an error occured.
	Rml::Core::Decorator* InstanceDecorator(const Rml::Core::String& name, const Rml::Core::PropertyDictionary& properties);
	/// Releases the given decorator.
	/// @param decorator Decorator to release. This is guaranteed to have been constructed by this instancer.
	void ReleaseDecorator(Rml::Core::Decorator* decorator);

	/// Releases the instancer.
	void Release();
};

#endif
