// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Data/DataChannel_Bool.h"

void UDataChannel_Bool::SetValue(const bool NewValue)
{
	if (NewValue == Value) return;
	Value = NewValue;
	OnChangeValue.Broadcast(Value);
}
