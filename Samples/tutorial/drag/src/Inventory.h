#ifndef INVENTORY_H
#define INVENTORY_H

#include <RmlUi/Core/String.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>

/**
	@author Peter Curry
 */

class Inventory
{
public:
	/// Constructs a new inventory and opens its window.
	/// @param[in] title The title of the new inventory.
	/// @param[in] position The position of the inventory window.
	/// @param[in] context The context to open the inventory window in.
	Inventory(const Rml::Core::String& title, const Rml::Core::Vector2f& position, Rml::Core::Context* context);
	/// Destroys the inventory and closes its window.
	~Inventory();

	/// Adds a brand-new item into this inventory.
	/// @param[in] name The name of this item.
	void AddItem(const Rml::Core::String& name);

private:
	Rml::Core::ElementDocument* document;
};

#endif
